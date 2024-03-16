#include <ultra64.h>

#include "sm64.h"
#include "segment_names.h"
#include "memory.h"
#include "fpv_camera.h"
#include "area.h"
#include "rendering_graph_node.h"
#include "engine/math_util.h"
#include "actors/group0.h"
#include "buffers/framebuffers.h"

#include "confroom.h"

static struct AllocOnlyPool *sConfRoomGraphPool = NULL;

static struct GraphNodeRoot *sConfRoomRoot = NULL;

static s32 sInitialized = FALSE;

s32 alloc_conf_room_pool(void) {
    if (sConfRoomGraphPool != NULL) return TRUE;

    sConfRoomGraphPool = alloc_only_pool_init(
        (main_pool_available() / 4) - sizeof(struct AllocOnlyPool),
        MEMORY_POOL_LEFT
    );

    return sConfRoomGraphPool != NULL;
}

s32 init_conf_room_graph_root(void) {
    if (sConfRoomRoot != NULL) return TRUE;
    if (sConfRoomGraphPool == NULL) return FALSE;

    sConfRoomRoot =
        (struct GraphNodeRoot *) process_geo_layout(sConfRoomGraphPool, (void *)geo_confroom_root);
    return TRUE;
}


s32 conf_room_check_should_init(void) {
    if (sInitialized) return sInitialized;

    sInitialized = sSegmentROMTable[SEGMENT_GROUP0_GEO] == (uintptr_t)_group0_geoSegmentRomStart;
    if (sInitialized) {
        if (!alloc_conf_room_pool() || !init_conf_room_graph_root()) sInitialized = FALSE;
    }
    return sInitialized;
}

s32 process_conf_room(void) {
    if (!sInitialized) return FALSE;

    geo_process_root(sConfRoomRoot, NULL, NULL, 0);

    return TRUE;
}

Gfx *geo_render_projector_screen(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    struct GraphNodeGenerated *currentGraphNode;
    Gfx *dlStart, *dlHead;
    dlStart = NULL;

    if (callContext == GEO_CONTEXT_RENDER) {
        currentGraphNode = (struct GraphNodeGenerated *) node;
        if (currentGraphNode->parameter != 0) SET_GRAPH_NODE_LAYER(currentGraphNode->fnNode.node.flags, currentGraphNode->parameter & 0xFF);

        dlStart = alloc_display_list(sizeof(Gfx) * 7);
        dlHead = dlStart;
        gSPDisplayList(dlHead++, mat_confroom_projectorscreen);
        gDPSetTextureImage(dlHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, gAuxBuffers[0]);
        gDPLoadSync(dlHead++);
        gSPDisplayList(dlHead++, confroom_projectorscreenMesh_mesh_layer_1_tri_0);
        gSPDisplayList(dlHead++, mat_confroom_white);
        gSPDisplayList(dlHead++, confroom_projectorscreenMesh_mesh_layer_1_tri_1);
        gSPEndDisplayList(dlHead);
    }

    return dlStart;
}
