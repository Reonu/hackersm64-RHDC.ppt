#include "src/game/envfx_snow.h"

const GeoLayout confroom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_FORCE, confroom_000exterior_mesh_layer_0),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_000exterior_mesh_layer_1),
		GEO_ASM(LAYER_OPAQUE, geo_render_projector_screen),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 8, 194, 339, confroom_confroomMesh_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_projector_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 11, 38, 386, confroom_tables_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 696, 194, 1582, confroom_office_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_office_mesh_layer_4),
			GEO_DISPLAY_LIST(LAYER_TRANSPARENT, confroom_office_mesh_layer_5),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
