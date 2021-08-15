from os.path import basename
import re
import sys

# A very limited parser whose job is to process the compatibility mapping
# files and retrieve type and attribute information until proper support is
# built into libsepol

# get the text in the next matching parens

class MiniCilParser:
    def __init__(self, policyFile):
        self.types = set() # types declared in mapping
        self.pubtypes = set()
        self.expandtypeattributes = {}
        self.typeattributes = set() # attributes declared in mapping
        self.typeattributesets = {} # sets defined in mapping
        self.rTypeattributesets = {} # reverse mapping of above sets
        self.apiLevel = None

        with open(policyFile, 'r') as infile:
            s = self._getNextStmt(infile)
            while s:
                self._parseStmt(s)
                s = self._getNextStmt(infile)
        fn = basename(policyFile)
        m = re.match(r"(\d+\.\d+).+\.cil", fn)
        if m:
            self.apiLevel = m.group(1)

    def unparse(self):
        def wrapParens(stmt):
            return "(" + stmt + ")"

        def joinWrapParens(entries):
            return wrapParens(" ".join(entries))

        result = ""
        for ty in sorted(self.types):
            result += joinWrapParens(["type", ty]) + "\n"

        for ta in sorted(self.typeattributes):
            result += joinWrapParens(["typeattribute", ta]) + "\n"

        for eta in sorted(self.expandtypeattributes.items(),
                          key=lambda x: x[0]):
            result += joinWrapParens(
                    ["expandtypeattribute", wrapParens(eta[0]), eta[1]]) + "\n"

        for tas in sorted(self.typeattributesets.items(), key=lambda x: x[0]):
            result += joinWrapParens(
                    ["typeattributeset", tas[0],
                     joinWrapParens(sorted(tas[1]))]) + "\n"

        return result

    def _getNextStmt(self, infile):
        parens = 0
        s = ""
        c = infile.read(1)
        # get to first statement
        while c and c != "(":
            c = infile.read(1)

        parens += 1
        c = infile.read(1)
        while c and parens != 0:
            s += c
            c = infile.read(1)
            if c == ';':
                # comment, get rid of rest of the line
                while c != '\n':
                    c = infile.read(1)
            elif c == '(':
                parens += 1
            elif c == ')':
                parens -= 1
        return s

    def _parseType(self, stmt):
        m = re.match(r"type\s+(.+)", stmt)
        self.types.add(m.group(1))
        return

    def _parseExpandtypeattribute(self, stmt):
        m = re.match(r"expandtypeattribute\s+\((.+)\)\s+(true|false)", stmt)
        self.expandtypeattributes[m.group(1)] = m.group(2)
        return

    def _parseTypeattribute(self, stmt):
        m = re.match(r"typeattribute\s+(.+)", stmt)
        self.typeattributes.add(m.group(1))
        return

    def _parseTypeattributeset(self, stmt):
        m = re.match(r"typeattributeset\s+(.+?)\s+\((.+?)\)", stmt, flags = re.M |re.S)
        ta = m.group(1)
        # this isn't proper expression parsing, but will do for our
        # current use
        tas = m.group(2).split()

        if self.typeattributesets.get(ta) is None:
            self.typeattributesets[ta] = set()
        self.typeattributesets[ta].update(set(tas))
        for t in tas:
            if self.rTypeattributesets.get(t) is None:
                self.rTypeattributesets[t] = set()
            self.rTypeattributesets[t].update([ta])

        # check to see if this typeattributeset is a versioned public type
        pub = re.match(r"(\w+)_\d+_\d+", ta)
        if pub is not None:
            self.pubtypes.add(pub.group(1))
        return

    def _parseStmt(self, stmt):
        if re.match(r"type\s+.+", stmt):
            self._parseType(stmt)
        elif re.match(r"typeattribute\s+.+", stmt):
            self._parseTypeattribute(stmt)
        elif re.match(r"typeattributeset\s+.+", stmt):
            self._parseTypeattributeset(stmt)
        elif re.match(r"expandtypeattribute\s+.+", stmt):
            self._parseExpandtypeattribute(stmt)
        return

if __name__ == '__main__':
    f = sys.argv[1]
    p = MiniCilParser(f)
