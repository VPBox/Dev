# Copyright 2018 - The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Command-line tool to build SEPolicy files."""

import argparse
import os
import subprocess
import sys

import file_utils


# All supported commands in this module.
# For each command, need to add two functions. Take 'build_cil' for example:
#   - setup_build_cil()
#     - Sets up command parsers and sets default function to do_build_cil().
#   - do_build_cil()
_SUPPORTED_COMMANDS = ('build_cil', 'filter_out')


def run_host_command(args, **kwargs):
    """Runs a host command and prints output."""
    if kwargs.get('shell'):
        command_log = args
    else:
        command_log = ' '.join(args)  # For args as a sequence.

    try:
        subprocess.check_call(args, **kwargs)
    except subprocess.CalledProcessError as err:
        sys.stderr.write(
            'build_sepolicy - failed to run command: {!r} (ret:{})\n'.format(
                command_log, err.returncode))
        sys.exit(err.returncode)


def do_build_cil(args):
    """Builds a sepolicy CIL (Common Intermediate Language) file.

    This functions invokes some host utils (e.g., secilc, checkpolicy,
    version_sepolicy) to generate a .cil file.

    Args:
        args: the parsed command arguments.
    """
    # Determines the raw CIL file name.
    input_file_name = os.path.splitext(args.input_policy_conf)[0]
    raw_cil_file = input_file_name + '_raw.cil'
    # Builds the raw CIL.
    file_utils.make_parent_dirs(raw_cil_file)
    checkpolicy_cmd = [args.checkpolicy_env]
    checkpolicy_cmd += [os.path.join(args.android_host_path, 'checkpolicy'),
                        '-C', '-M', '-c', args.policy_vers,
                        '-o', raw_cil_file, args.input_policy_conf]
    # Using shell=True to setup args.checkpolicy_env variables.
    run_host_command(' '.join(checkpolicy_cmd), shell=True)
    file_utils.filter_out([args.reqd_mask], raw_cil_file)

    # Builds the output CIL by versioning the above raw CIL.
    output_file = args.output_cil
    if output_file is None:
        output_file = input_file_name + '.cil'
    file_utils.make_parent_dirs(output_file)

    run_host_command([os.path.join(args.android_host_path, 'version_policy'),
                      '-b', args.base_policy, '-t', raw_cil_file,
                      '-n', args.treble_sepolicy_vers, '-o', output_file])
    if args.filter_out_files:
        file_utils.filter_out(args.filter_out_files, output_file)

    # Tests that the output file can be merged with the given CILs.
    if args.dependent_cils:
        merge_cmd = [os.path.join(args.android_host_path, 'secilc'),
                     '-m', '-M', 'true', '-G', '-N', '-c', args.policy_vers]
        merge_cmd += args.dependent_cils      # the give CILs to merge
        merge_cmd += [output_file, '-o', '/dev/null', '-f', '/dev/null']
        run_host_command(merge_cmd)


def setup_build_cil(subparsers):
    """Sets up command args for 'build_cil' command."""

    # Required arguments.
    parser = subparsers.add_parser('build_cil', help='build CIL files')
    parser.add_argument('-i', '--input_policy_conf', required=True,
                        help='source policy.conf')
    parser.add_argument('-m', '--reqd_mask', required=True,
                        help='the bare minimum policy.conf to use checkpolicy')
    parser.add_argument('-b', '--base_policy', required=True,
                        help='base policy for versioning')
    parser.add_argument('-t', '--treble_sepolicy_vers', required=True,
                        help='the version number to use for Treble-OTA')
    parser.add_argument('-p', '--policy_vers', required=True,
                        help='SELinux policy version')

    # Optional arguments.
    parser.add_argument('-c', '--checkpolicy_env',
                        help='environment variables passed to checkpolicy')
    parser.add_argument('-f', '--filter_out_files', nargs='+',
                        help='the pattern files to filter out the output cil')
    parser.add_argument('-d', '--dependent_cils', nargs='+',
                        help=('check the output file can be merged with '
                              'the dependent cil files'))
    parser.add_argument('-o', '--output_cil', help='the output cil file')

    # The function that performs the actual works.
    parser.set_defaults(func=do_build_cil)


def do_filter_out(args):
    """Removes all lines in one file that match any line in another file.

    Args:
        args: the parsed command arguments.
    """
    file_utils.filter_out(args.filter_out_files, args.target_file)

def setup_filter_out(subparsers):
    """Sets up command args for 'filter_out' command."""
    parser = subparsers.add_parser('filter_out', help='filter CIL files')
    parser.add_argument('-f', '--filter_out_files', required=True, nargs='+',
                        help='the pattern files to filter out the output cil')
    parser.add_argument('-t', '--target_file', required=True,
                        help='target file to filter')
    parser.set_defaults(func=do_filter_out)


def run(argv):
    """Sets up command parser and execuates sub-command."""
    parser = argparse.ArgumentParser()

    # Adds top-level arguments.
    parser.add_argument('-a', '--android_host_path', default='',
                        help='a path to host out executables')

    # Adds subparsers for each COMMAND.
    subparsers = parser.add_subparsers(title='COMMAND')
    for command in _SUPPORTED_COMMANDS:
        globals()['setup_' + command](subparsers)

    args = parser.parse_args(argv[1:])
    args.func(args)


if __name__ == '__main__':
    run(sys.argv)
