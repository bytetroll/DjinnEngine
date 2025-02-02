/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_BASE_APPLICATION Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Base.Application");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Async");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Reflection");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Async");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Serialization");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Memory");
	return Djinn::COM::kOK;
}

/* */