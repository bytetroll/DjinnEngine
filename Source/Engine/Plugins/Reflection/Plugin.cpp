/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_REFLECTION Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Reflection");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Logging");
	return Djinn::COM::kOK;
}

/* */