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

package aidl

import (
	"android/soong/android"
	"android/soong/cc"
	"android/soong/genrule"
	"android/soong/java"
	"android/soong/phony"
	"fmt"
	"io"
	"path/filepath"
	"strconv"
	"strings"
	"sync"

	"github.com/google/blueprint"
	"github.com/google/blueprint/pathtools"
	"github.com/google/blueprint/proptools"
)

var (
	aidlInterfaceSuffix = "_interface"
	aidlApiSuffix       = "-api"
	langCpp             = "cpp"
	langJava            = "java"
	langNdk             = "ndk"
	langNdkPlatform     = "ndk_platform"
	futureVersion       = "10000"

	pctx = android.NewPackageContext("android/aidl")

	aidlDirPrepareRule = pctx.StaticRule("aidlDirPrepareRule", blueprint.RuleParams{
		Command: `rm -rf "${outDir}" && mkdir -p "${outDir}" && ` +
			`touch ${out}`,
		Description: "create ${out}",
	}, "outDir")

	aidlCppRule = pctx.StaticRule("aidlCppRule", blueprint.RuleParams{
		Command: `mkdir -p "${headerDir}" && ` +
			`${aidlCmd} --lang=${lang} ${optionalFlags} --structured --ninja -d ${out}.d ` +
			`-h ${headerDir} -o ${outDir} ${imports} ${in}`,
		Depfile:     "${out}.d",
		Deps:        blueprint.DepsGCC,
		CommandDeps: []string{"${aidlCmd}"},
		Description: "AIDL ${lang} ${in}",
	}, "imports", "lang", "headerDir", "outDir", "optionalFlags")

	aidlJavaRule = pctx.StaticRule("aidlJavaRule", blueprint.RuleParams{
		Command: `${aidlCmd} --lang=java ${optionalFlags} --structured --ninja -d ${out}.d ` +
			`-o ${outDir} ${imports} ${in}`,
		Depfile:     "${out}.d",
		Deps:        blueprint.DepsGCC,
		CommandDeps: []string{"${aidlCmd}"},
		Description: "AIDL Java ${in}",
	}, "imports", "outDir", "optionalFlags")

	aidlDumpApiRule = pctx.StaticRule("aidlDumpApiRule", blueprint.RuleParams{
		Command: `rm -rf "${out}" && mkdir -p "${out}" && ` +
			`${aidlCmd} --dumpapi --structured ${imports} --out ${out} ${in}`,
		CommandDeps: []string{"${aidlCmd}"},
	}, "imports")

	aidlDumpMappingsRule = pctx.StaticRule("aidlDumpMappingsRule", blueprint.RuleParams{
		Command: `rm -rf "${outDir}" && mkdir -p "${outDir}" && ` +
			`${aidlCmd} --apimapping ${outDir}/intermediate.txt ${in} ${imports} && ` +
			`${aidlToJniCmd} ${outDir}/intermediate.txt ${out}`,
		CommandDeps: []string{"${aidlCmd}"},
	}, "imports", "outDir")

	aidlFreezeApiRule = pctx.AndroidStaticRule("aidlFreezeApiRule",
		blueprint.RuleParams{
			Command: `mkdir -p ${to} && rm -rf ${to}/* && ` +
				`${bpmodifyCmd} -w -m ${name} -parameter versions -a ${version} ${bp} && ` +
				`cp -rf ${in}/* ${to} && ` +
				`find ${to} -type f -exec bash -c ` +
				`"cat ${apiPreamble} {} > {}.temp; mv {}.temp {}" \; && ` +
				`touch ${out}`,
			CommandDeps: []string{"${bpmodifyCmd}"},
		}, "to", "name", "version", "bp", "apiPreamble")

	aidlCheckApiRule = pctx.StaticRule("aidlCheckApiRule", blueprint.RuleParams{
		Command: `(${aidlCmd} --checkapi ${old} ${new} && touch ${out}) || ` +
			`(cat ${messageFile} && exit 1)`,
		CommandDeps: []string{"${aidlCmd}"},
		Description: "AIDL CHECK API: ${new} against ${old}",
	}, "old", "new", "messageFile")

	aidlDiffApiRule = pctx.StaticRule("aidlDiffApiRule", blueprint.RuleParams{
		Command: `(diff -r -B -I '//.*' ${old} ${new} && touch ${out}) || ` +
			`(cat ${messageFile} && exit 1)`,
		Description: "Check equality of ${new} and ${old}",
	}, "old", "new", "messageFile")
)

func init() {
	pctx.HostBinToolVariable("aidlCmd", "aidl")
	pctx.HostBinToolVariable("bpmodifyCmd", "bpmodify")
	pctx.SourcePathVariable("aidlToJniCmd", "system/tools/aidl/build/aidl_to_jni.py")
	android.RegisterModuleType("aidl_interface", aidlInterfaceFactory)
	android.RegisterModuleType("aidl_mapping", aidlMappingFactory)
}

