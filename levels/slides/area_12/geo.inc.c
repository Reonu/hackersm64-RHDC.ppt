#include "src/game/envfx_snow.h"

const GeoLayout slides_area_12_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ROTATION_NODE_WITH_DL(LAYER_FORCE, 90, 0, 0, slides_dl_0slide10bg_002_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE_DECAL, 129, -125, 1, slides_dl_Bars_mesh_layer_2),
		GEO_DISPLAY_LIST(LAYER_FORCE, slides_dl_slide12titleMesh_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE_DECAL, 129, -125, 1, slides_dl_topBarVisual_mesh_layer_2),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout slides_area_12[] = {
	GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	GEO_OPEN_NODE(),
		GEO_ZBUFFER(1),
		GEO_OPEN_NODE(),
			GEO_CAMERA_FRUSTUM_WITH_FUNC(45.0000, 100, 30000, geo_camera_fov),
			GEO_OPEN_NODE(),
				GEO_CAMERA(CAMERA_MODE_8_DIRECTIONS, 100000, 0, -100000, 100000, -100, -100000, geo_camera_main),
				GEO_OPEN_NODE(),
					GEO_BRANCH(1, slides_area_12_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_MODE_NONE, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
