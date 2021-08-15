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
	"fmt"
	"sort"
	"strings"
	"sync"

	"github.com/google/blueprint"
	"github.com/google/blueprint/proptools"

	"android/soong/android"
	"android/soong/cc"
	"android/soong/genrule"
	"android/soong/java"
)

var (
	hidlInterfaceSuffix = "_interface"

	pctx = android.NewPackageContext("android/hidl")

	hidl      = pctx.HostBinToolVariable("hidl", "hidl-gen")
	vtsc      = pctx.HostBinToolVariable("vtsc", "vtsc")
	soong_zip = pctx.HostBinToolVariable("soong_zip", "soong_zip")

	hidlRule = pctx.StaticRule("hidlRule", blueprint.RuleParams{
		Depfile:     "${depfile}",
		Deps:        blueprint.DepsGCC,
		Command:     "rm -rf ${genDir} && ${hidl} -R -p . -d ${depfile} -o ${genDir} -L ${language} ${roots} ${fqName}",
		CommandDeps: []string{"${hidl}"},
		Description: "HIDL ${language}: ${in} => ${out}",
	}, "depfile", "fqName", "genDir", "language", "roots")

	hidlSrcJarRule = pctx.StaticRule("hidlSrcJarRule", blueprint.RuleParams{
		Depfile: "${depfile}",
		Deps:    blueprint.DepsGCC,
		Command: "rm -rf ${genDir} && " +
			"${hidl} -R -p . -d ${depfile} -o ${genDir}/srcs -L ${language} ${roots} ${fqName} && " +
			"${soong_zip} -o ${genDir}/srcs.srcjar -C ${genDir}/srcs -D ${genDir}/srcs",
		CommandDeps: []string{"${hidl}", "${soong_zip}"},
		Description: "HIDL ${language}: ${in} => srcs.srcjar",
	}, "depfile", "fqName", "genDir", "language", "roots")

	vtsRule = pctx.StaticRule("vtsRule", blueprint.RuleParams{
		Command:     "rm -rf ${genDir} && ${vtsc} -m${mode} -t${type} ${inputDir}/${packagePath} ${genDir}/${packagePath}",
		CommandDeps: []string{"${vtsc}"},
		Description: "VTS ${mode} ${type}: ${in} => ${out}",
	}, "mode", "type", "inputDir", "genDir", "packagePath")
)

func init() {
	android.RegisterModuleType("hidl_interface", hidlInterfaceFactory)
	android.RegisterMakeVarsProvider(pctx, makeVarsProvider)
}

type hidlGenProperties struct {
	Language   string
	FqName     string
	Root       string
	Interfaces []string
	Inputs     []string
	Outputs    []string
}

type hidlGenRule struct {
	android.ModuleBase

	properties hidlGenProperties

	genOutputDir android.Path
	genInputs    android.Paths
	genOutputs   android.WritablePaths
}

var _ android.SourceFileProducer = (*hidlGenRule)(nil)
var _ genrule.SourceFileGenerator = (*hidlGenRule)(nil)

func (g *hidlGenRule) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	g.genOutputDir = android.PathForModuleGen(ctx)

	for _, input := range g.properties.Inputs {
		g.genInputs = append(g.genInputs, android.PathForModuleSrc(ctx, input))
	}

	for _, output := range g.properties.Outputs {
		g.genOutputs = append(g.genOutputs, android.PathForModuleGen(ctx, output))
	}

	if g.properties.Language == "vts" && isVtsSpecPackage(ctx.ModuleName()) {
		vtsList := vtsList(ctx.AConfig())
		vtsListMutex.Lock()
		*vtsList = append(*vtsList, g.genOutputs.Paths()...)
		vtsListMutex.Unlock()
	}

	var fullRootOptions []string
	var currentPath android.OptionalPath
	ctx.VisitDirectDeps(func(dep android.Module) {
		switch t := dep.(type) {
		case *hidlInterface:
			fullRootOptions = append(fullRootOptions, t.properties.Full_root_option)
		case *hidlPackageRoot:
			if currentPath.Valid() {
				panic(fmt.Sprintf("Expecting only one path, but found %v %v", currentPath, t.getCurrentPath()))
			}

			currentPath = t.getCurrentPath()
		default:
			panic(fmt.Sprintf("Unrecognized hidlGenProperties dependency: %T", t))
		}
	})

	fullRootOptions = android.FirstUniqueStrings(fullRootOptions)

	inputs := g.genInputs
	if currentPath.Valid() {
		inputs = append(inputs, currentPath.Path())
	}

	rule := hidlRule
	if g.properties.Language == "java" {
		rule = hidlSrcJarRule
	}

	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:            rule,
		Inputs:          inputs,
		Output:          g.genOutputs[0],
		ImplicitOutputs: g.genOutputs[1:],
		Args: map[string]string{
			"depfile":  g.genOutputs[0].String() + ".d",
			"genDir":   g.genOutputDir.String(),
			"fqName":   g.properties.FqName,
			"language": g.properties.Language,
			"roots":    strings.Join(fullRootOptions, " "),
		},
	})
}

