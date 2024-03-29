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

static void _play_random_sound_from_array(u32 *soundArray, s32 length) {
    cur_obj_play_sound_2(soundArray[random_u16() % length]);
}
#define play_random_sound_from_array(soundArray) _play_random_sound_from_array(soundArray, ARRAY_COUNT(soundArray))

static void play_character_lead_in(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_lead_ins);
    } else {
        
    }
}
static void play_character_mumble(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_mumbles);
    } else {

    }
}
static void play_character_question(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_questions);
    } else {

    }
}
static void play_character_coffee_steal(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_coffee_steals);
    } else {

    }
}
static void play_character_coffee_sip(s32 isCathy) {
    if (isCathy) {
        play_random_sound_from_array(cathy_coffee_sips);
    } else {

    }
}


void bhv_dudeguy_init(void) {
    o->oAnimationIndex = BPARAM1;

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
            }
        }
    }

    /*if (gPlayer1Controller->buttonPressed & R_JPAD) {
        o->oAnimationIndex++;
    } else if (gPlayer1Controller->buttonPressed & L_JPAD) {
        o->oAnimationIndex--;
    }*/
}

#define SPLINE_GUY_PATROL_SPEED             meters_sec(1.6f)
#define SPLINE_GUY_RUN_SPEED                meters_sec(3.1f)
#define SPLINE_GUY_TURNING_SPEED            DEGREES(3)
#define SPLINE_GUY_TURNING_SPEED_RUNNING    DEGREES(6)
#define SPLINE_GUY_CHASE_SPEED              meters_sec(1.75f)
#define SPLINE_GUY_NEXT_POINT_THRESHOLD     160
#define SPLINE_GUY_PLAYER_START_CONVO_DIST  160
#define SPLINE_GUY_STEAL_SIP_START          102
#define SPLINE_GUY_STEAL_SIP_END            193

