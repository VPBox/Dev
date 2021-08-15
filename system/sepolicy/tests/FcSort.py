#!/usr/bin/env python
import sys
import os

class FileContextsNode:
    path = None
    fileType = None
    context = None
    Type = None
    meta = None
    stemLen = None
    strLen = None
    Type = None
    def __init__(self, path, fileType, context, meta, stemLen, strLen):
        self.path = path
        self.fileType = fileType
        self.context = context
        self.meta = meta
        self.stemLen = stemLen
        self.strlen = strLen
        self.Type = context.split(":")[2]

metaChars = frozenset(['.', '^', '$', '?', '*', '+', '|', '[', '(', '{'])
escapedMetaChars = frozenset(['\.', '\^', '\$', '\?', '\*', '\+', '\|', '\[', '\(', '\{'])

def getStemLen(path):
    global metaChars
    stemLen = 0
    i = 0
    while i < len(path):
        if path[i] == "\\":
            i += 1
        elif path[i] in metaChars:
            break
        stemLen += 1
        i += 1
    return stemLen


def getIsMeta(path):
    global metaChars
    global escapedMetaChars
    metaCharsCount = 0
    escapedMetaCharsCount = 0
    for c in metaChars:
        if c in path:
            metaCharsCount += 1
    for c in escapedMetaChars:
        if c in path:
            escapedMetaCharsCount += 1
    return metaCharsCount > escapedMetaCharsCount

def CreateNode(line):
    global metaChars
    if (len(line) == 0) or (line[0] == '#'):
        return None

    split = line.split()
    path = split[0].strip()
    context = split[-1].strip()
    fileType = None
    if len(split) == 3:
        fileType = split[1].strip()
    meta = getIsMeta(path)
    stemLen = getStemLen(path)
    strLen = len(path.replace("\\", ""))

    return FileContextsNode(path, fileType, context, meta, stemLen, strLen)

def ReadFileContexts(files):
    fc = []
    for f in files:
        fd = open(f)
        for line in fd:
            node = CreateNode(line.strip())
            if node != None:
                fc.append(node)
    return fc

# Comparator function for list.sort() based off of fc_sort.c
# Compares two FileContextNodes a and b and returns 1 if a is more
# specific or -1 if b is more specific.
def compare(a, b):
    # The regex without metachars is more specific
    if a.meta and not b.meta:
        return -1
    if b.meta and not a.meta:
        return 1

    # The regex with longer stemlen (regex before any meta characters) is more specific.
    if a.stemLen < b.stemLen:
        return -1
    if b.stemLen < a.stemLen:
        return 1

    # The regex with longer string length is more specific
    if a.strLen < b.strLen:
        return -1
    if b.strLen < a.strLen:
        return 1

    # A regex with a fileType defined (e.g. file, dir) is more specific.
    if a.fileType is None and b.fileType is not None:
        return -1
    if b.fileType is None and a.fileType is not None:
        return 1

    # Regexes are equally specific.
    return 0

def FcSort(files):
    for f in files:
        if not os.path.exists(f):
            sys.exit("Error: File_contexts file " + f + " does not exist\n")

    Fc = ReadFileContexts(files)
    Fc.sort(cmp=compare)

    return Fc

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("Usage: fc_sort.py <file_contexts 1> <file_contexts 2> <file_contexts 3>")

    FcSorted = FcSort(sys.argv[1:])
