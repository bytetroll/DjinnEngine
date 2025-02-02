/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_BASE_WINDOW Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Base.Window");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Application");
	return Djinn::COM::kOK;
}

/* */