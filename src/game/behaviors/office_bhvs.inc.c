#include "game/include_for_vsc.h"
#include "game/print.h"
#include "game/object_helpers.h"
#include "include/model_ids.h"

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


void bhv_dudeguy_init(void) {
    o->oAnimationIndex = BPARAM1;
}

void bhv_dudeguy_loop(void) {
    cur_obj_init_animation(o->oAnimationIndex);

    if (o->oTimer == 0)
        o->header.gfx.animInfo.animFrame = (random_u16() % o->header.gfx.animInfo.curAnim->loopEnd);

    if (gPlayer1Controller->buttonPressed & R_JPAD) {
        o->oAnimationIndex++;
    } else if (gPlayer1Controller->buttonPressed & L_JPAD) {
        o->oAnimationIndex--;
    }
}

enum SplineDudeGuyActions {
    SPLINE_GUY_WALKING,
    SPLINE_GUY_WAITING_TO_STOP,
    SPLINE_GUY_STOPPING,
    SPLINE_GUY_STOPPED,
    SPLINE_GUY_STARTING_TO_WALK,
    SPLINE_GUY_CHASING_PLAYER,
    SPLINE_GUY_CONVERSATION,
    SPLINE_GUY_RETURNING_TO_SPLINE,
};

#define SPLINE_GUY_PATROL_SPEED meters_sec(1.6f)
#define SPLINE_GUY_CHASE_SPEED  meters_sec(1.75f)
#define SPLINE_GUY_NEXT_POINT_THRESHOLD  160
#define SPLINE_GUY_PLAYER_START_CONVO_DIST 160

void bhv_spline_dudeguy_init(void) {
    o->oAnimationIndex = NPC_ANIM_WALKING;
    o->oAction = SPLINE_GUY_WALKING;
    o->oSplineDudeGuyPointIndex = 0;
    o->oForwardVel = SPLINE_GUY_PATROL_SPEED;

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

    switch (o->oAction) {
        case SPLINE_GUY_WALKING: {
            if (playerDist < SPLINE_GUY_PLAYER_START_CONVO_DIST) {
                Vec3f speakerPos = { o->oPosX, o->oPosY + 180, o->oPosZ };
                if (start_convo(speakerPos)) {
                    o->oAction = SPLINE_GUY_CONVERSATION;
                    o->oOldAngle = o->oFaceAngleYaw;
                    o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
                    o->oForwardVel = 0;
                    break;
                }
            }
            // f32 *curPoint = spline->points[pIndex];
            struct Object *splineStopper = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvSplineStopper), 75.f);
            o->oAnimationIndex = NPC_ANIM_WALKING;
            if (splineStopper != NULL) {
                if (o->oStopperObject != splineStopper) {
                    o->oAction = SPLINE_GUY_WAITING_TO_STOP;
                    o->oStopperObject = splineStopper;
                    break;
                }
            }
            f32 *nextPoint = spline->points[pIndex + 1];
            s16 goalAngle = atan2s(nextPoint[2] - pos[2], nextPoint[0] - pos[0]);
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw, goalAngle, DEGREES(2));
            if (o->oForwardVel < SPLINE_GUY_PATROL_SPEED) {
                o->oForwardVel += SPLINE_GUY_PATROL_SPEED * 0.2f;
            }
            if (o->oForwardVel > SPLINE_GUY_PATROL_SPEED) {
                o->oForwardVel = SPLINE_GUY_PATROL_SPEED;
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
        case SPLINE_GUY_RETURNING_TO_SPLINE:
            if (o->oTimer > 3 * 30) {
                // todo: more custom
                o->oAction = SPLINE_GUY_STARTING_TO_WALK;
                break;
            }
    }
    cur_obj_init_animation(o->oAnimationIndex);
}

void bhv_coffee_machine_init(void) {

}

void bhv_coffee_machine_loop(void) {
    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        o->oAnimState += 1;
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
};

void bhv_elevator_door_init(void) {
    if (BPARAM1 == 1)
        o->oElevatorDoorIsOtherDoor = 1;

    o->oAction = ELEVATOR_DOOR_IDLE;
}

void bhv_elevator_door_loop(void) {
    switch (o->oAction) {
        case ELEVATOR_DOOR_IDLE:
            if (gPlayer1Controller->buttonPressed & D_JPAD) {
                o->oAction = ELEVATOR_DOOR_OPEN;
            }
            break;
        case ELEVATOR_DOOR_OPEN:
            if (!o->oElevatorDoorIsOtherDoor) {
                o->oPosZ = approach_f32_symmetric(o->oPosZ, o->oHomeZ - 250.0f, 12.0f);
            } else {
                o->oPosZ = approach_f32_symmetric(o->oPosZ, o->oHomeZ + 250.0f, 12.0f);
            }
    }
}

enum ArmActions {
    ARM_INVISIBLE,
    ARM_RAISING,
    ARM_IDLE,
    ARM_COFFEE_STOLEN,
};

void bhv_arm_init(void) {
    o->oAction = ARM_INVISIBLE;
    o->oAnimState = 0;
    o->oAnimationIndex = 0;
}

void bhv_arm_loop(void) {
    switch (o->oAction) {
        case ARM_INVISIBLE:
            cur_obj_hide();
            o->oAnimState = 0;
            break;
        case ARM_RAISING:
            cur_obj_unhide(),
            o->oAnimationIndex = ARM_ANIM_RAISE;
            if ((cur_obj_check_if_at_animation_end()) && (o->oTimer > 0)) {
                o->oAction = ARM_IDLE;
            }
            break;
        case ARM_IDLE:
            cur_obj_unhide(),
            o->oAnimationIndex = ARM_ANIM_IDLE;
            break; 
        case ARM_COFFEE_STOLEN:
            cur_obj_unhide(),
            o->oAnimState;
            o->oAnimationIndex = ARM_ANIM_COFFEE_STOLEN;
            break;

    cur_obj_init_animation(o->oAnimationIndex);
    }
}

enum CoffeeCupActions {
    COFFEE_CUP_INVISIBLE,
    COFFEE_CUP_EMPTY,
    COFFEE_CUP_HALF,
    COFFEE_CUP_FULL,
};

void bhv_coffee_cup_init(void) {
    o->oAction = COFFEE_CUP_EMPTY;
}

void bhv_coffee_cup_loop(void) {
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