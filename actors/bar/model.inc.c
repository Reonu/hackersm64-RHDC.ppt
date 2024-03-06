Vtx bar_bar_mesh_layer_2_vtx_cull[8] = {
	{{ {-100, 0, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 36, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 36, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 0, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 36, 142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 36, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 0, -142}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx bar_bar_mesh_layer_2_vtx_0[20] = {
	{{ {-100, 0, 142}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {-100, 36, 142}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {-100, 36, -142}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-100, 0, -142}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {100, 0, -142}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {100, 36, -142}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {100, 36, 142}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {100, 0, 142}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {100, 0, 142}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {100, 36, 142}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {-100, 36, 142}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {-100, 0, 142}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {-100, 0, -142}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {100, 0, -142}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {100, 0, 142}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-100, 0, 142}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {100, 36, -142}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-100, 36, -142}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-100, 36, 142}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {100, 36, 142}, 0, {624, 240}, {0, 127, 0, 255} }},
};

Gfx bar_bar_mesh_layer_2_tri_0[] = {
	gsSPVertex(bar_bar_mesh_layer_2_vtx_0 + 0, 20, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSPEndDisplayList(),
};


Gfx mat_bar_orang_layer2[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, 0),
	gsSPGeometryMode(G_ZBUFFER | G_SHADE, 0),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_NOOP),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 87, 0, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bar_orang_layer2[] = {
	gsDPPipeSync(),
	gsSPGeometryMode(0, G_ZBUFFER | G_SHADE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_DECAL2),
	gsSPEndDisplayList(),
};

Gfx bar_bar_mesh_layer_2[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(bar_bar_mesh_layer_2_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_bar_orang_layer2),
	gsSPDisplayList(bar_bar_mesh_layer_2_tri_0),
	gsSPDisplayList(mat_revert_bar_orang_layer2),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

