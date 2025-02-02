/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_EDITOR_DATA_MESH Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Editor.Data.Mesh");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Editor.Data.Asset");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Mesh");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eBefore, "Editor.Layout");
	return Djinn::COM::kOK;
}

/* */