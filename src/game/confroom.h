#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"

#define NUM_CONFROOM_OBJECTS 32

s32 conf_room_check_should_init(void);
s32 process_conf_room(void);
Gfx *geo_render_projector_screen(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx);
