/* Generated file */
#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Platform/Detail/LibraryMemoryHooks.h>

DJINN_C_EXPORT_EDITOR_VIEWPORT Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {
	out.Description("<No description supplied>");
	out.Name("Editor.Viewport");
	out.AddDependency(Djinn::Host::PluginDependencyMode::eBefore, "Editor.Layout");
	return Djinn::COM::kOK;
}

/* */