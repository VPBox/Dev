#!/usr/bin/env python3

#
# Copyright (C) 2019 The Android Open Source Project
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
#

import sys

DEFAULT_TYPES_TO_JNI = {
  "boolean": "Z",
  "byte": "B",
  "char": "C",
  "short": "S",
  "int": "I",
  "long": "J",
  "float": "F",
  "double": "D",
  "void": "V",
  "String": "Ljava/lang/String;"
}

class AIDLMalformedSignatureException(Exception):
  """Line containing AIDL signature is invalid"""

def convert_type(aidl_type):
  if aidl_type.endswith("[]"):
    return "[" + convert_type(aidl_type[:-2])
  if aidl_type in DEFAULT_TYPES_TO_JNI:
    return DEFAULT_TYPES_TO_JNI[aidl_type]
  elif aidl_type.startswith("List<") | aidl_type.startswith("java.util.List<"):
    return "Ljava/util/List;"
  else:
    return "L" + aidl_type.replace(".", "/") + ";"

def convert_method(aidl_signature):
  aidl_signature = aidl_signature.split("|")
  if len(aidl_signature) != 4:
    raise AIDLMalformedSignatureException()
  class_name, method_name, args, return_type = aidl_signature
  # Filter out empty arguments since there will be trailing commas
  args = [x for x in args.split(",") if x]
  jni_signature = convert_type(class_name)
  jni_signature += "->"
  jni_signature += method_name
  jni_signature += "("
  params = [convert_type(x) for x in args]
  jni_signature += "".join(params)
  jni_signature += ")"
  jni_signature += convert_type(return_type)
  return jni_signature

def main(argv):
  if len(argv) != 3:
    print("Usage: %s <aidl-mappings> <jni-signature-mappings>" % argv[0])
    return -1

  aidl_mappings, jni_signature_mappings = argv[1:]

  line_index = 0
  skip_line = False
  with open(aidl_mappings) as input_file:
    with open(jni_signature_mappings, "w") as output_file:
      for line in input_file:
        if skip_line:
          skip_line = False
        elif line_index % 2 == 1:
          output_file.write(line)
        else:
          try:
            stripped_line = line.strip()
            output_file.write(convert_method(line.strip()))
            output_file.write("\n")
          except AIDLMalformedSignatureException:
            print("Malformed signature %s . Skipping..." % stripped_line)
            # The next line contains the location, need to skip it
            skip_line = True
        line_index += 1

if __name__ == "__main__":
  sys.exit(main(sys.argv))
