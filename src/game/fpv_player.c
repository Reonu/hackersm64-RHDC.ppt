#include <ultra64.h>

#include "sm64.h"
#include "fpv_player.h"
#include "fpv_camera.h"
#include "game_init.h"
#include "camera.h"
#include "level_update.h"
#include "area.h"
#include "memory.h"
#include "puppyprint.h"
#include "debug.h"
#include "confroom_collision.h"
#include "confroom_conversation.h"
#include "engine/math_util.h"
#include "actors/group0.h"
#include "confroom.h"
#include "cozy_print.h"
#include "src/audio/external.h"
#include "include/behavior_data.h"
#include "sm64.h"

#define STARTING_POSITION { -355.f, 2.0f, 2720.f }

static const FPVPlayer sInitFPVPlayerState = {
    .cont = NULL,
    .pos = STARTING_POSITION,
    .vel = { 0, 0, 0 },
    .runFac = 0,
    .headPos = PLAYER_EYE_SITTING,
    .energy = MAX_ENERGY,
    .dir = { 0, DEGREES(90), 0 },
    .hitbox = {
        .pos = STARTING_POSITION,
        .height = PLAYER_HEIGHT,
        .radius = PLAYER_RADIUS
    },
    .curSpace = NULL,
    .actionState = PLAYER_INTRO_CUTSCENE,
    .focusPointActive = FALSE,
    .crouching = FALSE,
    .arm = NULL,
    .coffeeCup = NULL,
    .chasingNPC = NULL,
    .sipsLeft = 0,
    .coffeeStolen = FALSE,
    .canSit = FALSE,
    .hasRespawned = FALSE,
    .currentTutorial = -1,
    .tutorialTimer = 0,
    .curTutorialDone = 0,
    .energyLowFirstTime = 0,
    .firstCoffee = 0,
    .inConfroom = 0,
    .confroomFirstTime = 0,
    .coffeeTracker = 0,
    .lastSlideTimer = 0,
#ifdef SLIDE_DEBUG
    .godMode = FALSE,
    .instaGo = FALSE,
#endif
};

FPVPlayer gFPVPlayer = {
    .cont = NULL,
    .pos = STARTING_POSITION,
    .vel = { 0, 0, 0 },
    .runFac = 0,
    .headPos = PLAYER_EYE_SITTING,
    .energy = MAX_ENERGY,
    .dir = { 0, DEGREES(90), 0 },
    .hitbox = {
        .pos = STARTING_POSITION,
        .height = PLAYER_HEIGHT,
        .radius = PLAYER_RADIUS
    },
    .curSpace = NULL,
    .actionState = PLAYER_INTRO_CUTSCENE,
    .focusPointActive = FALSE,
    .crouching = FALSE,
    .arm = NULL,
    .coffeeCup = NULL,
    .chasingNPC = NULL,
    .sipsLeft = 0,
    .coffeeStolen = FALSE,
    .canSit = FALSE,
    .hasRespawned = FALSE,
    .currentTutorial = -1,
    .tutorialTimer = 0,
    .curTutorialDone = 0,
    .energyLowFirstTime = 0,
    .firstCoffee = 0,
    .inConfroom = 0,
    .confroomFirstTime = 0,
    .coffeeTracker = 0,
    .lastSlideTimer = 0,
#ifdef SLIDE_DEBUG
    .godMode = FALSE,
    .instaGo = FALSE,
#endif
};

#define update_sec(s) (1.0f / (30.0f * (s)))

