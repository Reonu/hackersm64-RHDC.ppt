#include "game/include_for_vsc.h"
#include "game/print.h"
#include "game/object_helpers.h"
#include "include/model_ids.h"
#include "engine/math_util.h"
#include "game/emutest.h"
#include "game/fpv_player.h"

enum CoffeeCupActions {
    COFFEE_CUP_INVISIBLE,
    COFFEE_CUP_EMPTY,
    COFFEE_CUP_HALF,
    COFFEE_CUP_FULL,
};

u32 cathy_lead_ins[] = {
    SOUND_CATHY_LEAD_IN_GET_A_LOAD_OF_THIS,
    SOUND_CATHY_LEAD_IN_HAPPY_HOUR,
    SOUND_CATHY_LEAD_IN_KELLY_WAS_LIKE,
    SOUND_CATHY_LEAD_IN_MITCHELL,
    SOUND_CATHY_LEAD_IN_YOURE_NEVER_GONNA_BELIEVE,
};
u32 cathy_mumbles[] = {
    SOUND_CATHY_MUMBLE_AND_ILL_BE_NICE,
    SOUND_CATHY_MUMBLE_FOO_FOO_CACHOO,
    SOUND_CATHY_MUMBLE_PADDLE_DRUN,
    SOUND_CATHY_MUMBLE_UGH_RIGHT,
};
u32 cathy_questions[] = {
    SOUND_CATHY_QUESTION_PLAN_STAN,
    SOUND_CATHY_QUESTION_WAIT_SHH_HES_COMING,
    SOUND_CATHY_QUESTION_WHAT_ABOUT_YO,
    SOUND_CATHY_QUESTION_WHAT_SAY_YOU,
};
u32 cathy_coffee_steals[] = {
    SOUND_CATHY_COFFEE_STEAL_HOWD_YOU_KNOW,
    SOUND_CATHY_COFFEE_STEAL_ILL_TAKE_THAT,
    SOUND_CATHY_COFFEE_STEAL_SUCH_A_SWEETHEART,
};
u32 cathy_coffee_sips[] = {
    SOUND_CATHY_COFFEE_SIP_AHHH,
    SOUND_CATHY_COFFEE_SIP_MMM,
    SOUND_CATHY_COFFEE_SIP_GOOD_MORNIN,
};

u32 matty_lead_ins[] = {
    SOUND_MATTY_LEAD_IN_BANDWIDTH,
    SOUND_MATTY_LEAD_IN_LOW_HANGIN_FRUIT,
    SOUND_MATTY_LEAD_IN_RUN_PAST_YOU,
    SOUND_MATTY_LEAD_IN_THERE_YOU_ARE,
    SOUND_MATTY_LEAD_IN_WHEELHOUSE,
};
u32 matty_mumbles[] = {
    SOUND_MATTY_MUMBLE_DUCKS_IN_A_ROW,
    SOUND_MATTY_MUMBLE_FIRST_AND_FOREMOST,
    SOUND_MATTY_MUMBLE_SYNERGY,
    SOUND_MATTY_MUMBLE_THE_NEW_NORMAL,
};
u32 matty_questions[] = {
    SOUND_MATTY_QUESTION_CIRCLE_BACK,
    SOUND_MATTY_QUESTION_HIT_ME,
    SOUND_MATTY_QUESTION_HOWS_THAT_SOUND,
    SOUND_MATTY_QUESTION_TRIGGER,
};
u32 matty_coffee_steals[] = {
    SOUND_MATTY_COFFEE_STEAL_GOT_MY_BACK,
    SOUND_MATTY_COFFEE_STEAL_PROMOTION,
    SOUND_MATTY_COFFEE_STEAL_I_LOVE_YOU,
};
u32 matty_coffee_sips[] = {
    SOUND_MATTY_COFFEE_SIP_MMM,
    SOUND_MATTY_COFFEE_SIP_OOH,
    SOUND_MATTY_COFFEE_SIP_SMACK,
};

static void _play_random_sound_from_array(u32 *soundArray, s32 length) {
    // cur_obj_play_sound_2(soundArray[(s32)(random_float() * length)]);
    cur_obj_play_sound_2(soundArray[random_u16() % length]);
}
#define play_random_sound_from_array(soundArray) _play_random_sound_from_array(soundArray, ARRAY_COUNT(soundArray))

static void play_character_lead_in(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_lead_ins);
    } else {
        play_random_sound_from_array(matty_lead_ins);
    }
}
static void play_character_mumble(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_mumbles);
    } else {
        play_random_sound_from_array(matty_mumbles);
    }
}
static void play_character_question(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_questions);
    } else {
        play_random_sound_from_array(matty_questions);
    }
}
static void play_character_coffee_steal(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_coffee_steals);
    } else {
        play_random_sound_from_array(matty_coffee_steals);
    }
}
static void play_character_coffee_sip(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_coffee_sips);
    } else {
        play_random_sound_from_array(matty_coffee_sips);
    }
}


void bhv_dudeguy_init(void) {
    o->oAnimationIndex = BPARAM1;

    if (o->oAnimationIndex == NPC_ANIM_EATING && gOfficeState.checkpoint != 0) {
        o->oEatingGuySpoke = 1;
    }
    /*if (gEmulator == EMU_CONSOLE) {
        o->oDrawingDistance = 2000.f;
    } else {
        o->oDrawingDistance = 100.f;
    }*/
}

