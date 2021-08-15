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

import json
import apex_manifest_pb2
from google.protobuf.json_format import Parse
from google.protobuf.json_format import ParseError

class ApexManifestError(Exception):
	def __init__(self, errmessage):
		# Apex Manifest parse error (extra fields) or if required fields not present
		self.errmessage = errmessage

def ValidateApexManifest(manifest_raw):
	try:
		manifest_json = json.loads(manifest_raw)
		manifest_pb = Parse(json.dumps(manifest_json), apex_manifest_pb2.ApexManifest())
	except (ParseError, ValueError) as err:
		raise ApexManifestError(err)
	# Checking required fields
	if manifest_pb.name == "":
		raise ApexManifestError("'name' field is required.")
	if manifest_pb.version == 0:
		raise ApexManifestError("'version' field is required.")
	return manifest_pb
