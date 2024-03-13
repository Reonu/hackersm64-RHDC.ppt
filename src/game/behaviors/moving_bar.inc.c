#include "behavior_data.h"
#include "engine/math_util.h"
#include "game/game_init.h"
#include "game/level_update.h"
#include "game/spawn_sound.h"ç
#include "game/print.h"

#define Y_SCALE o->header.gfx.scale[1]

/*
--MOVING BARS--
BPARAM2 controls the maximum size of the bar
BPARAM3 controls the speed for the bar's growth
BPARAM4 applies a phase offset
*/

void bhv_moving_bar_init(void) {
    o->oBarTimer += BPARAM4 * 200;

    if (o->behavior == segmented_to_virtual(bhvMovingBar)) {
        o->oPrimRGB = 0xFF5700;
    } else if (o->behavior == segmented_to_virtual(bhvMovingBarRed)) {
        o->oPrimRGB = 0xFF0000;
    } else if (o->behavior == segmented_to_virtual(bhvMovingBarYellow)) {
        o->oPrimRGB = 0xFFFF00;
    } else if (o->behavior == segmented_to_virtual(bhvMovingBarPink)) {
        o->oPrimRGB = 0xC800CE;
    } else if (o->behavior == segmented_to_virtual(bhvMovingBarBlue)) {
        o->oPrimRGB = 0x004AFF;
    } else {
        o->oPrimRGB = 0xFF5700;
    }
}

void bhv_moving_bar_loop(void) {
    Y_SCALE = ((sins(o->oBarTimer) + 1.1 ) * BPARAM2) * 0.2f;

    o->oBarTimer += BPARAM3 * 2; 

    if (o->behavior != segmented_to_virtual(bhvMovingBarYellow)) {
        load_object_collision_model();
    }

    if (BPARAM4) {
        //print_text_fmt_int(20,20,"",o->oScaleY);
    }
    
}

/*
--DONUT GRAPH
it


rotates
*/
void bhv_donut_loop(void) {
    o->oFaceAngleRoll += 128;
    load_object_collision_model();
}

void bhv_correlation_loop(void) {
    o->oPosY -= (sins(o->oBitfsPlatformTimer) * BPARAM2);
    o->oBitfsPlatformTimer += 0x300;
    load_object_collision_model();
}

/*CHECKPOINT FLAG*/

void bhv_checkpoint_flag_loop(void) {
    if (o->oCheckpointFlagCounter == 0) {
        if (o->oDistanceToMario < 100.f) {
            o->oCheckpointFlagCounter++;
            vec3_copy(gMarioRespawn,gMarioState->pos);
            cur_obj_play_sound_2(SOUND_GENERAL2_RIGHT_ANSWER);
        }
    }
    
    if (o->oCheckpointFlagCounter == 1) {
        o->oPrimRGB = 0xFF0000;
    } else {
        o->oPrimRGB = 0x787878;
    }
}

void bhv_cheesing_loop(void) {
    if (gMarioState->action == ACT_QUICKSAND_DEATH) {
        o->oPosY = o->oHomeY;
    } else {
        o->oPosY = o->oHomeY - 3000;
    }
}

void bhv_starting_point_init(void) {
    gMarioState->pos[0] = o->oPosX;
    gMarioState->pos[1] = o->oPosY;
    gMarioState->pos[2] = o->oPosZ;
    
    gMarioState->marioObj->oPosX = o->oPosX;
    gMarioState->marioObj->oPosY = o->oPosY;
    gMarioState->marioObj->oPosZ = o->oPosZ;

    gMarioState->marioObj->header.gfx.pos[0] = o->oPosX;
    gMarioState->marioObj->header.gfx.pos[1] = o->oPosY;
    gMarioState->marioObj->header.gfx.pos[2] = o->oPosZ;

    vec3f_copy(gMarioRespawn,gMarioState->pos);
    mark_obj_for_deletion(o);  
}