#define Y_SCALE o->header.gfx.scale[1]

void bhv_moving_bar_loop(void) {
    if (BPARAM3) {
        Y_SCALE += (sins(o->oBitfsPlatformTimer) * BPARAM2) / 64.0f;
    } else {
        Y_SCALE += (sins(o->oBitfsPlatformTimer) * BPARAM2) / 255.0f;
    }
    
    if (BPARAM3) {
        o->oBitfsPlatformTimer += BPARAM3 * 6; 
    } else {
        o->oBitfsPlatformTimer += 0x100;
    }
    load_object_collision_model();
}

void bhv_donut_loop(void) {
    o->oFaceAngleRoll += 128;
    load_object_collision_model();
}

void bhv_correlation_loop(void) {
    o->oPosY -= (sins(o->oBitfsPlatformTimer) * BPARAM2);
    o->oBitfsPlatformTimer += 0x300;
    load_object_collision_model();
}