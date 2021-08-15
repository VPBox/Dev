#!/usr/bin/python
#
# Copyright (C) 2018 The Android Open Source Project
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

# Make sure that simpleperf's inferno is on the PYTHONPATH, e.g., run as
# PYTHONPATH=$PYTHONPATH:$ANDROID_BUILD_TOP/system/extras/simpleperf/scripts/inferno python ..

import argparse
import itertools
import sqlite3


class Callsite(object):
    def __init__(self, dso_id, sym_id):
        self.dso_id = dso_id
        self.sym_id = sym_id
        self.count = 0
        self.child_map = {}
        self.id = self._get_next_callsite_id()

    def add(self, dso_id, sym_id):
        if (dso_id, sym_id) in self.child_map:
            return self.child_map[(dso_id, sym_id)]
        new_callsite = Callsite(dso_id, sym_id)
        self.child_map[(dso_id, sym_id)] = new_callsite
        return new_callsite

    def child_count_to_self(self):
        self.count = reduce(lambda x, y: x + y[1].count, self.child_map.iteritems(), 0)

    def trim(self, local_threshold_in_percent, global_threshold):
        local_threshold = local_threshold_in_percent * 0.01 * self.count
        threshold = max(local_threshold, global_threshold)
        for k, v in self.child_map.items():
            if v.count < threshold:
                del self.child_map[k]
        for _, v in self.child_map.iteritems():
            v.trim(local_threshold_in_percent, global_threshold)

    def _get_str(self, id, m):
        if id in m:
            return m[id]
        return str(id)

    def print_callsite_ascii(self, depth, indent, dsos, syms):

        print '  ' * indent + "%s (%s) [%d]" % (self._get_str(self.sym_id, syms),
                                                self._get_str(self.dso_id, dsos),
                                                self.count)
        if depth == 0:
            return
        for v in sorted(self.child_map.itervalues, key=lambda x: x.count, reverse=True):
            v.print_callsite_ascii(depth - 1, indent + 1, dsos, syms)

    # Functions for flamegraph compatibility.

    callsite_counter = 0

    @classmethod
    def _get_next_callsite_id(cls):
        cls.callsite_counter += 1
        return cls.callsite_counter

    def create_children_list(self):
        self.children = sorted(self.child_map.itervalues(), key=lambda x: x.count, reverse=True)

    def generate_offset(self, start_offset):
        self.offset = start_offset
        child_offset = start_offset
        for child in self.children:
            child_offset = child.generate_offset(child_offset)
        return self.offset + self.count

    def svgrenderer_compat(self, dsos, syms):
        self.create_children_list()
        self.method = self._get_str(self.sym_id, syms)
        self.dso = self._get_str(self.dso_id, dsos)
        self.offset = 0
        for c in self.children:
            c.svgrenderer_compat(dsos, syms)

    def weight(self):
        return float(self.count)

    def get_max_depth(self):
        if self.child_map:
            return max([c.get_max_depth() for c in self.child_map.itervalues()]) + 1
        return 1


