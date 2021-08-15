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

import (
	"errors"
	"path/filepath"
	"regexp"
	"strings"
)

type fqName struct {
	packageComponents []string
	minor             string
	major             string
}

// See system/tools/hidl/FQName.h
var component = "[a-zA-Z_][a-zA-Z_0-9]*"
var pkg = "(" + component + "(?:[.]" + component + ")*)"
var digits = "([0-9]+)"
var re_package = regexp.MustCompile("^" + pkg + "@" + digits + "." + digits + "$")

func parseFqName(f string) (*fqName, error) {
	matches := re_package.FindAllStringSubmatch(f, 3)

	if matches == nil {
		return nil, errors.New("Poorly formed hal interface name: '" + f + "' must match '" + re_package.String() + "'")
	}

	ret := fqName{
		packageComponents: strings.Split(matches[0][1], "."),
		major:             matches[0][2],
		minor:             matches[0][3],
	}

	return &ret, nil
}

func (f *fqName) inPackage(pkg string) bool {
	components := strings.Split(pkg, ".")

	if len(components) > len(f.packageComponents) {
		return false
	}

	for i, v := range components {
		if f.packageComponents[i] != v {
			return false
		}
	}

	return true
}

func (f *fqName) pkg() string {
	return strings.Join(f.packageComponents, ".")
}
func (f *fqName) version() string {
	return f.major + "." + f.minor
}
func (f *fqName) sanitizedVersion() string {
	return "V" + f.major + "_" + f.minor
}
func (f *fqName) string() string {
	return f.pkg() + "@" + f.version()
}
func (f *fqName) sanitizedString() string {
	return f.pkg() + "-V" + f.version() // NOT sanitizedVersion
}

func (f *fqName) dir() string {
	return filepath.Join(filepath.Join(f.packageComponents...), f.version()) + "/"
}
func (f *fqName) sanitizedDir() string {
	return filepath.Join(filepath.Join(f.packageComponents...), f.sanitizedVersion()) + "/"
}

func (f *fqName) fileGroupName() string {
	return f.string() + "_hal"
}
func (f *fqName) sourcesName() string {
	return f.string() + "_genc++"
}
func (f *fqName) headersName() string {
	return f.string() + "_genc++_headers"
}
func (f *fqName) javaName() string {
	return f.sanitizedString() + "-java"
}
func (f *fqName) javaSharedName() string {
	return f.sanitizedString() + "-java-shallow"
}
func (f *fqName) javaSourcesName() string {
	return f.sanitizedString() + "-java_gen_java"
}
func (f *fqName) javaConstantsName() string {
	return f.sanitizedString() + "-java-constants"
}
func (f *fqName) javaConstantsSourcesName() string {
	return f.sanitizedString() + "-java-constants_gen_java"
}
func (f *fqName) adapterName() string {
	return f.string() + "-adapter"
}
func (f *fqName) adapterSourcesName() string {
	return f.string() + "-adapter_genc++"
}
func (f *fqName) adapterHelperName() string {
	return f.string() + "-adapter-helper"
}
func (f *fqName) adapterHelperSourcesName() string {
	return f.string() + "-adapter-helper_genc++"
}
func (f *fqName) adapterHelperHeadersName() string {
	return f.string() + "-adapter-helper_genc++_headers"
}

func (f *fqName) vtsSpecName() string {
	return f.string() + "-vts.spec"
}
func (f *fqName) vtsDriverSourcesName() string {
	return f.string() + "-vts.driver_genc++"
}
func (f *fqName) vtsDriverHeadersName() string {
	return f.string() + "-vts.driver_genc++_headers"
}
func (f *fqName) vtsDriverName() string {
	return f.string() + "-vts.driver"
}
func (f *fqName) vtsProfilerSourcesName() string {
	return f.string() + "-vts.profiler_genc++"
}
func (f *fqName) vtsProfilerHeadersName() string {
	return f.string() + "-vts.profiler_genc++_headers"
}
func (f *fqName) vtsProfilerName() string {
	return f.string() + "-vts.profiler"
}
