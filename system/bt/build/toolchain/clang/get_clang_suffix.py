import os
import subprocess
import re
import sys

def which(cmd):
  for p in os.environ["PATH"].split(os.pathsep):
    clang_path = os.path.join(p, cmd)
    if os.path.exists(clang_path):
      return clang_path
  return None

CLANG_VERSION_REGEX=".*version\s*([0-9]*\.[0-9]*)\.*"
clang_path = which("clang++")
clang_version_major = 0
clang_version_minor = 0

if clang_path:
  clang_version_out = subprocess.Popen([clang_path, "--version"],
    stdout=subprocess.PIPE).communicate()[0]
  clang_version_match = re.search(CLANG_VERSION_REGEX, clang_version_out)
  clang_version_str = clang_version_match.group(1)
  clang_version_array = clang_version_str.split('.')
  clang_version_major = int(clang_version_array[0])
  clang_version_minor = int(clang_version_array[1])

if clang_version_major >= 3 and clang_version_minor >= 5:
  print ""
else:
  # Loop in support clang version only
  clang_version_major = 3
  clang_version_minor = 9
  while clang_version_major >= 3 and clang_version_minor >= 5:
    clang_version_str = "%d.%d" % (clang_version_major, clang_version_minor)
    clang_path = which("clang++-" + clang_version_str)
    if clang_path:
      print clang_version_str
      sys.exit(0)
    clang_version_minor -= 1
    if clang_version_minor < 0:
      clang_version_minor = 9
      clang_version_major -= 1
  print "None"
