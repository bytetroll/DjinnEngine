/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_EDITOR_WINDOW Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Editor.Window");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, "Plugins.Base.Window");
	return Djinn::COM::kOK;
}

/* */