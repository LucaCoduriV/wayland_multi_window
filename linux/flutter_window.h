//
// Created by yangbin on 2022/1/11.
//

#ifndef WAYLAND_MULTI_WINDOW_WINDOWS_FLUTTER_WINDOW_H_
#define WAYLAND_MULTI_WINDOW_WINDOWS_FLUTTER_WINDOW_H_

#include <cstdint>
#include <memory>
#include <cmath>

#include <gtk/gtk.h>
#include <flutter_linux/flutter_linux.h>

#include "base_flutter_window.h"

class FlutterWindowCallback {

 public:
  virtual void OnWindowClose(int64_t id) = 0;

  virtual void OnWindowDestroy(int64_t id) = 0;

};

class FlutterWindow : public BaseFlutterWindow {

 public:
  FlutterWindow() = default;
  FlutterWindow(int64_t id, const std::string &args, const std::shared_ptr<FlutterWindowCallback> &callback);
  ~FlutterWindow() override;

  WindowChannel *GetWindowChannel() override;

  // Should not be public but I am lazy
  std::weak_ptr<FlutterWindowCallback> callback_;
  int64_t id_;

 protected:

  GtkWindow *GetWindow() override {
    return GTK_WINDOW(window_);
  }

  GtkWidget *window_ = nullptr;






  std::unique_ptr<WindowChannel> window_channel_;

};

#endif //WAYLAND_MULTI_WINDOW_WINDOWS_FLUTTER_WINDOW_H_