class SqliteReader(object):
    def __init__(self):
        self.root = Callsite("root", "root")
        self.dsos = {}
        self.syms = {}

    def open(self, f):
        self._conn = sqlite3.connect(f)
        self._c = self._conn.cursor()

    def close(self):
        self._conn.close()

    def read(self, local_threshold_in_percent, global_threshold_in_percent, limit,
             skip_simpleperf):
        # Read aux tables first, as we need to find the kernel symbols.
        def read_table(name, dest_table):
            self._c.execute('select id, name from %s' % (name))
            while True:
                rows = self._c.fetchmany(100)
                if not rows:
                    break
                for row in rows:
                    dest_table[row[0]] = row[1]

        print 'Reading DSOs'
        read_table('dsos', self.dsos)

        print 'Reading symbol strings'
        read_table('syms', self.syms)

        kernel_sym_id = None
        for i, v in self.syms.iteritems():
            if v == '[kernel]':
                kernel_sym_id = i
                break

        skip_query_str = ""
        if skip_simpleperf:
            self._c.execute('select id from pids where name = "simpleperf"')
            pid_row = self._c.fetchone()
            if pid_row:
                skip_query_join = "as st join samples sa on st.sample_id = sa.id "
                skip_query_str = skip_query_join + "where sa.pid_id != %d" % (pid_row[0])

        query_prefix = 'select sample_id, depth, dso_id, sym_id from stacks '
        query_suffix = ' order by sample_id asc, depth desc'

        print 'Reading samples'
        self._c.execute(query_prefix + skip_query_str + query_suffix)

        last_sample_id = None
        chain = None
        count = 0
        while True:
            rows = self._c.fetchmany(100)

            if not rows:
                break
            for row in rows:
                if row[3] == kernel_sym_id and row[1] == 0:
                    # Skip kernel.
                    continue
                if row[0] != last_sample_id:
                    last_sample_id = row[0]
                    chain = self.root
                chain = chain.add(row[2], row[3])
                chain.count = chain.count + 1
            count = count + len(rows)
            if limit is not None and count >= limit:
                print 'Breaking as limit is reached'
                break

        self.root.child_count_to_self()
        global_threshold = global_threshold_in_percent * 0.01 * self.root.count
        self.root.trim(local_threshold_in_percent, global_threshold)

    def print_data_ascii(self, depth):
        self.root.print_callsite_ascii(depth, 0, self.dsos, self.syms)

    def get_script_js(self):
        # Try to get the data directly (if packaged with embedded_loader).
        import os.path
        import sys
        if '__loader__' in globals():
            try:
                js = __loader__.get_data(os.path.join(os.path.dirname(__file__), "script.js"))
                if js is not None and len(js) > 0:
                    return js
            except:
                pass
        # See if we can find it another way.
        rel_paths = [
            # Maybe we're being run packaged.
            "script.js",
            # Maybe we're being run directly.
            "../../simpleperf/scripts/inferno/script.js",
        ]
        for rel_path in rel_paths:
            script_js = os.path.join(os.path.dirname(__file__), rel_path)
            if os.path.exists(script_js):
                with open(script_js, 'r') as script_f:
                    return script_f.read()
        return None

    def print_svg(self, filename, depth):
        from svg_renderer import render_svg
        self.root.svgrenderer_compat(self.dsos, self.syms)
        self.root.generate_offset(0)
        f = open(filename, 'w')
        f.write('''
<html>
<body>
<div id='flamegraph_id' style='font-family: Monospace;'>
<style type="text/css"> .s { stroke:black; stroke-width:0.5; cursor:pointer;} </style>
<style type="text/css"> .t:hover { cursor:pointer; } </style>
''')

        class FakeProcess:
            def __init__(self):
                self.props = {'trace_offcpu': False}
        fake_process = FakeProcess()
        render_svg(fake_process, self.root, f, 'hot')

        f.write('''
</div>
''')

        # Emit script.js, if we can find it.
        script_data = self.get_script_js()
        if script_data is not None:
            f.write('<script>\n')
            f.write(script_data)
            f.write('''
</script>
<br/><br/>
<div>Navigate with WASD, zoom in with SPACE, zoom out with BACKSPACE.</div>
<script>document.addEventListener('DOMContentLoaded', flamegraphInit);</script>
</body>
</html>
''')
        f.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='''Translate a perfprofd database into a flame
                                                    representation''')

    parser.add_argument('file', help='the sqlite database to use', metavar='file', type=str)

    parser.add_argument('--html-out', help='output file for HTML flame graph', type=str)
    parser.add_argument('--threshold', help='child threshold in percent', type=float, default=5)
    parser.add_argument('--global-threshold', help='global threshold in percent', type=float,
                        default=.1)
    parser.add_argument('--depth', help='depth to print to', type=int, default=10)
    parser.add_argument('--limit', help='limit to given number of stack trace entries', type=int)
    parser.add_argument('--skip-simpleperf', help='skip simpleperf samples', action='store_const',
                        const=True)

    args = parser.parse_args()
    if args is not None:
        sql_out = SqliteReader()
        sql_out.open(args.file)
        sql_out.read(args.threshold, args.global_threshold, args.limit,
                     args.skip_simpleperf is not None)
        if args.html_out is None:
            sql_out.print_data_ascii(args.depth)
        else:
            sql_out.print_svg(args.html_out, args.depth)
        sql_out.close()