void bhv_dudeguy_loop(void) {

    switch (BPARAM4) {
        case ACTIVE_CONFROOM:
            if (!gFPVPlayer.inConfroom) {
                o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;   
                return;             
            } else {
                o->activeFlags |= ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;                
            }
            break;
        case ACTIVE_OFFICE:
            if (gFPVPlayer.inConfroom) {
                o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
                return;              
            } else {
                o->activeFlags |= ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;                
            }            
            break;
    }    

    if (gFPVPlayer.actionState == PLAYER_INTRO_CUTSCENE) {
        o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
        return;    
    } else if (!BPARAM4) {
        o->activeFlags |= ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;          
    }

    cur_obj_init_animation(o->oAnimationIndex);

    if (o->oTimer == 0)
        o->header.gfx.animInfo.animFrame = (random_u16() % o->header.gfx.animInfo.curAnim->loopEnd);

    if (BPARAM1 == NPC_ANIM_EATING && !o->oEatingGuySpoke) {
        if (gFPVPlayer.sipsLeft) {
            o->oAnimationIndex = NPC_ANIM_EATING_TALKING;
            if (cur_obj_check_if_at_animation_end()) {
                gFPVPlayer.currentTutorial = 2;
                o->oEatingGuySpoke = 1;
                o->oAnimationIndex = NPC_ANIM_EATING;
            } else if (o->header.gfx.animInfo.animFrame == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_DONE_MAKING_COFFEE);
            }
        }
    }

    /*if (gPlayer1Controller->buttonPressed & R_JPAD) {
        o->oAnimationIndex++;
    } else if (gPlayer1Controller->buttonPressed & L_JPAD) {
        o->oAnimationIndex--;
    }*/
}



void bhv_presenting_dudeguy_init(void) {
    o->oAnimationIndex = NPC_ANIM_IDLE;
    if (gOfficeState.checkpoint != 0) {
        o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_COME_BACK;
    } else {
        o->oAction = PRESENTING_DUDEGUY_START;
    }
    cur_obj_init_animation(o->oAnimationIndex);
}

void bhv_presenting_dudeguy_loop(void) {
    switch (o->oAction) {
        case PRESENTING_DUDEGUY_START:
            o->oAnimationIndex = NPC_ANIM_IDLE;
            if (gFPVPlayer.inConfroom) {
                o->oAction = PRESENTING_DUDEGUY_WALKING_IN_FIRST_TIME;
            }
            break;
        case PRESENTING_DUDEGUY_WALKING_IN_FIRST_TIME:
            o->oAnimationIndex = NPC_ANIM_WALKING_IN;
            if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_WALKING_IN);
            } else if (cur_obj_check_if_at_animation_end()) {
                o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_SIT;
            }
            break;
        case PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_SIT:
            o->oAnimationIndex = NPC_ANIM_IDLE;
            if (gFPVPlayer.actionState == PLAYER_PRESENTING) {
                o->oAction = PRESENTING_DUDEGUY_PRESENTING;
            }
            break;
        case PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_LEAVE_CONFROOM:
            o->oAnimationIndex = NPC_ANIM_IDLE;
            if (!gFPVPlayer.inConfroom) {
                o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_COME_BACK;
            } else if (gFPVPlayer.actionState == PLAYER_PRESENTING) {
                o->oAction = PRESENTING_DUDEGUY_PRESENTING;
            }
            break;
        case PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_COME_BACK:
            o->oAnimationIndex = NPC_ANIM_IDLE;
            if (gFPVPlayer.inConfroom) {
                o->oAction = PRESENTING_DUDEGUY_WALKING_IN_AGAIN;
            }
            break;
        case PRESENTING_DUDEGUY_PRESENTING:
            o->oAnimationIndex = NPC_ANIM_PRESENTING;
            if (gFPVPlayer.actionState != PLAYER_PRESENTING) {
                o->oAction = PRESENTING_DUDEGUY_GET_UP;
            } else if (gCurrAreaIndex == 19) {
                o->oAction = PRESENTING_DUDEGUY_ENDING;
            }
            break;
        case PRESENTING_DUDEGUY_GET_UP:
            o->oAnimationIndex = NPC_ANIM_TALKING_GENERIC;
            if (o->oTimer > 97) {
                o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_LEAVE_CONFROOM;
            } else if (gFPVPlayer.actionState == PLAYER_PRESENTING) {
                o->oAction = PRESENTING_DUDEGUY_PRESENTING;
            } else if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_YOU_NEED_COFFEE_NOW);
            }
            break;
        case PRESENTING_DUDEGUY_WALKING_IN_AGAIN:
            o->oAnimationIndex = NPC_ANIM_TALKING_GENERIC;
            if (o->oTimer > 87) {
                o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_SIT;
            } else if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_OPE_YOURE_BACK);
            }
            break;
        case PRESENTING_DUDEGUY_TURN_OFF_LIGHTS:
            o->oAnimationIndex = NPC_ANIM_TALKING_GENERIC;
            if (o->oTimer > 116) {
                o->oAction = PRESENTING_DUDEGUY_WAITING_FOR_PLAYER_TO_SIT;
            } else if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_TURN_THE_LIGHTS_OFF);
            } else if (gFPVPlayer.actionState == PLAYER_PRESENTING) {
                o->oAction = PRESENTING_DUDEGUY_PRESENTING;
            }
            break;
        case PRESENTING_DUDEGUY_ENDING:
            o->oAnimationIndex = NPC_ANIM_TALKING;
            if (o->oTimer == 1) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_END_OF_PRESENTATION);
            }
            break;
    }
    if (!gFPVPlayer.inConfroom) {
        o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;   
        return;             
    } else {
        o->activeFlags |= ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;                
    }
    //print_text_fmt_int(20, 20, "%d", o->oAction);
    cur_obj_init_animation(o->oAnimationIndex);
}

#define SPLINE_GUY_WALK_SPEED               meters_sec(1.6f)
#define SPLINE_GUY_SPRINT_SPEED             meters_sec(3.05f)
#define SPLINE_GUY_TURNING_SPEED            DEGREES(3)
#define SPLINE_GUY_TURNING_SPEED_RUNNING    DEGREES(6)
#define SPLINE_GUY_CHASE_SPEED_STAGE_1      meters_sec(1.95f)
#define SPLINE_GUY_CHASE_SPEED_STAGE_2      meters_sec(2.1f)
#define SPLINE_GUY_NEXT_POINT_THRESHOLD     160
#define SPLINE_GUY_PLAYER_START_CONVO_DIST  80
#define SPLINE_GUY_PLAYER_START_CHASE_DIST  360
#define SPLINE_GUY_PLAYER_START_CHASE_DIST_BEHIND  260
#define SPLINE_GUY_PLAYER_STOP_CHASE_DIST   500
#define SPLINE_GUY_STEAL_SIP_START          102
#define SPLINE_GUY_STEAL_SIP_END            193

