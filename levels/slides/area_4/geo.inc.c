#include "src/game/envfx_snow.h"

const GeoLayout slides_area_4_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_FORCE, slides_dl_0BgSlide4_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, -69, -1158, 0, slides_dl_1mmmMesh_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, -1555, -326, 0, slides_dl_CuMMesh_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, 1376, -1326, 0, slides_dl_dogecollab_001Mesh_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, 236, 81, 0, slides_dl_magicallandsMesh_mesh_layer_0),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, 97, 1148, 0, slides_dl_slide4titleMesh_mesh_layer_0),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout slides_area_4[] = {
	GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	GEO_OPEN_NODE(),
		GEO_ZBUFFER(1),
		GEO_OPEN_NODE(),
			GEO_CAMERA_FRUSTUM_WITH_FUNC(45.0000, 100, 30000, geo_camera_fov),
			GEO_OPEN_NODE(),
				GEO_CAMERA(CAMERA_MODE_8_DIRECTIONS, 300000, 0, 0, 300000, -100, 0, geo_camera_main),
				GEO_OPEN_NODE(),
					GEO_BRANCH(1, slides_area_4_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_MODE_NONE, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
