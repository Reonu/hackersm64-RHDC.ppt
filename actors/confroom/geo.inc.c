#include "src/game/envfx_snow.h"

const GeoLayout confroom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_choose_room),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
		GEO_OPEN_NODE(),
			GEO_ASM(LAYER_OPAQUE, geo_render_projector_screen),
			GEO_ASM(LAYER_FORCE, geo_set_confroom_lights),
			GEO_DISPLAY_LIST(LAYER_FORCE, confroom_confroomMesh_mesh_layer_0),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_confroomMesh_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_ASM(1, geo_choose_room),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
		GEO_OPEN_NODE(),
			GEO_ASM(LAYER_FORCE, geo_set_confroom_lights),
			GEO_DISPLAY_LIST(LAYER_FORCE, confroom_office_mesh_layer_0),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_office_mesh_layer_1),
			GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_office_mesh_layer_4),
			GEO_DISPLAY_LIST(LAYER_TRANSPARENT, confroom_office_mesh_layer_5),
		GEO_CLOSE_NODE(),
		GEO_ASM(2, geo_choose_room),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 0, 0),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_FORCE, confroom_partialConfroom_mesh_layer_0),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_partialConfroom_mesh_layer_1),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
