#include "src/game/envfx_snow.h"

const GeoLayout light_switch_down_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_FORCE, geo_set_confroom_lights),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, light_switch_down_confroomMesh_002_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