// wrap(p, a, s) = [p + v + s for v in a]
func wrap(prefix string, strs []string, suffix string) []string {
	ret := make([]string, len(strs))
	for i, v := range strs {
		ret[i] = prefix + v + suffix
	}
	return ret
}

// concat(a...) = sum((i for i in a), [])
func concat(sstrs ...[]string) []string {
	var ret []string
	for _, v := range sstrs {
		ret = append(ret, v...)
	}
	return ret
}

func isRelativePath(path string) bool {
	if path == "" {
		return true
	}
	return filepath.Clean(path) == path && path != ".." &&
		!strings.HasPrefix(path, "../") && !strings.HasPrefix(path, "/")
}

type aidlGenProperties struct {
	Srcs     []string
	AidlRoot string // base directory for the input aidl file
	Imports  []string
	Lang     string // target language [java|cpp|ndk]
	BaseName string
	GenLog   bool
	Version  string
}

type aidlGenRule struct {
	android.ModuleBase

	properties aidlGenProperties

	implicitInputs android.Paths
	importFlags    string

	genOutDir    android.ModuleGenPath
	genHeaderDir android.ModuleGenPath
	genOutputs   android.WritablePaths
}

var _ android.SourceFileProducer = (*aidlGenRule)(nil)
var _ genrule.SourceFileGenerator = (*aidlGenRule)(nil)

func (g *aidlGenRule) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	genDirTimestamp := android.PathForModuleGen(ctx, "timestamp")
	g.implicitInputs = append(g.implicitInputs, genDirTimestamp)

	var importPaths []string
	ctx.VisitDirectDeps(func(dep android.Module) {
		if importedAidl, ok := dep.(*aidlInterface); ok {
			importPaths = append(importPaths, importedAidl.properties.Full_import_paths...)
		} else if api, ok := dep.(*aidlApi); ok {
			// When compiling an AIDL interface, also make sure that each
			// version of the interface is compatible with its previous version
			for _, path := range api.checkApiTimestamps {
				g.implicitInputs = append(g.implicitInputs, path)
			}
		}
	})
	g.importFlags = strings.Join(wrap("-I", importPaths, ""), " ")

	srcs := android.PathsWithModuleSrcSubDir(ctx, android.PathsForModuleSrc(ctx, g.properties.Srcs), g.properties.AidlRoot)

	g.genOutDir = android.PathForModuleGen(ctx)
	g.genHeaderDir = android.PathForModuleGen(ctx, "include")
	for _, src := range srcs {
		g.genOutputs = append(g.genOutputs, g.generateBuildActionsForSingleAidl(ctx, src))
	}

	// This is to clean genOutDir before generating any file
	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:      aidlDirPrepareRule,
		Implicits: srcs,
		Output:    genDirTimestamp,
		Args: map[string]string{
			"outDir": g.genOutDir.String(),
		},
	})
}

func (g *aidlGenRule) generateBuildActionsForSingleAidl(ctx android.ModuleContext, src android.Path) android.WritablePath {
	var outFile android.WritablePath
	if g.properties.Lang == langJava {
		outFile = android.PathForModuleGen(ctx, pathtools.ReplaceExtension(src.Rel(), "java"))
	} else {
		outFile = android.PathForModuleGen(ctx, pathtools.ReplaceExtension(src.Rel(), "cpp"))
	}

	var optionalFlags []string
	if g.properties.Version != "" {
		optionalFlags = append(optionalFlags, "--version "+g.properties.Version)
	}

	if g.properties.Lang == langJava {
		ctx.ModuleBuild(pctx, android.ModuleBuildParams{
			Rule:      aidlJavaRule,
			Input:     src,
			Implicits: g.implicitInputs,
			Output:    outFile,
			Args: map[string]string{
				"imports":       g.importFlags,
				"outDir":        g.genOutDir.String(),
				"optionalFlags": strings.Join(optionalFlags, " "),
			},
		})
	} else {
		typeName := strings.TrimSuffix(filepath.Base(src.Rel()), ".aidl")
		packagePath := filepath.Dir(src.Rel())
		baseName := typeName
		// TODO(b/111362593): aidl_to_cpp_common.cpp uses heuristics to figure out if
		//   an interface name has a leading I. Those same heuristics have been
		//   moved here.
		if len(baseName) >= 2 && baseName[0] == 'I' &&
			strings.ToUpper(baseName)[1] == baseName[1] {
			baseName = strings.TrimPrefix(typeName, "I")
		}

		prefix := ""
		if g.properties.Lang == langNdk || g.properties.Lang == langNdkPlatform {
			prefix = "aidl"
		}

		var headers android.WritablePaths
		headers = append(headers, g.genHeaderDir.Join(ctx, prefix, packagePath,
			typeName+".h"))
		headers = append(headers, g.genHeaderDir.Join(ctx, prefix, packagePath,
			"Bp"+baseName+".h"))
		headers = append(headers, g.genHeaderDir.Join(ctx, prefix, packagePath,
			"Bn"+baseName+".h"))

		if g.properties.GenLog {
			optionalFlags = append(optionalFlags, "--log")
		}

		aidlLang := g.properties.Lang
		if aidlLang == langNdkPlatform {
			aidlLang = "ndk"
		}

		ctx.ModuleBuild(pctx, android.ModuleBuildParams{
			Rule:            aidlCppRule,
			Input:           src,
			Implicits:       g.implicitInputs,
			Output:          outFile,
			ImplicitOutputs: headers,
			Args: map[string]string{
				"imports":       g.importFlags,
				"lang":          aidlLang,
				"headerDir":     g.genHeaderDir.String(),
				"outDir":        g.genOutDir.String(),
				"optionalFlags": strings.Join(optionalFlags, " "),
			},
		})
	}

	return outFile
}

