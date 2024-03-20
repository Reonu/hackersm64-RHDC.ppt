void bhv_dudeguy_loop(void) {
    cur_obj_init_animation(o->oAnimationIndex);
    if (gPlayer1Controller->buttonPressed & R_JPAD) {
        o->oAnimationIndex++;
    } else if (gPlayer1Controller->buttonPressed & L_JPAD) {
        o->oAnimationIndex--;
    }

    o->oFaceAngleYaw += DEGREES(5);
}