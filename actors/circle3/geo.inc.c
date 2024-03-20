#include "src/game/envfx_snow.h"

const GeoLayout circle3_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE_DECAL, circle3_circle3_mesh_layer_2),
		GEO_DISPLAY_LIST(LAYER_ALPHA, circle3_circle3_mesh_layer_4),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
