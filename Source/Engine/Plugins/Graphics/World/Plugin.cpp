/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GRAPHICS_WORLD Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Graphics.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Graph");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.System");
	return Djinn::COM::kOK;
}

/* */