func (g *hidlGenRule) GeneratedSourceFiles() android.Paths {
	return g.genOutputs.Paths()
}

func (g *hidlGenRule) Srcs() android.Paths {
	return g.genOutputs.Paths()
}

func (g *hidlGenRule) GeneratedDeps() android.Paths {
	return g.genOutputs.Paths()
}

func (g *hidlGenRule) GeneratedHeaderDirs() android.Paths {
	return android.Paths{g.genOutputDir}
}

func (g *hidlGenRule) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, g.properties.FqName+hidlInterfaceSuffix)
	ctx.AddDependency(ctx.Module(), nil, wrap("", g.properties.Interfaces, hidlInterfaceSuffix)...)
	ctx.AddDependency(ctx.Module(), nil, g.properties.Root)
}

func hidlGenFactory() android.Module {
	g := &hidlGenRule{}
	g.AddProperties(&g.properties)
	android.InitAndroidModule(g)
	return g
}

type vtscProperties struct {
	Mode        string
	Type        string
	SpecName    string // e.g. foo-vts.spec
	Outputs     []string
	PackagePath string // e.g. android/hardware/foo/1.0/
}

type vtscRule struct {
	android.ModuleBase

	properties vtscProperties

	genOutputDir android.Path
	genInputDir  android.Path
	genInputs    android.Paths
	genOutputs   android.WritablePaths
}

var _ android.SourceFileProducer = (*vtscRule)(nil)
var _ genrule.SourceFileGenerator = (*vtscRule)(nil)

func (g *vtscRule) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	g.genOutputDir = android.PathForModuleGen(ctx)

	ctx.VisitDirectDeps(func(dep android.Module) {
		if specs, ok := dep.(*hidlGenRule); ok {
			g.genInputDir = specs.genOutputDir
			g.genInputs = specs.genOutputs.Paths()
		}
	})

	for _, output := range g.properties.Outputs {
		g.genOutputs = append(g.genOutputs, android.PathForModuleGen(ctx, output))
	}

	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:    vtsRule,
		Inputs:  g.genInputs,
		Outputs: g.genOutputs,
		Args: map[string]string{
			"mode":        g.properties.Mode,
			"type":        g.properties.Type,
			"inputDir":    g.genInputDir.String(),
			"genDir":      g.genOutputDir.String(),
			"packagePath": g.properties.PackagePath,
		},
	})
}

func (g *vtscRule) GeneratedSourceFiles() android.Paths {
	return g.genOutputs.Paths()
}

func (g *vtscRule) Srcs() android.Paths {
	return g.genOutputs.Paths()
}

func (g *vtscRule) GeneratedDeps() android.Paths {
	return g.genOutputs.Paths()
}

func (g *vtscRule) GeneratedHeaderDirs() android.Paths {
	return android.Paths{g.genOutputDir}
}

func (g *vtscRule) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, g.properties.SpecName)
}

func vtscFactory() android.Module {
	g := &vtscRule{}
	g.AddProperties(&g.properties)
	android.InitAndroidModule(g)
	return g
}

