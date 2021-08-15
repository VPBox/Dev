#!/usr/bin/python

import sys
import getopt

def usage():
    print "powersum.py [OPTIONS] HZ VOLTAGE [FILE]"
    print "OPTIONS: "
    print "-o OFFSET: subtract OFFSET from all data points"
    print "\nHZ: samples per second in FILE or stdin"
    sys.exit(0)

offset = 0.0
voltage = 4.3

parsedargv,argvrem = getopt.getopt(sys.argv[1:], "vo:w:l:h", ["help"])
for o,a in parsedargv:
    if o == '-o': offset = float(a)
    if o == '-h' or o == '--help': usage()

hz = float(argvrem[0])
voltage = float(argvrem[1])
if len(argvrem) > 1:
    f = open(argvrem[2], "r")
else:
    f = sys.stdin

totalpower = 0.0
samplectr = 0

for line in f:
    try:
        val = float(line.split(" ")[1]) # xxx take 2nd arg in line
        val -= offset
    except:
        print "Can't parse data line, did you remember the timestamp?"
        print "data was: %s" % line
        sys.exit(1)

    samplectr+=1
    totalpower += val/hz

avecurrent = totalpower * hz *1000 / samplectr
avepower = avecurrent * voltage

print "%.3f %.3f" % (avecurrent, avepower)
