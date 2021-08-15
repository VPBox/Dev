#!/usr/bin/python2
#
# Copyright (C) 2013 The Android Open Source Project
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

"""Block diff utility."""

from __future__ import print_function

# pylint: disable=import-error
import argparse
import sys


class BlockDiffError(Exception):
  pass


def BlockDiff(block_size, file1, file2, name1, name2, max_length=-1):
  """Performs a binary diff of two files by blocks.

  Args:
    block_size: the size of a block to diff by
    file1: first file object
    file2: second file object
    name1: name of first file (for error reporting)
    name2: name of second file (for error reporting)
    max_length: the maximum length to read/diff in bytes (optional)
  Returns:
    A list of (start, length) pairs representing block extents that differ
    between the two files.
  Raises:
    BlockDiffError if there were errors while diffing.

  """
  if max_length < 0:
    max_length = sys.maxint
  diff_list = []
  num_blocks = extent_start = extent_length = 0
  while max_length or extent_length:
    read_length = min(max_length, block_size)
    data1 = file1.read(read_length)
    data2 = file2.read(read_length)
    if len(data1) != len(data2):
      raise BlockDiffError('read %d bytes from %s but %d bytes from %s' %
                           (len(data1), name1, len(data2), name2))

    if data1 != data2:
      # Data is different, mark it down.
      if extent_length:
        # Stretch the current diff extent.
        extent_length += 1
      else:
        # Start a new diff extent.
        extent_start = num_blocks
        extent_length = 1
    elif extent_length:
      # Record the previous extent.
      diff_list.append((extent_start, extent_length))
      extent_length = 0

    # Are we done reading?
    if not data1:
      break

    max_length -= len(data1)
    num_blocks += 1

  return diff_list


def main(argv):
  # Parse command-line arguments.
  parser = argparse.ArgumentParser(
      description='Compare FILE1 and FILE2 by blocks.',
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

  parser.add_argument('-b', '--block-size', metavar='NUM', type=int,
                      default=4096, help='the block size to use')
  parser.add_argument('-m', '--max-length', metavar='NUM', type=int, default=-1,
                      help='maximum number of bytes to compare')
  parser.add_argument('file1', metavar='FILE1')
  parser.add_argument('file2', metavar='FILE2')

  args = parser.parse_args(argv[1:])

  # Perform the block diff.
  try:
    with open(args.file1) as file1:
      with open(args.file2) as file2:
        diff_list = BlockDiff(args.block_size, file1, file2,
                              args.file1, args.file2, args.max_length)
  except BlockDiffError as e:
    print('Error: ' % e, file=sys.stderr)
    return 2

  # Print the diff, if such was found.
  if diff_list:
    total_diff_blocks = 0
    for extent_start, extent_length in diff_list:
      total_diff_blocks += extent_length
      print('%d->%d (%d)' %
            (extent_start, extent_start + extent_length, extent_length))

    print('total diff: %d blocks' % total_diff_blocks)
    return 1

  return 0


if __name__ == '__main__':
  sys.exit(main(sys.argv))
