#include "src/game/envfx_snow.h"

const GeoLayout spring_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0x01, geo_set_prim_color),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, spring_spring_model_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, spring_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
