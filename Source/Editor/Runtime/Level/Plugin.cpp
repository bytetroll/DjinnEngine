/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_EDITOR_LEVEL Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Editor.Level");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.UI.Controls");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Editor.World");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Game.Scene");
	return Djinn::COM::kOK;
}

/* */