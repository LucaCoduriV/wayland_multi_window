//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <desktop_lifecycle/desktop_lifecycle_plugin.h>
#include <wayland_multi_window/wayland_multi_window_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) desktop_lifecycle_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "DesktopLifecyclePlugin");
  desktop_lifecycle_plugin_register_with_registrar(desktop_lifecycle_registrar);
  g_autoptr(FlPluginRegistrar) wayland_multi_window_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "WaylandMultiWindowPlugin");
  wayland_multi_window_plugin_register_with_registrar(wayland_multi_window_registrar);
}
