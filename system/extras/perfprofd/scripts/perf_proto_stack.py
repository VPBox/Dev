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

# Super simplistic printer of a perfprofd output proto. Illustrates
# how to parse and traverse a perfprofd output proto in Python.

# This relies on libunwindstack's unwind_symbol. Build with
#   mmma system/core/libunwindstack

import argparse
from datetime import datetime
import itertools
import json
import logging
from multiprocessing.dummy import Pool as ThreadPool
import os.path
from sorted_collection import SortedCollection
import subprocess
from threading import Timer

# Generate with:
#  aprotoc -I=external/perf_data_converter/src/quipper \
#      --python_out=system/extras/perfprofd/scripts \
#      external/perf_data_converter/src/quipper/perf_data.proto
#  aprotoc -I=external/perf_data_converter/src/quipper -I=system/extras/perfprofd \
#      --python_out=system/extras/perfprofd/scripts \
#      system/extras/perfprofd/perfprofd_record.proto
import perfprofd_record_pb2
import perf_data_pb2

# Make sure that symbol is on the PYTHONPATH, e.g., run as
# PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/development/scripts python ...
import symbol
from symbol import SymbolInformation

logging.basicConfig(format='%(message)s')

# This is wrong. But then the symbol module is a bad quagmire.
# TODO: Check build IDs.
symbol.SetAbi(["ABI: 'arm64'"])


class MmapState(object):

    def __init__(self):
        self._list = SortedCollection((), lambda x: x[0])

    def add_map(self, start, length, pgoff, name):
        map_tuple = (start, length, pgoff, name)
        self._list.insert(map_tuple)

    def find(self, addr):
        try:
            map_tuple = self._list.find_le(addr)
            if addr < map_tuple[0] + map_tuple[1]:
                return map_tuple
            return None
        except ValueError:
            return None

    def copy(self):
        ret = MmapState()
        ret._list = self._list.copy()
        return ret

    def __str__(self):
        return 'MmapState: ' + self._list.__str__()

    def __repr__(self):
        return self.__str__()


class SymbolMap(object):
    def __init__(self, min_v):
        self._list = SortedCollection((), lambda x: x[0])
        self._min_vaddr = min_v

    def add_symbol(self, start, length, name):
        tuple = (start, length, name)
        self._list.insert(tuple)

    def find(self, addr):
        try:
            tuple = self._list.find_le(addr)
            if addr < tuple[0] + tuple[1]:
                return tuple[2]
            return None
        except ValueError:
            return None

    def copy(self):
        ret = SymbolMap()
        ret._list = self._list.copy()
        return ret

    def __str__(self):
        return "SymbolMap: " + self._list.__str__()

    def __repr__(self):
        return self.__str__()


def intern_uni(u):
    return intern(u.encode('ascii', 'replace'))


def collect_tid_names(perf_data):
    tid_name_map = {}
    for event in perf_data.events:
        if event.HasField('comm_event'):
            tid_name_map[event.comm_event.tid] = intern_uni(event.comm_event.comm)
    return tid_name_map


def create_symbol_maps(profile):
    symbol_maps = {}

    for si in profile.Extensions[perfprofd_record_pb2.symbol_info]:
        map = SymbolMap(si.min_vaddr)
        symbol_maps[si.filename] = map
        for sym in si.symbols:
            map.add_symbol(sym.addr, sym.size, intern_uni(sym.name))
    return symbol_maps


def update_mmap_states(event, state_map):
    if event.HasField('mmap_event'):
        mmap_event = event.mmap_event
        # Skip kernel stuff.
        if mmap_event.tid == 0:
            return
        # Create new map, if necessary.
        if mmap_event.pid not in state_map:
            state_map[mmap_event.pid] = MmapState()
        state_map[mmap_event.pid].add_map(mmap_event.start, mmap_event.len, mmap_event.pgoff,
                                          intern_uni(mmap_event.filename))
    elif event.HasField('fork_event'):
        fork_event = event.fork_event
        # Skip threads
        if fork_event.pid == fork_event.ppid:
            return

        if fork_event.ppid not in state_map:
            logging.warn("fork from %d without map", fork_event.ppid)
            return
        state_map[fork_event.pid] = state_map[fork_event.ppid].copy()


skip_dso = set()
vaddr = {}


