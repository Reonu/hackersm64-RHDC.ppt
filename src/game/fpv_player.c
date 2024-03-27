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

#define STARTING_POSITION { -355.f, 2.0f, 2720.f }

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
    .sipsLeft = 0,
    // .sipsLeft = 3,
    .coffeeStolen = FALSE,
    .canSit = FALSE,
    .hasRespawned = FALSE,
#ifdef SLIDE_DEBUG
    .godMode = FALSE,
    .instaGo = FALSE,
#endif
};

#define update_sec(s) (1.0f / (30.0f * (s)))

void deplete_energy(s32 amt) {
    gFPVPlayer.energy -= amt;
    if (gFPVPlayer.energy < 0) gFPVPlayer.energy = 0;
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
    }

    if (player->cont->buttonDown & D_CBUTTONS) {
        player->dir[0] -= ROT_VT_SPEED;
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
    if ((player->cont->buttonPressed & PLAYER_BTN_START_PRESENTATION) && (player->canSit)) {
        player->actionState = PLAYER_PRESENTING;
        return FALSE;
    }

    if (gCurConvo.state != CONVO_INACTIVE) {
        player->actionState = PLAYER_CONVO_QTE;
        return TRUE;
    }

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

#ifdef SLIDE_DEBUG
    if (player->instaGo) {
        player->pos[0] = sittingPos[0];
        player->pos[1] = sittingPos[1];
        player->pos[2] = sittingPos[2];
        player->instaGo = 0;        
    }
#endif

    if (player->cont->buttonPressed & PLAYER_BTN_STOP_PRESENTATION) {
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
    static s32 init = FALSE;
    if (init) return;
    init = TRUE;
    FPVPlayer *player = &gFPVPlayer;
    player->cont = gPlayer1Controller;
    // TODO: Use correct initial action state
    player->actionState = PLAYER_INTRO_CUTSCENE;
    // player->actionState = PLAYER_FREE;
    // TODO: copy init position from group0 data export

    confroom_initialize_collision();

    Cylinder hitbox;
    vec3f_copy(hitbox.pos, player->pos);
    hitbox.height = PLAYER_TOP_DIST_FROM_EYE;
    hitbox.radius = PLAYER_RADIUS;

    player->curSpace = find_space(&hitbox);
}

void update_cam_from_player(FPVPlayer *player, FPVCamState *cam) {
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
            goalFov = isZooming ? FOV_75MM : FOV_35MM;
        } else if (isZooming && player->actionState ==  PLAYER_FREE) {
            goalFov = FOV_50MM;
        }
        cam->fov = approach_f32_asymptotic(cam->fov, goalFov, 0.15f);
    }

    if (player->arm) {
        f32 *armPos = &player->arm->oPosX;
        Vec3f armOffset = { 0, 0, 0 };
        vec3_set_dist_and_angle(gVec3fZero, armOffset, 22, 0, cam->dir[1] + DEGREES(-90));
        vec3f_sum(armPos, cam->pos, armOffset);

        player->arm->oFaceAnglePitch = -cam->dir[0];
        player->arm->oFaceAngleYaw = cam->dir[1];
        player->arm->oFaceAngleRoll = cam->dir[2];
    }
}

// returns TRUE if gameplay is active
s32 update_player(void) {
    init_player();
    update_convo();

    FPVPlayer *player = &gFPVPlayer;

    s32 continueUpdate = TRUE;
    while (continueUpdate) {
        switch (player->actionState) {
            case PLAYER_INTRO_CUTSCENE:
                continueUpdate = update_intro_cutscene(player);
                print_small_text_buffered(20, 8, "intro", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case PLAYER_FREE:
                continueUpdate = update_free(player);
                print_small_text_buffered(20, 8, "free", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case PLAYER_BREWING_COFFEE:
                continueUpdate = update_brewing_coffee(player);
                print_small_text_buffered(20, 8, "coffee", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case PLAYER_PRESENTING:
                continueUpdate = update_presenting(player);
                print_small_text_buffered(20, 8, "presenting", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
                break;
            case PLAYER_CONVO_QTE: {
                continueUpdate = update_convo_qte(player);
                print_small_text_buffered(20, 8, "qte", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
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

    if (player->actionState == PLAYER_PRESENTING && !player->hasRespawned) {
        print_small_text_buffered(100, SCREEN_HEIGHT - 20, "Press START anytime to retry current slide",PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_VANILLA);
        if (gPlayer1Controller->buttonPressed & START_BUTTON) {
            player->hasRespawned = TRUE;
        }
    }
    

    update_cam_from_player(player, &gFPVCam);

#ifdef SLIDE_DEBUG
    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        player->godMode ^= 1;
        gConfroomLights ^= 1;
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
    FPVPlayer *player = &gFPVPlayer;
    Gfx *gfx = *head;
    s32 xEnergyWidth = roundf(remap(player->energy, 0, MAX_ENERGY, 0, (SCREEN_WIDTH / 3) - BAR_MARGIN));
    s32 r, g, b;
    f32 energyPercent = ((f32)player->energy / (f32)MAX_ENERGY);
    if (energyPercent < 0.25f) {
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
