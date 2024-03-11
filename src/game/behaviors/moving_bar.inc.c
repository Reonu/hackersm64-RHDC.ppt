#include "behavior_data.h"

#define Y_SCALE o->header.gfx.scale[1]

/*
--MOVING BARS--
//BPARAM2 controls the maximum size of the bar
//BPARAM3 controls the speed for the bar's growth
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