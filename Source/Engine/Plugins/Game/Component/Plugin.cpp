/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GAME_COMPONENT Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Game.Component");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Logging");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.World");
	return Djinn::COM::kOK;
}

/* */