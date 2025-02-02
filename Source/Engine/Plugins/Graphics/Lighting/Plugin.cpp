/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GRAPHICS_LIGHTING Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Graphics.Lighting");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Common");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Camera");
	return Djinn::COM::kOK;
}

/* */