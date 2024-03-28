Vtx coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_cull[8] = {
	{{ {-9, -7, 9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, 7, 9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, 7, -9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-9, -7, -9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, -7, 9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, 7, 9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, 7, -9}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {9, -7, -9}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_0[18] = {
	{{ {0, 5, -8}, 0, {1008, 59}, {0, 24, 131, 255} }},
	{{ {0, 7, -9}, 0, {1008, -16}, {0, 233, 131, 255} }},
	{{ {9, 7, -2}, 0, {803, -16}, {119, 232, 218, 255} }},
	{{ {9, 5, -2}, 0, {803, 59}, {119, 23, 217, 255} }},
	{{ {6, 7, 9}, 0, {598, -16}, {73, 229, 101, 255} }},
	{{ {6, 5, 8}, 0, {598, 59}, {74, 20, 101, 255} }},
	{{ {-6, 7, 9}, 0, {394, -16}, {183, 229, 101, 255} }},
	{{ {-6, 5, 8}, 0, {394, 59}, {182, 20, 101, 255} }},
	{{ {-9, 7, -2}, 0, {189, -16}, {137, 232, 218, 255} }},
	{{ {-9, 5, -2}, 0, {189, 59}, {137, 23, 217, 255} }},
	{{ {0, 7, -9}, 0, {-16, -16}, {0, 233, 131, 255} }},
	{{ {0, 5, -8}, 0, {-16, 59}, {0, 24, 131, 255} }},
	{{ {-7, -7, -1}, 0, {189, 496}, {166, 171, 226, 255} }},
	{{ {0, -7, -6}, 0, {-16, 496}, {0, 172, 161, 255} }},
	{{ {7, -7, -1}, 0, {803, 496}, {90, 171, 226, 255} }},
	{{ {-4, -7, 6}, 0, {394, 496}, {201, 170, 76, 255} }},
	{{ {4, -7, 6}, 0, {598, 496}, {55, 170, 76, 255} }},
	{{ {0, -7, -6}, 0, {1008, 496}, {0, 172, 161, 255} }},
};

Gfx coffee_cup_full_coffeefullobject_mesh_layer_1_tri_0[] = {
	gsSPVertex(coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_0 + 0, 18, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(3, 2, 4, 0, 3, 4, 5, 0),
	gsSP2Triangles(5, 4, 6, 0, 5, 6, 7, 0),
	gsSP2Triangles(7, 6, 8, 0, 7, 8, 9, 0),
	gsSP2Triangles(9, 8, 10, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 9, 11, 0, 12, 11, 13, 0),
	gsSP2Triangles(12, 13, 14, 0, 14, 15, 12, 0),
	gsSP2Triangles(14, 16, 15, 0, 14, 5, 16, 0),
	gsSP2Triangles(14, 3, 5, 0, 17, 3, 14, 0),
	gsSP2Triangles(17, 0, 3, 0, 16, 5, 7, 0),
	gsSP2Triangles(16, 7, 15, 0, 15, 7, 9, 0),
	gsSP1Triangle(15, 9, 12, 0),
	gsSPEndDisplayList(),
};

Vtx coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_1[5] = {
	{{ {9, 5, -2}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {0, 5, -8}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {-9, 5, -2}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {6, 5, 8}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {-6, 5, 8}, 0, {-16, 1008}, {0, 127, 0, 255} }},
};

Gfx coffee_cup_full_coffeefullobject_mesh_layer_1_tri_1[] = {
	gsSPVertex(coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_1 + 0, 5, 0),
	gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
	gsSP1Triangle(2, 4, 3, 0),
	gsSPEndDisplayList(),
};


Gfx mat_coffee_cup_full_cup[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0, PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0),
	gsSPGeometryMode(G_CULL_BACK, 0),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 179, 82, 255),
	gsSPEndDisplayList(),
};

Gfx mat_revert_coffee_cup_full_cup[] = {
	gsDPPipeSync(),
	gsSPGeometryMode(0, G_CULL_BACK),
	gsSPEndDisplayList(),
};

Gfx mat_coffee_cup_full_coffee_liquid[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0, PRIMITIVE, 0, SHADE, 0, 0, 0, 0, 0),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 111, 78, 55, 255),
	gsSPEndDisplayList(),
};

Gfx coffee_cup_full_coffeefullobject_mesh_layer_1[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(coffee_cup_full_coffeefullobject_mesh_layer_1_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_coffee_cup_full_cup),
	gsSPDisplayList(coffee_cup_full_coffeefullobject_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_coffee_cup_full_cup),
	gsSPDisplayList(mat_coffee_cup_full_coffee_liquid),
	gsSPDisplayList(coffee_cup_full_coffeefullobject_mesh_layer_1_tri_1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

