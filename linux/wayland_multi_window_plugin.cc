#include "include/wayland_multi_window/wayland_multi_window_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

#include <cstring>

#include "multi_window_manager.h"
#include "wayland_multi_window_plugin_internal.h"

#define WAYLAND_MULTI_WINDOW_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), wayland_multi_window_plugin_get_type(), \
                              WaylandMultiWindowPlugin))

struct _WaylandMultiWindowPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(WaylandMultiWindowPlugin, wayland_multi_window_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void wayland_multi_window_plugin_handle_method_call(
    WaylandMultiWindowPlugin *self,
    FlMethodCall *method_call) {
  g_autoptr(FlMethodResponse) response;

  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "createWindow") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto *arguments = fl_value_get_string(args);
    auto window = MultiWindowManager::Instance()->Create(arguments);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_int(window)));
  } else if (strcmp(method, "show") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(args);
    MultiWindowManager::Instance()->Show(window_id);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "hide") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(args);
    MultiWindowManager::Instance()->Hide(window_id);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "close") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(args);
    MultiWindowManager::Instance()->Close(window_id);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "center") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(args);
    MultiWindowManager::Instance()->Center(window_id);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "setFrame") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
    auto left = fl_value_get_float(fl_value_lookup_string(args, "left"));
    auto top = fl_value_get_float(fl_value_lookup_string(args, "top"));
    auto width = fl_value_get_float(fl_value_lookup_string(args, "width"));
    auto height = fl_value_get_float(fl_value_lookup_string(args, "height"));
    MultiWindowManager::Instance()->SetFrame(window_id, left, top, width, height);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "setTitle") == 0) {
    auto *args = fl_method_call_get_args(method_call);
    auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
    auto title = fl_value_get_string(fl_value_lookup_string(args, "title"));
    MultiWindowManager::Instance()->SetTitle(window_id, title);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "getAllSubWindowIds") == 0) {
    auto window_ids = MultiWindowManager::Instance()->GetAllSubWindowIds();
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(
        fl_value_new_int64_list(window_ids.data(), window_ids.size())));
  } else if (strcmp(method, "setLayerSize") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
        auto height = fl_value_get_float(fl_value_lookup_string(args, "height"));
        auto width = fl_value_get_float(fl_value_lookup_string(args, "width"));
        MultiWindowManager::Instance()->SetLayerShellSize(window_id, width, height);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    } else if (strcmp(method, "autoExclusiveZoneIsEnabled") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(args);
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "enableAutoExclusiveZone") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(args);
        MultiWindowManager::Instance()->EnableAutoExclusiveZone(window_id);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    } else if (strcmp(method, "getAnchor") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
//        auto edge = fl_value_get_string(fl_value_lookup_string(args, "edge"));
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "getExclusiveZone") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(args);
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "getKeyboardInteractivity") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(args);
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "getKeyboardMode") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(args);
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "getLayer") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(args);
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "getMargin") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
//        auto edge = fl_value_get_string(fl_value_lookup_string(args, "edge"));
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "setAnchor") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
        auto edge = fl_value_get_string(fl_value_lookup_string(args, "edge"));
        auto anchor = fl_value_get_bool(fl_value_lookup_string(args, "anchor"));
        MultiWindowManager::Instance()->SetLayerShellAnchor(window_id, edge, anchor);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));

    } else if (strcmp(method, "setExclusiveZone") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
        auto size = fl_value_get_int(fl_value_lookup_string(args, "size"));
        MultiWindowManager::Instance()->SetLayerShellExclusiveZone(window_id, size);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    } else if (strcmp(method, "setKeyboardInteractivity") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
//        auto interactivity = fl_value_get_bool(fl_value_lookup_string(args, "interactivity"));
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "setKeyboardMode") == 0) {
//        auto *args = fl_method_call_get_args(method_call);
//        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
//        auto mode = fl_value_get_string(fl_value_lookup_string(args, "mode"));
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    } else if (strcmp(method, "setLayer") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
        auto layer = fl_value_get_string(fl_value_lookup_string(args, "layer"));
        MultiWindowManager::Instance()->SetLayerShellLayer(window_id, layer);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    } else if (strcmp(method, "setMargin") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto window_id = fl_value_get_int(fl_value_lookup_string(args, "windowId"));
        auto edge = fl_value_get_string(fl_value_lookup_string(args, "edge"));
        auto margin = fl_value_get_int(fl_value_lookup_string(args, "margin"));
        MultiWindowManager::Instance()->SetLayerShellMargin(window_id, edge, margin);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    }else if (strcmp(method, "createLayerShell") == 0) {
        auto *args = fl_method_call_get_args(method_call);
        auto *arguments = fl_value_get_string(args);
        auto window = MultiWindowManager::Instance()->CreateLayerShell(arguments);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_int(window)));
    } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void wayland_multi_window_plugin_dispose(GObject *object) {
  G_OBJECT_CLASS(wayland_multi_window_plugin_parent_class)->dispose(object);
}

static void wayland_multi_window_plugin_class_init(WaylandMultiWindowPluginClass *klass) {
  G_OBJECT_CLASS(klass)->dispose = wayland_multi_window_plugin_dispose;
}

static void wayland_multi_window_plugin_init(WaylandMultiWindowPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data) {
  WaylandMultiWindowPlugin *plugin = WAYLAND_MULTI_WINDOW_PLUGIN(user_data);
  wayland_multi_window_plugin_handle_method_call(plugin, method_call);
}

void wayland_multi_window_plugin_register_with_registrar_internal(FlPluginRegistrar *registrar) {
  WaylandMultiWindowPlugin *plugin = WAYLAND_MULTI_WINDOW_PLUGIN(
      g_object_new(wayland_multi_window_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "mixin.one/flutter_multi_window",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}

void wayland_multi_window_plugin_register_with_registrar(FlPluginRegistrar *registrar) {
  wayland_multi_window_plugin_register_with_registrar_internal(registrar);
  auto view = fl_plugin_registrar_get_view(registrar);
  auto window = gtk_widget_get_toplevel(GTK_WIDGET(view));
  if (GTK_IS_WINDOW(window)) {
    auto window_channel = WindowChannel::RegisterWithRegistrar(registrar, 0);
    MultiWindowManager::Instance()->AttachMainWindow(window, std::move(window_channel));
  } else {
    g_critical("can not find GtkWindow instance for main window.");
  }
}
