import 'dart:io';
import 'dart:ui';

import 'package:flutter/services.dart';

import 'channels.dart';
import 'layer_shell_controller.dart';

class LayerShellControllerMainImpl extends LayerShellController {
  final MethodChannel _channel = multiWindowChannel;

  // the id of this window
  final int _id;

  LayerShellControllerMainImpl(this._id);

  @override
  int get windowId => _id;

  @override
  Future<void> close() {
    return _channel.invokeMethod('close', _id);
  }

  @override
  Future<void> hide() {
    return _channel.invokeMethod('hide', _id);
  }

  @override
  Future<void> show() {
    return _channel.invokeMethod('show', _id);
  }

  @override
  Future<void> setLayerSize(Size size) {
    return _channel.invokeMethod('setLayerSize', <String, dynamic>{
      'windowId': _id,
      'height': size.height,
      'width': size.width,
    });
  }

  @override
  Future<void> setTitle(String title) {
    return _channel.invokeMethod('setTitle', <String, dynamic>{
      'windowId': _id,
      'title': title,
    });
  }

  // @override
  // Future<void> resizable(bool resizable) {
  //   if (Platform.isMacOS) {
  //     return _channel.invokeMethod('resizable', <String, dynamic>{
  //       'windowId': _id,
  //       'resizable': resizable,
  //     });
  //   } else {
  //     throw MissingPluginException(
  //       'This functionality is only available on macOS',
  //     );
  //   }
  // }

  @override
  Future<void> setFrameAutosaveName(String name) {
    return _channel.invokeMethod('setFrameAutosaveName', <String, dynamic>{
      'windowId': _id,
      'name': name,
    });
  }

  @override
  Future<bool> autoExclusiveZoneIsEnabled() {
    return _channel.invokeMethod('autoExclusiveZoneIsEnabled', _id) as Future<bool>;
  }

  @override
  Future<void> enableAutoExclusiveZone() {
    return _channel.invokeMethod('enableAutoExclusiveZone', _id);
  }

  @override
  Future<bool> getAnchor(LayerEdge edge) {
    return _channel.invokeMethod('getAnchor', <String, dynamic>{
      'windowId': _id,
      'edge': edge.toString(),
    }) as Future<bool>;
  }

  @override
  Future<int> getExclusiveZone() {
    return _channel.invokeMethod('getExclusiveZone', _id) as Future<int>;
  }

  @override
  Future<bool> getKeyboardInteractivity() {
    return _channel.invokeMethod('getKeyboardInteractivity', _id) as Future<bool>;
  }

  @override
  Future<LayerShellKeyboardMode> getKeyboardMode() async {
    final response = await (_channel.invokeMethod('getKeyboardMode', _id) as Future<String>);
    return LayerShellKeyboardMode.values.firstWhere((element) => element.toString() == response);
  }

  @override
  Future<LayerSurface> getLayer() async {
    final response = await (_channel.invokeMethod('getLayer', _id) as Future<String>);
    return LayerSurface.values.firstWhere((element) => element.toString() == response);
  }

  @override
  Future<int> getMargin(LayerEdge edge) {
    return _channel.invokeMethod('getMargin', <String, dynamic>{
      'windowId': _id,
      'edge': edge.toString(),
    }) as Future<int>;
  }

  @override
  Future<void> setAnchor(LayerEdge edge, bool anchor) {
    return _channel.invokeMethod('setAnchor', <String, dynamic>{
      'windowId': _id,
      'edge': edge.toString(),
      'anchor': anchor,
    });
  }

  @override
  Future<void> setExclusiveZone(int size) {
    return _channel.invokeMethod('setExclusiveZone', <String, dynamic>{
      'windowId': _id,
      'size': size,
    });
  }

  @override
  Future<void> setKeyboardInteractivity(bool interactivity) {
    return _channel.invokeMethod('setKeyboardInteractivity', <String, dynamic>{
      'windowId': _id,
      'interactivity': interactivity,
    });
  }

  @override
  Future<void> setKeyboardMode(LayerShellKeyboardMode mode) {
    return _channel.invokeMethod('setKeyboardMode', <String, dynamic>{
      'windowId': _id,
      'mode': mode.toString(),
    });
  }

  @override
  Future<void> setLayer(LayerSurface layer) {
    return _channel.invokeMethod('setLayer', <String, dynamic>{
      'windowId': _id,
      'layer': layer.toString(),
    });
  }

  @override
  Future<void> setMargin(LayerEdge edge, int margin) {
    return _channel.invokeMethod('setMargin', <String, dynamic>{
      'windowId': _id,
      'edge': edge.toString(),
      'margin': margin,
    });
  }
}