void process_tutorial(FPVPlayer *player) {
    Vec3f starting_pos = STARTING_POSITION;
    switch  (player->currentTutorial) {
        case 0: // Welcome, movement controls
            if (vec3f_lat_dist(player->pos,starting_pos) > 100.f) {
                if (player->tutorialTimer++ >= 120) {
                    player->curTutorialDone = 1;
                    player->tutorialTimer = 0;
                }
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 1: // Operate coffee machine
            if (player->sipsLeft) {
                player->curTutorialDone = 1;
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 2: // Go to confroom
            if (player->tutorialTimer++ > 120) {
                player->curTutorialDone = 1;
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 3: // Enter presentation
            if (player->actionState == PLAYER_PRESENTING) {
                player->curTutorialDone = 1;
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 4: // Grab a coffee first
            if (player->tutorialTimer++ > 120) {
                player->curTutorialDone = 1;
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 5: // Low energy for the first time
            if (((f32)player->energy / (f32)MAX_ENERGY) >= 0.26f) {
                player->curTutorialDone = 1;
            } else {
                player->curTutorialDone = 0;
            }
            break;
        case 06: // Hide tutorial
            if (player->tutorialTimer++ > 120) {
                player->curTutorialDone = 1;
                gTutorialFinished = 1;
            } else {
                player->curTutorialDone = 0;
            }           
    }
}

void deplete_energy(s32 amt) {
    gFPVPlayer.energy -= amt;
    if (gFPVPlayer.energy < 0) gFPVPlayer.energy = 0;
}

void replenish_energy(s32 amt) {
    gFPVPlayer.energy += amt;
    if (gFPVPlayer.energy > MAX_ENERGY) gFPVPlayer.energy = MAX_ENERGY;
}

static s32 check_and_drink_coffee(FPVPlayer *player) {
    if (
        player->sipsLeft &&
        player->arm &&
        player->arm->oAction != ARM_DRINKING &&
        (player->cont->buttonPressed & PLAYER_BTN_DRINK_COFFEE)
    ) {
        player->arm->oAction = ARM_DRINKING;
        return TRUE;
    }
    return player->arm && player->arm->oAction == ARM_DRINKING;
}

static void update_direction(FPVPlayer *player) {
    if (player->cont->buttonDown & L_CBUTTONS) {
        player->dir[1] += ROT_HZ_SPEED;
    }

    if (player->cont->buttonDown & R_CBUTTONS) {
        player->dir[1] -= ROT_HZ_SPEED;
    }

    if (player->cont->buttonDown & U_CBUTTONS) {
        player->dir[0] += ROT_VT_SPEED;
        if (player->dir[0] > DEGREES(90) - ROT_VT_SPEED) {
            player->dir[0] = DEGREES(90) - ROT_VT_SPEED;
        }
    }

    if (player->cont->buttonDown & D_CBUTTONS) {
        player->dir[0] -= ROT_VT_SPEED;
        if (player->dir[0] < DEGREES(-90) + ROT_VT_SPEED) {
            player->dir[0] = DEGREES(-90) + ROT_VT_SPEED;
        }
    }
}

static s32 get_move_dir(FPVPlayer *player, f32 *moveDir) {
    if (player->cont->stickMag < 1) {
        vec3_zero(moveDir);
        return FALSE;
    }
    Vec3f stickDir = { -player->cont->stickX, 0, player->cont->stickY };
    vec3f_normalize(stickDir);
    f32 magFac = (player->cont->stickMag / 64.0f);
    stickDir[0] *= magFac;
    stickDir[2] *= magFac;
    rotate_in_xz(moveDir, stickDir, player->dir[1]);
    return TRUE;
}

#ifdef SLIDE_DEBUG
static void print_debug_fpv_info(FPVPlayer *player) {
    static char buf[64];
    sprintf(buf, "%4d %4d %4d", roundf(player->pos[0]), roundf(player->pos[1]), roundf(player->pos[2]));
    print_small_text_buffered(20, 40, buf, PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
}
#else
#define print_debug_fpv_info(...)
#endif

static s32 update_intro_cutscene(FPVPlayer *player) {
    player->headPos = PLAYER_EYE_DEFAULT;
    player->crouching = FALSE;
    if (gIntroCutscene == 2) {
        player->actionState = PLAYER_FREE;
    }
    player->headPos = approach_f32(player->headPos, player->crouching ? PLAYER_EYE_CROUCHING : PLAYER_EYE_DEFAULT, 10, 10);
    update_direction(player);

    return FALSE;
}

static s32 update_free(FPVPlayer *player) {
    // this isn't debug anymore!!!!
    if ((player->cont->buttonPressed & PLAYER_BTN_START_PRESENTATION)) {
        if (player->canSit == 1) {
            if (player->coffeeTracker == 1) {
                player->coffeeTracker = 2;
            }
            player->actionState = PLAYER_PRESENTING;
            return FALSE;
        } else if (player->canSit == -1) {
            struct Object *presentationGuy = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvPresentingDudeGuy),5000.f);
            presentationGuy->oAction = PRESENTING_DUDEGUY_TURN_OFF_LIGHTS;
        }
    }

    if (gCurConvo.state != CONVO_INACTIVE) {
        player->actionState = PLAYER_CONVO_QTE;
        return TRUE;
    }
    
    s32 drinkingCoffee = check_and_drink_coffee(player);
    if (drinkingCoffee) player->cont->stickMag /= 6;

    player->dir[2] = 0;

    player->focusPointActive = FALSE;

    update_direction(player);
    Vec3f moveDir;
    s32 tryingToMove = get_move_dir(player, moveDir);
    player->crouching = (player->cont->buttonDown & PLAYER_BTN_CROUCH) ? TRUE : FALSE;
    player->running = (player->cont->buttonPressed & PLAYER_BTN_RUN) ? TRUE : FALSE;
    if (player->running) {
        player->runFac = 1; 
    } else {
        player->runFac = approach_f32_symmetric(player->runFac, 0, 0.1f); 
    }

    f32 floorHeight = find_confroom_floor(player->pos);    
    if (floorHeight > player->pos[1]) {
        player->pos[1] = floorHeight;
    }
    s32 onGround = roundf(player->pos[1]) == roundf(floorHeight);

    if (onGround && tryingToMove) {
        deplete_energy(E_COST_MOVING);

        Vec3f accel;
        vec3_scale_dest(accel, moveDir, PLAYER_MAX_ACCEL);
        vec3f_add(player->vel, accel);

        Vec3f xzVel = { player->vel[0], 0, player->vel[2] };
        f32 curSpeedSq = vec3_sumsq(xzVel);
        f32 maxSpeed = (player->cont->stickMag / 64.0f);
        if (player->crouching) {
            maxSpeed *= PLAYER_MAX_SPEED_CROUCH;
        } else if (player->runFac > 0) {
            maxSpeed *= smoothstop(PLAYER_MAX_SPEED_WALK, PLAYER_MAX_SPEED_RUN, player->runFac);
        } else {
            maxSpeed *= PLAYER_MAX_SPEED_WALK;
        }

        if (curSpeedSq > maxSpeed*maxSpeed) {
            f32 curSpeed = sqrtf(curSpeedSq);
            vec3f_normalize(xzVel);
            curSpeed = approach_f32(curSpeed, maxSpeed, PLAYER_DECEL, PLAYER_DECEL);
            player->vel[0] = xzVel[0] * curSpeed;
            player->vel[2] = xzVel[2] * curSpeed;
        }
    } else if (onGround) {
        deplete_energy(player->crouching ? E_COST_CROUCHING : E_COST_STANDING);
        player->vel[0] = approach_f32(player->vel[0], 0, PLAYER_DECEL, PLAYER_DECEL);
        player->vel[2] = approach_f32(player->vel[2], 0, PLAYER_DECEL, PLAYER_DECEL);
    }

#ifdef PLAYER_BTN_JUMP
    if (
        (player->cont->buttonPressed & PLAYER_BTN_JUMP) &&
        roundf(player->pos[1]) == roundf(floorHeight)
    ) {
        player->vel[1] += PLAYER_JUMP_VEL;
    }
#endif

    player->headPos = approach_f32(player->headPos, player->crouching ? PLAYER_EYE_CROUCHING : PLAYER_EYE_DEFAULT, 10, 10);
    if (player->curSpace && player->curSpace->max_y < player->pos[1] + player->headPos + PLAYER_TOP_DIST_FROM_EYE + 5) {
        player->headPos = player->curSpace->max_y - player->pos[1] - PLAYER_TOP_DIST_FROM_EYE - 5;
    }

    Cylinder hitbox;
    vec3f_copy(hitbox.pos, player->pos);
    vec3f_add(hitbox.pos, player->vel);
    hitbox.pos[1] += 30; // add step threshold
    hitbox.height = player->headPos + PLAYER_TOP_DIST_FROM_EYE;
    hitbox.radius = PLAYER_RADIUS;

    AABB *newSpace = find_space(&hitbox);
    hitbox.pos[1] -= 30;
    // no space found at all, player is completely stuck!
    assert(!(newSpace == NULL && player->curSpace == NULL), "Could not find a player space!");

    if (newSpace != NULL) {
        if (newSpace != player->curSpace) {
            if (hitbox.pos[1] < newSpace->min_y) {
                hitbox.pos[1] = newSpace->min_y;
            }
            player->curSpace = newSpace;
        }
    }

    resolve_exit_space(&hitbox, player->curSpace, player->pos);
    vec3f_copy(hitbox.pos, player->pos);
    resolve_confroom_collisions(&hitbox, player->pos);

    if (player->curSpace->max_y < player->pos[1] + player->headPos + PLAYER_TOP_DIST_FROM_EYE) {
        player->vel[1] = 0;
    }

    floorHeight = find_confroom_floor(player->pos);
    if (player->pos[1] > floorHeight) {
        player->vel[1] -= PLAYER_GRAVITY;
        if (player->vel[1] < PLAYER_TERM_VEL) player->vel[1] = PLAYER_TERM_VEL;
    } else {
        player->pos[1] = floorHeight;
        player->vel[1] = 0;
    }

    print_debug_fpv_info(player);

    return FALSE;   
}

static s32 update_brewing_coffee(FPVPlayer *player) {
    player->focusPointActive = TRUE;
    // TODO: Use confroom_coffeeMachinePos
    // vec3f_copy(player->focusPoint, (f32 *)segmented_to_virtual(confroom_coffeeMachinePos));
    deplete_energy(E_COST_STANDING);
    return FALSE;
}

static s32 update_convo_qte(FPVPlayer *player) {
    // player->focusPointActive = TRUE;
    if (gCurConvo.state == CONVO_INACTIVE) {
        player->actionState = PLAYER_FREE;
        return TRUE;
    }
    // TODO: focus towards convo point
    s16 goal;
    vec3f_get_yaw(player->pos, gCurConvo.speakerPos, &goal);
    player->dir[1] = approach_angle(player->dir[1], goal, DEGREES(10));
    s16 goalPitch = (player->cont->buttonDown & PLAYER_BTN_ZOOM) ? DEGREES(-1) : DEGREES(-12);
    player->dir[0] = approach_angle_asymp(player->dir[0], goalPitch, 0.15f);
    deplete_energy(E_COST_STANDING);
    return FALSE;
}

static void stop_presenting(FPVPlayer *player) {
    player->actionState = PLAYER_FREE;
    player->focusPointActive = FALSE;
    player->crouching = FALSE;
    vec3_zero(player->vel);
}

static s32 update_presenting(FPVPlayer *player) {
    static f32 sittingSpeed = 2.0f;
    f32 *sittingPos = (f32 *)segmented_to_virtual(confroom_cameraPos);
    player->chasingNPC = NULL;

#ifdef SLIDE_DEBUG
    if (player->instaGo) {
        player->pos[0] = sittingPos[0];
        player->pos[1] = sittingPos[1];
        player->pos[2] = sittingPos[2];
        player->instaGo = 0;        
    }
#endif

    check_and_drink_coffee(player);

    if ((player->cont->buttonPressed & PLAYER_BTN_STOP_PRESENTATION) && (gCurrAreaIndex != 19)) {
        player->pos[0] = sittingPos[0];
        player->pos[1] = sittingPos[1];
        player->pos[2] = sittingPos[2];
        stop_presenting(player);
        return FALSE;
    }

    if (player->cont->buttonDown & R_CBUTTONS) {
        player->dir[2] += ROT_TILT_SPEED;
        if (player->dir[2] > ROT_TILT_MAX) player->dir[2] = ROT_TILT_MAX;
    } else if (player->cont->buttonDown & L_CBUTTONS) {
        player->dir[2] -= ROT_TILT_SPEED;
        if (player->dir[2] < -ROT_TILT_MAX) player->dir[2] = -ROT_TILT_MAX;
    } else {
        player->dir[2] = approach_angle(player->dir[2], 0, ROT_TILT_SPEED);
    }

    vec3f_copy(player->focusPoint, (f32 *)segmented_to_virtual(confroom_projScreenPos));
    
    player->pos[0] = approach_f32(player->pos[0], sittingPos[0], sittingSpeed, sittingSpeed);
    player->pos[1] = approach_f32(player->pos[1],             0, sittingSpeed, sittingSpeed);
    player->pos[2] = approach_f32(player->pos[2], sittingPos[2], sittingSpeed, sittingSpeed);

    vec3_get_angle(player->pos, player->focusPoint, &player->dir[0], &player->dir[1]);
    player->focusPointActive = TRUE;
    player->headPos = approach_f32(player->headPos, PLAYER_EYE_SITTING, 6, 6);

    deplete_energy(E_COST_PRESENTING);
    return FALSE;
}

void init_player(void) {
    FPVPlayer *player = &gFPVPlayer;
    // save these, might already be set
    struct Object *arm = player->arm;
    struct Object *coffeeCup = player->coffeeCup;

    gFPVPlayer = sInitFPVPlayerState;

    // reattach these
    player->arm = arm;
    player->coffeeCup = coffeeCup;

    player->cont = gPlayer1Controller;
    player->actionState = PLAYER_INTRO_CUTSCENE;

    confroom_initialize_collision();

    Cylinder hitbox;
    vec3f_copy(hitbox.pos, player->pos);
    hitbox.height = PLAYER_TOP_DIST_FROM_EYE;
    hitbox.radius = PLAYER_RADIUS;

    player->curSpace = find_space(&hitbox);
}

void update_cam_from_player(FPVPlayer *player, FPVCamState *cam) {
    static f32 presentationFac = 0;
    static f32 armDrinkingFac = 0;

    vec3f_copy_y_off(cam->pos, player->pos, player->headPos);
    vec3s_copy(cam->dir, player->dir);
    if (player->dir[2] != 0) {
        f32 waistY = cam->pos[1] - PLAYER_WAIST_DIST_FROM_EYE;
        cam->pos[1] = waistY;

        register f32 sy = sins(player->dir[2]);
        register f32 cy = coss(player->dir[2]);

        cam->pos[0] += sy * PLAYER_WAIST_DIST_FROM_EYE;
        cam->pos[1] += cy * PLAYER_WAIST_DIST_FROM_EYE;
    }
    if (player->dir[0] < DEGREES(-90) || player->dir[0] > DEGREES(90)) {
        cam->dir[2] += DEGREES(180);
    }
    player->focusPointFac = approach_f32(player->focusPointFac, (f32)player->focusPointActive, update_sec(0.5f), update_sec(0.5f));

    vec3_set_dist_and_angle(cam->pos, cam->focus, 1000, player->dir[0], player->dir[1]);
    f32 fac = smoothstop(0, 1, player->focusPointFac);
    approach_vec3f_asymptotic(cam->focus, player->focusPoint, fac, fac, fac);

    if (player->actionState == PLAYER_PRESENTING) {
        cam->fov = approach_f32_asymptotic(cam->fov, 30, 0.15f);
    } else {
        f32 goalFov = FOV_24MM;
        s32 isZooming = (player->cont->buttonDown & PLAYER_BTN_ZOOM);
        if (player->actionState ==  PLAYER_CONVO_QTE) {
            goalFov = isZooming ? FOV_75MM : FOV_24MM;
        } else if (isZooming && player->actionState ==  PLAYER_FREE) {
            goalFov = FOV_50MM;
        }
        cam->fov = approach_f32_asymptotic(cam->fov, goalFov, 0.15f);
    }

    if (player->arm) {
        if (player->actionState == PLAYER_PRESENTING) {
            presentationFac = approach_f32_symmetric(presentationFac, 1, 0.075f);
        } else {
            presentationFac = approach_f32_symmetric(presentationFac, 0, 0.075f);
        }

        if (player->arm->oAction == ARM_DRINKING) {
            armDrinkingFac = approach_f32_symmetric(armDrinkingFac, 1, 0.15f);
        } else {
            armDrinkingFac = approach_f32_symmetric(armDrinkingFac, 0, 0.1f);
        }

        f32 *armPos = &player->arm->oPosX;
        Vec3f armOffset = { 0, 0, 0 };
        vec3_set_dist_and_angle(gVec3fZero, armOffset, 22, 0, cam->dir[1] + DEGREES(-90));
        vec3f_sum(armPos, cam->pos, armOffset);

        f32 armYOffset = lerpf(0, -12, presentationFac);
        armPos[0] -= armYOffset * 0.5f;
        armPos[1] += armYOffset;
        armPos[2] -= armYOffset;

        player->arm->oFaceAnglePitch = approach_angle_asymp(
            -cam->dir[0],
            approach_angle_asymp(DEGREES(4.5f), 0, smoothstep(0, 1, armDrinkingFac)),
            presentationFac
        );
        player->arm->oFaceAngleYaw = approach_angle_asymp(
            cam->dir[1],
            approach_angle_asymp(DEGREES(-4), 0, smoothstep(0, 1, armDrinkingFac)),
            presentationFac
        );
        player->arm->oFaceAngleRoll = cam->dir[2];
    }
}

void update_ending_cam(void) {
    static s16 movement1 = 0;
    static s16 movement2 = DEGREES(90);
    static s16 movement3 = DEGREES(70);

    FPVCamState *cam = &gFPVCam;
    if (gOfficeState.pauseTimer > ENDING_OCEAN_START) {
        movement1 += DEGREES(1);
        movement2 += DEGREES(2.27f);
        movement3 += DEGREES(5.2f);
        f32 *camPos = (f32 *)segmented_to_virtual(EndingCamera);
        f32 *camTarget = (f32 *)segmented_to_virtual(EndingCameraTarget);

        f32 offset1 = sins(movement1) * 0.5 + sins(movement2) * 0.3f + sins(movement3) * 0.2f;
        f32 offset2 = coss(movement1) * 0.5 + coss(movement2) * 0.3f + coss(movement3) * 0.2f;

        offset1 *= 5.0f;
        offset2 *= 5.0f;

        vec3f_copy(cam->pos, camPos);
        cam->pos[0] += offset1;
        cam->pos[1] += offset1;
        cam->pos[2] += offset2;

        vec3f_copy(cam->focus, camTarget);
        cam->pos[0] += offset2;
        cam->pos[1] += offset2;
        cam->pos[2] += offset1;

        f32 fovFac = remap(CLAMP(gOfficeState.pauseTimer, ENDING_OCEAN_START, ENDING_OCEAN_END), ENDING_OCEAN_START, ENDING_OCEAN_END, 0, 1);
        cam->fov = smoothstop(FOV_50MM, FOV_100MM, fovFac);
    }
}

#define STAGE_2_AREA_THRESHOLD 6
#define STAGE_3_AREA_THRESHOLD 12

// returns TRUE if gameplay is active
s32 update_player(void) {
    static s32 didInit = FALSE;
    if (!didInit) {
        init_player();
        didInit = TRUE;
    }

    FPVPlayer *player = &gFPVPlayer;

    if ((gMarioState != NULL) && (gMarioState->action != ACT_STAR_DANCE_WATER) && (gMarioState->action != ACT_STAR_DANCE_NO_EXIT) && (gMarioState->action != ACT_STAR_DANCE_EXIT)) {
        
    }
    if (
        (player->cont->buttonPressed & START_BUTTON) &&
        (!gOfficeState.paused || gOfficeState.paused == PAUSE_STATE_PAUSED)
    ) {
        if (
            player->actionState == PLAYER_PRESENTING && (
                (gMarioState->action == ACT_STAR_DANCE_WATER) ||
                (gMarioState->action == ACT_STAR_DANCE_NO_EXIT) ||
                (gMarioState->action == ACT_PULLING_DOOR) ||
                (gMarioState->action == ACT_PUSHING_DOOR) ||
                (gMarioState->action == ACT_STAR_DANCE_EXIT)
            )
        ) {
            // dooo??????
        } else {
            gOfficeState.paused = !gOfficeState.paused;
        }
    }

    if (gOfficeState.paused) {
        if (gOfficeState.paused == PAUSE_STATE_END) update_ending_cam();
        return FALSE;
    }

    update_convo();

    s32 continueUpdate = TRUE;
    while (continueUpdate) {
        switch (player->actionState) {
            case PLAYER_INTRO_CUTSCENE:
                continueUpdate = update_intro_cutscene(player);
            #ifdef SLIDE_DEBUG
                print_small_text_buffered(20, 8, "intro", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            #endif
                break;
            case PLAYER_FREE:
                continueUpdate = update_free(player);
            #ifdef SLIDE_DEBUG
                print_small_text_buffered(20, 8, "free", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            #endif
                break;
            case PLAYER_BREWING_COFFEE:
                continueUpdate = update_brewing_coffee(player);
            #ifdef SLIDE_DEBUG
                print_small_text_buffered(20, 8, "coffee", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            #endif
                break;
            case PLAYER_PRESENTING:
                continueUpdate = update_presenting(player);
            #ifdef SLIDE_DEBUG
                print_small_text_buffered(20, 8, "presenting", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            #endif
                break;
            case PLAYER_CONVO_QTE: {
                continueUpdate = update_convo_qte(player);
            #ifdef SLIDE_DEBUG
                print_small_text_buffered(20, 8, "qte", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
            #endif
                break;
            }
        }
#ifdef SLIDE_DEBUG
        if (player->godMode) {
            print_small_text_buffered(20, 18, "godmode", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
        }

        switch (gOfficeState.stage) {
            case OFFICE_STAGE_INTRO:
                print_small_text_buffered(20, 28, "stage intro", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case OFFICE_STAGE_1:
                print_small_text_buffered(20, 28, "stage 1", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case OFFICE_STAGE_2:
                print_small_text_buffered(20, 28, "stage 2", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case OFFICE_STAGE_3:
                print_small_text_buffered(20, 28, "stage 3", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
        }
#endif
    }

    // Respawning info during first time presenting
    if (player->actionState == PLAYER_PRESENTING && !player->hasRespawned) {
        print_small_text_buffered(100, SCREEN_HEIGHT - 20, "Press START anytime to retry current slide",PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
        if (gPlayer1Controller->buttonPressed & START_BUTTON) {
            player->hasRespawned = TRUE;
        }
    }
    // Don't let the player into the confroom during the intro if they don't have a coffee
    if ((!player->sipsLeft) && (gOfficeState.stage == OFFICE_STAGE_INTRO) && ((player->pos[2] < 250.f))) { 
        player->pos[2] = 250.f;
        player->currentTutorial = 4;
        player->tutorialTimer = 0;
        player->curTutorialDone = 0;
    } 

    //Hardcode energy during intro, before the first coffee
    if (gOfficeState.stage == OFFICE_STAGE_INTRO) { 
        if ((!player->sipsLeft) && (!player->firstCoffee)) {
            player->energy = 200;
        }
        if (player->sipsLeft) {
            player->firstCoffee = 1;
        }
    }

    if ((player->currentTutorial >= 0) && (!gTutorialFinished)) {
        process_tutorial(player);
    }
    update_cam_from_player(player, &gFPVCam);

    /*if (point_in_aabb_2d(&gOfficeSpaces[0], gFPVPlayer.pos)) {
        gConfroomLights = 0;
    } else {
        gConfroomLights = 1;
    }*/

    // Immediately end the tutorial if the player presses Dpad down at the start
    if ((player->currentTutorial == 0) && (gPlayer1Controller->buttonPressed & D_JPAD)) {
        player->currentTutorial = 6;
    }

    // Increase stage every time the player returns with coffee
    if (player->coffeeTracker == 2) {
        if (gOfficeState.stage < OFFICE_STAGE_3) {
            gOfficeState.stage += 1;
            player->coffeeTracker = 0;
        }
    }
    // Force stage 3 if the player gets to a certain slide
    if ((gCurrAreaIndex >= STAGE_3_AREA_THRESHOLD) && (gOfficeState.stage < OFFICE_STAGE_3)) {
        gOfficeState.stage = OFFICE_STAGE_3;
    }

    // Is the player in the conference room?
     if (!(point_in_aabb_2d(&gOfficeSpaces[0], gFPVPlayer.pos))) {
        player->inConfroom = 0;
     } else {
        player->inConfroom = 1;
        player->confroomFirstTime = 1;
        player->chasingNPC = NULL;
     }

    // Get fired IDIOT
    if (player->energy == 0) {
        gOfficeState.paused = PAUSE_STATE_FIRED;
    }

    // Get woken up IDIOT
    if (gCurrAreaIndex == 19) {
        player->lastSlideTimer++;
        player->energy = MAX_ENERGY;
        if (player->lastSlideTimer >= 300 ) {
            gOfficeState.paused = PAUSE_STATE_END;
        }
    }

#ifdef SLIDE_DEBUG
    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        player->godMode ^= 1;
        // gOfficeState.paused = PAUSE_STATE_END;
    }
    if (player->godMode) {
        player->energy = MAX_ENERGY;
    }

    if (gPlayer1Controller->buttonPressed & U_JPAD) {
        gFPVPlayer.actionState = PLAYER_PRESENTING;
        gFPVPlayer.instaGo = 1;
    }

    if (gPlayer1Controller->buttonPressed & L_JPAD) {
        start_previous_office_stage();
    } else if (gPlayer1Controller->buttonPressed & R_JPAD) {
        start_next_office_stage();
    }
#endif
    
    return player->actionState == PLAYER_PRESENTING;
}

#define BAR_HEIGHT 8
#define BAR_MARGIN 8
#define BAR_RIGHT ((SCREEN_WIDTH / 3) - BAR_MARGIN)
#define BAR_TOP (SCREEN_HEIGHT - (BAR_MARGIN + BAR_HEIGHT))

void render_player_hud(Gfx **head) {
    if (gOfficeState.paused == PAUSE_STATE_END) return;

    if (gCurrAreaIndex == 19) return;

    FPVPlayer *player = &gFPVPlayer;
    Gfx *gfx = *head;
    s32 xEnergyWidth = roundf(remap(player->energy, 0, MAX_ENERGY, 0, (SCREEN_WIDTH / 3) - BAR_MARGIN));
    s32 r, g, b;
    f32 energyPercent = ((f32)player->energy / (f32)MAX_ENERGY);
    if (energyPercent < 0.30f) {
        if (player->energyLowFirstTime == 0) {
            player->energyLowFirstTime = 1;
        }
        r = CLAMP(255 * (energyPercent * 5.0f),0,255);
        g = 0;
        b = 0;
    } else if (energyPercent < 0.5f) {
        r = 255;
        g = 255;
        b = 127 * energyPercent;
    } else if (energyPercent < 0.6f) {
        r = 127 * energyPercent;
        g = 255;
        b = 0;
    } else {
        r = 50 * energyPercent;
        g = 255;
        b = 50 * energyPercent;
    }
    render_rect(&gfx, BAR_MARGIN, BAR_TOP, xEnergyWidth, BAR_HEIGHT, r, g, b, TRUE);
    render_rect_xlu(&gfx, xEnergyWidth + BAR_MARGIN, BAR_TOP, BAR_RIGHT - xEnergyWidth, BAR_HEIGHT, r, g, b, 50, TRUE);

    *head = gfx;
}
