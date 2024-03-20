#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

#include "dudeguy/model.inc.c"
#include "confroom/model.inc.c"
#include "bar/model.inc.c"
#include "bar/collision.inc.c"
#include "donut1/model.inc.c"
#include "donut1/collision.inc.c"
#include "donut2/model.inc.c"
#include "donut2/collision.inc.c"
#include "donut3/model.inc.c"
#include "donut3/collision.inc.c"
#include "correlation/model.inc.c"
#include "correlation/collision.inc.c"
#include "bar_red/collision.inc.c"
#include "bar_pink/collision.inc.c"
#include "checkpoint_flag/model.inc.c"
#include "bar_blue/collision.inc.c"


#include "spring/model.inc.c"

#include "cheesing/model.inc.c"
#include "donut4/model.inc.c"
#include "donut4/collision.inc.c"
#include "donut5/model.inc.c"
#include "donut5/collision.inc.c"
#include "donut6/model.inc.c"
#include "donut6/collision.inc.c"
#include "silver_star/model.inc.c"
#include "dudeguy/anims/data.inc.c"
#include "dudeguy/anims/table.inc.c"
#include "projector_screen/model.inc.c"
#include "circle1/model.inc.c"
#include "circle1/collision.inc.c"
#include "circle2/collision.inc.c"
#include "circle2/model.inc.c"
#include "circle3/collision.inc.c"
#include "circle3/model.inc.c"