#include "game/print.h"
#include "game/object_list_processor.h"
#include "src/engine/surface_collision.h"
#include "game/game_init.h"
#include "engine/math_util.h"

void bhv_spring_init(void) {
    switch (BPARAM2) {
        case 0x00:
            o->oFriction = 40.0f;
            o->oPrimRGB = 0x00FF00;
            break;
        case 0x01:
            o->oFriction = 90.0f;
            o->oPrimRGB = 0xFFFF00;
            break;
        case 0x02:
            o->oFriction = 126.0f;
            o->oPrimRGB = 0xFF0000;
            break;
    }
    o->oGravity = 2.5f;
}

void bhv_spring_loop(void) {
    if (o->oAction == 1) {
        if (o->oTimer == 1){
                if (gMarioState->heldObj == NULL) {
                    gMarioState->action = ACT_DOUBLE_JUMP;
                }
            }
        if (o->oTimer < 4) {
            o->header.gfx.scale[1] -= 0.125f;
        }
        else if (o->oTimer < 8) {
            o->header.gfx.scale[1] += 0.125f;
        }
        else {
            o->header.gfx.scale[1] = 1;
            o->oAction = 0;
        }
    }
    if ((gMarioState->pos[1] <= gMarioState->floorHeight) || ((gMarioState->heldObj != NULL) && (gMarioState->heldObj == o))) {
    o->oInteractType = INTERACT_GRABBABLE;
    o->oFlags |= OBJ_FLAG_HOLDABLE;
    } else {
    o->oInteractType = INTERACT_SPRING;
    o->oFlags &= ~OBJ_FLAG_HOLDABLE;
    }

    if (find_floor_height(o->oPosX,o->oPosY,o->oPosZ) < o->oPosY) {
        o->oInteractType = INTERACT_GRABBABLE;
        o->oFlags |= OBJ_FLAG_HOLDABLE;       
    }
    
    if (o->oInteractType == INTERACT_GRABBABLE) {
            switch (o->oHeldState) {
            case HELD_FREE:
                breakable_box_small_idle_loop();
                break;

            case HELD_HELD:
                cur_obj_disable_rendering();
                cur_obj_become_intangible();
                break;

            case HELD_THROWN:
                breakable_box_small_get_dropped();
                break;

            case HELD_DROPPED:
                breakable_box_small_get_dropped();
                break;
        }
    }

    if (gMarioState->heldObj != NULL) {
        if (gMarioState->heldObj == gCurrentObject) {
            gMarioState->heldObj->oPrimRGB = o->oPrimRGB;
        }
    }
    if (g2dMode) {
        o->oPosZ = 0;
        if (o->oPosY < -2500) {
            o->oPosX = o->oHomeX;
            o->oPosY = o->oHomeY;
            o->oPosZ = o->oHomeZ;
        }
        o->oFaceAngleYaw = DEGREES(90);
    }

}