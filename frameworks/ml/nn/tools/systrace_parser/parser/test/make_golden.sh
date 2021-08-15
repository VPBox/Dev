#!/bin/bash

set -e
cd $(dirname $(realpath $0))
../../parse_systrace.py hdrnet.html > hdrnet.txt
../../parse_systrace.py cpu.html > cpu.txt
../../parse_systrace.py unittest.html > unittest.txt
../../parse_systrace.py --per-execution hdrnet.html > hdrnet-bm.txt
