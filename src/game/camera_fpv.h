#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"

struct FPVCamState {
    Vec3f pos;
    Vec3f focus;
    f32 fov;
    Vec3s dir;
};

Gfx *geo_camera_fpv_perspective(s32 callContext, struct GraphNode *g, UNUSED void *context);
Gfx *geo_camera_fpv_main(s32 callContext, struct GraphNode *g, void *context);