void bhv_spline_dudeguy_init(void) {
    o->oAnimationIndex = NPC_ANIM_WALKING;
    o->oAction = SPLINE_GUY_WALKING;
    o->oSplineDudeGuyPointIndex = 0;
    o->oForwardVel = SPLINE_GUY_WALK_SPEED;

    if (BPARAM4) {
        o->oDudeGuyRoomsActive = BPARAM4;
    }

    ConfroomObjectSplineRef *spline = &gConfroomSplines[BPARAM2];
    s32 pIndex = o->oSplineDudeGuyPointIndex;
    f32 *curPoint = spline->points[pIndex];
    f32 *nextPoint = spline->points[pIndex + 1];
    o->oFaceAngleYaw = atan2s(nextPoint[2] - curPoint[2], nextPoint[0] - curPoint[0]);
    vec3f_copy(&o->oPosX, curPoint);
}

// returns TRUE if it should break
s32 spline_guy_player_interact(f32 *pos, f32 playerDist) {
    if (gCurConvo.coolDownTimer == 0 && playerDist < SPLINE_GUY_PLAYER_START_CONVO_DIST) {
#ifdef SLIDE_DEBUG
        if (gFPVPlayer.sipsLeft && !gFPVPlayer.godMode) { 
#else
        if (gFPVPlayer.sipsLeft) { 
#endif

            gFPVPlayer.sipsLeft = 0;
            gFPVPlayer.coffeeTracker = 0;
            gFPVPlayer.coffeeStolen = TRUE;
            gFPVPlayer.chasingNPC = NULL;
            o->oAction = SPLINE_GUY_STOLE_COFFEE;
            o->oSubAction = 0;
            o->oOldAngle = o->oFaceAngleYaw;
            o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
            o->oForwardVel = 0;
            play_character_coffee_steal(cur_obj_has_model(MODEL_CHATTY_KATHY));
            return TRUE;
        } else {
            Vec3f speakerPos = { o->oPosX, o->oPosY + 180, o->oPosZ };
            if (start_convo(speakerPos)) {
                gFPVPlayer.chasingNPC = NULL;
                o->oAction = SPLINE_GUY_CONVERSATION;
                o->oOldAngle = o->oFaceAngleYaw;
                o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
                o->oForwardVel = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void spline_guy_update_position(s16 goalAngle, f32 patrolSpeed, f32 turningSpeed, s32 resolveCollisions) {
    f32 *pos = &o->oPosX;
    f32 *vel = &o->oVelX;
    o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw, goalAngle, turningSpeed);
    if (o->oForwardVel < patrolSpeed) {
        o->oForwardVel += patrolSpeed * 0.1f;
    }
    if (o->oForwardVel > patrolSpeed) {
        o->oForwardVel = patrolSpeed;
    }
    vel[0] = sins(o->oFaceAngleYaw) * o->oForwardVel;
    vel[2] = coss(o->oFaceAngleYaw) * o->oForwardVel;
    vec3f_add(pos, vel);
    if (!resolveCollisions) return;
    Cylinder hitbox;
    vec3f_copy(hitbox.pos, pos);
    hitbox.height = 160.0f; // idk lol
    hitbox.radius = PLAYER_RADIUS;
    resolve_exit_space(&hitbox, &gOfficeSpaces[2], pos);
    vec3f_copy(hitbox.pos, pos);
    resolve_confroom_collisions(&hitbox, pos);
}

s32 should_chase_player(f32 *pos, f32 playerDist) {
    if (
        gOfficeState.stage == OFFICE_STAGE_INTRO ||
        gFPVPlayer.curSpace != &gOfficeSpaces[2]
    ) {
        return FALSE;
    }

    s16 angleToPlayer;
    vec3f_get_yaw(pos, gFPVPlayer.pos, &angleToPlayer);
    f32 chaseDist = SPLINE_GUY_PLAYER_START_CHASE_DIST;
    if (abs_angle_diff(angleToPlayer, o->oFaceAngleYaw) > DEGREES(89)) {
        chaseDist = SPLINE_GUY_PLAYER_START_CHASE_DIST_BEHIND;
    }

    return playerDist < chaseDist;
}

s32 closer_than_other_chasing_npc(f32 playerDist) {
    if (!gFPVPlayer.chasingNPC) return TRUE;

    f32 otherDist = vec3f_lat_dist(&gFPVPlayer.chasingNPC->oPosX, gFPVPlayer.pos);

    return playerDist < otherDist;
}

#define SPLINE_STOPPER_ANGLE DEGREES((o->oStopperObject->oBehParams >> 16) & 0xFF)

extern s32 cur_obj_play_sound_at_anim_range(s8 startFrame1, s8 startFrame2, u32 sound);

void bhv_spline_dudeguy_loop(void) {
    ConfroomObjectSplineRef *spline = &gConfroomSplines[BPARAM2];
    s32 pIndex = o->oSplineDudeGuyPointIndex;
    f32 *pos = &o->oPosX;

    f32 playerDist = vec3f_lat_dist(pos, gFPVPlayer.pos);
    f32 patrolSpeed;
    f32 turningSpeed;

    if ((BPARAM3 > 0) && (BPARAM3 == gOfficeState.stage)) {
        cur_obj_set_model(MODEL_CHATTY_KATHY);
    } else {
        cur_obj_set_model(MODEL_DUDEGUY);
    }

    switch (BPARAM4) {
        case ACTIVE_CONFROOM:
            if (!gFPVPlayer.inConfroom) {
                o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
                return;                
            } else {
                o->activeFlags |= ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;                
            }
            break;
        case ACTIVE_OFFICE:
            if (gFPVPlayer.inConfroom) {
                o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;     
                return;           
            } else {
                o->activeFlags |= ACTIVE_FLAG_ACTIVE;
                o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;                
            }            
            break;
    }

    if (gFPVPlayer.actionState == PLAYER_INTRO_CUTSCENE) {
        o->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE; 
        return;      
    } else if (!BPARAM4) {
        o->activeFlags |= ACTIVE_FLAG_ACTIVE;
        o->header.gfx.node.flags |= GRAPH_RENDER_HAS_ANIMATION | GRAPH_RENDER_ACTIVE;          
    }

    s32 isRunning = gOfficeState.stage == OFFICE_STAGE_3;

    if (isRunning) {
        patrolSpeed = SPLINE_GUY_SPRINT_SPEED;
        turningSpeed = SPLINE_GUY_TURNING_SPEED_RUNNING;
    } else {
        turningSpeed = SPLINE_GUY_TURNING_SPEED;
        if (o->oAction == SPLINE_GUY_CHASING_PLAYER) {
            switch (gOfficeState.stage) {
                case OFFICE_STAGE_1:
                    patrolSpeed = SPLINE_GUY_CHASE_SPEED_STAGE_1;
                    break;
                case OFFICE_STAGE_2:
                    patrolSpeed = SPLINE_GUY_CHASE_SPEED_STAGE_2;
                    break;
            }
        } else {
            patrolSpeed = SPLINE_GUY_WALK_SPEED;
        }
        
    }

    switch (o->oAction) {
        case SPLINE_GUY_WALKING: {
            if (spline_guy_player_interact(pos, playerDist)) {
                gFPVPlayer.chasingNPC = NULL;
                break;
            } else if (
                gCurConvo.coolDownTimer == 0 &&
                playerDist > SPLINE_GUY_PLAYER_START_CONVO_DIST &&
                should_chase_player(pos, playerDist) &&
                closer_than_other_chasing_npc(playerDist)
            ) {
                o->oAction = SPLINE_GUY_CHASING_PLAYER;
                cur_obj_play_sound_2(SOUND_OFFICE_MGS);
                gFPVPlayer.chasingNPC = o;
                break;
            }
            // f32 *curPoint = spline->points[pIndex];
            struct Object *splineStopper = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvSplineStopper), 75.f);
            if (gOfficeState.stage == OFFICE_STAGE_3) {
                o->oAnimationIndex = NPC_ANIM_SPRINTING_QUARTER_SPEED;
            } else {
                o->oAnimationIndex = NPC_ANIM_WALKING_QUARTER_SPEED;
            }
            
            if (splineStopper != NULL) {
                if (o->oStopperObject != splineStopper) {
                    o->oAction = SPLINE_GUY_WAITING_TO_STOP;
                    o->oStopperObject = splineStopper;
                    break;
                }
            }
            f32 *nextPoint = spline->points[pIndex + 1];
            s16 goalAngle = atan2s(nextPoint[2] - pos[2], nextPoint[0] - pos[0]);
            spline_guy_update_position(goalAngle, patrolSpeed, turningSpeed, FALSE);

            if (vec3f_lat_dist(nextPoint, pos) < SPLINE_GUY_NEXT_POINT_THRESHOLD) {
                o->oSplineDudeGuyPointIndex++;
                if (o->oSplineDudeGuyPointIndex >= spline->size - 1) {
                    o->oSplineDudeGuyPointIndex = 0;
                    //o->oFinishedSplineLoop = 1;
                    o->oStopperObject = NULL;
                }
            }
            break;
        }
        case SPLINE_GUY_CHASING_PLAYER: {
            if (spline_guy_player_interact(pos, playerDist)) {
                break;
            } else if (
                gCurConvo.coolDownTimer != 0 ||
                playerDist > SPLINE_GUY_PLAYER_STOP_CHASE_DIST ||
                gFPVPlayer.curSpace != &gOfficeSpaces[2] ||
                gFPVPlayer.chasingNPC != o
            ) {
                o->oAction = SPLINE_GUY_STARTING_TO_WALK;
                f32 *nextPoint = spline->points[pIndex + 1];
                o->oOldAngle = atan2s(nextPoint[2] - pos[2], nextPoint[0] - pos[0]);

                if (gFPVPlayer.chasingNPC == o) {
                    gFPVPlayer.chasingNPC = NULL;
                }
                break;
            }

            if (gOfficeState.stage == OFFICE_STAGE_3) {
                o->oAnimationIndex = NPC_ANIM_SPRINTING_QUARTER_SPEED;
            } else {
                o->oAnimationIndex = NPC_ANIM_WALKING_QUARTER_SPEED;
            }

            s16 goalAngle;
            vec3f_get_yaw(pos, gFPVPlayer.pos, &goalAngle);
            spline_guy_update_position(goalAngle, patrolSpeed, turningSpeed, TRUE);

            if (isRunning) {
                cur_obj_play_sound_at_anim_range(4, 36,  (SOUND_ACTION_TERRAIN_STEP & ~SOUND_DISCRETE) + SOUND_TERRAIN_STONE);
            } else {
                cur_obj_play_sound_at_anim_range(7, 58, (SOUND_ACTION_TERRAIN_STEP & ~SOUND_DISCRETE) + SOUND_TERRAIN_GRASS);
            }

            break;
        }
        case SPLINE_GUY_WAITING_TO_STOP: {
            if (o->header.gfx.animInfo.animFrame == 0) {
                o->oOldAngle = o->oFaceAngleYaw;
                o->oAction = SPLINE_GUY_STOPPING;
            }
            break;
        }
        case SPLINE_GUY_STOPPING: {
            o->oAnimationIndex = NPC_ANIM_STOPWALKING;
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,(SPLINE_STOPPER_ANGLE), DEGREES(10));
            if (o->oTimer > 0) {
                cur_obj_extend_animation_if_at_end();
            }
            if ((s16)o->oFaceAngleYaw == (s16)SPLINE_STOPPER_ANGLE) {
                o->oAction = SPLINE_GUY_STOPPED;
            }
            break;
        }
        case SPLINE_GUY_STOPPED:
            o->oAnimationIndex = (o->oStopperObject->oBehParams >> 24) & 0xFF; //bparam1
            o->oFaceAngleYaw = SPLINE_STOPPER_ANGLE; //bparam2
            if ((cur_obj_check_if_at_animation_end()) && (o->oTimer > 0)) {
                o->oAction = SPLINE_GUY_STARTING_TO_WALK;
            }
            break;
        case SPLINE_GUY_STARTING_TO_WALK:
            o->oAnimationIndex = NPC_ANIM_IDLE;
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,o->oOldAngle,DEGREES(5));

            if (o->oFaceAngleYaw == o->oOldAngle) {
                o->oAction = SPLINE_GUY_WALKING;
            }
            break;
        case SPLINE_GUY_CONVERSATION: {
            s32 isCathy = cur_obj_has_model(MODEL_CHATTY_KATHY);
            s16 target = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);

            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw, target, DEGREES(10));
            if (gCurConvo.state == CONVO_INACTIVE) {
                o->oAction = SPLINE_GUY_RETURNING_TO_SPLINE;
                o->oAnimationIndex = NPC_ANIM_IDLE;
            } else if (gCurConvo.state == CONVO_TALKING) {
                o->oAnimationIndex = NPC_ANIM_TALKING;
                if (gCurConvo.conversationalist == o) {
                    switch (gCurConvo.timer) {
                        case 1:
                            play_character_lead_in(isCathy);
                            break;
                        case 76:
                            play_character_mumble(isCathy);
                            break;
                        case 76+120:
                            play_character_question(isCathy);
                            break;
                    }
                }
            } else {
                if (isCathy) {
                    o->oAnimationIndex = NPC_ANIM_AWAITING_RESPONSE_FEMALE;
                } else {
                    o->oAnimationIndex = NPC_ANIM_AWAITING_RESPONSE_MALE;
                }
            }
            break;
        }
        case SPLINE_GUY_STOLE_COFFEE: {
            s32 isCathy = cur_obj_has_model(MODEL_CHATTY_KATHY);
            o->oAnimationIndex = NPC_ANIM_STEAL_COFFEE;
            if (o->oTimer > 0 && cur_obj_check_anim_frame(SPLINE_GUY_STEAL_SIP_END)) {
                if (o->oSubAction == 3) {
                    o->oAction = SPLINE_GUY_RETURNING_TO_SPLINE;
                    o->oAnimationIndex = NPC_ANIM_IDLE;
                    gFPVPlayer.coffeeCup->oAction = COFFEE_CUP_INVISIBLE;
                } else {
                    o->header.gfx.animInfo.animFrame = SPLINE_GUY_STEAL_SIP_START;
                }
            }
            if (cur_obj_check_anim_frame(SPLINE_GUY_STEAL_SIP_START)) {
                o->oSubAction++;
                play_character_coffee_sip(isCathy);
            };

            break;
        }
        case SPLINE_GUY_RETURNING_TO_SPLINE:
            if (o->oTimer > 3 * 30) {
                // todo: more custom
                o->oAction = SPLINE_GUY_STARTING_TO_WALK;
                break;
            }
    }
    if (o->oAnimationIndex == NPC_ANIM_WALKING_QUARTER_SPEED) {
        f32 animSpeed = (4.0f*0.97f) * (o->oForwardVel / patrolSpeed);
        cur_obj_init_animation_with_accel_and_sound(o->oAnimationIndex, animSpeed);
    } else if (o->oAnimationIndex == NPC_ANIM_SPRINTING_QUARTER_SPEED) {
        f32 animSpeed = (4.0f*0.92f) * (o->oForwardVel / (patrolSpeed));
        cur_obj_init_animation_with_accel_and_sound(o->oAnimationIndex, animSpeed);
    } else {
        cur_obj_init_animation(o->oAnimationIndex);
    }
}