func (g *aidlGenRule) GeneratedSourceFiles() android.Paths {
	return g.genOutputs.Paths()
}

func (g *aidlGenRule) Srcs() android.Paths {
	return g.genOutputs.Paths()
}

func (g *aidlGenRule) GeneratedDeps() android.Paths {
	return g.genOutputs.Paths()
}

func (g *aidlGenRule) GeneratedHeaderDirs() android.Paths {
	return android.Paths{g.genHeaderDir}
}

func (g *aidlGenRule) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, wrap("", g.properties.Imports, aidlInterfaceSuffix)...)
	ctx.AddDependency(ctx.Module(), nil, g.properties.BaseName+aidlApiSuffix)
}

func aidlGenFactory() android.Module {
	g := &aidlGenRule{}
	g.AddProperties(&g.properties)
	android.InitAndroidModule(g)
	return g
}

type aidlApiProperties struct {
	BaseName string
	Inputs   []string
	Imports  []string
	Api_dir  *string
	Versions []string
	AidlRoot string // base directory for the input aidl file
}

type aidlApi struct {
	android.ModuleBase

	properties aidlApiProperties

	// for triggering api check for version X against version X-1
	checkApiTimestamps android.WritablePaths

	// for triggering freezing API as the new version
	freezeApiTimestamp android.WritablePath
}

func (m *aidlApi) apiDir() string {
	if m.properties.Api_dir != nil {
		return *(m.properties.Api_dir)
	} else {
		return "api"
	}
}

// Version of the interface at ToT if it is frozen
func (m *aidlApi) validateCurrentVersion(ctx android.ModuleContext) string {
	if len(m.properties.Versions) == 0 {
		return "1"
	} else {
		latestVersion := m.properties.Versions[len(m.properties.Versions)-1]

		i, err := strconv.ParseInt(latestVersion, 10, 64)
		if err != nil {
			ctx.PropertyErrorf("versions", "must be integers")
			return ""
		}

		return strconv.FormatInt(i+1, 10)
	}
}

func (m *aidlApi) createApiDumpFromSource(ctx android.ModuleContext) (apiDir android.WritablePath, apiFiles android.WritablePaths) {
	var importPaths []string
	ctx.VisitDirectDeps(func(dep android.Module) {
		if importedAidl, ok := dep.(*aidlInterface); ok {
			importPaths = append(importPaths, importedAidl.properties.Full_import_paths...)
		}
	})

	var srcs android.Paths
	for _, input := range m.properties.Inputs {
		path := android.PathForModuleSrc(ctx, input)
		path = android.PathWithModuleSrcSubDir(ctx, path, m.properties.AidlRoot)
		srcs = append(srcs, path)
	}

	apiDir = android.PathForModuleOut(ctx, "dump")
	for _, src := range srcs {
		apiFiles = append(apiFiles, android.PathForModuleOut(ctx, "dump", src.Rel()))
	}
	imports := strings.Join(wrap("-I", importPaths, ""), " ")
	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:            aidlDumpApiRule,
		Inputs:          srcs,
		Output:          apiDir,
		ImplicitOutputs: apiFiles,
		Args: map[string]string{
			"imports": imports,
		},
	})
	return apiDir, apiFiles
}