def find_vaddr(vaddr_map, filename):
    if filename in vaddr_map:
        return vaddr_map[filename]

    path = "%s/%s" % (symbol.SYMBOLS_DIR, filename)
    if not os.path.isfile(path):
        logging.warn('Cannot find %s for min_vaddr', filename)
        vaddr_map[filename] = 0
        return 0

    try:
        # Use "-W" to have single-line format.
        res = subprocess.check_output(['readelf', '-lW', path])
        lines = res.split("\n")
        reading_headers = False
        min_vaddr = None

        def min_fn(x, y):
            return y if x is None else min(x, y)
        # Using counting loop for access to next line.
        for i in range(0, len(lines) - 1):
            line = lines[i].strip()
            if reading_headers:
                if line == "":
                    # Block is done, won't find anything else.
                    break
                if line.startswith("LOAD"):
                    # Look at the current line to distinguish 32-bit from 64-bit
                    line_split = line.split()
                    if len(line_split) >= 8:
                        if " R E " in line:
                            # Found something expected. So parse VirtAddr.
                            try:
                                min_vaddr = min_fn(min_vaddr, int(line_split[2], 0))
                            except ValueError:
                                pass
                    else:
                        logging.warn('Could not parse readelf line %s', line)
            else:
                if line.strip() == "Program Headers:":
                    reading_headers = True

        if min_vaddr is None:
            min_vaddr = 0
        logging.debug("min_vaddr for %s is %d", filename, min_vaddr)
        vaddr_map[filename] = min_vaddr
    except subprocess.CalledProcessError:
        logging.warn('Error finding min_vaddr for %s', filename)

        vaddr_map[filename] = 0
    return vaddr_map[filename]


unwind_symbols_cache = {}
unwind_symbols_warn_missing_cache = set()


def run_unwind_symbols(filename, offset_hex):
    path = "%s/%s" % (symbol.SYMBOLS_DIR, filename)
    if not os.path.isfile(path):
        if path not in unwind_symbols_warn_missing_cache:
            logging.warn('Cannot find %s for unwind_symbols', filename)
            unwind_symbols_warn_missing_cache.add(path)
        return None

    if (path, offset_hex) in unwind_symbols_cache:
        pair = unwind_symbols_cache[(path, offset_hex)]
        if pair is None:
            return None
        return [(pair[0], pair[1], filename)]

    try:
        res = subprocess.check_output(['unwind_symbols', path, offset_hex])
        lines = res.split("\n")
        for line in lines:
            if line.startswith('<0x'):
                parts = line.split(' ', 1)
                if len(parts) == 2:
                    # Get offset, too.
                    offset = 0
                    plus_index = parts[0].find('>+')
                    if plus_index > 0:
                        offset_str = parts[0][plus_index + 2:-1]
                        try:
                            offset = int(offset_str)
                        except ValueError:
                            logging.warn('error parsing offset from %s', parts[0])

                    # TODO C++ demangling necessary.
                    logging.debug('unwind_symbols: %s %s -> %s +%d', filename, offset_hex, parts[1],
                                  offset)
                    sym = intern(parts[1])
                    unwind_symbols_cache[(path, offset_hex)] = (sym, offset)
                    return [(sym, offset, filename)]
    except subprocess.CalledProcessError:
        logging.warn('Failed running unwind_symbols for %s', filename)
        unwind_symbols_cache[(path, offset_hex)] = None
        return None


def decode_with_symbol_lib(name, addr_rel_hex):
    info = SymbolInformation(name, addr_rel_hex)
    # As-is, only info[0] (inner-most inlined function) is recognized.
    (source_symbol, source_location, object_symbol_with_offset) = info[0]

    def parse_symbol_lib_output(s):
        i = s.rfind('+')
        if i > 0:
            try:
                off = int(s[i + 1:])
                return (s[0:i], off)
            except ValueError:
                pass
        return (s, 0)

    ret = []

    if object_symbol_with_offset is not None:
        pair = parse_symbol_lib_output(object_symbol_with_offset)
        ret.append((intern(pair[0]), pair[1], name))

    if source_symbol is not None:
        iterinfo = iter(info)
        next(iterinfo)
        for (sym_inlined, loc_inlined, _) in iterinfo:
            # TODO: Figure out what's going on here:
            if sym_inlined is not None:
                pair = parse_symbol_lib_output(sym_inlined)
                ret.insert(0, (intern(pair[0]), pair[1], name))
    if len(ret) > 0:
        return ret
    return None