type hidlInterfaceProperties struct {
	// Vndk properties for interface library only.
	cc.VndkProperties

	// List of .hal files which compose this interface.
	Srcs []string

	// List of hal interface packages that this library depends on.
	Interfaces []string

	// Package root for this package, must be a prefix of name
	Root string

	// Unused/deprecated: List of non-TypeDef types declared in types.hal.
	Types []string

	// Whether to generate the Java library stubs.
	// Default: true
	Gen_java *bool

	// Whether to generate a Java library containing constants
	// expressed by @export annotations in the hal files.
	Gen_java_constants bool

	// Whether to generate VTS-related testing libraries.
	Gen_vts *bool

	// example: -randroid.hardware:hardware/interfaces
	Full_root_option string `blueprint:"mutated"`
}

// TODO(b/119771576): These properties are shared by all Android modules, and we are specifically
// calling these out to be copied to every create module. However, if a new property is added, it
// could break things because this code has no way to know about that.
type manuallyInheritCommonProperties struct {
	Enabled          *bool
	Compile_multilib *string
	Target           struct {
		Host struct {
			Compile_multilib *string
		}
		Android struct {
			Compile_multilib *string
		}
	}
	Proprietary               *bool
	Owner                     *string
	Vendor                    *bool
	Soc_specific              *bool
	Device_specific           *bool
	Product_specific          *bool
	Product_services_specific *bool
	Recovery                  *bool
	Init_rc                   []string
	Vintf_fragments           []string
	Required                  []string
	Notice                    *string
	Dist                      struct {
		Targets []string
		Dest    *string
		Dir     *string
		Suffix  *string
	}
}

type hidlInterface struct {
	android.ModuleBase

	properties              hidlInterfaceProperties
	inheritCommonProperties manuallyInheritCommonProperties
}

func processSources(mctx android.LoadHookContext, srcs []string) ([]string, []string, bool) {
	var interfaces []string
	var types []string // hidl-gen only supports types.hal, but don't assume that here

	hasError := false

	for _, v := range srcs {
		if !strings.HasSuffix(v, ".hal") {
			mctx.PropertyErrorf("srcs", "Source must be a .hal file: "+v)
			hasError = true
			continue
		}

		name := strings.TrimSuffix(v, ".hal")

		if strings.HasPrefix(name, "I") {
			baseName := strings.TrimPrefix(name, "I")
			interfaces = append(interfaces, baseName)
		} else {
			types = append(types, name)
		}
	}

	return interfaces, types, !hasError
}

func processDependencies(mctx android.LoadHookContext, interfaces []string) ([]string, []string, bool) {
	var dependencies []string
	var javaDependencies []string

	hasError := false

	for _, v := range interfaces {
		name, err := parseFqName(v)
		if err != nil {
			mctx.PropertyErrorf("interfaces", err.Error())
			hasError = true
			continue
		}
		dependencies = append(dependencies, name.string())
		javaDependencies = append(javaDependencies, name.javaName())
	}

	return dependencies, javaDependencies, !hasError
}

func removeCoreDependencies(mctx android.LoadHookContext, dependencies []string) []string {
	var ret []string

	for _, i := range dependencies {
		if !isCorePackage(i) {
			ret = append(ret, i)
		}
	}

	return ret
}

