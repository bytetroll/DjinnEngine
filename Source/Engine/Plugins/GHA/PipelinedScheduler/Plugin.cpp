/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GHA_PIPELINESCHEDULER Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.GHA.PipelineScheduler");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Logging");
	return Djinn::COM::kOK;
}

/* */