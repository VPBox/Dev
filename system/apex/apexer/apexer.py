#!/usr/bin/env python
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

"""
apexer is a command line tool for creating an APEX file, a package format
for system components.

Typical usage: apexer input_dir output.apex

"""

import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tempfile
import uuid
import xml.etree.ElementTree as ET
from apex_manifest import ValidateApexManifest
from apex_manifest import ApexManifestError

tool_path_list = None
BLOCK_SIZE = 4096

def ParseArgs(argv):
  parser = argparse.ArgumentParser(description='Create an APEX file')
  parser.add_argument('-f', '--force', action='store_true',
                      help='force overwriting output')
  parser.add_argument('-v', '--verbose', action='store_true',
                      help='verbose execution')
  parser.add_argument('--manifest', default='apex_manifest.json',
                      help='path to the APEX manifest file')
  parser.add_argument('--android_manifest',
                      help='path to the AndroidManifest file. If omitted, a default one is created and used')
  parser.add_argument('--assets_dir',
                      help='an assets directory to be included in the APEX')
  parser.add_argument('--file_contexts',
                      help='selinux file contexts file. Required for "image" APEXs.')
  parser.add_argument('--canned_fs_config',
                      help='canned_fs_config specifies uid/gid/mode of files. Required for ' +
                           '"image" APEXS.')
  parser.add_argument('--key',
                      help='path to the private key file. Required for "image" APEXs.')
  parser.add_argument('--pubkey',
                      help='path to the public key file. Used to bundle the public key in APEX for testing.')
  parser.add_argument('input_dir', metavar='INPUT_DIR',
                      help='the directory having files to be packaged')
  parser.add_argument('output', metavar='OUTPUT',
                      help='name of the APEX file')
  parser.add_argument('--payload_type', metavar='TYPE', required=False, default="image",
                      choices=["zip", "image"],
                      help='type of APEX payload being built "zip" or "image"')
  parser.add_argument('--override_apk_package_name', required=False,
                      help='package name of the APK container. Default is the apex name in --manifest.')
  parser.add_argument('--android_jar_path', required=False,
                      default="prebuilts/sdk/current/public/android.jar",
                      help='path to use as the source of the android API.')
  apexer_path_in_environ = "APEXER_TOOL_PATH" in os.environ
  parser.add_argument('--apexer_tool_path', required=not apexer_path_in_environ,
                      default=os.environ['APEXER_TOOL_PATH'].split(":") if apexer_path_in_environ else None,
                      type=lambda s: s.split(":"),
                      help="""A list of directories containing all the tools used by apexer (e.g.
                              mke2fs, avbtool, etc.) separated by ':'. Can also be set using the
                              APEXER_TOOL_PATH environment variable""")
  parser.add_argument('--target_sdk_version', required=False,
                      help='Default target SDK version to use for AndroidManifest.xml')
  return parser.parse_args(argv)

def FindBinaryPath(binary):
  for path in tool_path_list:
    binary_path = os.path.join(path, binary)
    if os.path.exists(binary_path):
      return binary_path
  raise Exception("Failed to find binary " + binary + " in path " + ":".join(tool_path_list))

def RunCommand(cmd, verbose=False, env=None):
  env = env or {}
  env.update(os.environ.copy())

  cmd[0] = FindBinaryPath(cmd[0])

  if verbose:
    print("Running: " + " ".join(cmd))
  p = subprocess.Popen(
      cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env)
  output, _ = p.communicate()

  if verbose or p.returncode is not 0:
    print(output.rstrip())

  assert p.returncode is 0, "Failed to execute: " + " ".join(cmd)

  return (output, p.returncode)

def GetDirSize(dir_name):
  size = 0
  for dirpath, _, filenames in os.walk(dir_name):
    size += RoundUp(os.path.getsize(dirpath), BLOCK_SIZE)
    for f in filenames:
      size += RoundUp(os.path.getsize(os.path.join(dirpath, f)), BLOCK_SIZE)
  return size

