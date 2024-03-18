#pragma once

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"

enum AABBExits {
    AABB_NO_EXIT    = 0,
    AABB_EXIT_X_NEG = BIT(1),
    AABB_EXIT_X_POS = BIT(2),
    AABB_EXIT_X     = AABB_EXIT_X_NEG | AABB_EXIT_X_POS,
    AABB_EXIT_Y_NEG = BIT(3),
    AABB_EXIT_Y_POS = BIT(4),
    AABB_EXIT_Y     = AABB_EXIT_Y_NEG | AABB_EXIT_Y_POS,
    AABB_EXIT_Z_NEG = BIT(5),
    AABB_EXIT_Z_POS = BIT(6),
    AABB_EXIT_Z     = AABB_EXIT_Z_NEG | AABB_EXIT_Z_POS,
};

// pos/scale
// root is @ center
typedef f32 AABB_src[2][3];

typedef struct {
    Vec3f pos;
    Vec3f dim;
    f32 min_x;
    f32 max_x;
    f32 min_y;
    f32 max_y;
    f32 min_z;
    f32 max_z;
} AABB;

// root is at bottom
typedef struct {
    Vec3f pos;
    f32 radius;
    f32 height;
} Cylinder;

void confroom_initialize_collision(void);
AABB *find_space(Cylinder *cyl);
u32 resolve_exit_space(Cylinder *cyl, AABB *aabb, Vec3f res);
void resolve_confroom_collisions(Cylinder *cyl, Vec3f res);
f32 find_confroom_floor(Vec3f pos);
