# Copyright (C) 2015 The Android Open Source Project
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

import math
import os
import random
import shutil
import subprocess
import sys
import tempfile

blocksize = 4096
roots = 2

def corrupt(image, offset, length):
    print "corrupting %d bytes at offset %d" % (length, offset)
    f = os.open(image, os.O_WRONLY)
    os.lseek(f, offset, os.SEEK_SET)
    os.write(f, os.urandom(length))
    os.close(f)

def corruptmax(image, roots):
    size = os.stat(image).st_size

    blocks = int(math.ceil(float(size) / blocksize))
    rounds = int(math.ceil(float(blocks) / (255 - roots)))

    max_errors = int(math.floor(rounds * roots / 2)) * blocksize
    offset = random.randrange(0, size - max_errors)

    corrupt(image, offset, max_errors)

def encode(image, fec, roots):
    if subprocess.call([ "fec", "--roots= " + str(roots), image, fec ]) != 0:
        raise Exception("encoding failed")

def decode(image, fec, output):
    return subprocess.call([ "fec", "--decode", image, fec, output ])

def compare(a, b):
    return subprocess.call([ "cmp", "-s", a, b ])

def simg2img(image, output):
    print "creating a non-sparse copy of '%s' to '%s'" % (image, output)
    if subprocess.call([ "simg2img", image, output]) != 0:
        raise Exception("simg2img failed")

def main(argv):
    image = argv[0]

    temp_img = tempfile.NamedTemporaryFile()
    temp_cor = tempfile.NamedTemporaryFile()
    temp_fec = tempfile.NamedTemporaryFile()
    temp_out = tempfile.NamedTemporaryFile()

    simg2img(image, temp_img.name)
    simg2img(image, temp_cor.name)

    encode(image, temp_fec.name, roots)
    corruptmax(temp_cor.name, roots)

    if decode(temp_cor.name, temp_fec.name, temp_out.name) != 0:
        raise Exception("FAILED: failed to correct maximum expected errors")

    if compare(temp_img.name, temp_out.name) != 0:
        raise Exception("FAILED: corrected file not identical")
    else:
        print "corrected content matches original"

    corrupt(temp_cor.name, 0, blocksize)

    if decode(temp_cor.name, temp_fec.name, temp_out.name) == 0:
        raise Exception("FAILED: corrected more than maximum number of errors?")

    print "PASSED"

if __name__ == '__main__':
    main(sys.argv[1:])
