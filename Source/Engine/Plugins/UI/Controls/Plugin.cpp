/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_UI_CONTROLS Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.UI.Controls");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.UI.Core");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.UI.UIML");
	return Djinn::COM::kOK;
}

/* */