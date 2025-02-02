/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GAME_WORLD Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Game.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Async");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.World");
	return Djinn::COM::kOK;
}

/* */