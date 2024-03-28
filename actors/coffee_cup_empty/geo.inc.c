#include "src/game/envfx_snow.h"

const GeoLayout coffee_cup_empty_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, coffee_cup_empty_Cylinder_002_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
