#include <ultra64.h>

#include "sm64.h"
#include "confroom.h"
#include "model_ids.h"
#include "object_fields.h"
#include "engine/math_util.h"
#include "behavior_data.h"
#include "actors/group0.h"

ConfroomObjectSpawn *gConfroomSpawns[] = {
    &spawn_dudeGuy1,
    &spawn_dudeGuyPhone,
};

s32 gNumConfroomSpawns = ARRAY_COUNT(gConfroomSpawns);

s32 spawn_confroom_objects(UNUSED s16 initOrUpdate, s32 reg) {
    for (int i = 0; i < ARRAY_COUNT(gConfroomSpawns); i++) {
        ConfroomObjectSpawn *spawn = (ConfroomObjectSpawn *)segmented_to_virtual(gConfroomSpawns[i]);
        const BehaviorScript *behaviorAddr = segmented_to_virtual((void *)spawn->bhv);
        struct Object *obj = &gConfroomObjectPool[i];
        obj->curBhvCommand = behaviorAddr;
        obj->behavior = behaviorAddr;

        obj->activeFlags = ACTIVE_FLAG_ACTIVE | ACTIVE_FLAG_ALLOCATED;
        obj->parentObj = obj;
        obj->prevObj = NULL;
        obj->collidedObjInteractTypes = 0;
        obj->numCollidedObjs = 0;
        for (i = 0; i < MAX_OBJECT_FIELDS; i++) {
            obj->rawData.asS32[i] = 0;
        }
        obj->unused1 = 0;
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
            BPARAM_NSHIFT(spawn->bp1, 1) |
            BPARAM_NSHIFT(spawn->bp2, 2) |
            BPARAM_NSHIFT(spawn->bp3, 3) |
            BPARAM_NSHIFT(spawn->bp4, 4);
        obj->oBehParams2ndByte = GET_BPARAM2(obj->oBehParams);
    }

    return reg;
}

s32 get_num_confroom_objects(void) {
    return gNumConfroomSpawns;
}
