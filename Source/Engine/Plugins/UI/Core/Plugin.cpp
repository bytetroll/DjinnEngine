/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_UI_CORE Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.UI.Core");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Window");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.Component");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.UI");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.Input");
	return Djinn::COM::kOK;
}

/* */