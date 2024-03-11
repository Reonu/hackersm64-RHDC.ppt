#include "src/game/envfx_snow.h"

const GeoLayout checkpoint_flag_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_FORCE, geo_set_prim_color),
		GEO_DISPLAY_LIST(LAYER_FORCE, checkpoint_flag_checkpoint_flag_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
