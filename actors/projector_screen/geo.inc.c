#include "src/game/envfx_snow.h"

const GeoLayout projector_screen_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, projector_screen_projectorscreenMesh_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
