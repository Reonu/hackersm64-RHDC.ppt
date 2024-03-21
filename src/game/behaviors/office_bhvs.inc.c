#include "game/include_for_vsc.h"

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
};

#define SPLINE_GUY_PATROL_SPEED meters_sec(1.6f)
#define SPLINE_GUY_CHASE_SPEED  meters_sec(1.75f)
#define SPLINE_GUY_NEXT_POINT_THRESHOLD  160

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

void bhv_spline_dudeguy_loop(void) {
    cur_obj_init_animation(o->oAnimationIndex);
    ConfroomObjectSplineRef *spline = &gConfroomSplines[BPARAM2];
    s32 pIndex = o->oSplineDudeGuyPointIndex;
    f32 *vel = &o->oVelX;
    f32 *pos = &o->oPosX;

    switch (o->oAction) {
        case SPLINE_GUY_WALKING: {
            // f32 *curPoint = spline->points[pIndex];
            struct Object *splineStopper = find_closest_office_obj_with_bhv(segmented_to_virtual(bhvSplineStopper), 200.f);
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
                o->oAction = SPLINE_GUY_STOPPING;
            }
            break;
        }
        case SPLINE_GUY_STOPPING: {
            o->oAnimationIndex = NPC_ANIM_STOPWALKING;
            if (o->header.gfx.animInfo.animFrame <= (o->header.gfx.animInfo.curAnim->loopEnd - 2)) {
                o->oAction = SPLINE_GUY_STOPPED;
            }
            break;
        }
        case SPLINE_GUY_STOPPED:
            o->oAnimationIndex = (o->oStopperObject->oBehParams >> 24) & 0xFF; 
            if (o->oTimer >= 120) {
                o->oAction = SPLINE_GUY_WALKING;
            }
            break;



    }
}