def decode_addr(addr, mmap_state, device_symbols):
    """Try to decode the given address against the current mmap table and device symbols.

    First, look up the address in the mmap state. If none is found, use a simple address
    heuristic to guess kernel frames on 64-bit devices.

    Next, check on-device symbolization for a hit.

    Last, try to symbolize against host information. First try the symbol module. However,
    as it is based on addr2line, it will not work for pure-gnu_debugdata DSOs (e.g., ART
    preopt artifacts). For that case, use libunwindstack's unwind_symbols.
    """

    map = mmap_state.find(addr)
    if map is None:
        # If it looks large enough, assume it's from
        # the kernel.
        if addr > 18000000000000000000:
            return [("[kernel]", 0, "[kernel]")]
        return [("%d (no mapped segment)" % addr, 0, None)]
    name = map[3]
    logging.debug('%d is %s (%d +%d)', addr, name, map[0], map[1])

    # Once relocation packer is off, it would be:
    #   offset = addr - map.start + map.pgoff
    # Right now it is
    #   offset = addr - map.start (+ min_vaddr)
    # Note that on-device symbolization doesn't include min_vaddr but
    # does include pgoff.
    offset = addr - map[0]

    if name in device_symbols:
        offset = offset + map[2]
        symbol = device_symbols[name].find(offset)
        if symbol is None:
            return [("%s (missing on-device symbol)" % (name), offset, name)]
        else:
            # TODO: Should we change the format?
            return [(symbol, 0, name)]
    offset = offset + find_vaddr(vaddr, name)
    if (name, offset) in skip_dso:
        # We already failed, skip symbol finding.
        return [(name, offset, name)]
    else:
        addr_rel_hex = intern("%x" % offset)
        ret = decode_with_symbol_lib(name, addr_rel_hex)
        if ret is not None and len(ret) != 0:
            # Addr2line may report oatexec+xyz. Let unwind_symbols take care of that.
            if len(ret) != 1 or ret[0][0] != 'oatexec':
                logging.debug('Got result from symbol module: %s', str(ret))
                return ret
        # Try unwind_symbols
        ret = run_unwind_symbols(name, addr_rel_hex)
        if ret is not None and len(ret) != 0:
            return ret
        logging.warn("Failed to find symbol for %s +%d (%d)", name, offset, addr)
        # Remember the fail.
        skip_dso.add((name, offset))
        return [(name, offset, name)]


def print_sample(sample, tid_name_map):
    if sample[0] in tid_name_map:
        pid_name = "%s (%d)" % (tid_name_map[sample[0]], sample[0])
    elif sample[0] == 0:
        pid_name = "kernel (0)"
    else:
        pid_name = "unknown (%d)" % (sample[0])

    if sample[1] in tid_name_map:
        tid_name = "%s (%d)" % (tid_name_map[sample[1]], sample[1])
    elif sample[1] == 0:
        tid_name = "kernel (0)"
    else:
        tid_name = "unknown (%d)" % (sample[1])
    print " %s - %s:" % (pid_name, tid_name)

    for sym in sample[2]:
        print "   %s +%d (%s)" % (sym[0], sym[1], sym[2])


def print_samples(samples, tid_name_map):
    for sample in samples:
        print_sample(sample, tid_name_map)


def symbolize_events(perf_data, device_symbols, tid_name_map, printSamples=False,
                     removeKernelTop=False):
    samples = []
    mmap_states = {}
    for event in perf_data.events:
        update_mmap_states(event, mmap_states)
        if event.HasField('sample_event'):
            sample_ev = event.sample_event
            # Handle sample.
            new_sample = None
            if sample_ev.pid in mmap_states:
                mmap_state = mmap_states[sample_ev.pid]
                ip_sym = decode_addr(sample_ev.ip, mmap_state, device_symbols)
                stack = ip_sym
                for cc_ip in sample_ev.callchain:
                    cc_sym = decode_addr(cc_ip, mmap_state, device_symbols)
                    stack.extend(cc_sym)
                if removeKernelTop:
                    while len(stack) > 1 and stack[0][0] == "[kernel]":
                        stack.pop(0)
                new_sample = (sample_ev.pid, sample_ev.tid, stack)
            else:
                # Handle kernel symbols specially.
                if sample_ev.pid == 0:
                    samples.append((0, sample_ev.tid, [("[kernel]", 0, "[kernel]")]))
                elif sample_ev.pid in tid_name_map:
                    samples.append((sample_ev.pid, sample_ev.tid, [(tid_name_map[sample_ev.pid], 0,
                                                                    None)]))
                else:
                    samples.append((sample_ev.pid, sample_ev.tid, [("[unknown]", 0, None)]))
            if new_sample is not None:
                samples.append(new_sample)
                if printSamples:
                    print_sample(new_sample, tid_name_map)
    return samples


def count_key_reduce_function(x, y, key_fn):
    key = key_fn(y)
    if key not in x:
        x[key] = 0
    x[key] += 1
    return x


