#include "game/include_for_vsc.h"
#include "game/print.h"

void bhv_dudeguy_init(void) {
    o->oAnimationIndex = BPARAM1;
}

void bhv_dudeguy_loop(void) {
    cur_obj_init_animation(o->oAnimationIndex);

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
                o->oAction = SPLINE_GUY_CONVERSATION;
                o->oFaceAngleYaw = atan2s(gFPVPlayer.pos[2] - pos[2], gFPVPlayer.pos[0] - pos[0]);
                o->oOldAngle = o->oFaceAngleYaw;
                o->oForwardVel = 0;
                gFPVPlayer.dir[1] = o->oFaceAngleYaw + DEGREES(180);
                Vec3f speakerPos = { o->oPosX, o->oPosY + 180, o->oPosZ };
                start_convo(3, speakerPos);
                break;
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
        case SPLINE_GUY_CONVERSATION:
            if (gCurConvo.state == CONVO_INACTIVE) {
                o->oAction = SPLINE_GUY_RETURNING_TO_SPLINE;
            } else if (gCurConvo.state == CONVO_TALKING) {
                o->oAnimationIndex = NPC_ANIM_TALKING;
            } else {
                o->oAnimationIndex = NPC_ANIM_IDLE;
            }
            break;
        case SPLINE_GUY_RETURNING_TO_SPLINE:
            if (o->oTimer > 3 * 30) {
                // todo: more custom
                o->oAction = SPLINE_GUY_STARTING_TO_WALK;
                break;
            }
    }
    cur_obj_init_animation(o->oAnimationIndex);
}
