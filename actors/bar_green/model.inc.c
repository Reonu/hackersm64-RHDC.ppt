Vtx bar_green_bar_003_mesh_layer_0_vtx_cull[8] = {
	{{ {-100, 0, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 36, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 36, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 0, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 36, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 36, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx bar_green_bar_003_mesh_layer_0_vtx_0[14] = {
	{{ {-100, 0, 142}, 0, {368, 1008}, {255, 255, 255, 255} }},
	{{ {-100, 36, 142}, 0, {624, 1008}, {255, 255, 255, 255} }},
	{{ {-100, 36, -142}, 0, {624, 752}, {255, 255, 255, 255} }},
	{{ {-100, 0, -142}, 0, {368, 752}, {255, 255, 255, 255} }},
	{{ {100, 0, 142}, 0, {368, 240}, {255, 255, 255, 255} }},
	{{ {-100, 36, 142}, 0, {624, -16}, {255, 255, 255, 255} }},
	{{ {-100, 0, 142}, 0, {368, -16}, {255, 255, 255, 255} }},
	{{ {100, 36, 142}, 0, {624, 240}, {255, 255, 255, 255} }},
	{{ {100, 0, -142}, 0, {368, 496}, {255, 255, 255, 255} }},
	{{ {-100, 0, -142}, 0, {112, 496}, {255, 255, 255, 255} }},
	{{ {-100, 0, 142}, 0, {112, 240}, {255, 255, 255, 255} }},
	{{ {100, 36, -142}, 0, {624, 496}, {255, 255, 255, 255} }},
	{{ {-100, 36, 142}, 0, {880, 240}, {255, 255, 255, 255} }},
	{{ {-100, 36, -142}, 0, {880, 496}, {255, 255, 255, 255} }},
};

Gfx bar_green_bar_003_mesh_layer_0_tri_0[] = {
	gsSPVertex(bar_green_bar_003_mesh_layer_0_vtx_0 + 0, 14, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
	gsSP2Triangles(8, 7, 4, 0, 9, 8, 4, 0),
	gsSP2Triangles(9, 4, 10, 0, 8, 11, 7, 0),
	gsSP2Triangles(11, 12, 7, 0, 11, 13, 12, 0),
	gsSPEndDisplayList(),
};


Gfx mat_bar_green_green_layer0[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, 0),
	gsSPGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING, 0),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_NOOP),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 3, 205, 0, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bar_green_green_layer0[] = {
	gsDPPipeSync(),
	gsSPGeometryMode(0, G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsDPSetColorDither(G_CD_MAGICSQ),
	gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx bar_green_bar_003_mesh_layer_0[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(bar_green_bar_003_mesh_layer_0_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_bar_green_green_layer0),
	gsSPDisplayList(bar_green_bar_003_mesh_layer_0_tri_0),
	gsSPDisplayList(mat_revert_bar_green_green_layer0),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