enum CoffeeMachineActions {
    COFFEE_MACHINE_WAITING,
    COFFEE_MACHINE_RUNNING,
    COFFEE_MACHINE_READY,
};

#define MAX_COFFEE_MACHINE_DIST 200.f

void bhv_coffee_machine_init(void) {
    o->oAction = COFFEE_MACHINE_WAITING;
    o->oCoffeeMachineShake = 2.0f;
    o->oCoffeeMachineShake2 = 2.0f;
}

void bhv_coffee_machine_loop(void) {
    switch (o->oAction) {
        case COFFEE_MACHINE_WAITING: {
            f32 *pos = &o->oPosX;
            o->oAnimState = 0;
            if (!gFPVPlayer.sipsLeft && vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST && gPlayer1Controller->buttonPressed & PLAYER_BTN_INTERACT) {
                o->oCoffeeMachineShake = 2.0f;
                o->oCoffeeMachineShake2 = 2.0f;
                o->oAction = COFFEE_MACHINE_RUNNING;
                o->oAnimState = 1;
            }
            break;
        }
        case COFFEE_MACHINE_RUNNING: {
            o->oPosX += o->oCoffeeMachineShake2;
            o->oPosY += o->oCoffeeMachineShake;
            //o->oPosZ += o->oCoffeeMachineShake2;
            o->oCoffeeMachineShake *= -1;
            if (o->oCoffeeMachineShake2 == 2.0f) {
                o->oCoffeeMachineShake2 = 0.0f; 
                 } 
            else if (o->oCoffeeMachineShake2 == 0.0f) { 
                o->oCoffeeMachineShake2 = -2.0f; 
                } 
            else if (o->oCoffeeMachineShake2 == -2.0f) { 
                o->oCoffeeMachineShake2 = 2.0f;
                }
            if (o->oTimer > 0 && (o->oTimer % 30 == 0)) {
                o->oAnimState += 1;
                if (o->oAnimState == 3) {
                    o->oAction = COFFEE_MACHINE_READY;
                }
            } else if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_COFFEEMACHINE);
            }
            break;
        }
        case COFFEE_MACHINE_READY: {
            o->oPosX = o->oHomeX;
            o->oPosY = o->oHomeY;
            o->oPosZ = o->oHomeZ;
            f32 *pos = &o->oPosX;
            if (vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST && gPlayer1Controller->buttonPressed & PLAYER_BTN_INTERACT) {
                o->oAction = COFFEE_MACHINE_WAITING;
                gFPVPlayer.sipsLeft = MAX_SIPS;
                gFPVPlayer.energy = MAX_ENERGY;
                gFPVPlayer.coffeeStolen = FALSE;
                gFPVPlayer.coffeeTracker = 1;
            }
        }

    }

    if (o->oAnimState == 0) {
        o->oPrimRGB = 0x2C2C2C;
    } else if (o->oAnimState <= 2) {
        o->oPrimRGB = 0xFF0000;
    } else {
        o->oPrimRGB = 0x00FF00;
    }
}

