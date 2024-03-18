#include <ultra64.h>

#include "sm64.h"
#include "segment_names.h"
#include "memory.h"
#include "fpv_camera.h"
#include "area.h"
#include "engine/math_util.h"
#include "actors/group0.h"
#include "confroom.h"
#include "confroom_collision.h"

static s32 sInitializedCollision = FALSE;

// spaces allowed in
static AABB_src *sSrcOfficeSpaces[] = {
    &confroom_spaceMainRoom,
    &confroom_spaceDoorToHall,
};
AABB gOfficeSpaces[ARRAY_COUNT(sSrcOfficeSpaces)] = {};

// spaces not allowed in
static AABB_src *sSrcColAABBs[] = {
    &confroom_colDesk1,
    &confroom_colDesk2,
    &confroom_colDesk3,
    &confroom_colDesk4,
};
AABB gColAABBs[ARRAY_COUNT(sSrcColAABBs)] = {};


f32 get_aabb_min(AABB *aabb, int dimensionIndex) {
    return aabb->pos[dimensionIndex] - aabb->dim[dimensionIndex];
}
ALWAYS_INLINE f32 get_aabb_min_x(AABB *aabb) { return get_aabb_min(aabb, 0); }
ALWAYS_INLINE f32 get_aabb_min_y(AABB *aabb) { return get_aabb_min(aabb, 1); }
ALWAYS_INLINE f32 get_aabb_min_z(AABB *aabb) { return get_aabb_min(aabb, 2); }

f32 get_aabb_max(AABB *aabb, int dimensionIndex) {
    return aabb->pos[dimensionIndex] + aabb->dim[dimensionIndex];
}
ALWAYS_INLINE f32 get_aabb_max_x(AABB *aabb) { return get_aabb_max(aabb, 0); }
ALWAYS_INLINE f32 get_aabb_max_y(AABB *aabb) { return get_aabb_max(aabb, 1); }
ALWAYS_INLINE f32 get_aabb_max_z(AABB *aabb) { return get_aabb_max(aabb, 2); }


void aabb_src_load(AABB *dest, AABB_src *src) {
    AABB_src *space = (AABB_src *)segmented_to_virtual(src);
    vec3f_copy(dest->pos, (*space)[0]);
    vec3f_copy(dest->dim, (*space)[1]);
    vec3_scale(dest->dim, 100);
    dest->min_x = get_aabb_min_x(dest);
    dest->min_y = get_aabb_min_y(dest);
    // dest->min_y = 0; // floor is always 0
    dest->min_z = get_aabb_min_z(dest);
    dest->max_x = get_aabb_max_x(dest);
    dest->max_y = get_aabb_max_y(dest);
    dest->max_z = get_aabb_max_z(dest);
}

void confroom_initialize_collision(void) {
    if (sInitializedCollision) return;
    for (int i = 0; i < ARRAY_COUNT(gOfficeSpaces); i++) {
        aabb_src_load(&gOfficeSpaces[i], sSrcOfficeSpaces[i]);
    }
    for (int i = 0; i < ARRAY_COUNT(gColAABBs); i++) {
        aabb_src_load(&gColAABBs[i], sSrcColAABBs[i]);
    }

    sInitializedCollision = TRUE;
}


s32 test_aabb_cyl(AABB *aabb, Cylinder *cyl) {
    f32 dX = absf(cyl->pos[0] - aabb->pos[0]);
    f32 dZ = absf(cyl->pos[2] - aabb->pos[2]);

    // check too far away on sides
    if (dX > aabb->dim[0] + cyl->radius) return FALSE;
    if (dZ > aabb->dim[2] + cyl->radius) return FALSE;
    
    if (cyl->pos[1] + cyl->height < aabb->min_y) return FALSE;
    if (cyl->pos[1] > aabb->max_y) return FALSE;

    f32 excessX = dX - aabb->dim[0];
    f32 excessZ = dZ - aabb->dim[2];
    if (excessX < 0) excessX = 0; // No excess if inside the AABB on X axis
    if (excessZ < 0) excessZ = 0; // No excess if inside the AABB on Z axis
    f32 cornerDistSq = sqrf(excessX) + sqrf(excessZ); // Square of distance from cylinder's projection to AABB's closest corner/edge

    if (cornerDistSq > sqrf(cyl->radius)) return FALSE;

    return TRUE;
}

s32 test_point_in_aabb(AABB *aabb, Vec3f pos) {
    if (pos[0] < aabb->min_x) return FALSE;
    if (pos[0] > aabb->max_x) return FALSE;
    if (pos[1] < aabb->min_y) return FALSE;
    if (pos[1] > aabb->max_y) return FALSE;
    if (pos[2] < aabb->min_z) return FALSE;
    if (pos[2] > aabb->max_z) return FALSE;

    return TRUE;
}

u32 test_cyl_exit_aabb(AABB *aabb, Cylinder *cyl) {
    u32 exitFlags = 0;

    COND_BIT(cyl->pos[0] - cyl->radius < aabb->min_x, exitFlags, AABB_EXIT_X_NEG);
    COND_BIT(cyl->pos[0] + cyl->radius > aabb->max_x, exitFlags, AABB_EXIT_X_POS);
    COND_BIT(cyl->pos[2] - cyl->radius < aabb->min_z, exitFlags, AABB_EXIT_Z_NEG);
    COND_BIT(cyl->pos[2] + cyl->radius > aabb->max_z, exitFlags, AABB_EXIT_Z_POS);

    COND_BIT(cyl->pos[1]               < aabb->min_y, exitFlags, AABB_EXIT_Y_NEG);
    COND_BIT(cyl->pos[1] + cyl->height > aabb->max_y, exitFlags, AABB_EXIT_Y_POS);
    return exitFlags;
}

