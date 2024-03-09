#include "src/game/envfx_snow.h"

const GeoLayout bar_red_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, bar_red_bar_001_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
