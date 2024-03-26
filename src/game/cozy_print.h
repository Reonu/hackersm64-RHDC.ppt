#pragma once

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"

void render_rgba16_tex_rect(Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[]);
void render_rgba16_tex_rect_xlu_val(Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[], s32 alpha, s32 val);
void render_rgba16_tex_rect_all(
    Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[], s32 alpha, s32 r, s32 g, s32 b);
void render_rect(Gfx **head, s32 x, s32 y, s32 width, s32 height, s32 r, s32 g, s32 b, s32 init);
void render_rect_xlu(Gfx **head, s32 x, s32 y, s32 width, s32 height, s32 r, s32 g, s32 b, s32 a, s32 init);