func (m *aidlApi) freezeApiDumpAsVersion(ctx android.ModuleContext, apiDumpDir android.Path, apiFiles android.Paths, version string) android.WritablePath {
	timestampFile := android.PathForModuleOut(ctx, "freezeapi_"+version+".timestamp")

	modulePath := android.PathForModuleSrc(ctx).String()

	var implicits android.Paths
	implicits = append(implicits, apiFiles...)

	apiPreamble := android.PathForSource(ctx, "system/tools/aidl/build/api_preamble.txt")
	implicits = append(implicits, apiPreamble)

	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:        aidlFreezeApiRule,
		Description: "Freezing AIDL API of " + m.properties.BaseName + " as version " + version,
		Implicits:   implicits,
		Output:      timestampFile,
		Args: map[string]string{
			"to":          filepath.Join(modulePath, m.apiDir(), version),
			"name":        m.properties.BaseName,
			"version":     version,
			"bp":          android.PathForModuleSrc(ctx, "Android.bp").String(),
			"apiPreamble": apiPreamble.String(),
		},
	})
	return timestampFile
}

func (m *aidlApi) checkCompatibility(ctx android.ModuleContext, oldApiDir android.Path, oldApiFiles android.Paths, newApiDir android.Path, newApiFiles android.Paths) android.WritablePath {
	newVersion := newApiDir.Base()
	timestampFile := android.PathForModuleOut(ctx, "checkapi_"+newVersion+".timestamp")
	messageFile := android.PathForSource(ctx, "system/tools/aidl/build/message_check_compatibility.txt")
	var implicits android.Paths
	implicits = append(implicits, oldApiFiles...)
	implicits = append(implicits, newApiFiles...)
	implicits = append(implicits, messageFile)
	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:      aidlCheckApiRule,
		Implicits: implicits,
		Output:    timestampFile,
		Args: map[string]string{
			"old":         oldApiDir.String(),
			"new":         newApiDir.String(),
			"messageFile": messageFile.String(),
		},
	})
	return timestampFile
}

func (m *aidlApi) checkEquality(ctx android.ModuleContext, oldApiDir android.Path, oldApiFiles android.Paths, newApiDir android.Path, newApiFiles android.Paths) android.WritablePath {
	newVersion := newApiDir.Base()
	timestampFile := android.PathForModuleOut(ctx, "checkapi_"+newVersion+".timestamp")
	messageFile := android.PathForSource(ctx, "system/tools/aidl/build/message_check_equality.txt")
	var implicits android.Paths
	implicits = append(implicits, oldApiFiles...)
	implicits = append(implicits, newApiFiles...)
	implicits = append(implicits, messageFile)
	ctx.ModuleBuild(pctx, android.ModuleBuildParams{
		Rule:      aidlDiffApiRule,
		Implicits: implicits,
		Output:    timestampFile,
		Args: map[string]string{
			"old":         oldApiDir.String(),
			"new":         newApiDir.String(),
			"messageFile": messageFile.String(),
		},
	})
	return timestampFile
}

func (m *aidlApi) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	currentVersion := m.validateCurrentVersion(ctx)

	if ctx.Failed() {
		return
	}

	currentDumpDir, currentApiFiles := m.createApiDumpFromSource(ctx)
	m.freezeApiTimestamp = m.freezeApiDumpAsVersion(ctx, currentDumpDir, currentApiFiles.Paths(), currentVersion)

	apiDirs := make(map[string]android.Path)
	apiFiles := make(map[string]android.Paths)
	for _, ver := range m.properties.Versions {
		apiDir := android.PathForModuleSrc(ctx, m.apiDir(), ver)
		apiDirs[ver] = apiDir
		apiFiles[ver] = ctx.Glob(filepath.Join(apiDir.String(), "**/*.aidl"), nil)
	}
	apiDirs[currentVersion] = currentDumpDir
	apiFiles[currentVersion] = currentApiFiles.Paths()

	// Check that version X is backward compatible with version X-1
	for i, newVersion := range m.properties.Versions {
		if i != 0 {
			oldVersion := m.properties.Versions[i-1]
			checkApiTimestamp := m.checkCompatibility(ctx, apiDirs[oldVersion], apiFiles[oldVersion], apiDirs[newVersion], apiFiles[newVersion])
			m.checkApiTimestamps = append(m.checkApiTimestamps, checkApiTimestamp)
		}
	}

	// ... and that the currentVersion (ToT) is backwards compatible with or
	// equal to the latest frozen version
	if len(m.properties.Versions) >= 1 {
		latestVersion := m.properties.Versions[len(m.properties.Versions)-1]
		var checkApiTimestamp android.WritablePath
		if ctx.Config().DefaultAppTargetSdkInt() != android.FutureApiLevel {
			// If API is frozen, don't allow any change to the API
			checkApiTimestamp = m.checkEquality(ctx, apiDirs[latestVersion], apiFiles[latestVersion], apiDirs[currentVersion], apiFiles[currentVersion])
		} else {
			// If not, allow backwards compatible changes to the API
			checkApiTimestamp = m.checkCompatibility(ctx, apiDirs[latestVersion], apiFiles[latestVersion], apiDirs[currentVersion], apiFiles[currentVersion])
		}
		m.checkApiTimestamps = append(m.checkApiTimestamps, checkApiTimestamp)
	}
}

