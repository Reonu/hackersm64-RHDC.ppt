#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"
#include "confroom.h"

extern ConfroomObjectSpawn gConfroomSpawns[];
extern s32 gNumConfroomSpawns;

s32 spawn_confroom_objects(UNUSED s16 initOrUpdate, s32 reg);
s32 get_num_confroom_objects(void);
