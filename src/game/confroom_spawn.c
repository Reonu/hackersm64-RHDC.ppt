#include <ultra64.h>

#include "sm64.h"
#include "confroom.h"
#include "fpv_player.h"
#include "model_ids.h"
#include "object_fields.h"
#include "engine/math_util.h"
#include "behavior_data.h"
#include "actors/group0.h"

#define CONF_SPLINE(var) { .size = ARRAY_COUNT(var), .points = var }

ConfroomObjectSplineRef gConfroomSplines[] = {
    // CONF_SPLINE(ExampleSplineArrayVariable),
    CONF_SPLINE(confroom_pathAroundDesk),
    CONF_SPLINE(confroom_pathNextToConfroom),
    CONF_SPLINE(confroom_pathNextToReception)
};

ConfroomObjectSpawn *gConfroomSpawns[] = {
    &spawn_dudeGuy1,
    &spawn_dudeGuyPhone,
    &spawn_dudeGuySpline,
    &spawn_dudeGuySpline2,
    &spawn_dudeGuySplineWater,
    &spawn_dudeGuyTyping,
    &spawn_dudeGuyTyping2,
    &spawn_dudeGuyEating,
    &spawn_dudeGuyBored,
    &spawn_splineStopperShelf,
    &spawn_splineStopperWater,
    &spawn_coffeeMachine,
    &spawn_elevatorDoorLeft,
    &spawn_elevatorDoorRight,
    &spawn_arm,
    &spawn_coffeeCup,
};

s32 gNumConfroomSpawns = ARRAY_COUNT(gConfroomSpawns);

s32 spawn_confroom_objects(UNUSED s16 initOrUpdate, s32 reg) {
    for (int i = 0; i < ARRAY_COUNT(gConfroomSplines); i++) {
        ConfroomObjectSplineRef *spline = &gConfroomSplines[i];
        // fix segmented address now
        spline->points = (Vec3f *)segmented_to_virtual(spline->points);
    }

    for (int i = 0; i < ARRAY_COUNT(gConfroomSpawns); i++) {
        ConfroomObjectSpawn *spawn = (ConfroomObjectSpawn *)segmented_to_virtual(gConfroomSpawns[i]);
        const BehaviorScript *behaviorAddr = segmented_to_virtual((void *)spawn->bhv);
        struct Object *obj = &gConfroomObjectPool[i];
        if ((void *)spawn->bhv == bhvArm) gFPVPlayer.arm = obj;
        if ((void *)spawn->bhv == bhvCoffeeCup) gFPVPlayer.coffeeCup = obj;

        obj->curBhvCommand = behaviorAddr;
        obj->behavior = behaviorAddr;

        obj->activeFlags = ACTIVE_FLAG_ACTIVE | ACTIVE_FLAG_ALLOCATED;
        obj->parentObj = obj;
        obj->prevObj = NULL;
        obj->collidedObjInteractTypes = 0;
        obj->numCollidedObjs = 0;
        for (int j = 0; j < MAX_OBJECT_FIELDS; j++) {
            obj->rawData.asS32[j] = 0;
        }
        obj->isOfficeObject = TRUE;
        obj->bhvStackIndex = 0;
        obj->bhvDelayTimer = 0;

        obj->hitboxRadius = 50.0f;
        obj->hitboxHeight = 100.0f;
        obj->hurtboxRadius = 0.0f;
        obj->hurtboxHeight = 0.0f;
        obj->hitboxDownOffset = 0.0f;
        obj->unused2 = 0;

        obj->platform = NULL;
        obj->collisionData = NULL;
        obj->oIntangibleTimer = -1;
        obj->oDamageOrCoinValue = 0;
        obj->oHealth = 2048;

        obj->oCollisionDistance = 1000.0f;
        obj->oDrawingDistance = 20000.0f;

        mtxf_identity(obj->transform);

        obj->respawnInfoType = RESPAWN_INFO_TYPE_NULL;
        obj->respawnInfo = NULL;

        obj->oDistanceToMario = 0.0f;
        obj->oRoom = -1;

        obj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
        vec3f_copy(obj->header.gfx.scale, gVec3fOne);
        obj->header.gfx.throwMatrix = NULL;
        obj->header.gfx.areaIndex = 0;

        geo_obj_init(
            (struct GraphNodeObject *) &obj->header.gfx,
            gLoadedGraphNodes[spawn->modelID],
            spawn->pos,
            spawn->angle
        );
        vec3f_copy(&obj->oPosX, obj->header.gfx.pos);
        obj->oFaceAnglePitch = spawn->angle[0];
        obj->oFaceAngleYaw =   spawn->angle[1];
        obj->oFaceAngleRoll =  spawn->angle[2];
        vec3_same(obj->header.gfx.scale, 1);
        obj->oBehParams =
            (spawn->bp1 << 24) |
            (spawn->bp2 << 16) |
            (spawn->bp3 << 8) |
            (spawn->bp4);
        obj->oBehParams2ndByte = GET_BPARAM2(obj->oBehParams);
    }

    return reg;
}

s32 get_num_confroom_objects(void) {
    return ARRAY_COUNT(gConfroomSpawns);
}
