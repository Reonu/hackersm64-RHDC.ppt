#include <ultra64.h>

#include "sm64.h"
#include "segment_names.h"
#include "memory.h"
#include "fpv_camera.h"
#include "area.h"
#include "rendering_graph_node.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "engine/behavior_script.h"
#include "actors/group0.h"
#include "buffers/framebuffers.h"
#include "object_list_processor.h"
#include "cozy_print.h"

#include "confroom.h"
#include "confroom_spawn.h"

static struct AllocOnlyPool *sConfRoomGraphPool = NULL;

static struct GraphNodeRoot *sConfRoomRoot = NULL;

static s32 sInitialized = FALSE;

struct Object gConfroomObjectPool[NUM_CONFROOM_OBJECTS];
struct GraphNode gConfroomObjectParent = {
    .type = GRAPH_NODE_TYPE_OBJECT_PARENT,
    .prev = NULL,
    .next = NULL,
    .parent = NULL,
    .children = NULL,
};

static s32 sInitializedConfroomObjectPool = FALSE;

OfficeState gOfficeState = {
    .stage = OFFICE_STAGE_INTRO,
    //.stage = OFFICE_STAGE_1,
    .presentationActive = FALSE,
    .paused = PAUSE_STATE_START,
    .pauseTimer = 0,
    .lightsOn = TRUE,
};

void init_office_state(void) {
    OfficeState *office = &gOfficeState;
    office->stage = OFFICE_STAGE_INTRO;
    office->presentationActive = FALSE;
    office->paused = PAUSE_STATE_START;
    // reset player
}

#ifdef SLIDE_DEBUG
void start_previous_office_stage(void) {
    OfficeState *office = &gOfficeState;
    if (office->stage != OFFICE_STAGE_INTRO) {
        office->stage--;
    }
}
#endif

void start_next_office_stage(void) {
    OfficeState *office = &gOfficeState;
    if (office->stage == OFFICE_STAGE_3) {
        // TODO: game over
    } else {
        office->stage++;
    }
}

s32 get_num_coffees_left(void) {
    OfficeState *office = &gOfficeState;
    return 4 - office->stage;
}

s32 get_num_req_qtes(void) {
    OfficeState *office = &gOfficeState;
    return office->stage;
}

s32 init_confroom_object_pool(void) {
    if (sInitializedConfroomObjectPool) return TRUE;

    init_scene_graph_node_links(&gConfroomObjectParent, GRAPH_NODE_TYPE_OBJECT_PARENT);

    for (int i = 0; i < NUM_CONFROOM_OBJECTS; i++) {
        gConfroomObjectPool[i].activeFlags = ACTIVE_FLAG_DEACTIVATED;
        struct GraphNodeObject *graphNode = &gConfroomObjectPool[i].header.gfx;
        init_graph_node_object(NULL, graphNode, 0, gVec3fZero, gVec3sZero, gVec3fOne);
        geo_add_child(&gConfroomObjectParent, &graphNode->node);
        graphNode->node.flags &= ~GRAPH_RENDER_ACTIVE;
    }
    
    return TRUE;
}

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
    init_confroom_object_pool();

    sConfRoomRoot =
        (struct GraphNodeRoot *) process_geo_layout(sConfRoomGraphPool, (void *)geo_confroom_root);
    // TODO: set gConfroomObjectParent as sharedChild of the GraphNodeObjectParent (see register_scene_graph_node)
    sConfRoomRoot->areaIndex = 0;
    return TRUE;
}


s32 conf_room_check_should_init(void) {
    if (sInitialized) return sInitialized;

    sInitialized = sSegmentROMTable[SEGMENT_GROUP0_GEO] == (uintptr_t)_group0_geoSegmentRomStart;
    if (sInitialized) {
        init_confroom_object_pool();
        if (!alloc_conf_room_pool() || !init_conf_room_graph_root()) sInitialized = FALSE;
    }
    return sInitialized;
}

s32 process_conf_room(void) {
    if (!sInitialized) return FALSE;

    geo_process_root(sConfRoomRoot, NULL, NULL, 0);

    return TRUE;
}

Gfx *geo_assign_confroom_object_parent_next(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeObjectParent *objParentNode = (struct GraphNodeObjectParent *)node->next;
        objParentNode->sharedChild = &gConfroomObjectParent;
        gConfroomObjectParent.parent = node->next;
    }

    return NULL;
}

Gfx *geo_assign_confroom_object_parent_prev(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    if (callContext == GEO_CONTEXT_CREATE) {
        struct GraphNodeObjectParent *objParentNode = (struct GraphNodeObjectParent *)node->prev;
        objParentNode->sharedChild = &gConfroomObjectParent;
        gConfroomObjectParent.parent = node->prev;
    }

    return NULL;
}