func hidlInterfaceMutator(mctx android.LoadHookContext, i *hidlInterface) {
	name, err := parseFqName(i.ModuleBase.Name())
	if err != nil {
		mctx.PropertyErrorf("name", err.Error())
	}

	if !name.inPackage(i.properties.Root) {
		mctx.PropertyErrorf("root", i.properties.Root+" must be a prefix of  "+name.string()+".")
	}
	if lookupPackageRoot(i.properties.Root) == nil {
		mctx.PropertyErrorf("interfaces", `Cannot find package root specification for package `+
			`root '%s' needed for module '%s'. Either this is a mispelling of the package `+
			`root, or a new hidl_package_root module needs to be added. For example, you can `+
			`fix this error by adding the following to <some path>/Android.bp:

hidl_package_root {
name: "%s",
path: "<some path>",
}

This corresponds to the "-r%s:<some path>" option that would be passed into hidl-gen.`,
			i.properties.Root, name, i.properties.Root, i.properties.Root)
	}

	interfaces, types, _ := processSources(mctx, i.properties.Srcs)

	if len(interfaces) == 0 && len(types) == 0 {
		mctx.PropertyErrorf("srcs", "No sources provided.")
	}

	dependencies, javaDependencies, _ := processDependencies(mctx, i.properties.Interfaces)
	cppDependencies := removeCoreDependencies(mctx, dependencies)

	if mctx.Failed() {
		return
	}

	shouldGenerateLibrary := !isCorePackage(name.string())
	// explicitly true if not specified to give early warning to devs
	shouldGenerateJava := proptools.BoolDefault(i.properties.Gen_java, true)
	shouldGenerateJavaConstants := i.properties.Gen_java_constants
	shouldGenerateVts := shouldGenerateLibrary && proptools.BoolDefault(i.properties.Gen_vts, true)

	var libraryIfExists []string
	if shouldGenerateLibrary {
		libraryIfExists = []string{name.string()}
	}

	// TODO(b/69002743): remove filegroups
	mctx.CreateModule(android.ModuleFactoryAdaptor(android.FileGroupFactory), &fileGroupProperties{
		Name: proptools.StringPtr(name.fileGroupName()),
		Srcs: i.properties.Srcs,
	}, &i.inheritCommonProperties)

	mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(name.sourcesName()),
	}, &hidlGenProperties{
		Language:   "c++-sources",
		FqName:     name.string(),
		Root:       i.properties.Root,
		Interfaces: i.properties.Interfaces,
		Inputs:     i.properties.Srcs,
		Outputs:    concat(wrap(name.dir(), interfaces, "All.cpp"), wrap(name.dir(), types, ".cpp")),
	}, &i.inheritCommonProperties)
	mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(name.headersName()),
	}, &hidlGenProperties{
		Language:   "c++-headers",
		FqName:     name.string(),
		Root:       i.properties.Root,
		Interfaces: i.properties.Interfaces,
		Inputs:     i.properties.Srcs,
		Outputs: concat(wrap(name.dir()+"I", interfaces, ".h"),
			wrap(name.dir()+"Bs", interfaces, ".h"),
			wrap(name.dir()+"BnHw", interfaces, ".h"),
			wrap(name.dir()+"BpHw", interfaces, ".h"),
			wrap(name.dir()+"IHw", interfaces, ".h"),
			wrap(name.dir(), types, ".h"),
			wrap(name.dir()+"hw", types, ".h")),
	}, &i.inheritCommonProperties)

	if shouldGenerateLibrary {
		mctx.CreateModule(android.ModuleFactoryAdaptor(cc.LibraryFactory), &ccProperties{
			Name:               proptools.StringPtr(name.string()),
			Recovery_available: proptools.BoolPtr(true),
			Vendor_available:   proptools.BoolPtr(true),
			Double_loadable:    proptools.BoolPtr(isDoubleLoadable(name.string())),
			Defaults:           []string{"hidl-module-defaults"},
			Generated_sources:  []string{name.sourcesName()},
			Generated_headers:  []string{name.headersName()},
			Shared_libs: concat(cppDependencies, []string{
				"libhidlbase",
				"libhidltransport",
				"libhwbinder",
				"liblog",
				"libutils",
				"libcutils",
			}),
			Export_shared_lib_headers: concat(cppDependencies, []string{
				"libhidlbase",
				"libhidltransport",
				"libhwbinder",
				"libutils",
			}),
			Export_generated_headers: []string{name.headersName()},
		}, &i.properties.VndkProperties, &i.inheritCommonProperties)
	}

	if shouldGenerateJava {
		mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
			Name: proptools.StringPtr(name.javaSourcesName()),
		}, &hidlGenProperties{
			Language:   "java",
			FqName:     name.string(),
			Root:       i.properties.Root,
			Interfaces: i.properties.Interfaces,
			Inputs:     i.properties.Srcs,
			Outputs:    []string{"srcs.srcjar"},
		}, &i.inheritCommonProperties)

		commonJavaProperties := javaProperties{
			Defaults:          []string{"hidl-java-module-defaults"},
			No_framework_libs: proptools.BoolPtr(true),
			Installable:       proptools.BoolPtr(true),
			Srcs:              []string{":" + name.javaSourcesName()},

			// This should ideally be system_current, but android.hidl.base-V1.0-java is used
			// to build framework, which is used to build system_current.  Use core_current
			// plus hwbinder.stubs, which together form a subset of system_current that does
			// not depend on framework.
			Sdk_version: proptools.StringPtr("core_current"),
			Libs:        []string{"hwbinder.stubs"},
		}

		mctx.CreateModule(android.ModuleFactoryAdaptor(java.LibraryFactory), &javaProperties{
			Name:        proptools.StringPtr(name.javaName()),
			Static_libs: javaDependencies,
		}, &i.inheritCommonProperties, &commonJavaProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(java.LibraryFactory), &javaProperties{
			Name: proptools.StringPtr(name.javaSharedName()),
			Libs: javaDependencies,
		}, &i.inheritCommonProperties, &commonJavaProperties)
	}

	if shouldGenerateJavaConstants {
		mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
			Name: proptools.StringPtr(name.javaConstantsSourcesName()),
		}, &hidlGenProperties{
			Language:   "java-constants",
			FqName:     name.string(),
			Root:       i.properties.Root,
			Interfaces: i.properties.Interfaces,
			Inputs:     i.properties.Srcs,
			Outputs:    []string{name.sanitizedDir() + "Constants.java"},
		}, &i.inheritCommonProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(java.LibraryFactory), &javaProperties{
			Name:              proptools.StringPtr(name.javaConstantsName()),
			Defaults:          []string{"hidl-java-module-defaults"},
			No_framework_libs: proptools.BoolPtr(true),
			Srcs:              []string{":" + name.javaConstantsSourcesName()},
		}, &i.inheritCommonProperties)
	}

	mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(name.adapterHelperSourcesName()),
	}, &hidlGenProperties{
		Language:   "c++-adapter-sources",
		FqName:     name.string(),
		Root:       i.properties.Root,
		Interfaces: i.properties.Interfaces,
		Inputs:     i.properties.Srcs,
		Outputs:    wrap(name.dir()+"A", concat(interfaces, types), ".cpp"),
	}, &i.inheritCommonProperties)
	mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(name.adapterHelperHeadersName()),
	}, &hidlGenProperties{
		Language:   "c++-adapter-headers",
		FqName:     name.string(),
		Root:       i.properties.Root,
		Interfaces: i.properties.Interfaces,
		Inputs:     i.properties.Srcs,
		Outputs:    wrap(name.dir()+"A", concat(interfaces, types), ".h"),
	}, &i.inheritCommonProperties)

	mctx.CreateModule(android.ModuleFactoryAdaptor(cc.LibraryFactory), &ccProperties{
		Name:              proptools.StringPtr(name.adapterHelperName()),
		Vendor_available:  proptools.BoolPtr(true),
		Defaults:          []string{"hidl-module-defaults"},
		Generated_sources: []string{name.adapterHelperSourcesName()},
		Generated_headers: []string{name.adapterHelperHeadersName()},
		Shared_libs: []string{
			"libbase",
			"libcutils",
			"libhidlbase",
			"libhidltransport",
			"libhwbinder",
			"liblog",
			"libutils",
		},
		Static_libs: concat([]string{
			"libhidladapter",
		}, wrap("", dependencies, "-adapter-helper"), cppDependencies, libraryIfExists),
		Export_shared_lib_headers: []string{
			"libhidlbase",
			"libhidltransport",
		},
		Export_static_lib_headers: concat([]string{
			"libhidladapter",
		}, wrap("", dependencies, "-adapter-helper"), cppDependencies, libraryIfExists),
		Export_generated_headers: []string{name.adapterHelperHeadersName()},
		Group_static_libs:        proptools.BoolPtr(true),
	}, &i.inheritCommonProperties)
	mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(name.adapterSourcesName()),
	}, &hidlGenProperties{
		Language:   "c++-adapter-main",
		FqName:     name.string(),
		Root:       i.properties.Root,
		Interfaces: i.properties.Interfaces,
		Inputs:     i.properties.Srcs,
		Outputs:    []string{"main.cpp"},
	}, &i.inheritCommonProperties)
	mctx.CreateModule(android.ModuleFactoryAdaptor(cc.TestFactory), &ccProperties{
		Name:              proptools.StringPtr(name.adapterName()),
		Generated_sources: []string{name.adapterSourcesName()},
		Shared_libs: []string{
			"libbase",
			"libcutils",
			"libhidlbase",
			"libhidltransport",
			"libhwbinder",
			"liblog",
			"libutils",
		},
		Static_libs: concat([]string{
			"libhidladapter",
			name.adapterHelperName(),
		}, wrap("", dependencies, "-adapter-helper"), cppDependencies, libraryIfExists),
		Group_static_libs: proptools.BoolPtr(true),
	}, &i.inheritCommonProperties)

	if shouldGenerateVts {
		vtsSpecs := concat(wrap(name.dir(), interfaces, ".vts"), wrap(name.dir(), types, ".vts"))

		mctx.CreateModule(android.ModuleFactoryAdaptor(hidlGenFactory), &nameProperties{
			Name: proptools.StringPtr(name.vtsSpecName()),
		}, &hidlGenProperties{
			Language:   "vts",
			FqName:     name.string(),
			Root:       i.properties.Root,
			Interfaces: i.properties.Interfaces,
			Inputs:     i.properties.Srcs,
			Outputs:    vtsSpecs,
		}, &i.inheritCommonProperties)

		mctx.CreateModule(android.ModuleFactoryAdaptor(vtscFactory), &nameProperties{
			Name: proptools.StringPtr(name.vtsDriverSourcesName()),
		}, &vtscProperties{
			Mode:        "DRIVER",
			Type:        "SOURCE",
			SpecName:    name.vtsSpecName(),
			Outputs:     wrap("", vtsSpecs, ".cpp"),
			PackagePath: name.dir(),
		}, &i.inheritCommonProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(vtscFactory), &nameProperties{
			Name: proptools.StringPtr(name.vtsDriverHeadersName()),
		}, &vtscProperties{
			Mode:        "DRIVER",
			Type:        "HEADER",
			SpecName:    name.vtsSpecName(),
			Outputs:     wrap("", vtsSpecs, ".h"),
			PackagePath: name.dir(),
		}, &i.inheritCommonProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(cc.LibraryFactory), &ccProperties{
			Name:                      proptools.StringPtr(name.vtsDriverName()),
			Defaults:                  []string{"VtsHalDriverDefaults"},
			Generated_sources:         []string{name.vtsDriverSourcesName()},
			Generated_headers:         []string{name.vtsDriverHeadersName()},
			Export_generated_headers:  []string{name.vtsDriverHeadersName()},
			Shared_libs:               wrap("", cppDependencies, "-vts.driver"),
			Export_shared_lib_headers: wrap("", cppDependencies, "-vts.driver"),
			Static_libs:               concat(cppDependencies, libraryIfExists),

			// TODO(b/126244142)
			Cflags: []string{"-Wno-unused-variable"},
		}, &i.inheritCommonProperties)

		mctx.CreateModule(android.ModuleFactoryAdaptor(vtscFactory), &nameProperties{
			Name: proptools.StringPtr(name.vtsProfilerSourcesName()),
		}, &vtscProperties{
			Mode:        "PROFILER",
			Type:        "SOURCE",
			SpecName:    name.vtsSpecName(),
			Outputs:     wrap("", vtsSpecs, ".cpp"),
			PackagePath: name.dir(),
		}, &i.inheritCommonProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(vtscFactory), &nameProperties{
			Name: proptools.StringPtr(name.vtsProfilerHeadersName()),
		}, &vtscProperties{
			Mode:        "PROFILER",
			Type:        "HEADER",
			SpecName:    name.vtsSpecName(),
			Outputs:     wrap("", vtsSpecs, ".h"),
			PackagePath: name.dir(),
		}, &i.inheritCommonProperties)
		mctx.CreateModule(android.ModuleFactoryAdaptor(cc.LibraryFactory), &ccProperties{
			Name:                      proptools.StringPtr(name.vtsProfilerName()),
			Defaults:                  []string{"VtsHalProfilerDefaults"},
			Generated_sources:         []string{name.vtsProfilerSourcesName()},
			Generated_headers:         []string{name.vtsProfilerHeadersName()},
			Export_generated_headers:  []string{name.vtsProfilerHeadersName()},
			Shared_libs:               wrap("", cppDependencies, "-vts.profiler"),
			Export_shared_lib_headers: wrap("", cppDependencies, "-vts.profiler"),
			Static_libs:               concat(cppDependencies, libraryIfExists),

			// TODO(b/126244142)
			Cflags: []string{"-Wno-unused-variable"},
		}, &i.inheritCommonProperties)
	}
}

