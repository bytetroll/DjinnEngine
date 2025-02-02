/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GHA_VULKAN Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.GHA.Vulkan");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Async");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Asset");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Shader");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Texture");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.GHA.VulkanShader");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.GHA.PipelineScheduler");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eBefore, "Plugins.Graphics.System");
	return Djinn::COM::kOK;
}

/* */