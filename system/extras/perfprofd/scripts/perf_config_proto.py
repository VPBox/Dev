#!/usr/bin/python
#
# Copyright (C) 2017 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Test converter of a Config proto.

# Generate with:
#  aprotoc -I=system/extras/perfprofd --python_out=system/extras/perfprofd/scripts \
#      system/extras/perfprofd/binder_interface/perfprofd_config.proto
#
# Note: it is necessary to do a '*' import to not have to jump through hoops
#       with reflective instantiation.
from perfprofd_config_pb2 import *

# Necessary for introspection.
from google.protobuf.descriptor import FieldDescriptor

import sys

PROTO_FIELD_TYPE_NAMES = {
    FieldDescriptor.TYPE_DOUBLE: "double",
    FieldDescriptor.TYPE_FLOAT: "float",
    FieldDescriptor.TYPE_INT64: "int64",
    FieldDescriptor.TYPE_UINT64: "uint64",
    FieldDescriptor.TYPE_INT32: "int32",
    FieldDescriptor.TYPE_FIXED64: "fixed64",
    FieldDescriptor.TYPE_FIXED32: "fixed32",
    FieldDescriptor.TYPE_BOOL: "bool",
    FieldDescriptor.TYPE_STRING: "string",
    FieldDescriptor.TYPE_GROUP: "group",
    FieldDescriptor.TYPE_MESSAGE: "message",
    FieldDescriptor.TYPE_BYTES: "bytes",
    FieldDescriptor.TYPE_UINT32: "uint32",
    FieldDescriptor.TYPE_ENUM: "enum",
    FieldDescriptor.TYPE_SFIXED32: "sfixed32",
    FieldDescriptor.TYPE_SFIXED64: "sfixed64",
    FieldDescriptor.TYPE_SINT32: "sint32",
    FieldDescriptor.TYPE_SINT64: "sint64",
}
def get_type_string(proto_field_type):
    if proto_field_type in PROTO_FIELD_TYPE_NAMES:
        return PROTO_FIELD_TYPE_NAMES[proto_field_type]
    return "unknown type"


def read_message(msg_descriptor, indent):
    istr = ' ' * indent
    print('%s%s' % (istr, msg_descriptor.name))
    # Create an instance
    instance = globals()[msg_descriptor.name]()

    # Fill fields.

    primitive_fields = [None]
    message_fields = [None]
    for field in msg_descriptor.fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            message_fields.append(field)
        else:
            primitive_fields.append(field)

    def table_loop(fields, field_fn):
        while True:
            # Print selection table
            maxlen = len(str(len(fields) - 1))
            def pad_index(key):
                while len(key) < maxlen:
                    key = ' ' + key
                return key

            for i in xrange(1, len(fields)):
                print('%s%s: %s' % (istr, pad_index(str(i)), fields[i].name))
            print('%s%s: done' % (istr, pad_index('0')))
            print('%s%s: end' % (istr, pad_index('!')))

            sel = raw_input('%s ? ' % (istr))
            if sel == '!':
                # Special-case input, end argument collection.
                return False

            try:
                sel_int = int(sel)
                if sel_int == 0:
                    return True

                if sel_int > 0 and sel_int < len(fields):
                    field = fields[sel_int]
                    if not field_fn(field):
                        return False
                else:
                    print('Not a valid input (%d)!' % (sel_int))
                    continue
            except:
                print('Not a valid input! (%s, %s)' % (sel, str(sys.exc_info()[0])))
                continue

#    # 1) Non-message-type fields.
    if len(primitive_fields) > 1:
        print('%s(Primitives)' % (istr))

        def primitive_fn(field):
            input = raw_input('%s  -> %s (%s): ' % (istr, field.name, get_type_string(field.type)))
            if input == '':
                # Skip this field
                return True
            if input == '!':
                # Special-case input, end argument collection.
                return False

            # Simplification: assume ints or bools or strings, but not floats
            if field.type == FieldDescriptor.TYPE_BOOL:
                input = input.lower()
                set_val = True if input == 'y' or input == 'true' or input == '1' else False
            elif field.type == FieldDescriptor.TYPE_STRING:
                set_val = input
            else:
                try:
                    set_val = int(input)
                except:
                    print('Could not parse input as integer!')
                    return True
            if field.label == FieldDescriptor.LABEL_REPEATED:
                getattr(instance, field.name).extend([set_val])
            else:
                setattr(instance, field.name, set_val)
            return True

        if not table_loop(primitive_fields, primitive_fn):
            return (instance, False)

    # 2) Message-type fields.
    if len(message_fields) > 1:
        print('%s(Nested messages)' % (istr))

        def message_fn(field):
            sub_msg, cont = read_message(field.message_type, indent + 4)
            if sub_msg is not None:
                if field.label == FieldDescriptor.LABEL_REPEATED:
                    # Repeated field, use extend.
                    getattr(instance, field.name).extend([sub_msg])
                else:
                    # Singular field, copy into.
                    getattr(instance, field.name).CopyFrom(sub_msg)
            return cont

        if not table_loop(message_fields, message_fn):
            return (instance, False)

    return (instance, True)


def collect_and_write(filename):
    config, _ = read_message(ProfilingConfig.DESCRIPTOR, 0)

    if config is not None:
        with open(filename, "wb") as f:
            f.write(config.SerializeToString())


def read_and_print(filename):
    config = ProfilingConfig()

    with open(filename, "rb") as f:
        config.ParseFromString(f.read())

    print config


def print_usage():
    print('Usage: python perf_config_proto.py (read|write) filename')


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print_usage()
    elif sys.argv[1] == 'read':
        read_and_print(sys.argv[2])
    elif sys.argv[1] == 'write':
        collect_and_write(sys.argv[2])
    else:
        print_usage()
