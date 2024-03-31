#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group0.h"
#include "behavior_data.h"

#include "bubble/geo.inc.c"
#include "walk_smoke/geo.inc.c"
#include "burn_smoke/geo.inc.c"
#include "small_water_splash/geo.inc.c"
#include "water_wave/geo.inc.c"
#include "sparkle/geo.inc.c"
#include "water_splash/geo.inc.c"
#include "sparkle_animation/geo.inc.c"
#include "mario/geo.inc.c"
#include "confroom/data.c"
#include "confroom/root.inc.c"

ConfroomObjectSpawn spawn_arm = {
    {   0,   999,  0 },
    { 0, 0, 0 },
    MODEL_ARM,
    (uintptr_t)bhvArm,
    0,
    0,
    0,
    0,
    0
};

ConfroomObjectSpawn spawn_coffeeCup = {
    {   0,   999,  0 },
    { 0, 0, 0 },
    MODEL_COFFEE_CUP_EMPTY,
    (uintptr_t)bhvCoffeeCup,
    0,
    0,
    0,
    0,
    0
};

#include "dudeguy/geo.inc.c"
#include "confroom/geo.inc.c"
#include "bar/geo.inc.c"
#include "donut1/geo.inc.c"
#include "donut2/geo.inc.c"
#include "donut3/geo.inc.c"

#include "correlation/geo.inc.c"
#include "checkpoint_flag/geo.inc.c"

#include "spring/geo.inc.c"

#include "cheesing/geo.inc.c"
#include "donut4/geo.inc.c"
#include "donut5/geo.inc.c"
#include "donut6/geo.inc.c"
#include "silver_star/geo.inc.c"
#include "projector_screen/geo.inc.c"
#include "circle1/geo.inc.c"
#include "circle2/geo.inc.c"
#include "circle3/geo.inc.c"

#include "dudeguy_phone/geo.inc.c"
#include "coffee_machine/geo.inc.c"
#include "elevator_door/geo.inc.c"
#include "chattykathy/geo.inc.c"
#include "arm/geo.inc.c"
#include "coffee_cup_full/geo.inc.c"
#include "coffee_cup_half/geo.inc.c"
#include "coffee_cup_empty/geo.inc.c"

#include "b_button/geo.inc.c"

#include "light_switch_up/geo.inc.c"
#include "light_switch_down/geo.inc.c"
#include "ending_dudeguy/geo.inc.c"
#include "exclamation_mark/geo.inc.c"