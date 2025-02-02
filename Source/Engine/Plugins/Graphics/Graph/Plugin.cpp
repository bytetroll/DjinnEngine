/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_PLUGINS_GRAPHICS_GRAPH Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Plugins.Graphics.Graph");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.System");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Logging");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Mesh");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Texture");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Data.Shader");
	return Djinn::COM::kOK;
}

/* */