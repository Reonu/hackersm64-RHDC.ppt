#include <ultra64.h>

#include "sm64.h"
#include "cozy_print.h"
#include "segment2.h"

void render_rgba16_tex_rect(Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[]) {
    Gfx *gfx = *head;

    gDPPipeSync(gfx++);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    gDPSetCombineLERP(gfx++,
        0, 0, 0, TEXEL0,
        0, 0, 0, TEXEL0,
        0, 0, 0, TEXEL0,
        0, 0, 0, TEXEL0
    );

    gDPSetRenderMode(gfx++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
    gDPSetTextureFilter(gfx++, G_TF_POINT);
    gDPLoadTextureTile(gfx++,
        tex,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        width,
        height,
        0, 0,
        (width - 1), (height - 1),
        NULL,
        (G_TX_NOMIRROR | G_TX_CLAMP),
        (G_TX_NOMIRROR | G_TX_CLAMP),
        0, 0,
        0, 0
    );
    gSPScisTextureRectangle(gfx++,
        qs102(x),
        qs102(y),
        qs102(x + width),
        qs102(y + height),
        G_TX_RENDERTILE,
        qs105(0),
        qs105(0),
        qs510(1),
        qs510(1)
    );
    gSPDisplayList(gfx++, dl_hud_img_end);
    *head = gfx;
}

void render_rgba16_tex_rect_xlu_val(Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[], s32 alpha, s32 val) {
    Gfx *gfx = *head;

    gDPPipeSync(gfx++);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    gDPSetCombineLERP(gfx++,
        /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ PRIMITIVE, 0, TEXEL0, 0,
        /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ PRIMITIVE, 0, TEXEL0, 0
    );
    gDPSetPrimColor(gfx++, 0, 0, val, val, val, alpha);

    gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetTextureFilter(gfx++, G_TF_POINT);
    gDPLoadTextureTile(gfx++,
        tex,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        width,
        height,
        0, 0,
        (width - 1), (height - 1),
        NULL,
        (G_TX_NOMIRROR | G_TX_CLAMP),
        (G_TX_NOMIRROR | G_TX_CLAMP),
        0, 0,
        0, 0
    );
    gSPScisTextureRectangle(gfx++,
        qs102(x),
        qs102(y),
        qs102(x + width),
        qs102(y + height),
        G_TX_RENDERTILE,
        qs105(0),
        qs105(0),
        qs510(1),
        qs510(1)
    );
    gSPDisplayList(gfx++, dl_hud_img_end);
    *head = gfx;
}

void render_rgba16_tex_rect_all(
    Gfx **head, s32 width, s32 height, s32 x, s32 y, const Texture tex[], s32 alpha, s32 r, s32 g, s32 b) {
    Gfx *gfx = *head;

    gDPPipeSync(gfx++);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    if (alpha >= 255) {
        gDPSetCombineLERP(gfx++,
            /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ 0, 0, 0, TEXEL0,
            /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ 0, 0, 0, TEXEL0
        );
    } else {
        gDPSetCombineLERP(gfx++,
            /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ PRIMITIVE, 0, TEXEL0, 0,
            /* col */ PRIMITIVE, 0, TEXEL0, 0, /* alpha */ PRIMITIVE, 0, TEXEL0, 0
        );
    }
    gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, alpha);

    gDPSetTextureFilter(gfx++, G_TF_POINT);
    gDPLoadTextureTile(gfx++,
        tex,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        width,
        height,
        0, 0,
        (width - 1), (height - 1),
        NULL,
        (G_TX_NOMIRROR | G_TX_CLAMP),
        (G_TX_NOMIRROR | G_TX_CLAMP),
        0, 0,
        0, 0
    );
    gSPScisTextureRectangle(gfx++,
        qs102(x),
        qs102(y),
        qs102(x + width),
        qs102(y + height),
        G_TX_RENDERTILE,
        qs105(0),
        qs105(0),
        qs510(1),
        qs510(1)
    );
    gSPDisplayList(gfx++, dl_hud_img_end);
    *head = gfx;
}

void render_rect(Gfx **head, s32 x, s32 y, s32 width, s32 height, s32 r, s32 g, s32 b, s32 init) {
    Gfx *gfx = *head;
    if (init) {
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetCombineLERP(gfx++,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, 1,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, 1
        );
        gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, 255);
    gDPFillRectangle(gfx++,
        x,
        y,
        x + width,
        y + height
    );
    gDPPipeSync(gfx++);
    *head = gfx;
}

void render_rect_xlu(Gfx **head, s32 x, s32 y, s32 width, s32 height, s32 r, s32 g, s32 b, s32 a, s32 init) {
    Gfx *gfx = *head;
    if (init) {
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetCombineLERP(gfx++,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE
        );
    }
    if (a == 255) {
        gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    } else {
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    }
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, a);
    gDPFillRectangle(gfx++,
        x,
        y,
        x + width,
        y + height
    );
    gDPPipeSync(gfx++);
    *head = gfx;
}

void render_rect_cld(Gfx **head, s32 x, s32 y, s32 width, s32 height, s32 r, s32 g, s32 b, s32 a, s32 init) {
    Gfx *gfx = *head;
    if (init) {
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetCombineLERP(gfx++,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE,
            0, 0, 0, PRIMITIVE
        );
    }
    if (a == 255) {
        gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    } else {
        gDPSetRenderMode(gfx++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    }
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, a);
    gDPFillRectangle(gfx++,
        x,
        y,
        x + width,
        y + height
    );
    gDPPipeSync(gfx++);
    *head = gfx;
}
