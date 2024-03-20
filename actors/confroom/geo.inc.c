#include "src/game/envfx_snow.h"

const GeoLayout confroom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_FORCE, confroom_000exterior_mesh_layer_0),
		GEO_ASM(LAYER_OPAQUE, geo_render_projector_screen),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_confroomMesh_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_projector_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_tables_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_office_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_office_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_officeDesk1_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_officeDesk1_001_mesh_layer_1),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 0, 1325, confroom_officeDesk1_002_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_officeDeskBig_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_officeDeskBig_mesh_layer_4),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, confroom_receptiondesk_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_ALPHA, confroom_receptiondesk_mesh_layer_4),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 0, 85, confroom_shelf_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
