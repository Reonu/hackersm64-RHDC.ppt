#include <ultra64.h>

#include "sm64.h"
#include "fpv_camera.h"
#include "camera.h"
#include "area.h"
#include "memory.h"
#include "engine/math_util.h"
#include "actors/group0.h"

static const FPVCamState sInitFPVCam = {
    .pos = {0, 0, 0},
    .focus = {0, 0, 0},
    .dir = {0, 0, 0},
    .fov = 30
};

FPVCamState gFPVCam = {
    .pos = {0, 0, 0},
    .focus = {0, 0, 0},
    .dir = {0, 0, 0},
    .fov = 30
};

void init_fpv_cam(void) {
    gFPVCam = sInitFPVCam;
}

/**
 * Sets the field of view for the GraphNodeCamera
 */
Gfx *geo_fpv_camera_perspective(s32 callContext, struct GraphNode *g, UNUSED void *context) {
    struct GraphNodePerspective *perspective = (struct GraphNodePerspective *) g;

    if (callContext == GEO_CONTEXT_RENDER) {

    }

    perspective->fov = gFPVCam.fov;
    return NULL;
}

void update_graph_node_fpv_camera(struct GraphNodeCamera *gc) {
    gc->rollScreen = gFPVCam.dir[2];
    // gFPVCam.dir[1] += DEGREES(1);
    vec3f_copy(gc->pos, gFPVCam.pos);
    // gFPVCam.focus[0] = gc->pos[0] + sins(gFPVCam.dir[1]) * 800;
    // gFPVCam.focus[2] = gc->pos[2] + coss(gFPVCam.dir[1]) * 800;
    vec3f_copy(gc->focus, gFPVCam.focus);
}

extern void create_camera(struct GraphNodeCamera *gc, struct AllocOnlyPool *pool);
Gfx *geo_fpv_camera_main(s32 callContext, struct GraphNode *g, void *context) {
    struct GraphNodeCamera *gc = (struct GraphNodeCamera *) g;
    switch (callContext) {
        case GEO_CONTEXT_CREATE:
            create_camera(gc, context);
            vec3f_copy(gFPVCam.pos, (f32 *)segmented_to_virtual(confroom_cameraPos));
            vec3f_copy(gFPVCam.focus, (f32 *)segmented_to_virtual(confroom_projScreenPos));
            break;
        case GEO_CONTEXT_RENDER:
            update_graph_node_fpv_camera(gc);
            break;
    }
    return NULL;
}
