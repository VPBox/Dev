// Copyright 2016 The Android Open Source Project
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
package fluoride

import (
	"strings"

	"android/soong/android"
	"android/soong/cc"
)

func init() {
	android.RegisterModuleType("fluoride_defaults", fluorideDefaultsFactory)
}

func fluorideDefaultsFactory() android.Module {
	module := cc.DefaultsFactory()
	android.AddLoadHook(module, fluorideDefaults)

	return module
}

func fluorideDefaults(ctx android.LoadHookContext) {
	type props struct {
		Include_dirs []string
		Cflags       []string
	}

	p := &props{}
	p.Cflags, p.Include_dirs = globalDefaults(ctx)

	ctx.AppendProperties(p)
}

func globalDefaults(ctx android.BaseContext) ([]string, []string) {
	var cflags []string
	var includeDirs []string

	board_bt_buildcfg_include_dir := ctx.DeviceConfig().BtConfigIncludeDir()
	if len(board_bt_buildcfg_include_dir) > 0 {
		cflags = append(cflags, "-DHAS_BDROID_BUILDCFG")
		board_bt_buildcfg_include_dir_list :=
			strings.Fields(board_bt_buildcfg_include_dir)
		for _, buildcfg_dir := range board_bt_buildcfg_include_dir_list {
			includeDirs = append(includeDirs, buildcfg_dir)
		}
	} else {
		cflags = append(cflags, "-DHAS_NO_BDROID_BUILDCFG")
	}

	return cflags, includeDirs
}
