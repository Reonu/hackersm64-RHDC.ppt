#include "src/game/envfx_snow.h"

const GeoLayout bar_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_FORCE, geo_set_prim_color),
		GEO_DISPLAY_LIST(LAYER_FORCE, bar_bar_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