enum ElevatorDoorActions {
    ELEVATOR_DOOR_IDLE,
    ELEVATOR_DOOR_OPEN,
    ELEVATOR_DOOR_CLOSED,
    ELEVATOR_DOOR_GONE,
};

void bhv_elevator_door_init(void) {
    if (BPARAM1 == 1)
        o->oElevatorDoorIsOtherDoor = 1;

    o->oAction = ELEVATOR_DOOR_IDLE;
}

void bhv_elevator_door_loop(void) {
    switch (o->oAction) {
        case ELEVATOR_DOOR_IDLE:
            if (gIntroCutscene >= INTRO_CUTSCENE_OPEN_ELEVATOR_DOORS) {
                o->oAction = ELEVATOR_DOOR_OPEN;
            }
            break;
        case ELEVATOR_DOOR_OPEN:
            if (!o->oElevatorDoorIsOtherDoor) {
                o->oPosZ = approach_f32_symmetric(o->oPosZ, o->oHomeZ - 250.0f, 12.0f);
            } else {
                o->oPosZ = approach_f32_symmetric(o->oPosZ, o->oHomeZ + 250.0f, 12.0f);
            }
            if (o->oTimer > 60) {
                o->oAction = ELEVATOR_DOOR_GONE;
            }
            break;
        case ELEVATOR_DOOR_GONE:
            cur_obj_hide();
            break;
    }
}

