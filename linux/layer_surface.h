//
// Created by luca on 15.11.23.
//

#ifndef LAYER_SURFACE_H
#define LAYER_SURFACE_H

#include <cstdint>
#include <memory>
#include <cmath>

#include <gtk/gtk.h>
#include <flutter_linux/flutter_linux.h>

#include "flutter_window.h"


class LayerSurface: public FlutterWindow {
public:
    LayerSurface(int64_t id, const std::string &args, const std::shared_ptr<FlutterWindowCallback> &callback);

    void setAnchor(const std::string &edge, bool anchor);
    void enableAutoExclusiveZone();
    void setMargin(const std::string &edge, int margin);
    void setLayer(const std::string &layer);
    void setExclusiveZone(int zone);
    void setSize(int width, int height);
};


#endif //LAYER_SURFACE_H
