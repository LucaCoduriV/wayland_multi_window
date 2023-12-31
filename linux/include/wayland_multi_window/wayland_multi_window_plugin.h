#ifndef FLUTTER_PLUGIN_WAYLAND_MULTI_WINDOW_PLUGIN_H_
#define FLUTTER_PLUGIN_WAYLAND_MULTI_WINDOW_PLUGIN_H_

#include <flutter_linux/flutter_linux.h>

G_BEGIN_DECLS

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __attribute__((visibility("default")))
#else
#define FLUTTER_PLUGIN_EXPORT
#endif

typedef struct _WaylandMultiWindowPlugin WaylandMultiWindowPlugin;
typedef struct {
  GObjectClass parent_class;
} WaylandMultiWindowPluginClass;

FLUTTER_PLUGIN_EXPORT GType wayland_multi_window_plugin_get_type();

FLUTTER_PLUGIN_EXPORT void wayland_multi_window_plugin_register_with_registrar(
    FlPluginRegistrar* registrar);

typedef void (*WindowCreatedCallback)(FlPluginRegistry *registry);

FLUTTER_PLUGIN_EXPORT void wayland_multi_window_plugin_set_window_created_callback(
    WindowCreatedCallback callback);


G_END_DECLS

#endif  // FLUTTER_PLUGIN_WAYLAND_MULTI_WINDOW_PLUGIN_H_