void bhv_arm_init(void) {
    o->oAction = ARM_INVISIBLE;
    o->oAnimState = 0;
    o->oAnimationIndex = 0;
    cur_obj_init_animation(o->oAnimationIndex);
}

void bhv_arm_loop(void) {
    switch (o->oAction) {
        case ARM_INVISIBLE:
            if (gFPVPlayer.sipsLeft) {
                o->oAction = ARM_RAISING;
            } else {
                cur_obj_hide();
                o->oAnimState = 0;
                return;
            }
        // fallthrough
        case ARM_RAISING:
            cur_obj_unhide();
            o->oAnimState = 0;
            o->oAnimationIndex = ARM_ANIM_RAISE;
            if ((cur_obj_check_if_at_animation_end()) && (o->oTimer > 0)) {
                o->oAction = ARM_IDLE;
            }
            break;
        case ARM_IDLE:
            cur_obj_unhide();
            o->oAnimState = 0;
            o->oAnimationIndex = ARM_ANIM_IDLE;
            if (gFPVPlayer.sipsLeft == 0 && gFPVPlayer.coffeeStolen) {
                o->oAction = ARM_COFFEE_STOLEN;
            }
            break; 
        case ARM_COFFEE_STOLEN:
            cur_obj_unhide();
            o->oAnimState = 1;
            o->oAnimationIndex = ARM_ANIM_COFFEE_STOLEN;
            if ((cur_obj_check_if_at_animation_end()) && (o->oTimer > 0)) {
                o->oAction = ARM_INVISIBLE;
                cur_obj_hide();
                return;
            }
            break;
        case ARM_DRINKING:
            o->oAnimationIndex = ARM_ANIM_DRINK;
            cur_obj_init_animation(o->oAnimationIndex);
            s32 eGain = roundf(((f32)E_GAIN_COFFEE_SIP) / (f32)o->header.gfx.animInfo.curAnim->loopEnd);
            replenish_energy(eGain);
            if (cur_obj_check_if_at_animation_end()) {
                gFPVPlayer.sipsLeft--;
                o->oAnimationIndex = ARM_ANIM_IDLE;
                cur_obj_init_animation(o->oAnimationIndex);
                if (gFPVPlayer.sipsLeft <= 0) {
                    gFPVPlayer.sipsLeft = 0;
                    gFPVPlayer.coffeeTracker = 0;
                    o->oAction = ARM_INVISIBLE;
                } else {
                    o->oAction = ARM_IDLE;
                }
            }
            return;
    }
    cur_obj_init_animation(o->oAnimationIndex);
}

void bhv_coffee_cup_init(void) {
    o->oAction = COFFEE_CUP_EMPTY;
}

void bhv_coffee_cup_loop(void) {
    if (gFPVPlayer.sipsLeft) {
        o->oAction = gFPVPlayer.sipsLeft;
    } else if (!gFPVPlayer.coffeeStolen) {
        o->oAction = COFFEE_CUP_INVISIBLE;
    }

    switch (o->oAction) {
        case COFFEE_CUP_INVISIBLE:
            cur_obj_hide();
            break;
        case COFFEE_CUP_EMPTY:
            cur_obj_set_model(MODEL_COFFEE_CUP_EMPTY);
            break;
        case COFFEE_CUP_HALF:
            cur_obj_set_model(MODEL_COFFEE_CUP_HALF);
            break;
        case COFFEE_CUP_FULL:
            cur_obj_set_model(MODEL_COFFEE_CUP_FULL);
            break;
    }

    if (o->oAction != COFFEE_CUP_INVISIBLE)
        cur_obj_unhide();
}

enum IntroKathyActions {
    INTRO_KATHY_WAKE_UP,
    INTRO_KATHY_TURN_TOWARDS_DOOR,
    INTRO_KATHY_WALK_OUT_OF_ELEVATOR,
    INTRO_KATHY_YOU_LOOK_TERRIBLE,
    INTRO_KATHY_GO_AWAY,
    INTRO_KATHY_DISAPPEAR,
};

void bhv_intro_kathy_init(void) {

    if (gOfficeState.checkpoint != 0) {
        o->oAction = INTRO_KATHY_DISAPPEAR;
    } else {
        o->oAction = INTRO_KATHY_WAKE_UP;
        gIntroCutscene = INTRO_CUTSCENE_START;
        o->oAnimationIndex = NPC_ANIM_INTRO_WAKE_UP;
        cur_obj_init_animation(o->oAnimationIndex);
    }    

    

}

