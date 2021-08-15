package ca_certificates

import (
	"path"
	"path/filepath"

	"github.com/google/blueprint/proptools"

	"android/soong/android"
	"android/soong/phony"
)

func init() {
	android.RegisterModuleType("ca_certificates", caCertificatesFactory)
	android.RegisterModuleType("ca_certificates_host", caCertificatesHostFactory)
}

type caCertificatesProperties struct {
	Src_dir            *string
	Dest_dir           *string
	Module_name_prefix *string
}

func caCertificatesLoadHook(
	ctx android.LoadHookContext, factory android.ModuleFactory, c *caCertificatesProperties) {
	// Find all files in src_dir.
	srcs, err := ctx.GlobWithDeps(path.Join(ctx.ModuleDir(), *c.Src_dir, "*"), nil)
	if err != nil || len(srcs) == 0 {
		ctx.PropertyErrorf("src_dir", "cannot find files to install")
		return
	}

	// Scan through the found files to create a prebuilt_etc module for each of them.
	requiredModuleNames := make([]string, len(srcs))
	for i, src := range srcs {
		etcProps := struct {
			Name     *string
			Src      *string
			Sub_dir  *string
			Filename *string
		}{}
		filename := filepath.Base(src)
		moduleName := *c.Module_name_prefix + filename
		etcProps.Name = proptools.StringPtr(moduleName)
		etcProps.Src = proptools.StringPtr(path.Join(*c.Src_dir, filename))
		etcProps.Sub_dir = c.Dest_dir
		etcProps.Filename = proptools.StringPtr(filename)
		ctx.CreateModule(android.ModuleFactoryAdaptor(factory), &etcProps)

		// Add it to the required module list of the parent phony rule.
		requiredModuleNames[i] = moduleName
	}

	phonyProps := struct {
		Required []string
	}{}
	phonyProps.Required = requiredModuleNames
	ctx.AppendProperties(&phonyProps)
}

func caCertificatesFactory() android.Module {
	p := phony.PhonyFactory()
	c := &caCertificatesProperties{}
	android.AddLoadHook(p, func(ctx android.LoadHookContext) {
		caCertificatesLoadHook(ctx, android.PrebuiltEtcFactory, c)
	})
	p.AddProperties(c)

	return p
}

func caCertificatesHostFactory() android.Module {
	p := phony.PhonyFactory()
	c := &caCertificatesProperties{}
	android.AddLoadHook(p, func(ctx android.LoadHookContext) {
		caCertificatesLoadHook(ctx, android.PrebuiltEtcHostFactory, c)
	})
	p.AddProperties(c)

	return p
}
