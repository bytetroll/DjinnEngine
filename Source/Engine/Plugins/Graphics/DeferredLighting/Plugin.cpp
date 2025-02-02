/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GRAPHICS_DEFERREDLIGHTING Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Graphics.DeferredLighting");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Graphics.Lighting");
	return Djinn::COM::kOK;
}

/* */