void bhv_intro_kathy_loop(void) {
    switch (o->oAction) {
        case INTRO_KATHY_WAKE_UP:
            o->oAnimationIndex = NPC_ANIM_INTRO_WAKE_UP;
            if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_CATHY_INTRO_WAKE_UP);
            }
            if ((o->oTimer > 2) && (cur_obj_check_if_at_animation_end())) {
                o->oAction = INTRO_KATHY_TURN_TOWARDS_DOOR;
                gIntroCutscene = INTRO_CUTSCENE_OPEN_ELEVATOR_DOORS;
                o->oAnimationIndex = NPC_ANIM_WALKING;
            }
            break;
        case INTRO_KATHY_TURN_TOWARDS_DOOR:
            o->oAnimationIndex = NPC_ANIM_WALKING;
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,0x4000, DEGREES(10));
            if (o->oFaceAngleYaw == 0x4000) {
                o->oAction = INTRO_KATHY_WALK_OUT_OF_ELEVATOR;
            }
            break;
        case INTRO_KATHY_WALK_OUT_OF_ELEVATOR:
            o->oAnimationIndex = NPC_ANIM_WALKING;
            if (o->oTimer > 90) {
                o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,DEGREES(270), DEGREES(10));
                if (o->oFaceAngleYaw <= DEGREES(-90)) {
                    o->oAction = INTRO_KATHY_YOU_LOOK_TERRIBLE;
                }
            } else {
                o->oPosX+= 3;
            }
            break;
        case INTRO_KATHY_YOU_LOOK_TERRIBLE:
            o->oAnimationIndex = NPC_ANIM_INTRO_YOU_LOOK_TERRIBLE;
            if (o->oTimer == 0) {
                cur_obj_play_sound_2(SOUND_CATHY_INTRO_YOU_LOOK_TERRIBLE);
            }
            if ((o->oTimer > 2) && (cur_obj_check_if_at_animation_end())) {
                o->oAction = INTRO_KATHY_GO_AWAY;
            }
            break;
        case INTRO_KATHY_GO_AWAY:
            o->oAnimationIndex = NPC_ANIM_WALKING;
            if (o->oPosX < 700) {
                o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,0x4000, DEGREES(10));
                o->oPosX += 7;
            } else {
                o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw,-0x8000, DEGREES(10));
                o->oPosZ -= 7;
            }
            if (o->oPosZ < 2400) {
                o->oAction = INTRO_KATHY_DISAPPEAR;
            }
            
            break;
        case INTRO_KATHY_DISAPPEAR:
            gIntroCutscene = INTRO_CUTSCENE_END;
            if (o->oTimer == 0) {
                gFPVPlayer.currentTutorial = 0;
            }
            cur_obj_set_model(MODEL_NONE);
            cur_obj_hide();
            break;
    }
    //print_text_fmt_int(20,20, "%d", o->oPosZ);
    //print_text_fmt_int(20,40, "%x", o->oFaceAngleYaw);
    if (!cur_obj_has_model(MODEL_NONE)) {
        cur_obj_init_animation(o->oAnimationIndex);
    }
    #ifdef SLIDE_DEBUG
    if (gPlayer1Controller->buttonPressed & R_TRIG)
        o->oAction = INTRO_KATHY_DISAPPEAR;
    #endif




}


#define MAX_DIST_LIGHT_SWITCH 150.f

enum BButtonActions {
    B_BUTTON_COFFEE_MACHINE,
    B_BUTTON_LIGHT_SWITCH,
    B_BUTTON_BENCH,
};

void bhv_b_button_init(void) {
    cur_obj_hide();
    o->oCoffeeMachine = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvCoffeeMachine),100.f);
    o->oLightSwitch = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvLightSwitch),100.f);

    if (o->oCoffeeMachine != NULL) {
        o->oAction = B_BUTTON_COFFEE_MACHINE;
    } else if (o->oLightSwitch != NULL) {
        o->oAction = B_BUTTON_LIGHT_SWITCH;
    } else {
        o->oAction = B_BUTTON_BENCH;
    }
}

void bhv_b_button_loop(void) {
    f32 *pos = &o->oPosX;
    f32 playerDist = 0;
    Vec3f parentPos;

    if (o->oCoffeeMachine != NULL) {
        parentPos[0] = o->oCoffeeMachine->oPosX; parentPos[1] = o->oCoffeeMachine->oPosY; parentPos[2] = o->oCoffeeMachine->oPosZ;
        playerDist = vec3f_lat_dist(parentPos, gFPVPlayer.pos);
    } else if (o->oLightSwitch != NULL) {
        parentPos[0] = o->oLightSwitch->oPosX; parentPos[1] = o->oLightSwitch->oPosY; parentPos[2] = o->oLightSwitch->oPosZ;
        playerDist = vec3f_lat_dist(parentPos, gFPVPlayer.pos);
    } else {
        playerDist = vec3f_lat_dist(pos, gFPVPlayer.pos);
    }

    switch (o->oAction) {
        case B_BUTTON_COFFEE_MACHINE:
            if (((o->oCoffeeMachine->oAction != COFFEE_MACHINE_WAITING) && (o->oCoffeeMachine->oAction != COFFEE_MACHINE_READY)) || (gFPVPlayer.sipsLeft) || (playerDist > MAX_COFFEE_MACHINE_DIST)) {
                cur_obj_hide();
            } else {
                if (!o->oCoffeeMachineTutorialTriggered) {
                    gFPVPlayer.curTutorialDone = 0;
                    gFPVPlayer.currentTutorial = 1;
                    o->oCoffeeMachineTutorialTriggered = 1;                   
                }
                cur_obj_unhide();
            }
            break;
        case B_BUTTON_LIGHT_SWITCH:
            if ((playerDist < MAX_DIST_LIGHT_SWITCH) && (gFPVPlayer.confroomFirstTime)) {
                cur_obj_unhide();
            } else {
                cur_obj_hide();
            }
            cur_obj_scale(0.12f);
            break;
        case B_BUTTON_BENCH:
            if ((gFPVPlayer.actionState != PLAYER_PRESENTING) && (playerDist < MAX_SITTING_DIST)) {
                if (!o->oSittingTutorialTriggered) {
                    gFPVPlayer.curTutorialDone = 0;
                    gFPVPlayer.currentTutorial = 3;
                    o->oSittingTutorialTriggered = 1;
                }
                if (!gOfficeState.lightsOn) {
                    gFPVPlayer.canSit = 1;
                } else {
                    gFPVPlayer.canSit = -1;
                }
                if (playerDist > 30.f) {
                    cur_obj_unhide();
                } else {
                    cur_obj_hide();
                }
            } else {
                gFPVPlayer.canSit = 0;
                cur_obj_hide(); 
            }
            break;            

    }
    o->oBButtonTimer += 1300;
    o->oPosY += sins(o->oBButtonTimer) * 1.1f;
    o->oFaceAngleYaw += 500;
}

