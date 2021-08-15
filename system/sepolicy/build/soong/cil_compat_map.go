// Copyright (C) 2018 The Android Open Source Project
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

// This file contains "se_cil_compat_map" module type used to build and install
// sepolicy backwards compatibility mapping files.

import (
	"android/soong/android"
	"fmt"
	"io"

	"github.com/google/blueprint"
	"github.com/google/blueprint/proptools"
)

var (
	pctx = android.NewPackageContext("android/soong/selinux")

	combine_maps    = pctx.HostBinToolVariable("combine_maps", "combine_maps")
	combineMapsCmd  = "${combine_maps} -t ${topHalf} -b ${bottomHalf} -o $out"
	combineMapsRule = pctx.StaticRule(
		"combineMapsRule",
		blueprint.RuleParams{
			Command:     combineMapsCmd,
			CommandDeps: []string{"${combine_maps}"},
		},
		"topHalf",
		"bottomHalf",
	)

	String        = proptools.String
	TopHalfDepTag = dependencyTag{name: "top"}
)

func init() {
	android.RegisterModuleType("se_cil_compat_map", cilCompatMapFactory)
	pctx.Import("android/soong/android")
}

func cilCompatMapFactory() android.Module {
	c := &cilCompatMap{}
	c.AddProperties(&c.properties)
	android.InitAndroidModule(c)
	return c
}

type cilCompatMapProperties struct {
	// se_cil_compat_map module representing a compatibility mapping file for
	// platform versions (x->y). Bottom half represents a mapping (y->z).
	// Together the halves are used to generate a (x->z) mapping.
	Top_half *string
	// list of source (.cil) files used to build an the bottom half of sepolicy
	// compatibility mapping file. bottom_half may reference the outputs of
	// other modules that produce source files like genrule or filegroup using
	// the syntax ":module". srcs has to be non-empty.
	Bottom_half []string
}

type cilCompatMap struct {
	android.ModuleBase
	properties cilCompatMapProperties
	// (.intermediate) module output path as installation source.
	installSource android.Path
}

type CilCompatMapGenerator interface {
	GeneratedMapFile() android.Path
}

type dependencyTag struct {
	blueprint.BaseDependencyTag
	name string
}

func expandTopHalf(ctx android.ModuleContext) android.OptionalPath {
	var topHalf android.OptionalPath
	ctx.VisitDirectDeps(func(dep android.Module) {
		depTag := ctx.OtherModuleDependencyTag(dep)
		switch depTag {
		case TopHalfDepTag:
			topHalf = android.OptionalPathForPath(dep.(CilCompatMapGenerator).GeneratedMapFile())
		}
	})
	return topHalf
}

func expandSeSources(ctx android.ModuleContext, srcFiles []string) android.Paths {
	expandedSrcFiles := make(android.Paths, 0, len(srcFiles))
	for _, s := range srcFiles {
		if m := android.SrcIsModule(s); m != "" {
			module := ctx.GetDirectDepWithTag(m, android.SourceDepTag)
			if module == nil {
				// Error will have been handled by ExtractSourcesDeps
				continue
			}
			if fg, ok := module.(*fileGroup); ok {
				// Core compatibility mapping files are under system/sepolicy/private.
				expandedSrcFiles = append(expandedSrcFiles, fg.SystemPrivateSrcs()...)
				// Partner extensions to the compatibility mapping in must be located in
				// BOARD_PLAT_PRIVATE_SEPOLICY_DIR
				expandedSrcFiles = append(expandedSrcFiles, fg.SystemExtPrivateSrcs()...)
			} else {
				ctx.ModuleErrorf("srcs dependency %q is not an selinux filegroup", m)
			}
		} else {
			p := android.PathForModuleSrc(ctx, s)
			expandedSrcFiles = append(expandedSrcFiles, p)
		}
	}
	return expandedSrcFiles
}

func (c *cilCompatMap) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	srcFiles := expandSeSources(ctx, c.properties.Bottom_half)

	for _, src := range srcFiles {
		if src.Ext() != ".cil" {
			ctx.PropertyErrorf("bottom_half", "%s has to be a .cil file.", src.String())
		}
	}

	bottomHalf := android.PathForModuleGen(ctx, "bottom_half")
	ctx.Build(pctx, android.BuildParams{
		Rule:   android.Cat,
		Output: bottomHalf,
		Inputs: srcFiles,
	})

	topHalf := expandTopHalf(ctx)
	if topHalf.Valid() {
		out := android.PathForModuleGen(ctx, c.Name())
		ctx.ModuleBuild(pctx, android.ModuleBuildParams{
			Rule:   combineMapsRule,
			Output: out,
			Implicits: []android.Path{
				topHalf.Path(),
				bottomHalf,
			},
			Args: map[string]string{
				"topHalf":    topHalf.String(),
				"bottomHalf": bottomHalf.String(),
			},
		})
		c.installSource = out
	} else {
		c.installSource = bottomHalf
	}
}

func (c *cilCompatMap) DepsMutator(ctx android.BottomUpMutatorContext) {
	android.ExtractSourcesDeps(ctx, c.properties.Bottom_half)
	if c.properties.Top_half != nil {
		ctx.AddDependency(c, TopHalfDepTag, String(c.properties.Top_half))
	}
}

func (c *cilCompatMap) AndroidMk() android.AndroidMkData {
	ret := android.AndroidMkData{
		OutputFile: android.OptionalPathForPath(c.installSource),
		Class:      "ETC",
	}
	ret.Extra = append(ret.Extra, func(w io.Writer, outputFile android.Path) {
		fmt.Fprintln(w, "LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux/mapping")
	})
	return ret
}

var _ CilCompatMapGenerator = (*cilCompatMap)(nil)

func (c *cilCompatMap) GeneratedMapFile() android.Path {
	return c.installSource
}
