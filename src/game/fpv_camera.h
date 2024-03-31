#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"

typedef struct {
    Vec3f pos;
    Vec3f focus;
    f32 fov;
    Vec3s dir;
} FPVCamState;

extern FPVCamState gFPVCam;

Gfx *geo_fpv_camera_perspective(s32 callContext, struct GraphNode *g, UNUSED void *context);
Gfx *geo_fpv_camera_main(s32 callContext, struct GraphNode *g, void *context);
void init_fpv_cam(void);
