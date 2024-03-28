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
    u8 activeOfficeStages;
} ConfroomObjectSpawn;

typedef struct {
    s32 size;
    Vec3f *points;
} ConfroomObjectSplineRef;

enum OfficeStages {
    OFFICE_STAGE_INTRO,
    OFFICE_STAGE_1,
    OFFICE_STAGE_2,
    OFFICE_STAGE_3,
};

enum OfficeStageFlags {
    OFFICE_STAGE_FLAG_ALL,
    OFFICE_STAGE_FLAG_1 = (1 << 0),
    OFFICE_STAGE_FLAG_2 = (1 << 1),
    OFFICE_STAGE_FLAG_3 = (1 << 2),
    OFFICE_STAGE_FLAG_INTRO = (1 << 3),
};

typedef struct {
    u8 stage; // OfficeStages
    u8 presentationActive;
    u8 paused;
    u8 lightsOn: 1;
} OfficeState;

void init_office_state(void);
void start_next_office_stage(void);
#ifdef SLIDE_DEBUG
void start_previous_office_stage(void);
#endif
s32 get_num_coffees_left(void);
s32 get_num_req_qtes(void);

extern OfficeState gOfficeState;
