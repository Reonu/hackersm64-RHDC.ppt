#define Y_SCALE o->header.gfx.scale[1]

void bhv_moving_bar_loop(void) {
    Y_SCALE += sins(o->oBitfsPlatformTimer) * 0.75;
    o->oBitfsPlatformTimer += 0x100;
}