void bhv_spline_dudeguy_init(void) {
    o->oAnimationIndex = NPC_ANIM_WALKING;
    o->oAction = SPLINE_GUY_WALKING;
    o->oSplineDudeGuyPointIndex = 0;
    o->oForwardVel = SPLINE_GUY_PATROL_SPEED;

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

struct Object *find_closest_office_obj_with_bhv(const BehaviorScript *behavior, f32 maxDist) {
    struct Object *closestObject = NULL;
    f32 dist = maxDist;
    f32 distTemp;
    u8 numObjects = get_num_confroom_objects();
    for (int i = 0; i < numObjects; i++) {
        if (gConfroomObjectPool[i].behavior == behavior) {
            distTemp = vec3f_lat_dist(&gCurrentObject->oPosX, &gConfroomObjectPool[i].oPosX);
            if (distTemp < dist) {
                dist = distTemp;
                closestObject = &gConfroomObjectPool[i];;
            }
        }
    }
    return closestObject;
}

#define SPLINE_STOPPER_ANGLE DEGREES((o->oStopperObject->oBehParams >> 16) & 0xFF)

void bhv_spline_dudeguy_loop(void) {
    ConfroomObjectSplineRef *spline = &gConfroomSplines[BPARAM2];
    s32 pIndex = o->oSplineDudeGuyPointIndex;
    f32 *vel = &o->oVelX;
    f32 *pos = &o->oPosX;

    f32 playerDist = vec3f_lat_dist(pos, gFPVPlayer.pos);
    f32 patrolSpeed;
    f32 turningSpeed;

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

    if (gOfficeState.stage == OFFICE_STAGE_3) {
        patrolSpeed = SPLINE_GUY_RUN_SPEED;
        turningSpeed = SPLINE_GUY_TURNING_SPEED_RUNNING;
    } else {
        turningSpeed = SPLINE_GUY_TURNING_SPEED;
        patrolSpeed = SPLINE_GUY_PATROL_SPEED;
    }



    switch (o->oAction) {
        case SPLINE_GUY_WALKING: {
            if (playerDist < SPLINE_GUY_PLAYER_START_CONVO_DIST) {
#ifdef SLIDE_DEBUG
                if (gFPVPlayer.sipsLeft && !gFPVPlayer.godMode) { 
#else
                if (gFPVPlayer.sipsLeft) { 
#endif

                    gFPVPlayer.sipsLeft = 0;
                    gFPVPlayer.coffeeStolen = TRUE;
                    o->oAction = SPLINE_GUY_STOLE_COFFEE;
                    o->oSubAction = 0;
                    o->oOldAngle = o->oFaceAngleYaw;
                    o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
                    o->oForwardVel = 0;
                    play_character_coffee_steal(cur_obj_has_model(MODEL_CHATTY_KATHY));
                } else {
                    Vec3f speakerPos = { o->oPosX, o->oPosY + 180, o->oPosZ };
                    if (start_convo(speakerPos)) {
                        o->oAction = SPLINE_GUY_CONVERSATION;
                        o->oOldAngle = o->oFaceAngleYaw;
                        o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
                        o->oForwardVel = 0;
                        break;
                    }
                }
            }
            // f32 *curPoint = spline->points[pIndex];
            struct Object *splineStopper = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvSplineStopper), 75.f);
            if (gOfficeState.stage == OFFICE_STAGE_3) {
                o->oAnimationIndex = NPC_ANIM_SPRINTING;
            } else {
                o->oAnimationIndex = NPC_ANIM_WALKING;
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
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw, goalAngle, turningSpeed);
            if (o->oForwardVel < patrolSpeed) {
                o->oForwardVel += patrolSpeed * 0.2f;
            }
            if (o->oForwardVel > patrolSpeed) {
                o->oForwardVel = patrolSpeed;
            }
            vel[0] = sins(o->oFaceAngleYaw) * o->oForwardVel;
            vel[2] = coss(o->oFaceAngleYaw) * o->oForwardVel;
            vec3f_add(pos, vel);
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
            } else if (gCurConvo.state == CONVO_TALKING) {
                o->oAnimationIndex = NPC_ANIM_TALKING;
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
    cur_obj_init_animation(o->oAnimationIndex);
}

enum CoffeeMachineActions {
    COFFEE_MACHINE_WAITING,
    COFFEE_MACHINE_RUNNING,
    COFFEE_MACHINE_READY,
};

#define MAX_COFFEE_MACHINE_DIST 200.f

void bhv_coffee_machine_init(void) {
    o->oAction = COFFEE_MACHINE_WAITING;
}

void bhv_coffee_machine_loop(void) {
    switch (o->oAction) {
        case COFFEE_MACHINE_WAITING: {
            f32 *pos = &o->oPosX;
            o->oAnimState = 0;
            if (!gFPVPlayer.sipsLeft && vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST && gPlayer1Controller->buttonPressed & PLAYER_BTN_INTERACT) {
                o->oAction = COFFEE_MACHINE_RUNNING;
                o->oAnimState = 1;
            }
            break;
        }
        case COFFEE_MACHINE_RUNNING: {
            if (o->oTimer > 0 && (o->oTimer % 30 == 0)) {
                o->oAnimState += 1;
                if (o->oAnimState == 3) {
                    o->oAction = COFFEE_MACHINE_READY;
                }
            }
            break;
        }
        case COFFEE_MACHINE_READY: {
            f32 *pos = &o->oPosX;
            if (vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST && gPlayer1Controller->buttonPressed & PLAYER_BTN_INTERACT) {
                o->oAction = COFFEE_MACHINE_WAITING;
                gFPVPlayer.sipsLeft = 3;
                gFPVPlayer.energy = MAX_ENERGY;
                gFPVPlayer.coffeeStolen = FALSE;
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
    o->oAction = INTRO_KATHY_WAKE_UP;
    gIntroCutscene = INTRO_CUTSCENE_START;
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
    
    #ifdef SLIDE_DEBUG
    if (gPlayer1Controller->buttonPressed & R_TRIG)
        o->oAction = INTRO_KATHY_DISAPPEAR;
    #endif

    if (!cur_obj_has_model(MODEL_NONE))
        cur_obj_init_animation(o->oAnimationIndex);

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
            if (playerDist < MAX_DIST_LIGHT_SWITCH) {
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
                gFPVPlayer.canSit = 1;
                if (playerDist > 30.f) {
                    cur_obj_unhide();
                } else {
                    cur_obj_unhide();
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

    /*switch (o->oAction) {
        case B_BUTTON_INVISIBLE:
            cur_obj_hide();
            break;
        case B_BUTTON_VISIBLE:
            if ((gFPVPlayer.actionState != PLAYER_PRESENTING) && (playerDist > 30.f)) {
                cur_obj_unhide();
            } else {
                cur_obj_hide(); //this is stupid
            }
            o->oBButtonTimer += 1300;
            o->oPosY += sins(o->oBButtonTimer) * 1.1f;
            o->oFaceAngleYaw += 500;
            break;
    }
    if (o->oCoffeeMachine != NULL) {
        if (((o->oCoffeeMachine->oAction != COFFEE_MACHINE_WAITING) && (o->oCoffeeMachine->oAction != COFFEE_MACHINE_READY)) || (gFPVPlayer.sipsLeft) || (playerDist > MAX_COFFEE_MACHINE_DIST)) {
            o->oAction = B_BUTTON_INVISIBLE;
        } else {
            o->oAction = B_BUTTON_VISIBLE;
        }

        if (playerDist < MAX_COFFEE_MACHINE_DIST && !o->oCoffeeMachineTutorialTriggered) {
            gFPVPlayer.curTutorialDone = 0;
            gFPVPlayer.currentTutorial = 1;
            o->oCoffeeMachineTutorialTriggered = 1;
        }
        

    } else if (o->oLightSwitch != NULL) {
        
    } else {
        if (playerDist > MAX_SITTING_DIST) { //set in fpv_player.h
            o->oAction = B_BUTTON_INVISIBLE;
            gFPVPlayer.canSit = FALSE;
        } else {
            if (gFPVPlayer.actionState != PLAYER_PRESENTING) {
                if (!o->oSittingTutorialTriggered) {
                    gFPVPlayer.curTutorialDone = 0;
                    gFPVPlayer.currentTutorial = 3;
                    o->oSittingTutorialTriggered = 1;
                }
                o->oAction = B_BUTTON_VISIBLE;
            }
            gFPVPlayer.canSit = TRUE;
        }
    }*/
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
    if ((vec3f_lat_dist(pos, gFPVPlayer.pos) < MAX_COFFEE_MACHINE_DIST) && (gPlayer1Controller->buttonPressed & B_BUTTON)) {
        gOfficeState.lightsOn ^= 1;
        if (cur_obj_has_model(MODEL_LIGHT_SWITCH_UP)) {
            cur_obj_set_model(MODEL_LIGHT_SWITCH_DOWN);
        } else {
            cur_obj_set_model(MODEL_LIGHT_SWITCH_UP);
        }
    }
}
