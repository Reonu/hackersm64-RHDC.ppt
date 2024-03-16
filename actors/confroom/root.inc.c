#include "game/fpv_camera.h"

const GeoLayout geo_confroom_root[] = {
   GEO_NODE_SCREEN_AREA(10, SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X, SCREEN_CENTER_Y),
   GEO_OPEN_NODE(),
      GEO_ZBUFFER(1),
      GEO_OPEN_NODE(),
         GEO_CAMERA_FRUSTUM_WITH_FUNC(45, 50, 10000, geo_fpv_camera_perspective),
         GEO_OPEN_NODE(),
            GEO_CAMERA(CAMERA_MODE_CLOSE, 0, 2000, 6000, 0, 0, 0, geo_fpv_camera_main),
            GEO_OPEN_NODE(),
               GEO_BRANCH(1, confroom_geo),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
