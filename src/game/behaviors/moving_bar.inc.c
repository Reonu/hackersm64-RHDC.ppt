#define Y_SCALE o->header.gfx.scale[1]

/*
--MOVING BARS--
//BPARAM2 controls the maximum size of the bar
//BPARAM3 controls the speed for the bar's growth
BPARAM4 applies a phase offset
*/

void bhv_moving_bar_init(void) {
    o->oBitfsPlatformTimer += BPARAM4 * 200;
}

void bhv_moving_bar_loop(void) {
    Y_SCALE = ((sins(o->oBitfsPlatformTimer) + 1.1 ) * BPARAM2) * 0.2f;

    o->oBitfsPlatformTimer += BPARAM3 * 2; 

    if (!obj_has_model(o,MODEL_BAR_YELLOW)) {
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