def print_histogram(samples, reduce_key_fn, label_key_fn, size):
    # Create a sorted list of top samples.
    sorted_count_list = sorted(
        reduce(lambda x, y: count_key_reduce_function(x, y, reduce_key_fn), samples, {}).
        iteritems(),
        cmp=lambda x, y: cmp(x[1], y[1]),
        reverse=True)
    sorted_count_topX = list(itertools.islice(sorted_count_list, size))

    # Print top-size samples.
    print 'Histogram top-%d:' % (size)
    for i in xrange(0, len(sorted_count_topX)):
        print '  %d: %s (%s)' % (i + 1, label_key_fn(sorted_count_topX[i][0]),
                                 sorted_count_topX[i][1])


def get_name(pid, tid_name_map):
    if pid in tid_name_map:
        return tid_name_map[pid]
    if pid == 0:
        return "[kernel]"
    return "[unknown]"


def create_cmd(args, f):
    ret = ['python', '-u', 'system/extras/perfprofd/scripts/perf_proto_stack.py']
    if args.syms is not None:
        ret.extend(['--syms', args.syms[0]])
    if args.print_samples is not None:
        ret.append('--print-samples')
    if args.skip_kernel_syms is not None:
        ret.append('--skip-kernel-syms')
    if args.print_pid_histogram is not None:
        ret.append('--print-pid-histogram')
    if args.print_sym_histogram is not None:
        ret.append('--print-sym-histogram')
    if args.print_dso_histogram is not None:

        ret.append('--print-dso-histogram')
    ret.extend(['--json-out', '%s.json' % (f)])
    ret.append(f)
    return ret


def run_cmd(x):

    args = x[0]
    f = x[1]
    cmd = create_cmd(args, f)
    logging.warn('Running on %s', f)
    success = False
    logging.debug('%r', cmd)
    err_out = open('%s.err' % (f), 'w')

    def kill(process):
        process.kill()
    start = datetime.now()

    p = subprocess.Popen(cmd, stderr=err_out)
    kill_timer = Timer(3600, kill, [p])
    try:
        kill_timer.start()
        p.communicate()
        success = True

    finally:
        kill_timer.cancel()
    err_out.close()
    end = datetime.now()
    logging.warn('Ended %s (%s)', f, str(end - start))
    return '%s: %r' % (f, success)


def parallel_runner(args):
    pool = ThreadPool(args.parallel)
    map_args = map(lambda f: (args, f), args.file)

    result = pool.map(run_cmd, map_args)
    pool.close()
    pool.join()
    print result


def run(args):
    if args.syms is not None:
        symbol.SYMBOLS_DIR = args.syms[0]
    print_symbols = args.print_samples is not None
    skip_kernel_syms = args.skip_kernel_syms is not None

    # TODO: accept argument for parsing.
    file = open(args.file[0], 'rb')
    data = file.read()

    file.close()

    profile = perf_data_pb2.PerfDataProto()
    # PerfprofdRecord()
    profile.ParseFromString(data)

    perf_data = profile

    print "Stats: ", perf_data.stats

    tid_name_map = collect_tid_names(perf_data)
    symbol_maps = create_symbol_maps(profile)

    samples = symbolize_events(perf_data, symbol_maps, tid_name_map, printSamples=print_symbols,
                               removeKernelTop=skip_kernel_syms)

    if args.print_pid_histogram is not None:
        print_histogram(samples, lambda x: x[0], lambda x: get_name(x, tid_name_map), 25)
    if args.print_sym_histogram is not None:
        print_histogram(samples, lambda x: x[2][0][0], lambda x: x, 100)
    if args.print_dso_histogram is not None:
        print_histogram(samples, lambda x: x[2][0][2], lambda x: x, 25)

    if args.json_out is not None:
        json_file = open(args.json_out[0], 'w')
        json_data = {'samples': samples, 'names': tid_name_map}
        json.dump(json_data, json_file)
        json_file.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process a perfprofd record.')

    parser.add_argument('file', help='proto file to parse', metavar='file', nargs='+')
    parser.add_argument('--syms', help='directory for symbols', nargs=1)
    parser.add_argument('--json-out', help='output file for JSON', nargs=1)
    parser.add_argument('--print-samples', help='print samples', action='store_const', const=True)
    parser.add_argument('--skip-kernel-syms', help='skip kernel symbols at the top of stack',
                        action='store_const', const=True)
    parser.add_argument('--print-pid-histogram', help='print a top-25 histogram of processes',
                        action='store_const', const=True)
    parser.add_argument('--print-sym-histogram', help='print a top-100 histogram of symbols',
                        action='store_const', const=True)
    parser.add_argument('--print-dso-histogram', help='print a top-25 histogram of maps',
                        action='store_const', const=True)
    parser.add_argument('--parallel', help='run parallel jobs', type=int)

    args = parser.parse_args()
    if args is not None:
        if args.parallel is not None:
            parallel_runner(args)
        else:
            run(args)
