/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_EDITOR_LAYOUT Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Editor.Layout");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.UI.Controls");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.UI.Desktop");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Editor.Level");
	return Djinn::COM::kOK;
}

/* */