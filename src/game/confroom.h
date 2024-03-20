#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"

#define NUM_CONFROOM_OBJECTS 32

s32 conf_room_check_should_init(void);
s32 process_conf_room(void);
Gfx *geo_render_projector_screen(s32 callContext, struct GraphNode *node, Mat4 mtx);
Gfx *geo_assign_confroom_object_parent_next(s32 callContext, struct GraphNode *node, Mat4 mtx);
Gfx *geo_assign_confroom_object_parent_prev(s32 callContext, struct GraphNode *node, Mat4 mtx);
Gfx *geo_render_confroom_objects(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx);
#if FALSE
Gfx *geo_debug_print(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx);
#endif

void update_confroom_objects(void);

extern struct Object gConfroomObjectPool[NUM_CONFROOM_OBJECTS];

typedef struct {
    Vec3f pos;
    Vec3s angle;
    u32 modelID;
    BehaviorScript bhv;
    u8 bp1;
    u8 bp2;
    u8 bp3;
    u8 bp4;
} ConfroomObjectSpawn;

typedef struct {
    s32 size;
    Vec3f *points;
} ConfroomObjectSplineRef;