func (h *hidlInterface) Name() string {
	return h.ModuleBase.Name() + hidlInterfaceSuffix
}
func (h *hidlInterface) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	visited := false
	ctx.VisitDirectDeps(func(dep android.Module) {
		if visited {
			panic("internal error, multiple dependencies found but only one added")
		}
		visited = true
		h.properties.Full_root_option = dep.(*hidlPackageRoot).getFullPackageRoot()
	})
	if !visited {
		panic("internal error, no dependencies found but dependency added")
	}

}
func (h *hidlInterface) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, h.properties.Root)
}

func hidlInterfaceFactory() android.Module {
	i := &hidlInterface{}
	i.AddProperties(&i.properties)
	i.AddProperties(&i.inheritCommonProperties)
	android.InitAndroidModule(i)
	android.AddLoadHook(i, func(ctx android.LoadHookContext) { hidlInterfaceMutator(ctx, i) })

	return i
}

var doubleLoadablePackageNames = []string{
	"android.frameworks.bufferhub@1.0",
	"android.hardware.cas@1.0",
	"android.hardware.cas.native@1.0",
	"android.hardware.configstore@",
	"android.hardware.drm@1.0",
	"android.hardware.drm@1.1",
	"android.hardware.drm@1.2",
	"android.hardware.graphics.allocator@",
	"android.hardware.graphics.bufferqueue@",
	"android.hardware.media@",
	"android.hardware.media.omx@",
	"android.hardware.memtrack@1.0",
	"android.hardware.neuralnetworks@",
	"android.hidl.allocator@",
	"android.hidl.token@",
	"android.system.suspend@1.0",
}

