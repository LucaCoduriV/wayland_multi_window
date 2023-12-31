//
// Created by yangbin on 2022/1/11.
//

#ifndef WAYLAND_MULTI_WINDOW_WINDOWS_MULTI_WINDOW_MANAGER_H_
#define WAYLAND_MULTI_WINDOW_WINDOWS_MULTI_WINDOW_MANAGER_H_

#include <cstdint>
#include <string>
#include <map>
#include <cmath>
#include <vector>

#include "base_flutter_window.h"
#include "flutter_window.h"

class MultiWindowManager : public std::enable_shared_from_this<MultiWindowManager>, public FlutterWindowCallback {

 public:
  static MultiWindowManager *Instance();

  MultiWindowManager();

  virtual ~MultiWindowManager();

  int64_t Create(const std::string &args);

  void AttachMainWindow(GtkWidget *main_flutter_window, std::unique_ptr<WindowChannel> window_channel);

  void Show(int64_t id);

  void Hide(int64_t id);

  void Close(int64_t id);

  void SetFrame(int64_t id, double_t x, double_t y, double_t width, double_t height);

  void Center(int64_t id);

  void SetTitle(int64_t id, const std::string &title);

  std::vector<int64_t> GetAllSubWindowIds();

  void OnWindowClose(int64_t id) override;

  void OnWindowDestroy(int64_t id) override;

  int64_t CreateLayerShell(const std::string &args);
  void SetLayerShellAnchor(int64_t id, const std::string &edge, bool anchor);
  void EnableAutoExclusiveZone(int64_t id);
  void SetLayerShellMargin(int64_t id, const std::string &edge, int margin);
  void SetLayerShellLayer(int64_t id, const std::string &layerSurface);
  void SetLayerShellExclusiveZone(int64_t id, int zone);
  void SetLayerShellSize(int64_t id, int width, int height);

 private:

  std::map<int64_t, std::unique_ptr<BaseFlutterWindow>> windows_;

  void HandleMethodCall(int64_t from_window_id,
                        int64_t target_window_id,
                        const gchar *method,
                        FlValue *arguments,
                        FlMethodCall *method_call
  );

};

#endif //WAYLAND_MULTI_WINDOW_WINDOWS_MULTI_WINDOW_MANAGER_H_
