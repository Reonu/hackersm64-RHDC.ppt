#include "src/game/envfx_snow.h"

const GeoLayout dudeguy_phone_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, -17, 683, -46, dudeguy_phone_chest_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_head_skinned_mesh_layer_1),
				GEO_ANIMATED_PART(LAYER_OPAQUE, -14, 244, -9, dudeguy_phone_head_mesh_layer_1),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_arm_1_skinned_mesh_layer_1),
				GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 116, 148, -9, 166, 90, 0),
				GEO_OPEN_NODE(),
					GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_left_arm_1_mesh_layer_1),
					GEO_OPEN_NODE(),
						GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_arm_2_skinned_mesh_layer_1),
						GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -6, 140, 14, -32, 81, 17),
						GEO_OPEN_NODE(),
							GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_left_arm_2_mesh_layer_1),
							GEO_OPEN_NODE(),
								GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_hand_skinned_mesh_layer_1),
								GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 126, 7, dudeguy_phone_left_hand_mesh_layer_1),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_leg_1_skinned_mesh_layer_1),
				GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 51, -184, 0, 175, 88, 4),
				GEO_OPEN_NODE(),
					GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_left_leg_1_mesh_layer_1),
					GEO_OPEN_NODE(),
						GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_calf_skinned_mesh_layer_1),
						GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 174, 0, 4, 0, -7),
						GEO_OPEN_NODE(),
							GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_left_calf_mesh_layer_1),
							GEO_OPEN_NODE(),
								GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_left_foot_skinned_mesh_layer_1),
								GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 330, 0, -93, 114, -26),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_left_foot_mesh_layer_1),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_arm_1_skinned_mesh_layer_1),
				GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -125, 148, -9, -167, 90, 0),
				GEO_OPEN_NODE(),
					GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_right_arm_1_mesh_layer_1),
					GEO_OPEN_NODE(),
						GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_arm_2_skinned_mesh_layer_1),
						GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -6, 142, -4, -32, 98, -15),
						GEO_OPEN_NODE(),
							GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_right_arm_2_mesh_layer_1),
							GEO_OPEN_NODE(),
								GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_hand_skinned_mesh_layer_1),
								GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 126, 7, dudeguy_phone_right_hand_mesh_layer_1),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_leg_1_skinned_mesh_layer_1),
				GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -55, -184, 0, -179, 86, 4),
				GEO_OPEN_NODE(),
					GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_right_leg_1_mesh_layer_1),
					GEO_OPEN_NODE(),
						GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_calf_skinned_mesh_layer_1),
						GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 172, 0, -1, 0, -7),
						GEO_OPEN_NODE(),
							GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_right_calf_mesh_layer_1),
							GEO_OPEN_NODE(),
								GEO_DISPLAY_LIST(LAYER_OPAQUE, dudeguy_phone_right_foot_skinned_mesh_layer_1),
								GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 330, 0, -93, 94, -8),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, dudeguy_phone_right_foot_mesh_layer_1),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