func (m *aidlApi) AndroidMk() android.AndroidMkData {
	return android.AndroidMkData{
		Custom: func(w io.Writer, name, prefix, moduleDir string, data android.AndroidMkData) {
			android.WriteAndroidMkData(w, data)
			targetName := m.properties.BaseName + "-freeze-api"
			fmt.Fprintln(w, ".PHONY:", targetName)
			fmt.Fprintln(w, targetName+":", m.freezeApiTimestamp.String())
		},
	}
}

func (m *aidlApi) DepsMutator(ctx android.BottomUpMutatorContext) {
	ctx.AddDependency(ctx.Module(), nil, wrap("", m.properties.Imports, aidlInterfaceSuffix)...)
}

func aidlApiFactory() android.Module {
	m := &aidlApi{}
	m.AddProperties(&m.properties)
	android.InitAndroidModule(m)
	return m
}

type aidlInterfaceProperties struct {
	// Vndk properties for interface library only.
	cc.VndkProperties

	// Whether the library can be installed on the vendor image.
	Vendor_available *bool
	// Top level directories for includes.
	// TODO(b/128940869): remove it if aidl_interface can depend on framework.aidl
	Include_dirs []string
	// Relative path for includes. By default assumes AIDL path is relative to current directory.
	// TODO(b/111117220): automatically compute by letting AIDL parse multiple files simultaneously
	Local_include_dir string

	// The owner of the module
	Owner *string

	// List of .aidl files which compose this interface. These may be globbed.
	Srcs []string

	Imports []string

	// Used by gen dependency to fill out aidl include path
	Full_import_paths []string `blueprint:"mutated"`

	// Directory where API dumps are. Default is "api".
	Api_dir *string

	// Previous API versions that are now frozen. The version that is last in
	// the list is considered as the most recent version.
	Versions []string

	Backend struct {
		Java struct {
			// Whether to generate Java code using Java binder APIs
			// Default: true
			Enabled *bool
			// Set to the version of the sdk to compile against
			// Default: system_current
			Sdk_version *string
		}
		Cpp struct {
			// Whether to generate C++ code using C++ binder APIs
			// Default: true
			Enabled *bool
			// Whether to generate additional code for gathering information
			// about the transactions
			// Default: false
			Gen_log *bool
		}
		Ndk struct {
			// Whether to generate C++ code using NDK binder APIs
			// Default: true
			Enabled *bool
			// Whether to generate additional code for gathering information
			// about the transactions
			// Default: false
			Gen_log *bool
		}
	}
}

type aidlInterface struct {
	android.ModuleBase

	properties aidlInterfaceProperties

	// Unglobbed sources
	rawSrcs []string
}

func (i *aidlInterface) shouldGenerateJavaBackend() bool {
	// explicitly true if not specified to give early warning to devs
	return i.properties.Backend.Java.Enabled == nil || *i.properties.Backend.Java.Enabled
}

func (i *aidlInterface) shouldGenerateCppBackend() bool {
	// explicitly true if not specified to give early warning to devs
	return i.properties.Backend.Cpp.Enabled == nil || *i.properties.Backend.Cpp.Enabled
}

func (i *aidlInterface) shouldGenerateNdkBackend() bool {
	// explicitly true if not specified to give early warning to devs
	return i.properties.Backend.Ndk.Enabled == nil || *i.properties.Backend.Ndk.Enabled
}

func (i *aidlInterface) checkAndUpdateSources(mctx android.LoadHookContext) {
	prefix := mctx.ModuleDir()
	for _, source := range i.properties.Srcs {
		if pathtools.IsGlob(source) {
			globbedSrcFiles, err := mctx.GlobWithDeps(filepath.Join(prefix, source), nil)
			if err != nil {
				mctx.ModuleErrorf("glob: %s", err.Error())
			}
			for _, globbedSrc := range globbedSrcFiles {
				relativeGlobbedSrc, err := filepath.Rel(prefix, globbedSrc)
				if err != nil {
					panic(err)
				}

				i.rawSrcs = append(i.rawSrcs, relativeGlobbedSrc)
			}
		} else {
			i.rawSrcs = append(i.rawSrcs, source)
		}
	}

	if len(i.rawSrcs) == 0 {
		mctx.PropertyErrorf("srcs", "No sources provided.")
	}

	for _, source := range i.rawSrcs {
		if !strings.HasSuffix(source, ".aidl") {
			mctx.PropertyErrorf("srcs", "Source must be a .aidl file: "+source)
			continue
		}

		relativePath, err := filepath.Rel(i.properties.Local_include_dir, source)
		if err != nil || !isRelativePath(relativePath) {
			mctx.PropertyErrorf("srcs", "Source is not in local_include_dir: "+source)
		}
	}
}

