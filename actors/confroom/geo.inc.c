#include "src/game/envfx_snow.h"

const GeoLayout confroom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -6, 234, 270, 0, 90, 0, confroom_projector_mesh_layer_1),
		GEO_TRANSLATE_NODE(LAYER_OPAQUE, 0, 147, 911),
		GEO_OPEN_NODE(),
			GEO_ASM(LAYER_OPAQUE, geo_render_projector_screen),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 133, 0, confroom_room_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 11, 38, 386, confroom_tables_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
