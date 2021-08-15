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
	"sync"

	"github.com/google/blueprint/proptools"

	"android/soong/android"
)

func init() {
	android.RegisterModuleType("hidl_package_root", hidlPackageRootFactory)
}

type hidlPackageRoot struct {
	android.ModuleBase

	properties struct {
		// Path to the package root from android build root. It is recommended not to set this and
		// use the current path. This will be deprecated in the future.
		Path *string

		// True to require a current.txt API file here.
		//
		// When false, it uses the file only when it exists.
		Use_current *bool
	}

	currentPath android.OptionalPath
}

func (r *hidlPackageRoot) getFullPackageRoot() string {
	return "-r" + r.Name() + ":" + *r.properties.Path
}

func (r *hidlPackageRoot) getCurrentPath() android.OptionalPath {
	return r.currentPath
}

func (r *hidlPackageRoot) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	if r.properties.Path == nil {
		r.properties.Path = proptools.StringPtr(ctx.ModuleDir())
	}

	if proptools.BoolDefault(r.properties.Use_current, false) {
		r.currentPath = android.OptionalPathForPath(android.PathForModuleSrc(ctx, "current.txt"))
	} else {
		r.currentPath = android.ExistentPathForSource(ctx, ctx.ModuleDir(), "current.txt")
	}
}
func (r *hidlPackageRoot) DepsMutator(ctx android.BottomUpMutatorContext) {
}

var packageRootsMutex sync.Mutex
var packageRoots []*hidlPackageRoot

func hidlPackageRootFactory() android.Module {
	r := &hidlPackageRoot{}
	r.AddProperties(&r.properties)
	android.InitAndroidModule(r)

	packageRootsMutex.Lock()
	packageRoots = append(packageRoots, r)
	packageRootsMutex.Unlock()

	return r
}

func lookupPackageRoot(name string) *hidlPackageRoot {
	for _, i := range packageRoots {
		if i.ModuleBase.Name() == name {
			return i
		}
	}
	return nil
}
