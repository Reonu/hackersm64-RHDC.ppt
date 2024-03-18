#include "src/game/envfx_snow.h"

const GeoLayout confroom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_OPAQUE, geo_render_projector_screen),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 133, 0, confroom_confroomMesh_mesh_layer_1),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -6, 234, 270, 0, 90, 0, confroom_projector_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 11, 38, 386, confroom_tables_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 1067, 194, 1121, confroom_office_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 542, 71, 404, confroom_officeDesk1_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 1295, 71, 404, confroom_officeDesk1_001_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 839, 100, 1040, confroom_officeDeskBig_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_officeDeskBig_mesh_layer_4),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