func (i *aidlInterface) checkImports(mctx android.LoadHookContext) {
	for _, anImport := range i.properties.Imports {
		other := lookupInterface(anImport)

		if other == nil {
			mctx.PropertyErrorf("imports", "Import does not exist: "+anImport)
		}

		if i.shouldGenerateCppBackend() && !other.shouldGenerateCppBackend() {
			mctx.PropertyErrorf("backend.cpp.enabled",
				"C++ backend not enabled in the imported AIDL interface %q", anImport)
		}

		if i.shouldGenerateNdkBackend() && !other.shouldGenerateNdkBackend() {
			mctx.PropertyErrorf("backend.ndk.enabled",
				"NDK backend not enabled in the imported AIDL interface %q", anImport)
		}
	}
}

func (i *aidlInterface) versionedName(version string) string {
	name := i.ModuleBase.Name()
	if version != futureVersion && version != "" {
		name = name + "-V" + version
	}
	return name
}

func (i *aidlInterface) srcsForVersion(mctx android.LoadHookContext, version string) (srcs []string, base string) {
	if version == futureVersion || version == "" {
		return i.rawSrcs, i.properties.Local_include_dir
	} else {
		var apiDir string
		if i.properties.Api_dir != nil {
			apiDir = *(i.properties.Api_dir)
		} else {
			apiDir = "api"
		}
		base = filepath.Join(apiDir, version)
		full_paths, err := mctx.GlobWithDeps(filepath.Join(mctx.ModuleDir(), base, "**/*.aidl"), nil)
		if err != nil {
			panic(err)
		}
		for _, path := range full_paths {
			// Here, we need path local to the module
			srcs = append(srcs, strings.TrimPrefix(path, mctx.ModuleDir()+"/"))
		}
		return srcs, base
	}
}

func aidlInterfaceHook(mctx android.LoadHookContext, i *aidlInterface) {
	if !isRelativePath(i.properties.Local_include_dir) {
		mctx.PropertyErrorf("local_include_dir", "must be relative path: "+i.properties.Local_include_dir)
	}
	var importPaths []string
	importPaths = append(importPaths, filepath.Join(mctx.ModuleDir(), i.properties.Local_include_dir))
	importPaths = append(importPaths, i.properties.Include_dirs...)

	i.properties.Full_import_paths = importPaths

	i.checkAndUpdateSources(mctx)
	i.checkImports(mctx)

	if mctx.Failed() {
		return
	}

	var libs []string

	currentVersion := ""
	if len(i.properties.Versions) > 0 {
		currentVersion = futureVersion
	}

	if i.shouldGenerateCppBackend() {
		libs = append(libs, addCppLibrary(mctx, i, currentVersion, langCpp))
		for _, version := range i.properties.Versions {
			addCppLibrary(mctx, i, version, langCpp)
		}
	}

	if i.shouldGenerateNdkBackend() {
		// TODO(b/119771576): inherit properties and export 'is vendor' computation from cc.go
		if !proptools.Bool(i.properties.Vendor_available) {
			libs = append(libs, addCppLibrary(mctx, i, currentVersion, langNdk))
			for _, version := range i.properties.Versions {
				addCppLibrary(mctx, i, version, langNdk)
			}
		}
		// TODO(b/121157555): combine with '-ndk' variant
		libs = append(libs, addCppLibrary(mctx, i, currentVersion, langNdkPlatform))
		for _, version := range i.properties.Versions {
			addCppLibrary(mctx, i, version, langNdkPlatform)
		}
	}

	libs = append(libs, addJavaLibrary(mctx, i, currentVersion))
	for _, version := range i.properties.Versions {
		addJavaLibrary(mctx, i, version)
	}

	addApiModule(mctx, i)

	// Reserve this module name for future use
	mctx.CreateModule(android.ModuleFactoryAdaptor(phony.PhonyFactory), &phonyProperties{
		Name:     proptools.StringPtr(i.ModuleBase.Name()),
		Required: libs,
	})
}

