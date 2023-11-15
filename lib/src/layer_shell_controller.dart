import 'dart:ui';

import 'layer_shell_controller_impl.dart';

// Not sure that i will implement these:
// gboolean	gtk_layer_is_layer_window ()
// void	gtk_layer_set_namespace ()
// const char *	gtk_layer_get_namespace ()
// void	gtk_layer_set_monitor ()
// GdkMonitor *	gtk_layer_get_monitor ()

/// The [WindowController] instance that is used to control this window.
abstract class LayerShellController {
  LayerShellController ();

  factory LayerShellController.fromWindowId(int id) {
    return LayerShellControllerMainImpl(id);
  }

  factory  LayerShellController.main() {
    return LayerShellControllerMainImpl(0);
  }

  /// The id of the window.
  /// 0 means the main window.
  int get windowId;

  /// Close the window.
  Future<void> close();

  /// Show the window.
  Future<void> show();

  /// Hide the window.
  Future<void> hide();

  /// Set the window frame rect.
  Future<void> setLayerSize(Size size);

  /// Set the window's title.
  Future<void> setTitle(String title);

  // Whether the window can be resized. Available only on macOS.
  //
  // Most useful for ensuring windows *cannot* be resized. Windows are
  // resizable by default, so there is no need to explicitly define a window
  // as resizable by calling this function.
  // Future<void> resizable(bool resizable);

  /// Available only on macOS.
  Future<void> setFrameAutosaveName(String name);

  Future<void>	setLayer(LayerSurface layer);
  Future<LayerSurface>	getLayer();
  Future<void>	setAnchor(LayerEdge edge, bool anchor);
  Future<bool>	getAnchor(LayerEdge edge);
  Future<void>	setMargin(LayerEdge edge, int margin);
  Future<int>	getMargin(LayerEdge edge);
  Future<void>	setExclusiveZone(int size);
  Future<int>	getExclusiveZone();
  Future<void>	enableAutoExclusiveZone();
  Future<bool>	autoExclusiveZoneIsEnabled();
  Future<void>	setKeyboardMode(LayerShellKeyboardMode mode);
  Future<LayerShellKeyboardMode> getKeyboardMode();
  Future<void>	setKeyboardInteractivity(bool interactivity);
  Future<bool>	getKeyboardInteractivity();
}

enum LayerShellKeyboardMode{
  none,
  exclusive,
  onDemand,
  entryNumber,
}

enum LayerSurface{
  background,
  bottom,
  top,
  overlay,
}

enum LayerEdge{
  left,
  right,
  top,
  bottom,
}
