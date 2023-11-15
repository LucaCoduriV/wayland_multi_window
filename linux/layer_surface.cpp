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

extern bool rustdesk_is_subwindow;
namespace {
    WindowCreatedCallback _g_window_created_callback;
}

extern gboolean DrawCallback(GtkWidget* widget, cairo_t* cr, gpointer data);

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
    gtk_window_set_decorated(GTK_WINDOW(window_), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(window_), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(window_), GTK_WIN_POS_CENTER);
    // try setting icon for rustdesk, which uses the system cache
    // mainly for the icon in appimage.
    GtkIconTheme *theme = gtk_icon_theme_get_default();
    gint icons[4] = {256, 128, 64, 32};
    for (int i = 0; i < 4; i++) {
        GdkPixbuf *icon = gtk_icon_theme_load_icon(theme, "rustdesk", icons[i], GTK_ICON_LOOKUP_NO_SVG, NULL);
        if (icon != nullptr) {
            gtk_window_set_icon(GTK_WINDOW(window_), icon);
        }
    }

    // set gtk header bar
    // fix for the frame of sub window exists after hide header bar on wayland
    const char *display_server = getenv("WAYLAND_DISPLAY");
    if (display_server) {
        GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
        gtk_widget_show(GTK_WIDGET(header_bar));
        gtk_header_bar_set_title(header_bar, "");
        gtk_header_bar_set_show_close_button(header_bar, TRUE);
        gtk_window_set_titlebar(GTK_WINDOW(window_), GTK_WIDGET(header_bar));
    }
    g_autoptr(FlDartProject)
    project = fl_dart_project_new();
    const char *entrypoint_args[] = {"multi_window", g_strdup_printf("%ld", id_), args.c_str(), nullptr};
    fl_dart_project_set_dart_entrypoint_arguments(project, const_cast<char **>(entrypoint_args));

    FlView *fl_view = fl_view_new(project);
    gtk_container_add(GTK_CONTAINER(window_), GTK_WIDGET(fl_view));

    if (_g_window_created_callback) {
        _g_window_created_callback(FL_PLUGIN_REGISTRY(fl_view));
    }
    // indicate to plugin injections using extern
    rustdesk_is_subwindow = true;
    fl_register_plugins(FL_PLUGIN_REGISTRY(fl_view));
    g_autoptr(FlPluginRegistrar)
    wayland_multi_window_registrar =
            fl_plugin_registry_get_registrar_for_plugin(FL_PLUGIN_REGISTRY(fl_view), "waylandMultiWindowPlugin");
    wayland_multi_window_plugin_register_with_registrar_internal(wayland_multi_window_registrar);

    window_channel_ = WindowChannel::RegisterWithRegistrar(wayland_multi_window_registrar, id_);

    // Refer to: https://github.com/alexmercerind/flutter_acrylic
    //
    // Set a transparent background for RustDesk.
    // Because we need to control the whole remote screen better, especially for the edge part of the remote screen. We gives a padding for the flutter view, and set a transparent border surround with the flutter view.
    gtk_widget_set_app_paintable(GTK_WIDGET(window_), TRUE);
    auto screen = gdk_screen_get_default();
    auto visual = gdk_screen_get_rgba_visual(screen);
    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(GTK_WIDGET(window_), visual);
    }
    g_signal_connect(G_OBJECT(window_), "draw", G_CALLBACK(DrawCallback), this);

    gtk_widget_show(GTK_WIDGET(window_));
    gtk_widget_show(GTK_WIDGET(fl_view));

    g_signal_connect(window_, "delete-event", G_CALLBACK(onWindowClose), this);
    g_signal_connect(window_, "window-state-event",
                     G_CALLBACK(onWindowStateChange), this);
    g_signal_connect(window_, "focus-in-event",
                     G_CALLBACK(onWindowFocus), this);
    g_signal_connect(window_, "focus-out-event",
                     G_CALLBACK(onWindowBlur), this);
    g_signal_connect(window_, "configure-event",
                     G_CALLBACK(onWindowMove), this);
    g_signal_connect(window_, "check-resize",
                     G_CALLBACK(onWindowResize), this);
    g_signal_connect(window_, "show", G_CALLBACK(onWindowShow),
                     this);
    g_signal_connect(window_, "hide", G_CALLBACK(onWindowHide),
                     this);
    // enhance drag
    g_signal_connect(window_, "event-after", G_CALLBACK(onWindowEventAfter),
                     this);
    this->findEventBox(GTK_WIDGET(fl_view));
    this->pressedEmissionHook = g_signal_add_emission_hook(
            g_signal_lookup("button-press-event", GTK_TYPE_WIDGET), 0,
            onMousePressHook, this, NULL);
    g_signal_add_emission_hook(
            g_signal_lookup("button-release-event", GTK_TYPE_WIDGET), 0,
            onMouseReleaseHook, this, NULL);

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
