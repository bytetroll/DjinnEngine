/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_BASE_FILESYSTEM Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Base.FileSystem");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Async");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Logging");
	return Djinn::COM::kOK;
}

/* */