func isDoubleLoadable(name string) bool {
	for _, pkgname := range doubleLoadablePackageNames {
		if strings.HasPrefix(name, pkgname) {
			return true
		}
	}
	return false
}

// packages in libhidltransport
var coreDependencyPackageNames = []string{
	"android.hidl.base@",
	"android.hidl.manager@",
}

func isCorePackage(name string) bool {
	for _, pkgname := range coreDependencyPackageNames {
		if strings.HasPrefix(name, pkgname) {
			return true
		}
	}
	return false
}

// TODO(b/126383715): centralize this logic/support filtering in core VTS build
var coreVtsSpecs = []string{
	"android.frameworks.",
	"android.hardware.",
	"android.hidl.",
	"android.system.",
}

func isVtsSpecPackage(name string) bool {
	for _, pkgname := range coreVtsSpecs {
		if strings.HasPrefix(name, pkgname) {
			return true
		}
	}
	return false
}

var vtsListKey = android.NewOnceKey("vtsList")

func vtsList(config android.Config) *android.Paths {
	return config.Once(vtsListKey, func() interface{} {
		return &android.Paths{}
	}).(*android.Paths)
}

var vtsListMutex sync.Mutex

func makeVarsProvider(ctx android.MakeVarsContext) {
	vtsList := vtsList(ctx.Config()).Strings()
	sort.Strings(vtsList)

	ctx.Strict("VTS_SPEC_FILE_LIST", strings.Join(vtsList, " "))
}