def GetFilesAndDirsCount(dir_name):
  count = 0;
  for root, dirs, files in os.walk(dir_name):
    count += (len(dirs) + len(files))
  return count

def RoundUp(size, unit):
  assert unit & (unit - 1) == 0
  return (size + unit - 1) & (~(unit - 1))

def PrepareAndroidManifest(package, version):
  template = """\
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
  package="{package}" android:versionCode="{version}">
  <!-- APEX does not have classes.dex -->
  <application android:hasCode="false" />
</manifest>
"""
  return template.format(package=package, version=version)

def ValidateAndroidManifest(package, android_manifest):
  tree = ET.parse(android_manifest)
  manifest_tag = tree.getroot()
  package_in_xml = manifest_tag.attrib['package']
  if package_in_xml != package:
    raise Exception("Package name '" + package_in_xml + "' in '" + android_manifest +
                    " differ from package name '" + package + "' in the apex_manifest.json")

def ValidateArgs(args):
  if not os.path.exists(args.manifest):
    print("Manifest file '" + args.manifest + "' does not exist")
    return False

  if not os.path.isfile(args.manifest):
    print("Manifest file '" + args.manifest + "' is not a file")
    return False

  if args.android_manifest is not None:
    if not os.path.exists(args.android_manifest):
      print("Android Manifest file '" + args.android_manifest + "' does not exist")
      return False

    if not os.path.isfile(args.android_manifest):
      print("Android Manifest file '" + args.android_manifest + "' is not a file")
      return False

  if not os.path.exists(args.input_dir):
    print("Input directory '" + args.input_dir + "' does not exist")
    return False

  if not os.path.isdir(args.input_dir):
    print("Input directory '" + args.input_dir + "' is not a directory")
    return False

  if not args.force and os.path.exists(args.output):
    print(args.output + ' already exists. Use --force to overwrite.')
    return False

  if args.payload_type == "image":
    if not args.key:
      print("Missing --key {keyfile} argument!")
      return False

    if not args.file_contexts:
      print("Missing --file_contexts {contexts} argument!")
      return False

    if not args.canned_fs_config:
      print("Missing --canned_fs_config {config} argument!")
      return False

  return True

