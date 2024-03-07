#define Y_SCALE o->header.gfx.scale[1]

void bhv_moving_bar_loop(void) {
    Y_SCALE += (sins(o->oBitfsPlatformTimer) * BPARAM2) / 255.0f;
    o->oBitfsPlatformTimer += 0x100;
    load_object_collision_model();
}

void bhv_donut_loop(void) {
    o->oFaceAngleRoll += 128;
    load_object_collision_model();
}