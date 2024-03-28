#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "confroom_collision.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"

enum FPVPlayerActionStates {
    PLAYER_FREE,
    PLAYER_BREWING_COFFEE,
    PLAYER_PRESENTING,
    PLAYER_CONVO_QTE,
    PLAYER_INTRO_CUTSCENE
};

typedef struct {
    struct Controller *cont;
    Vec3f pos;
    Vec3f vel;
    f32 runFac;
    f32 headPos;
    Vec3f focusPoint;
    f32 focusPointFac; // 0 to 1, 0 uses player direction and 1 uses focus point directly
    s32 energy;
    Vec3s dir;
    Cylinder hitbox;
    AABB *curSpace;
    struct Object *arm;
    struct Object *coffeeCup;
    u8 sipsLeft;
    u8 actionState;
    u8 focusPointActive: 1;
    u8 crouching: 1;
    u8 running: 1;
    u8 coffeeStolen: 1;
    u8 canSit: 1;
    u8 hasRespawned: 1;
    s8 currentTutorial;
    u32 tutorialTimer;
    u8 curTutorialDone;
#ifdef SLIDE_DEBUG
    u8 godMode: 1;
    u8 instaGo: 1;
#endif
} FPVPlayer;

extern FPVPlayer gFPVPlayer;

#define PLAYER_HEIGHT        183.0f
#define PLAYER_RADIUS        30.0f
#define PLAYER_EYE_DEFAULT   165.0f
#define PLAYER_EYE_SITTING   135.0f
#define PLAYER_EYE_CROUCHING 99.0f

#define PLAYER_TOP_DIST_FROM_EYE (PLAYER_HEIGHT - PLAYER_EYE_DEFAULT)
#define PLAYER_WAIST_DIST_FROM_EYE 66.0f

#define MINUTES_TO_FRAMES(t) (s32)(60 * 30 * (t))

#define MAX_ENERGY MINUTES_TO_FRAMES(4.0f)

#define E_COST_PRESENTING 1
#define E_COST_CROUCHING  1
#define E_COST_STANDING   2
#define E_COST_MOVING     3

#define E_GAIN_KITCHEN    5
#define E_GAIN_COFFEE_SIP (MAX_ENERGY / 4)

#define ROT_HZ_SPEED   0x200
#define ROT_VT_SPEED   0x200

#define ROT_TILT_SPEED 0x100
#define ROT_TILT_MAX   DEGREES(45)

// average walking speed in meters/second is 1.4
// converting this to cm/frame:
#define PLAYER_MAX_SPEED_WALK   meters_sec(1.6f)
#define PLAYER_MAX_SPEED_RUN    meters_sec(3.0f)
#define PLAYER_MAX_SPEED_CROUCH meters_sec(0.4f)
#define PLAYER_MAX_ACCEL (PLAYER_MAX_SPEED_WALK * 0.2f)
#define PLAYER_DECEL     (PLAYER_MAX_SPEED_WALK * 0.35f)

#define PLAYER_GRAVITY   meters_sec(9.8f*0.2f)
#define PLAYER_TERM_VEL  meters_sec(-53.0f * 0.2f)
#define PLAYER_JUMP_VEL  meters_sec(12.0f)


// controls
// PLAYER_FREE
// #define PLAYER_BTN_JUMP     A_BUTTON
#define PLAYER_BTN_INTERACT B_BUTTON
#define PLAYER_BTN_RUN      A_BUTTON
#define PLAYER_BTN_CROUCH   Z_TRIG
#define PLAYER_BTN_ZOOM     R_TRIG
#define PLAYER_BTN_START_PRESENTATION PLAYER_BTN_INTERACT
// PLAYER_PRESENTING
#define PLAYER_BTN_STOP_PRESENTATION L_TRIG
#define PLAYER_BTN_DRINK_COFFEE      R_TRIG

// Max distance from seat to sit in front of presentation
#define MAX_SITTING_DIST 225.f

void init_player(void);
s32 update_player(void);
void render_player_hud(Gfx **head);
void deplete_energy(s32 amt);
void replenish_energy(s32 amt);