Gfx *geo_render_confroom_objects(UNUSED s32 callContext, UNUSED struct GraphNode *node, UNUSED Mat4 mtx) {
    if (gConfroomObjectParent.children != NULL) {
        geo_process_node_and_siblings(gConfroomObjectParent.children);
    }
    return NULL;
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
        gSPDisplayList(dlHead++, mat_projector_screen_projectorscreen);
        gDPSetTextureImage(dlHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, gAuxBuffers[0]);
        gDPLoadSync(dlHead++);
        gSPDisplayList(dlHead++, projector_screen_projectorscreenMesh_mesh_layer_1_tri_0);
        gSPDisplayList(dlHead++, mat_projector_screen_whiteUnlit);
        gSPDisplayList(dlHead++, projector_screen_projectorscreenMesh_mesh_layer_1_tri_1);
        gSPEndDisplayList(dlHead);
    }

    return dlStart;
}

#if FALSE
Gfx *geo_debug_print(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    osSyncPrintf("ping\n");
    return NULL;
}
#endif

void update_confroom_objects(void) {
    if (gOfficeState.paused && gOfficeState.paused != PAUSE_STATE_END) return;

    u32 stageFlag = gOfficeState.stage == OFFICE_STAGE_INTRO
        ? OFFICE_STAGE_FLAG_INTRO
        : (1 << (gOfficeState.stage - 1));
    for (int i = 0; i < get_num_confroom_objects(); i++) {
        struct Object *obj = &gConfroomObjectPool[i];
        if (!obj->activeOfficeStages || (obj->activeOfficeStages & stageFlag)) {
            obj->activeFlags |= ACTIVE_FLAG_ACTIVE;
            obj->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;
            gCurrentObject = obj;
            cur_obj_update();
        } else {
            obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
            obj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
        }
    }
}

void render_pause_hud(Gfx **head) {
    static s32 lastState = PAUSE_STATE_UNPAUSED;

    if (gOfficeState.paused != lastState) {
        gOfficeState.pauseTimer = 0;
    } else {
        gOfficeState.pauseTimer++;
        if (gOfficeState.pauseTimer > 0x8000000) gOfficeState.pauseTimer = 0x8000000;
    }

    lastState = gOfficeState.paused;

    if (!gOfficeState.paused) return;
    switch (gOfficeState.paused) {
        case PAUSE_STATE_PAUSED: {
            Gfx *gfx = *head;
            render_rect_xlu(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 100, TRUE);
            *head = gfx;
            print_set_envcolour(255, 255, 255, 255);
            print_small_text(SCREEN_WIDTH / 2,  20, "PAUSE", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            break;
        }
        case PAUSE_STATE_START: {
            Gfx *gfx = *head;
            render_rect(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 20, 20, 20, TRUE);
            *head = gfx;
            print_set_envcolour(255, 255, 255, 255);
            print_small_text(SCREEN_WIDTH / 2,  20, "Make sure your game volume is high enough.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  40, "Press start to wake up...", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            break;
        }
        case PAUSE_STATE_FIRED: {
            Gfx *gfx = *head;
            render_rect_xlu(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 127, 0, 0, MIN(gOfficeState.pauseTimer, 255), TRUE);
            *head = gfx;
            print_set_envcolour(255, 255, 255, MIN(gOfficeState.pauseTimer, 255));
            print_small_text(SCREEN_WIDTH / 2,  20, "You were fired.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            break;
        }
        case PAUSE_STATE_END: {
            Gfx *gfx = *head;
            s32 finalAlpha = 0;
            if (gOfficeState.pauseTimer < ENDING_OCEAN_START) {
                s32 alpha = roundf(remap(gOfficeState.pauseTimer, 0, ENDING_OCEAN_START, 0, 255));
                render_rect_xlu(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF, 0xFF, 0xFF, alpha, TRUE);
            } else if (gOfficeState.pauseTimer < ENDING_OCEAN_FADE_OUT_END) {
                finalAlpha = 255;
                s32 alpha = roundf(remap(gOfficeState.pauseTimer, ENDING_OCEAN_START, ENDING_OCEAN_FADE_OUT_END, 255, 0));
                render_rect_xlu(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF, 0xFF, 0xFF, alpha, TRUE);
            } else if (gOfficeState.pauseTimer <= ENDING_OCEAN_END_FADE_OUT_START) {
                finalAlpha = 255;
            } else if (gOfficeState.pauseTimer > ENDING_OCEAN_END_FADE_OUT_START) {
                finalAlpha = roundf(remap(MIN(gOfficeState.pauseTimer, ENDING_OCEAN_END), ENDING_OCEAN_END_FADE_OUT_START, ENDING_OCEAN_END, 255, 0));
            }
            *head = gfx;
            print_set_envcolour(255, 255, 255, finalAlpha);
            print_small_text(SCREEN_WIDTH / 2,  SCREEN_HEIGHT - 32, "The End", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            break;
        }
    }
}

