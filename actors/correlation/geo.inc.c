#include "src/game/envfx_snow.h"

const GeoLayout correlation_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE_DECAL, correlation_Cube_001_mesh_layer_2),
	GEO_CLOSE_NODE(),
	GEO_END(),
};