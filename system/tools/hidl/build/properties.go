// Copyright (C) 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package hidl

type nameProperties struct {
	Name *string
}

type fileGroupProperties struct {
	Name  *string
	Owner *string
	Srcs  []string
}

type ccProperties struct {
	Name                      *string
	Owner                     *string
	Defaults                  []string
	Vendor_available          *bool
	Recovery_available        *bool
	Generated_sources         []string
	Generated_headers         []string
	Group_static_libs         *bool
	Shared_libs               []string
	Static_libs               []string
	Export_shared_lib_headers []string
	Export_static_lib_headers []string
	Export_generated_headers  []string
	Double_loadable           *bool
	Cflags                    []string
}

type javaProperties struct {
	Name              *string
	Owner             *string
	Defaults          []string
	No_framework_libs *bool
	Installable       *bool
	Sdk_version       *string
	Srcs              []string
	Libs              []string
	Static_libs       []string
}
