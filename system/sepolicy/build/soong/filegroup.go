// Copyright 2018 Google Inc. All rights reserved.
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

package selinux

import (
	"android/soong/android"
	"path/filepath"
)

func init() {
	android.RegisterModuleType("se_filegroup", FileGroupFactory)
}

func FileGroupFactory() android.Module {
	module := &fileGroup{}
	module.AddProperties(&module.properties)
	android.InitAndroidModule(module)
	return module
}

type fileGroupProperties struct {
	// list of source file suffixes used to collect selinux policy files.
	// Source files will be looked up in the following local directories:
	// system/sepolicy/{public, private, vendor, reqd_mask}
	// and directories specified by following config variables:
	// BOARD_SEPOLICY_DIRS, BOARD_ODM_SEPOLICY_DIRS
	// BOARD_PLAT_PUBLIC_SEPOLICY_DIR, BOARD_PLAT_PRIVATE_SEPOLICY_DIR
	Srcs []string
}

type fileGroup struct {
	android.ModuleBase
	properties fileGroupProperties

	systemPublicSrcs   android.Paths
	systemPrivateSrcs  android.Paths
	systemVendorSrcs   android.Paths
	systemReqdMaskSrcs android.Paths

	systemExtPublicSrcs  android.Paths
	systemExtPrivateSrcs android.Paths

	vendorSrcs android.Paths
	odmSrcs    android.Paths
}

// Source files from system/sepolicy/public
func (fg *fileGroup) SystemPublicSrcs() android.Paths {
	return fg.systemPublicSrcs
}

// Source files from system/sepolicy/private
func (fg *fileGroup) SystemPrivateSrcs() android.Paths {
	return fg.systemPrivateSrcs
}

// Source files from system/sepolicy/vendor
func (fg *fileGroup) SystemVendorSrcs() android.Paths {
	return fg.systemVendorSrcs
}

// Source files from system/sepolicy/reqd_mask
func (fg *fileGroup) SystemReqdMaskSrcs() android.Paths {
	return fg.systemReqdMaskSrcs
}

// Source files from BOARD_PLAT_PUBLIC_SEPOLICY_DIR
func (fg *fileGroup) SystemExtPublicSrcs() android.Paths {
	return fg.systemExtPublicSrcs
}

// Source files from BOARD_PLAT_PRIVATE_SEPOLICY_DIR
func (fg *fileGroup) SystemExtPrivateSrcs() android.Paths {
	return fg.systemExtPrivateSrcs
}

// Source files from BOARD_SEPOLICY_DIRS
func (fg *fileGroup) VendorSrcs() android.Paths {
	return fg.vendorSrcs
}

// Source files from BOARD_ODM_SEPOLICY_DIRS
func (fg *fileGroup) OdmSrcs() android.Paths {
	return fg.odmSrcs
}

func (fg *fileGroup) findSrcsInDirs(ctx android.ModuleContext, dirs []string) android.Paths {
	result := android.Paths{}
	for _, f := range fg.properties.Srcs {
		for _, d := range dirs {
			path := filepath.Join(d, f)
			files, _ := ctx.GlobWithDeps(path, nil)
			for _, f := range files {
				result = append(result, android.PathForSource(ctx, f))
			}
		}
	}
	return result
}

func (fg *fileGroup) findSrcsInDir(ctx android.ModuleContext, dir string) android.Paths {
	return fg.findSrcsInDirs(ctx, []string{dir})
}

func (fg *fileGroup) DepsMutator(ctx android.BottomUpMutatorContext) {}

func (fg *fileGroup) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	fg.systemPublicSrcs = fg.findSrcsInDir(ctx, filepath.Join(ctx.ModuleDir(), "public"))
	fg.systemPrivateSrcs = fg.findSrcsInDir(ctx, filepath.Join(ctx.ModuleDir(), "private"))
	fg.systemVendorSrcs = fg.findSrcsInDir(ctx, filepath.Join(ctx.ModuleDir(), "vendor"))
	fg.systemReqdMaskSrcs = fg.findSrcsInDir(ctx, filepath.Join(ctx.ModuleDir(), "reqd_mask"))

	fg.systemExtPublicSrcs = fg.findSrcsInDirs(ctx, ctx.DeviceConfig().PlatPublicSepolicyDirs())
	fg.systemExtPrivateSrcs = fg.findSrcsInDirs(ctx, ctx.DeviceConfig().PlatPrivateSepolicyDirs())

	fg.vendorSrcs = fg.findSrcsInDirs(ctx, ctx.DeviceConfig().VendorSepolicyDirs())
	fg.odmSrcs = fg.findSrcsInDirs(ctx, ctx.DeviceConfig().OdmSepolicyDirs())
}