func addCppLibrary(mctx android.LoadHookContext, i *aidlInterface, version string, lang string) string {
	cppSourceGen := i.versionedName(version) + "-" + lang + "-source"
	cppModuleGen := i.versionedName(version) + "-" + lang

	srcs, base := i.srcsForVersion(mctx, version)
	if len(srcs) == 0 {
		// This can happen when the version is about to be frozen; the version
		// directory is created but API dump hasn't been copied there.
		// Don't create a library for the yet-to-be-frozen version.
		return ""
	}

	genLog := false
	if lang == langCpp {
		genLog = proptools.Bool(i.properties.Backend.Cpp.Gen_log)
	} else if lang == langNdk || lang == langNdkPlatform {
		genLog = proptools.Bool(i.properties.Backend.Ndk.Gen_log)
	}

	mctx.CreateModule(android.ModuleFactoryAdaptor(aidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(cppSourceGen),
	}, &aidlGenProperties{
		Srcs:     srcs,
		AidlRoot: base,
		Imports:  concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Lang:     lang,
		BaseName: i.ModuleBase.Name(),
		GenLog:   genLog,
		Version:  version,
	})

	importExportDependencies := wrap("", i.properties.Imports, "-"+lang)
	var libJSONCppDependency []string
	var staticLibDependency []string
	var sdkVersion *string
	var stl *string
	var cpp_std *string
	if lang == langCpp {
		importExportDependencies = append(importExportDependencies, "libbinder", "libutils")
		if genLog {
			libJSONCppDependency = []string{"libjsoncpp"}
		}
		sdkVersion = nil
		stl = nil
		cpp_std = nil
	} else if lang == langNdk {
		importExportDependencies = append(importExportDependencies, "libbinder_ndk")
		if genLog {
			staticLibDependency = []string{"libjsoncpp_ndk"}
		}
		sdkVersion = proptools.StringPtr("current")
		stl = proptools.StringPtr("c++_shared")
	} else if lang == langNdkPlatform {
		importExportDependencies = append(importExportDependencies, "libbinder_ndk")
		if genLog {
			libJSONCppDependency = []string{"libjsoncpp"}
		}
	} else {
		panic("Unrecognized language: " + lang)
	}

	mctx.CreateModule(android.ModuleFactoryAdaptor(cc.LibraryFactory), &ccProperties{
		Name:                      proptools.StringPtr(cppModuleGen),
		Owner:                     i.properties.Owner,
		Vendor_available:          i.properties.Vendor_available,
		Defaults:                  []string{"aidl-cpp-module-defaults"},
		Generated_sources:         []string{cppSourceGen},
		Generated_headers:         []string{cppSourceGen},
		Export_generated_headers:  []string{cppSourceGen},
		Static:                    staticLib{Whole_static_libs: libJSONCppDependency},
		Shared:                    sharedLib{Shared_libs: libJSONCppDependency, Export_shared_lib_headers: libJSONCppDependency},
		Static_libs:               staticLibDependency,
		Shared_libs:               importExportDependencies,
		Export_shared_lib_headers: importExportDependencies,
		Sdk_version:               sdkVersion,
		Stl:                       stl,
		Cpp_std:                   cpp_std,
		Cflags:                    []string{"-Wextra", "-Wall", "-Werror"},
	}, &i.properties.VndkProperties)

	return cppModuleGen
}

func addJavaLibrary(mctx android.LoadHookContext, i *aidlInterface, version string) string {
	javaSourceGen := i.versionedName(version) + "-java-source"
	javaModuleGen := i.versionedName(version) + "-java"

	srcs, base := i.srcsForVersion(mctx, version)
	if len(srcs) == 0 {
		// This can happen when the version is about to be frozen; the version
		// directory is created but API dump hasn't been copied there.
		// Don't create a library for the yet-to-be-frozen version.
		return ""
	}

	sdkVersion := proptools.StringDefault(i.properties.Backend.Java.Sdk_version, "system_current")

	mctx.CreateModule(android.ModuleFactoryAdaptor(aidlGenFactory), &nameProperties{
		Name: proptools.StringPtr(javaSourceGen),
	}, &aidlGenProperties{
		Srcs:     srcs,
		AidlRoot: base,
		Imports:  concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Lang:     langJava,
		BaseName: i.ModuleBase.Name(),
		Version:  version,
	})

	mctx.CreateModule(android.ModuleFactoryAdaptor(java.LibraryFactory), &javaProperties{
		Name:              proptools.StringPtr(javaModuleGen),
		Owner:             i.properties.Owner,
		Installable:       proptools.BoolPtr(true),
		Defaults:          []string{"aidl-java-module-defaults"},
		No_framework_libs: proptools.BoolPtr(true),
		Sdk_version:       proptools.StringPtr(sdkVersion),
		Static_libs:       wrap("", i.properties.Imports, "-java"),
		Srcs:              []string{":" + javaSourceGen},
	})

	return javaModuleGen
}

func addApiModule(mctx android.LoadHookContext, i *aidlInterface) string {
	apiModule := i.ModuleBase.Name() + aidlApiSuffix
	mctx.CreateModule(android.ModuleFactoryAdaptor(aidlApiFactory), &nameProperties{
		Name: proptools.StringPtr(apiModule),
	}, &aidlApiProperties{
		BaseName: i.ModuleBase.Name(),
		Inputs:   i.rawSrcs,
		Imports:  concat(i.properties.Imports, []string{i.ModuleBase.Name()}),
		Api_dir:  i.properties.Api_dir,
		AidlRoot: i.properties.Local_include_dir,
		Versions: i.properties.Versions,
	})
	return apiModule
}

