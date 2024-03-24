Vtx coffee_cup_empty_coffeemptyobject_mesh_layer_1_vtx_cull[8] = {
	{{ {-9, -9, 7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, 9, 7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, 9, -7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, -9, -7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, -9, 7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, 9, 7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, 9, -7}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, -9, -7}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx coffee_cup_empty_coffeemptyobject_mesh_layer_1_vtx_0[17] = {
	{{ {0, -6, 7}, 0, {1008, 496}, {0, 161, 84, 255} }},
	{{ {0, -9, -7}, 0, {1008, -16}, {0, 131, 23, 255} }},
	{{ {9, -2, -7}, 0, {803, -16}, {119, 218, 24, 255} }},
	{{ {7, -1, 7}, 0, {803, 496}, {90, 226, 85, 255} }},
	{{ {6, 9, -7}, 0, {598, -16}, {73, 101, 27, 255} }},
	{{ {4, 6, 7}, 0, {598, 496}, {55, 76, 86, 255} }},
	{{ {-6, 9, -7}, 0, {394, -16}, {183, 101, 27, 255} }},
	{{ {-4, 6, 7}, 0, {394, 496}, {201, 76, 86, 255} }},
	{{ {-9, -2, -7}, 0, {189, -16}, {137, 218, 24, 255} }},
	{{ {-7, -1, 7}, 0, {189, 496}, {166, 226, 85, 255} }},
	{{ {0, -9, -7}, 0, {-16, -16}, {0, 131, 23, 255} }},
	{{ {0, -6, 7}, 0, {-16, 496}, {0, 161, 84, 255} }},
	{{ {-7, -1, 7}, 0, {518, 676}, {166, 226, 85, 255} }},
	{{ {0, -6, 7}, 0, {752, 506}, {0, 161, 84, 255} }},
	{{ {7, -1, 7}, 0, {986, 676}, {90, 226, 85, 255} }},
	{{ {-4, 6, 7}, 0, {608, 951}, {201, 76, 86, 255} }},
	{{ {4, 6, 7}, 0, {896, 951}, {55, 76, 86, 255} }},
};

Gfx coffee_cup_empty_coffeemptyobject_mesh_layer_1_tri_0[] = {
	gsSPVertex(coffee_cup_empty_coffeemptyobject_mesh_layer_1_vtx_0 + 0, 17, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(3, 2, 4, 0, 3, 4, 5, 0),
	gsSP2Triangles(5, 4, 6, 0, 5, 6, 7, 0),
	gsSP2Triangles(7, 6, 8, 0, 7, 8, 9, 0),
	gsSP2Triangles(9, 8, 10, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 14, 15, 12, 0),
	gsSP1Triangle(14, 16, 15, 0),
	gsSPEndDisplayList(),
};


Gfx mat_coffee_cup_empty_cup[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0, PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0),
	gsSPGeometryMode(G_CULL_BACK, 0),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 179, 82, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_coffee_cup_empty_cup[] = {
	gsDPPipeSync(),
	gsSPGeometryMode(0, G_CULL_BACK),
	gsSPEndDisplayList(),
};

Gfx coffee_cup_empty_coffeemptyobject_mesh_layer_1[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(coffee_cup_empty_coffeemptyobject_mesh_layer_1_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_coffee_cup_empty_cup),
	gsSPDisplayList(coffee_cup_empty_coffeemptyobject_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_coffee_cup_empty_cup),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