void bhv_point_light_loop(void) {
    u8 r;
    u8 g;
    u8 b;
    //u8 falloff;
    Vec3f pos;
    
    pos[0] = o->oPosX;
    pos[1] = o->oPosY;
    pos[2] = o->oPosZ;

    r = BPARAM1;
    g = BPARAM2;
    b = BPARAM4;

    emit_light(pos, r, g, b, 4, 50, 8, 5);
}

void bhv_light_switch_loop(void) {
    f32 *pos = &o->oPosX;
    if ((vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST) && (gPlayer1Controller->buttonPressed & PLAYER_BTN_INTERACT) && (gFPVPlayer.actionState != PLAYER_CONVO_QTE) && (gFPVPlayer.confroomFirstTime)) {
        gOfficeState.lightsOn ^= 1;
        if (cur_obj_has_model(MODEL_LIGHT_SWITCH_UP)) {
            cur_obj_set_model(MODEL_LIGHT_SWITCH_DOWN);
        } else {
            cur_obj_set_model(MODEL_LIGHT_SWITCH_UP);
        }
    }
}

enum EndingDudeGuyActions {
    ENDING_DUDEGUY_IDLE,
    ENDING_DUDEGUY_PLAY_ANIM,
};

void bhv_ending_dudeguy_init(void) {
    o->oAction = ENDING_DUDEGUY_IDLE,
    o->oAnimationIndex = NPC_ANIM_ENDING_CUTSCENE;
    cur_obj_init_animation(o->oAnimationIndex);
}

void bhv_ending_dudeguy_loop(void) {
    if (gOfficeState.paused == PAUSE_STATE_END && gOfficeState.pauseTimer >= ENDING_OCEAN_START) {
        if (o->oAction != ENDING_DUDEGUY_PLAY_ANIM) {
            o->oAction = ENDING_DUDEGUY_PLAY_ANIM;
            cur_obj_init_animation(o->oAnimationIndex);
            o->header.gfx.animInfo.animFrame = 0;
        }
    } 

    switch (o->oAction) {
        case ENDING_DUDEGUY_IDLE:
            cur_obj_hide();
            o->header.gfx.animInfo.animFrame = 0;
            break;
        case ENDING_DUDEGUY_PLAY_ANIM:
            cur_obj_unhide();
            cur_obj_init_animation(o->oAnimationIndex);
            if (cur_obj_check_anim_frame(79) && o->oSubAction < 2) {
                o->header.gfx.animInfo.animFrame = 0;
                o->oSubAction++;
            }
            if (cur_obj_check_anim_frame(80)) {
                cur_obj_play_sound_2(SOUND_OFFICE_SFX_JUST_A_NIGHTMARE);
            }

            if (cur_obj_check_if_at_animation_end()) {
                o->header.gfx.animInfo.animFrame = ENDING_OCEAN_TEXT_THE_END;
            }
            break;
    }
}

enum ExclamationMarkActions {
    EXCLAMATION_MARK_BEGONE,
    EXCLAMATION_MARK_APPEAR,
    EXCLAMATION_MARK_DISAPPEAR,
    EXCLAMATION_MARK_WAITING,
};

void bhv_exclamation_mark_init(void) {
    cur_obj_hide();
    o->oAction = EXCLAMATION_MARK_BEGONE;
}

void bhv_exclamation_mark_loop(void) {
    struct Object *chasingDude = gFPVPlayer.chasingNPC;

    switch (o->oAction) {
        case EXCLAMATION_MARK_BEGONE:
            cur_obj_hide();
            if (chasingDude != NULL) {
                o->oAction = EXCLAMATION_MARK_APPEAR;
            }
            Y_SCALE = 0.10f;
            X_SCALE = 3.f;
            break;
        case EXCLAMATION_MARK_APPEAR:
            cur_obj_unhide();
            Y_SCALE = approach_f32(Y_SCALE, 1.f, 0.2, 0.2);
            X_SCALE = approach_f32(X_SCALE, 1.f, 0.6f, 0.6f);
            if (o->oTimer > 60) {
                o->oAction = EXCLAMATION_MARK_DISAPPEAR;
            }
            if (chasingDude != NULL) {
                o->oPosX = chasingDude->oPosX;
                o->oPosZ = chasingDude->oPosZ;
                o->oPosY = chasingDude->oPosY + 270.f;
            } else {
                o->oAction = EXCLAMATION_MARK_BEGONE;
            }
            break;
        case EXCLAMATION_MARK_DISAPPEAR:
            X_SCALE = Y_SCALE = Z_SCALE = approach_f32(X_SCALE, 0, 0.3f, 0.3f);
            if (o->oTimer > 30) {
                o->oAction = EXCLAMATION_MARK_WAITING;
            }
            if (chasingDude != NULL) {
                o->oPosX = chasingDude->oPosX;
                o->oPosZ = chasingDude->oPosZ;
                o->oPosY = chasingDude->oPosY + 270.f;
            } else {
                o->oAction = EXCLAMATION_MARK_BEGONE;
            }
            break;
        case EXCLAMATION_MARK_WAITING:
            if (chasingDude == NULL) {
                o->oAction = EXCLAMATION_MARK_BEGONE;
            }
            cur_obj_hide();
            break;
    }
}
