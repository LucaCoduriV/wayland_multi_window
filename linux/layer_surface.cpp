//
// Created by luca on 15.11.23.
//

#include "layer_surface.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

#include <cairo/cairo.h>

#include "include/wayland_multi_window/wayland_multi_window_plugin.h"
#include "wayland_multi_window_plugin_internal.h"
#include <gtk-layer-shell/gtk-layer-shell.h>

extern void fl_register_plugins(FlPluginRegistry *registry);

namespace {
    WindowCreatedCallback _g_window_created_callback;
}

//extern gboolean DrawCallback(GtkWidget* widget, cairo_t* cr, gpointer data);

LayerSurface::LayerSurface(
        int64_t id,
        const std::string &args,
        const std::shared_ptr <FlutterWindowCallback> &callback
        )
{
    callback_ = callback;
    id_ = id;

    window_ = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_layer_init_for_window(GTK_WINDOW(window_));
    gtk_window_set_default_size(GTK_WINDOW(window_), 1280, 720);
    gtk_window_set_title(GTK_WINDOW(window_), "");
    gtk_window_set_position(GTK_WINDOW(window_), GTK_WIN_POS_CENTER);
    gtk_widget_show(GTK_WIDGET(window_));

    g_signal_connect(window_, "destroy", G_CALLBACK(+[](GtkWidget *, gpointer arg) {
      auto *self = static_cast<FlutterWindow *>(arg);
      if (auto callback = self->callback_.lock()) {
        callback->OnWindowClose(self->id_);
        callback->OnWindowDestroy(self->id_);
      }
    }), this);

    g_autoptr(FlDartProject)
        project = fl_dart_project_new();
    const char *entrypoint_args[] = {"multi_window", g_strdup_printf("%ld", id_), args.c_str(), nullptr};
    fl_dart_project_set_dart_entrypoint_arguments(project, const_cast<char **>(entrypoint_args));

    auto fl_view = fl_view_new(project);
    gtk_widget_show(GTK_WIDGET(fl_view));
    gtk_container_add(GTK_CONTAINER(window_), GTK_WIDGET(fl_view));

    if (_g_window_created_callback) {
        _g_window_created_callback(FL_PLUGIN_REGISTRY(fl_view));
    }
    g_autoptr(FlPluginRegistrar)
        wayland_multi_window_registrar =
        fl_plugin_registry_get_registrar_for_plugin(FL_PLUGIN_REGISTRY(fl_view), "WaylandMultiWindowPlugin");
    wayland_multi_window_plugin_register_with_registrar_internal(wayland_multi_window_registrar);

    window_channel_ = WindowChannel::RegisterWithRegistrar(wayland_multi_window_registrar, id_);

    gtk_widget_grab_focus(GTK_WIDGET(fl_view));
    gtk_widget_hide(GTK_WIDGET(window_));
}

void LayerSurface::setAnchor(const std::string &edge, bool anchor) {
    if (std::strcmp(edge.c_str(), "LayerEdge.left") == 0){
        gtk_layer_set_anchor(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_LEFT, anchor);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.right") == 0){
        gtk_layer_set_anchor(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_RIGHT, anchor);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.top") == 0){
        gtk_layer_set_anchor(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_TOP, anchor);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.bottom") == 0){
        gtk_layer_set_anchor(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_BOTTOM, anchor);
    }
}

void LayerSurface::enableAutoExclusiveZone() {
    gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(window_));
}

void LayerSurface::setMargin(const std::string &edge, int margin) {
    if (std::strcmp(edge.c_str(), "LayerEdge.left") == 0){
        gtk_layer_set_margin(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_LEFT, margin);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.right") == 0){
        gtk_layer_set_margin(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_RIGHT, margin);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.top") == 0){
        gtk_layer_set_margin(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_TOP, margin);
    } else if (std::strcmp(edge.c_str(), "LayerEdge.bottom") == 0){
        gtk_layer_set_margin(GTK_WINDOW(window_), GTK_LAYER_SHELL_EDGE_BOTTOM, margin);
    }
}

void LayerSurface::setLayer(const std::string &layer) {
    if (std::strcmp(layer.c_str(), "LayerSurface.background") == 0){
        gtk_layer_set_layer(GTK_WINDOW(window_), GTK_LAYER_SHELL_LAYER_BACKGROUND);
    } else if (std::strcmp(layer.c_str(), "LayerSurface.bottom") == 0){
        gtk_layer_set_layer(GTK_WINDOW(window_), GTK_LAYER_SHELL_LAYER_BOTTOM);
    } else if (std::strcmp(layer.c_str(), "LayerSurface.top") == 0){
        gtk_layer_set_layer(GTK_WINDOW(window_), GTK_LAYER_SHELL_LAYER_TOP);
    } else if (std::strcmp(layer.c_str(), "LayerSurface.overlay") == 0){
        gtk_layer_set_layer(GTK_WINDOW(window_), GTK_LAYER_SHELL_LAYER_OVERLAY);
    }
}

void LayerSurface::setSize(int width, int height) {
    gtk_widget_set_size_request (GTK_WIDGET(window_), width, height);
    gtk_window_resize(GTK_WINDOW(window_), width, height);
}

void LayerSurface::setExclusiveZone(int zone) {
    gtk_layer_set_exclusive_zone(GTK_WINDOW(window_), zone);
}
