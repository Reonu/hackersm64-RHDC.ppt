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
#include "fpv_player.h"
#include "fpv_camera.h"
#include "cozy_print.h"
#include "level_update.h"
#include "game_init.h"
#include "seq_ids.h"
#include "mario.h"
#include "audio/external.h"
#include "src/game/save_file.h"

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

static const OfficeState sInitialOfficeState = {
    .stage = OFFICE_STAGE_INTRO,
    .presentationActive = FALSE,
    .paused = PAUSE_STATE_START,
    .pauseTimer = 0,
    .lightsOn = TRUE,
    .fadeOutTimer = 0,
    .fadeOutLength = 0
};

OfficeState gOfficeState = {
    .stage = OFFICE_STAGE_INTRO,
    .presentationActive = FALSE,
    .paused = PAUSE_STATE_START,
    .pauseTimer = 0,
    .lightsOn = TRUE,
    .fadeOutTimer = 0,
    .fadeOutLength = 0,
    .checkpoint = 0
};

void init_office_state(void) {
    OfficeState *office = &gOfficeState;
    office->stage = OFFICE_STAGE_INTRO;
    office->presentationActive = FALSE;
    if (save_file_get_office_checkpoint() != 0) {
        office->paused = PAUSE_STATE_MAIN_MENU;
    } else {
        office->paused = PAUSE_STATE_START;
    }
    
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

void reset_confroom_objects(void) {
    init_scene_graph_node_links(&gConfroomObjectParent, GRAPH_NODE_TYPE_OBJECT_PARENT);

    for (int i = 0; i < NUM_CONFROOM_OBJECTS; i++) {
        gConfroomObjectPool[i].activeFlags = ACTIVE_FLAG_DEACTIVATED;
        struct GraphNodeObject *graphNode = &gConfroomObjectPool[i].header.gfx;
        init_graph_node_object(NULL, graphNode, 0, gVec3fZero, gVec3sZero, gVec3fOne);
        geo_add_child(&gConfroomObjectParent, &graphNode->node);
        graphNode->node.flags &= ~GRAPH_RENDER_ACTIVE;
    }
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

void start_fadeout(s32 time) {
    gOfficeState.fadeOutTimer = time;
    gOfficeState.fadeOutLength = time;
}

void reset_game(void) {
    reset_confroom_objects();
    init_player();
    spawn_confroom_objects(0, 0);
    init_fpv_cam();
    gOfficeState = sInitialOfficeState;
    start_fadeout(30);

    initiate_warp(LEVEL_SLIDES, 1, 0x0A, 8);
    warp_special(0);
    gOneUpdate = TRUE;
}

void render_fadeout(Gfx **head) {
    if (gOfficeState.fadeOutLength) {
        gOfficeState.fadeOutTimer--;
        if (gOfficeState.fadeOutTimer <= 0) {
            gOfficeState.fadeOutTimer = 0;
            gOfficeState.fadeOutLength = 0;
            return;
        }

        s32 alpha = roundf(
            smoothstep(255, 0,
                remap(
                    gOfficeState.fadeOutTimer,
                    gOfficeState.fadeOutLength, 0,
                    0, 1
                )
            )
        );
        Gfx *gfx = *head;
        render_rect_cld(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 20, 20, 20, alpha, TRUE);
        *head = gfx;
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

    if ((lastState == PAUSE_STATE_START || lastState == PAUSE_STATE_MAIN_MENU) && !gOfficeState.paused) {
        func_80321080(0);
    }
    lastState = gOfficeState.paused;

    if (!gOfficeState.paused) {
        return;
    }

    switch (gOfficeState.paused) {
        case PAUSE_STATE_PAUSED: {
            Gfx *gfx = *head;
            render_rect_cld(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 180, TRUE);
            render_tiled_texrect_rgba16(&gfx, pause_controls, 256, 128, (SCREEN_WIDTH - 256) / 2, (SCREEN_HEIGHT - 128) / 2);
            *head = gfx;
            print_set_envcolour(255, 255, 255, 255);
            print_small_text(SCREEN_WIDTH / 2,  20, "PAUSE", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            if (gFPVPlayer.actionState == PLAYER_PRESENTING) {
                print_small_text(SCREEN_WIDTH / 2,  SCREEN_HEIGHT - 40, "Press L to respawn Mario.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
                if (gPlayer1Controller->buttonPressed & L_TRIG) {
                    move_mario_to_respawn(gMarioState, DEATH_TYPE_MANUAL_RESPAWN);
                    gOfficeState.paused = PAUSE_STATE_UNPAUSED;
                }
            }
            break;
        }
        case PAUSE_STATE_START: {
            Gfx *gfx = *head;
            render_rect(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 20, 20, 20, TRUE);
            *head = gfx;
            play_secondary_music(SEQ_SEA, 0, 255,0);
            print_set_envcolour(255, 255, 255, 255);
            print_small_text(SCREEN_WIDTH / 2,  20, "You're on a boat.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  40, "You're no longer a product manager.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  60, "You're finally living your dream as a fisherman.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  80, "But..", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  180, "Check that your game volume is high enough, and", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            print_small_text(SCREEN_WIDTH / 2,  200, "press START to begin...", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);

            if (gPlayer1Controller->buttonPressed & START_BUTTON) {
                gOfficeState.paused = FALSE;
                start_fadeout(30);
            }

            break;
        }
        case PAUSE_STATE_FIRED: {
            const s32 fadeLen = 45;

            f32 alphaFac = remap(MIN(gOfficeState.pauseTimer, fadeLen), 0, fadeLen, 0, 1);
            s32 alpha = roundf(smoothstep(0, 255, alphaFac));

            Gfx *gfx = *head;
            render_rect_cld(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 127, 0, 0, alpha, TRUE);
            *head = gfx;

            print_set_envcolour(255, 255, 255, alpha);
            print_small_text(SCREEN_WIDTH / 2,  SCREEN_HEIGHT / 2, "You were fired.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);

            if (gOfficeState.pauseTimer >= fadeLen) {
                alphaFac = remap(MIN(gOfficeState.pauseTimer, fadeLen + 30), fadeLen, fadeLen + 30, 0, 1);
                print_set_envcolour(255, 255, 255, roundf(smoothstep(0, 255, alphaFac)));
                print_small_text(SCREEN_WIDTH / 2,  (SCREEN_HEIGHT / 2) + 40, "Press the reset button on your console.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);

                // if (gPlayer1Controller->buttonPressed & START_BUTTON) {
                //     reset_game();
                //     lastState = PAUSE_STATE_UNPAUSED;
                // }
            }

            break;
        }
        case PAUSE_STATE_END: {
            if (gOfficeState.pauseTimer == 0) {
                play_secondary_music(SEQ_SEA, 0, 255, ENDING_OCEAN_START);
            }

            Gfx *gfx = *head;
            if (gOfficeState.pauseTimer < ENDING_OCEAN_START) {
                s32 alpha = roundf(remap(gOfficeState.pauseTimer, 0, ENDING_OCEAN_START, 0, 255));
                render_rect_cld(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF, 0xFF, 0xFF, alpha, TRUE);
            } else if (gOfficeState.pauseTimer < ENDING_OCEAN_FADE_OUT_END) {
                s32 alpha = roundf(remap(gOfficeState.pauseTimer, ENDING_OCEAN_START, ENDING_OCEAN_FADE_OUT_END, 255, 0));
                render_rect_cld(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF, 0xFF, 0xFF, alpha, TRUE);
            }
            *head = gfx;

            if (gOfficeState.pauseTimer > ENDING_OCEAN_TEXT_THE_END) {
                s32 clampTimer = CLAMP(gOfficeState.pauseTimer, ENDING_OCEAN_TEXT_THE_END, ENDING_OCEAN_TEXT_THE_END + 30*3);
                s32 alpha = roundf(remap(clampTimer, ENDING_OCEAN_TEXT_THE_END, ENDING_OCEAN_TEXT_THE_END + 30*3, 0, 255));
                print_set_envcolour(255, 255, 255, alpha);
                print_small_text(SCREEN_WIDTH / 2,  SCREEN_HEIGHT - 32, "The End", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);
            }

            s32 minAlpha = 255;
            if (gOfficeState.pauseTimer > ENDING_OCEAN_TEXT_CREDITS2 + 30*6) {
                s32 a = ENDING_OCEAN_TEXT_CREDITS2 + 30*6;
                s32 b = a + 30*2;
                s32 clampTimer = CLAMP(gOfficeState.pauseTimer, a, b);
                minAlpha = roundf(remap(clampTimer, a, b, 255, 0));
            }

            if (minAlpha && gOfficeState.pauseTimer > ENDING_OCEAN_TEXT_CREATED) {
                s32 clampTimer = CLAMP(gOfficeState.pauseTimer, ENDING_OCEAN_TEXT_CREATED, ENDING_OCEAN_TEXT_CREATED + 30*3);
                s32 alpha = roundf(remap(clampTimer, ENDING_OCEAN_TEXT_CREATED, ENDING_OCEAN_TEXT_CREATED + 30*3, 0, 255));
                alpha = MIN(alpha, minAlpha);
                print_set_envcolour(255, 255, 255, alpha);
                print_small_text(16,  16, "created by reonu & thecozies", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            }
            if (minAlpha && gOfficeState.pauseTimer > ENDING_OCEAN_TEXT_CREDITS2) {
                s32 clampTimer = CLAMP(gOfficeState.pauseTimer, ENDING_OCEAN_TEXT_CREDITS2, ENDING_OCEAN_TEXT_CREDITS2 + 30*3);
                s32 alpha = roundf(remap(clampTimer, ENDING_OCEAN_TEXT_CREDITS2, ENDING_OCEAN_TEXT_CREDITS2 + 30*3, 0, 255));
                alpha = MIN(alpha, minAlpha);
                print_set_envcolour(255, 255, 255, alpha);
                print_small_text(16, 34,
                "special thanks to:\n"
                "- lurf\n"
                "- ArcticJaguar725\n"
                "- MrComit\n"
                "- anonymous_moose\n"
                "- nesdude\n"
                "- wiseguy",
                PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            }
            break;
        }
        case PAUSE_STATE_MAIN_MENU:
            Gfx *gfx = *head;
            render_rect(&gfx, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 20, 20, 20, TRUE);
            *head = gfx;
            play_secondary_music(SEQ_SEA, 0, 255,0);
            print_set_envcolour(255, 255, 255, 255);
            print_small_text(SCREEN_WIDTH / 2,  20, "Press A to start from checkpoint.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA);     
            print_small_text(SCREEN_WIDTH / 2,  40, "Press B to start over.", PRINT_TEXT_ALIGN_CENTER, PRINT_ALL, FONT_VANILLA); 
            if (gPlayer1Controller->buttonPressed & A_BUTTON) {
                gOfficeState.checkpoint = save_file_get_office_checkpoint();
                init_player();
                gOfficeState.paused = FALSE;
                start_fadeout(30);
            } else if (gPlayer1Controller->buttonPressed & B_BUTTON) {
                gOfficeState.paused = PAUSE_STATE_START;
                save_file_set_office_checkpoint(0);
                start_fadeout(30);               
            }
    }
}

