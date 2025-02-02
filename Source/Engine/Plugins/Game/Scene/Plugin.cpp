/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GAME_SCENE Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Game.Scene");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Window");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.Component");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.Input");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Scene");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Camera");
	return Djinn::COM::kOK;
}

/* */