def CreateApex(args, work_dir):
  if not ValidateArgs(args):
    return False

  if args.verbose:
    print "Using tools from " + str(tool_path_list)

  try:
    with open(args.manifest, "r") as f:
      manifest_raw = f.read()
      manifest_apex = ValidateApexManifest(manifest_raw)
  except ApexManifestError as err:
    print("'" + args.manifest + "' is not a valid manifest file")
    print err.errmessage
    return False
  except IOError:
    print("Cannot read manifest file: '" + args.manifest + "'")
    return False

  # create an empty ext4 image that is sufficiently big
  # sufficiently big = size + 16MB margin
  size_in_mb = (GetDirSize(args.input_dir) / (1024*1024)) + 16

  content_dir = os.path.join(work_dir, 'content')
  os.mkdir(content_dir)

  # APEX manifest is also included in the image. The manifest is included
  # twice: once inside the image and once outside the image (but still
  # within the zip container).
  manifests_dir = os.path.join(work_dir, 'manifests')
  os.mkdir(manifests_dir)
  manifest_file = os.path.join(manifests_dir, 'apex_manifest.json')
  if args.verbose:
    print('Copying ' + args.manifest + ' to ' + manifest_file)
  shutil.copyfile(args.manifest, manifest_file)

  if args.payload_type == 'image':
    key_name = os.path.basename(os.path.splitext(args.key)[0])

    if manifest_apex.name != key_name:
      print("package name '" + manifest_apex.name + "' does not match with key name '" + key_name + "'")
      return False
    img_file = os.path.join(content_dir, 'apex_payload.img')

    # margin is for files that are not under args.input_dir. this consists of
    # one inode for apex_manifest.json and 11 reserved inodes for ext4.
    # TOBO(b/122991714) eliminate these details. use build_image.py which
    # determines the optimal inode count by first building an image and then
    # count the inodes actually used.
    inode_num_margin = 12
    inode_num = GetFilesAndDirsCount(args.input_dir) + inode_num_margin

    cmd = ['mke2fs']
    cmd.extend(['-O', '^has_journal']) # because image is read-only
    cmd.extend(['-b', str(BLOCK_SIZE)])
    cmd.extend(['-m', '0']) # reserved block percentage
    cmd.extend(['-t', 'ext4'])
    cmd.extend(['-I', '256']) # inode size
    cmd.extend(['-N', str(inode_num)])
    uu = str(uuid.uuid5(uuid.NAMESPACE_URL, "www.android.com"))
    cmd.extend(['-U', uu])
    cmd.extend(['-E', 'hash_seed=' + uu])
    cmd.append(img_file)
    cmd.append(str(size_in_mb) + 'M')
    RunCommand(cmd, args.verbose, {"E2FSPROGS_FAKE_TIME": "1"})

    # Compile the file context into the binary form
    compiled_file_contexts = os.path.join(work_dir, 'file_contexts.bin')
    cmd = ['sefcontext_compile']
    cmd.extend(['-o', compiled_file_contexts])
    cmd.append(args.file_contexts)
    RunCommand(cmd, args.verbose)

    # Add files to the image file
    cmd = ['e2fsdroid']
    cmd.append('-e') # input is not android_sparse_file
    cmd.extend(['-f', args.input_dir])
    cmd.extend(['-T', '0']) # time is set to epoch
    cmd.extend(['-S', compiled_file_contexts])
    cmd.extend(['-C', args.canned_fs_config])
    cmd.append('-s') # share dup blocks
    cmd.append(img_file)
    RunCommand(cmd, args.verbose, {"E2FSPROGS_FAKE_TIME": "1"})

    cmd = ['e2fsdroid']
    cmd.append('-e') # input is not android_sparse_file
    cmd.extend(['-f', manifests_dir])
    cmd.extend(['-T', '0']) # time is set to epoch
    cmd.extend(['-S', compiled_file_contexts])
    cmd.extend(['-C', args.canned_fs_config])
    cmd.append('-s') # share dup blocks
    cmd.append(img_file)
    RunCommand(cmd, args.verbose, {"E2FSPROGS_FAKE_TIME": "1"})

    # Resize the image file to save space
    cmd = ['resize2fs']
    cmd.append('-M') # shrink as small as possible
    cmd.append(img_file)
    RunCommand(cmd, args.verbose, {"E2FSPROGS_FAKE_TIME": "1"})


    cmd = ['avbtool']
    cmd.append('add_hashtree_footer')
    cmd.append('--do_not_generate_fec')
    cmd.extend(['--algorithm', 'SHA256_RSA4096'])
    cmd.extend(['--key', args.key])
    cmd.extend(['--prop', "apex.key:" + key_name])
    # Set up the salt based on manifest content which includes name
    # and version
    salt = hashlib.sha256(manifest_raw).hexdigest()
    cmd.extend(['--salt', salt])
    cmd.extend(['--image', img_file])
    RunCommand(cmd, args.verbose)

    # Get the minimum size of the partition required.
    # TODO(b/113320014) eliminate this step
    info, _ = RunCommand(['avbtool', 'info_image', '--image', img_file], args.verbose)
    vbmeta_offset = int(re.search('VBMeta\ offset:\ *([0-9]+)', info).group(1))
    vbmeta_size = int(re.search('VBMeta\ size:\ *([0-9]+)', info).group(1))
    partition_size = RoundUp(vbmeta_offset + vbmeta_size, BLOCK_SIZE) + BLOCK_SIZE

    # Resize to the minimum size
    # TODO(b/113320014) eliminate this step
    cmd = ['avbtool']
    cmd.append('resize_image')
    cmd.extend(['--image', img_file])
    cmd.extend(['--partition_size', str(partition_size)])
    RunCommand(cmd, args.verbose)
  else:
    img_file = os.path.join(content_dir, 'apex_payload.zip')
    cmd = ['soong_zip']
    cmd.extend(['-o', img_file])
    cmd.extend(['-C', args.input_dir])
    cmd.extend(['-D', args.input_dir])
    cmd.extend(['-C', manifests_dir])
    cmd.extend(['-D', manifests_dir])
    RunCommand(cmd, args.verbose)

  # package the image file and APEX manifest as an APK.
  # The AndroidManifest file is automatically generated if not given.
  android_manifest_file = os.path.join(work_dir, 'AndroidManifest.xml')
  if not args.android_manifest:
    if args.verbose:
      print('Creating AndroidManifest ' + android_manifest_file)
    with open(android_manifest_file, 'w+') as f:
      app_package_name = manifest_apex.name
      f.write(PrepareAndroidManifest(app_package_name, manifest_apex.version))
  else:
    ValidateAndroidManifest(manifest_apex.name, args.android_manifest)
    shutil.copyfile(args.android_manifest, android_manifest_file)

  # copy manifest to the content dir so that it is also accessible
  # without mounting the image
  shutil.copyfile(args.manifest, os.path.join(content_dir, 'apex_manifest.json'))

  # copy the public key, if specified
  if args.pubkey:
    shutil.copyfile(args.pubkey, os.path.join(content_dir, "apex_pubkey"))

  apk_file = os.path.join(work_dir, 'apex.apk')
  cmd = ['aapt2']
  cmd.append('link')
  cmd.extend(['--manifest', android_manifest_file])
  if args.override_apk_package_name:
    cmd.extend(['--rename-manifest-package', args.override_apk_package_name])
  # This version from apex_manifest.json is used when versionCode isn't
  # specified in AndroidManifest.xml
  cmd.extend(['--version-code', str(manifest_apex.version)])
  if manifest_apex.versionName:
    cmd.extend(['--version-name', manifest_apex.versionName])
  if args.target_sdk_version:
    cmd.extend(['--target-sdk-version', args.target_sdk_version])
  if args.assets_dir:
    cmd.extend(['-A', args.assets_dir])
  # Default value for minSdkVersion.
  cmd.extend(['--min-sdk-version', '29'])
  cmd.extend(['-o', apk_file])
  cmd.extend(['-I', args.android_jar_path])
  RunCommand(cmd, args.verbose)

  zip_file = os.path.join(work_dir, 'apex.zip')
  cmd = ['soong_zip']
  cmd.append('-d') # include directories
  cmd.extend(['-C', content_dir]) # relative root
  cmd.extend(['-D', content_dir]) # input dir
  for file_ in os.listdir(content_dir):
    if os.path.isfile(os.path.join(content_dir, file_)):
      cmd.extend(['-s', file_]) # don't compress any files
  cmd.extend(['-o', zip_file])
  RunCommand(cmd, args.verbose)

  unaligned_apex_file = os.path.join(work_dir, 'unaligned.apex')
  cmd = ['merge_zips']
  cmd.append('-j') # sort
  cmd.append(unaligned_apex_file) # output
  cmd.append(apk_file) # input
  cmd.append(zip_file) # input
  RunCommand(cmd, args.verbose)

  # Align the files at page boundary for efficient access
  cmd = ['zipalign']
  cmd.append('-f')
  cmd.append(str(BLOCK_SIZE))
  cmd.append(unaligned_apex_file)
  cmd.append(args.output)
  RunCommand(cmd, args.verbose)

  if (args.verbose):
    print('Created ' + args.output)

  return True


class TempDirectory(object):
  def __enter__(self):
    self.name = tempfile.mkdtemp()
    return self.name

  def __exit__(self, *unused):
    shutil.rmtree(self.name)


def main(argv):
  global tool_path_list
  args = ParseArgs(argv)
  tool_path_list = args.apexer_tool_path
  with TempDirectory() as work_dir:
    success = CreateApex(args, work_dir)

  if not success:
    sys.exit(1)


if __name__ == '__main__':
  main(sys.argv[1:])
