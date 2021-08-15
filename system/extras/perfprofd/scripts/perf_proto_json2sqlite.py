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

import argparse
import itertools
import json
import sqlite3


class SqliteWriter(object):
    def __init__(self):
        self.sample_count = 0
        self.dso_map = {}
        self.pid_map = {}
        self.tid_map = {}
        self.symbol_map = {}

    def open(self, out):
        self._conn = sqlite3.connect(out)
        self._c = self._conn.cursor()
        # Ensure tables exist
        # The sample replicates pid and tid.
        try:
            self._c.execute('''CREATE TABLE pids (id integer PRIMARY KEY AUTOINCREMENT,
                                                  name text)''')
            self._c.execute('''CREATE TABLE tids (id integer PRIMARY KEY AUTOINCREMENT,
                                                  name text)''')
            self._c.execute('''CREATE TABLE syms (id integer PRIMARY KEY AUTOINCREMENT,
                                                  name text)''')
            self._c.execute('''CREATE TABLE dsos (id integer PRIMARY KEY AUTOINCREMENT,
                                                  name text)''')
            self._c.execute('''CREATE TABLE samples (id integer PRIMARY KEY AUTOINCREMENT,
                                                     pid_id int not null,
                                                     tid_id int not null)
                                                     ''')
            self._c.execute('''CREATE TABLE stacks (sample_id int not null,
                                                    depth int not null,
                                                    dso_id int not null,
                                                    sym_id int not null,
                                                    offset int not null,
                                                    primary key (sample_id, depth))
                                                    ''')
        except sqlite3.OperationalError:
            pass  # ignore

    def close(self):
        self._conn.commit()
        self._conn.close()

    def insert_into_tmp_or_get(self, name, table_dict, table_dict_tmp):
        if name in table_dict:
            return table_dict[name]
        if name in table_dict_tmp:
            return table_dict_tmp[name]
        index = len(table_dict) + len(table_dict_tmp)
        table_dict_tmp[name] = index
        return index

    def prepare(self):
        self.dso_tmp_map = {}
        self.pid_tmp_map = {}
        self.tid_tmp_map = {}
        self.symbol_tmp_map = {}
        self.samples_tmp_list = []
        self.stacks_tmp_list = []

    def write_sqlite_index_table(self, table_dict, table_name):
        for key, value in table_dict.iteritems():
            self._c.execute("insert into {tn} values (?,?)".format(tn=table_name), (value, key))

    def flush(self):
        self.write_sqlite_index_table(self.pid_tmp_map, 'pids')
        self.write_sqlite_index_table(self.tid_tmp_map, 'tids')
        self.write_sqlite_index_table(self.dso_tmp_map, 'dsos')
        self.write_sqlite_index_table(self.symbol_tmp_map, 'syms')

        for sample in self.samples_tmp_list:
            self._c.execute("insert into samples values (?,?,?)", sample)
        for stack in self.stacks_tmp_list:
            self._c.execute("insert into stacks values (?,?,?,?,?)", stack)

        self.pid_map.update(self.pid_tmp_map)
        self.tid_map.update(self.tid_tmp_map)
        self.dso_map.update(self.dso_tmp_map)
        self.symbol_map.update(self.symbol_tmp_map)

        self.dso_tmp_map = {}
        self.pid_tmp_map = {}
        self.tid_tmp_map = {}
        self.symbol_tmp_map = {}
        self.samples_tmp_list = []
        self.stacks_tmp_list = []

    def add_sample(self, sample, tid_name_map):
        sample_id = self.sample_count
        self.sample_count = self.sample_count + 1

        def get_name(pid, name_map):
            if pid in name_map:
                return name_map[pid]
            pid_str = str(pid)
            if pid_str in name_map:
                return name_map[pid_str]
            if pid == 0:
                return "[kernel]"
            return "[unknown]"

        pid_name = get_name(sample[0], tid_name_map)
        pid_id = self.insert_into_tmp_or_get(pid_name, self.pid_map, self.pid_tmp_map)
        tid_name = get_name(sample[1], tid_name_map)
        tid_id = self.insert_into_tmp_or_get(tid_name, self.tid_map, self.tid_tmp_map)

        self.samples_tmp_list.append((sample_id, pid_id, tid_id))

        stack_depth = 0
        for entry in sample[2]:
            sym_id = self.insert_into_tmp_or_get(entry[0], self.symbol_map, self.symbol_tmp_map)
            dso = entry[2]
            if dso is None:
                dso = "None"
            dso_id = self.insert_into_tmp_or_get(dso, self.dso_map, self.dso_tmp_map)

            self.stacks_tmp_list.append((sample_id, stack_depth, dso_id, sym_id, entry[1]))

            stack_depth = stack_depth + 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='''Process a set of perfprofd JSON files produced
                                                    by perf_proto_stack.py into SQLite database''')

    parser.add_argument('file', help='JSON files to parse and combine', metavar='file', nargs='+')

    parser.add_argument('--sqlite-out', help='SQLite database output', type=str,
                        default='sqlite.db')

    args = parser.parse_args()
    if args is not None:
        sql_out = SqliteWriter()
        sql_out.open(args.sqlite_out)
        sql_out.prepare()

        for f in args.file:
            print 'Processing %s' % (f)
            fp = open(f, 'r')
            data = json.load(fp)
            fp.close()

            for sample in data['samples']:
                sql_out.add_sample(sample, data['names'])

            sql_out.flush()

        sql_out.close()
