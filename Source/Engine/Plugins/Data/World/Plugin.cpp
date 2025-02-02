/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_DATA_WORLD Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Data.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Asset");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Serialization");
	return Djinn::COM::kOK;
}

/* */