func (i *aidlInterface) Name() string {
	return i.ModuleBase.Name() + aidlInterfaceSuffix
}
func (i *aidlInterface) GenerateAndroidBuildActions(ctx android.ModuleContext) {
}
func (i *aidlInterface) DepsMutator(ctx android.BottomUpMutatorContext) {
}

var aidlInterfaceMutex sync.Mutex
var aidlInterfaces []*aidlInterface

func aidlInterfaceFactory() android.Module {
	i := &aidlInterface{}
	i.AddProperties(&i.properties)
	android.InitAndroidModule(i)
	android.AddLoadHook(i, func(ctx android.LoadHookContext) { aidlInterfaceHook(ctx, i) })

	aidlInterfaceMutex.Lock()
	aidlInterfaces = append(aidlInterfaces, i)
	aidlInterfaceMutex.Unlock()

	return i
}

func lookupInterface(name string) *aidlInterface {
	for _, i := range aidlInterfaces {
		if i.ModuleBase.Name() == name {
			return i
		}
	}
	return nil
}

type aidlMappingProperties struct {
	// Source file of this prebuilt.
	Srcs   []string `android:"arch_variant"`
	Output string
}

type aidlMapping struct {
	android.ModuleBase
	properties     aidlMappingProperties
	outputFilePath android.WritablePath
}

func (s *aidlMapping) DepsMutator(ctx android.BottomUpMutatorContext) {
	android.ExtractSourcesDeps(ctx, s.properties.Srcs)
}

func addItemsToMap(dest map[string]bool, src []string) {
	for _, item := range src {
		dest[item] = true
	}
}

func (s *aidlMapping) GenerateAndroidBuildActions(ctx android.ModuleContext) {
	var srcs android.Paths
	var all_import_dirs map[string]bool = make(map[string]bool)

	ctx.VisitDirectDeps(func(module android.Module) {
		for _, property := range module.GetProperties() {
			if jproperty, ok := property.(*java.CompilerProperties); ok {
				for _, src := range jproperty.Srcs {
					if strings.HasSuffix(src, ".aidl") {
						full_path := android.PathForModuleSrc(ctx, src)
						srcs = append(srcs, full_path)
						all_import_dirs[filepath.Dir(full_path.String())] = true
					} else if pathtools.IsGlob(src) {
						globbedSrcFiles, err := ctx.GlobWithDeps(src, nil)
						if err == nil {
							for _, globbedSrc := range globbedSrcFiles {
								full_path := android.PathForModuleSrc(ctx, globbedSrc)
								all_import_dirs[full_path.String()] = true
							}
						}
					}
				}
			} else if jproperty, ok := property.(*java.CompilerDeviceProperties); ok {
				addItemsToMap(all_import_dirs, jproperty.Aidl.Include_dirs)
				for _, include_dir := range jproperty.Aidl.Export_include_dirs {
					var full_path = filepath.Join(ctx.ModuleDir(), include_dir)
					all_import_dirs[full_path] = true
				}
				for _, include_dir := range jproperty.Aidl.Local_include_dirs {
					var full_path = filepath.Join(ctx.ModuleSubDir(), include_dir)
					all_import_dirs[full_path] = true
				}
			}
		}
	})

	var import_dirs []string
	for dir := range all_import_dirs {
		import_dirs = append(import_dirs, dir)
	}
	imports := strings.Join(wrap("-I", import_dirs, ""), " ")
	s.outputFilePath = android.PathForModuleOut(ctx, s.properties.Output)
	outDir := android.PathForModuleGen(ctx)
	ctx.Build(pctx, android.BuildParams{
		Rule:   aidlDumpMappingsRule,
		Inputs: srcs,
		Output: s.outputFilePath,
		Args: map[string]string{
			"imports": imports,
			"outDir":  outDir.String(),
		},
	})
}

func InitAidlMappingModule(s *aidlMapping) {
	s.AddProperties(&s.properties)
}

func aidlMappingFactory() android.Module {
	module := &aidlMapping{}
	InitAidlMappingModule(module)
	android.InitAndroidModule(module)
	return module
}

func (m *aidlMapping) AndroidMk() android.AndroidMkData {
	return android.AndroidMkData{
		Custom: func(w io.Writer, name, prefix, moduleDir string, data android.AndroidMkData) {
			android.WriteAndroidMkData(w, data)
			targetName := m.Name()
			fmt.Fprintln(w, ".PHONY:", targetName)
			fmt.Fprintln(w, targetName+":", m.outputFilePath.String())
		},
	}
}