AABB *find_space(Cylinder *cyl) {
    for (int i = 0; i < ARRAY_COUNT(gOfficeSpaces); i++) {
        AABB *aabb = &gOfficeSpaces[i];
        if (test_point_in_aabb(aabb, cyl->pos)) return aabb;
    }
    return NULL;
}

// NOTE: does not properly resolve spaces that have a dimension less than the cyl radius
u32 resolve_exit_space(Cylinder *cyl, AABB *aabb, Vec3f res) {
    vec3f_copy(res, cyl->pos);

    u32 exitFlags = test_cyl_exit_aabb(aabb, cyl);
    if (!exitFlags) return exitFlags;
    f32 radius = cyl->radius;

    if (exitFlags & AABB_EXIT_X_NEG) res[0] = aabb->min_x + radius;
    if (exitFlags & AABB_EXIT_X_POS) res[0] = aabb->max_x - radius;

    if (exitFlags & AABB_EXIT_Y_NEG) res[1] = aabb->min_y;
    if (exitFlags & AABB_EXIT_Y_POS) res[1] = aabb->max_y - cyl->height;

    if (exitFlags & AABB_EXIT_Z_NEG) res[2] = aabb->min_z + radius;
    if (exitFlags & AABB_EXIT_Z_POS) res[2] = aabb->max_z - radius;

    return exitFlags;
}

void closest_point_on_line_segment(Vec3f pA, Vec3f pB, Vec3f point, Vec3f res)
{
    Vec3f dAB;
    vec3f_diff(dAB, pB, pA);

    Vec3f dPA;
    vec3f_diff(dPA, point, pA);

    f32 t = vec3f_dot(dPA, dAB) / vec3f_dot(dAB, dAB);
    t = CLAMP(t, 0, 1);
    vec3_scale(dAB, t);

    vec3f_add(dAB, pA);
    vec3f_copy(res, dAB);
}

void vec3f_2d_copy(Vec3f dest, Vec3f src) {
    dest[0] = src[0];
    dest[2] = src[2];
}

void vec3f_set_2d(Vec3f dest, f32 x, f32 z) {
    dest[0] = x;
    dest[2] = z;
}

void aabb_closest_edge_compare(Vec3f pA, Vec3f pB, Vec3f point, f32 a, f32 b, f32 c, f32 d, Cylinder *cyl) {
    Vec3f diff, res;
    vec3f_set_2d(pA, a, b);
    vec3f_set_2d(pB, c, d);
    closest_point_on_line_segment(pA, pB, point, res);
    vec3f_diff(diff, point, res);
    f32 mag = vec3_mag(diff);
    f32 overlap = cyl->radius - mag;

    if (overlap > 0) {
        vec3f_normalize(diff);
        vec3_scale(diff, overlap);
        vec3f_add(point, diff);
    }
}

void resolve_aabb_cyl_collision(AABB *aabb, Cylinder *cyl) {
    Vec3f pA, pB;
    Vec3f cylPos;
    vec3f_2d_copy(cylPos, cyl->pos);
    cylPos[1] = pA[1] = pB[1] = 0;

    aabb_closest_edge_compare(pA, pB, cylPos, aabb->min_x, aabb->min_z, aabb->min_x, aabb->max_z, cyl);
    aabb_closest_edge_compare(pA, pB, cylPos, aabb->max_x, aabb->min_z, aabb->max_x, aabb->max_z, cyl);
    aabb_closest_edge_compare(pA, pB, cylPos, aabb->min_x, aabb->min_z, aabb->max_x, aabb->min_z, cyl);
    aabb_closest_edge_compare(pA, pB, cylPos, aabb->min_x, aabb->max_z, aabb->max_x, aabb->max_z, cyl);

    vec3f_2d_copy(cyl->pos, cylPos);
}

void resolve_confroom_collisions(Cylinder *cyl, Vec3f res) {
    for (int i = 0; i < ARRAY_COUNT(gColAABBs); i++) {
        AABB *aabb = &gColAABBs[i];
        if (test_aabb_cyl(aabb, cyl)) {
            resolve_aabb_cyl_collision(aabb, cyl);
        }
    }
    vec3f_copy(res, cyl->pos);
}

static f32 point_in_aabb_2d(AABB *aabb, Vec3f point) {
    return (
        point[0] > aabb->min_x &&
        point[0] < aabb->max_x &&
        point[2] > aabb->min_z &&
        point[2] < aabb->max_z
    );
}

static f32 point_above_aabb_2d_top(AABB *aabb, Vec3f point) {
    return (
        point[1] > aabb->max_y &&
        point_in_aabb_2d(aabb, point)
    );
}

static f32 point_above_aabb_2d_bottom(AABB *aabb, Vec3f point) {
    return (
        point[1] > aabb->min_y &&
        point_in_aabb_2d(aabb, point)
    );
}

f32 find_confroom_floor(Vec3f pos) {
    f32 floorHeight = -1;
    Vec3f point;
    vec3f_copy_y_off(point, pos, 30);

    for (int i = 0; i < ARRAY_COUNT(gOfficeSpaces); i++) {
        AABB *aabb = &gOfficeSpaces[i];
        if (point_above_aabb_2d_bottom(aabb, point)) {
            if (aabb->min_y > floorHeight) floorHeight = aabb->min_y;
        }
    }
    for (int i = 0; i < ARRAY_COUNT(gColAABBs); i++) {
        AABB *aabb = &gColAABBs[i];
        if (point_above_aabb_2d_top(aabb, point)) {
            if (aabb->max_y > floorHeight) floorHeight = aabb->max_y;
        }
    }
    return floorHeight + 1;
}
