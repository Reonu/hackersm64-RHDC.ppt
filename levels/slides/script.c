#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"


/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels/slides/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_slides_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x7, _slides_segment_7SegmentRomStart, _slides_segment_7SegmentRomEnd), 
	LOAD_YAY0(0xa, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, slides_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		MARIO_POS(0x01, 0, -1278, -94, 0),
		OBJECT(MODEL_STAR, 1545, 605, 91, 0, 0, 0, (0x01 << 24), bhvStar),
		OBJECT(MODEL_NONE, -1278, -94, 0, 0, 0, 0, 0x000A0000, bhvSpinAirborneWarp),
		TERRAIN(slides_area_1_collision),
		MACRO_OBJECTS(slides_area_1_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(10, slides_area_10),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BAR, 346, -1374, 21, 0, 0, 0, (140 << 16) | (254 << 8), bhvMovingBarPink),
		OBJECT(MODEL_BAR, -236, -1374, 21, 0, 0, 0, (120 << 16) | (240 << 8), bhvMovingBarRed),
		OBJECT(MODEL_BAR, -854, -1374, 21, 0, 0, 0, (130 << 16) | (110 << 8), bhvMovingBarBlue),
		OBJECT(MODEL_STAR, 1298, 772, 33, 0, 0, 0, (0x0A << 24), bhvStar),
		OBJECT(MODEL_BAR, -1859, 268, 21, 0, 0, -90, (210 << 16) | (110 << 8) | (200), bhvMovingBar),
		TERRAIN(slides_area_10_collision),
		MACRO_OBJECTS(slides_area_10_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(11, slides_area_11),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_SLIDES, 0x0B, 0x02, WARP_NO_CHECKPOINT),
		WARP_NODE(0x02, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_NONE, 1588, 543, -4, 0, -180, 0, (0x01 << 16), bhvDoorWarp),
		OBJECT(MODEL_STAR, -2199, -103, 33, 0, 0, 0, (0x0B << 24), bhvStar),
		OBJECT(MODEL_NONE, 2108, -959, 0, 0, 0, 0, (0x02 << 16), bhvInstantActiveWarp),
		TERRAIN(slides_area_11_collision),
		MACRO_OBJECTS(slides_area_11_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(12, slides_area_12),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_SLIDES, 0x0B, 0x02, WARP_NO_CHECKPOINT),
		WARP_NODE(0x02, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_CHEESING, 0, 0, 0, 0, 0, 0, 0x00000000, bhvCheesing),
		OBJECT(MODEL_GOOMBA, 272, -1351, 52, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 1150, -751, 54, 0, 0, 0, (0x01), bhvGoomba),
		OBJECT(MODEL_SPRING, -1073, -151, 0, 0, 0, 0, (0x02 << 16), bhvSpring),
		OBJECT(MODEL_SPRING, -204, -1333, 0, 0, 0, 0, (0x01 << 16), bhvSpring),
		OBJECT(MODEL_STAR, -2012, 879, 0, 0, 0, 0, (0x0C << 24), bhvStar),
		OBJECT(MODEL_NONE, -1542, -1351, 0, 0, 0, 0, 0x00000000, bhvStartingPoint),
		TERRAIN(slides_area_12_collision),
		MACRO_OBJECTS(slides_area_12_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(13, slides_area_13),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_SLIDES, 0x0B, 0x02, WARP_NO_CHECKPOINT),
		WARP_NODE(0x02, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_STAR, 1890, 584, 0, 0, 0, 0, (0x0D << 24), bhvStar),
		OBJECT(MODEL_NONE, -2092, 878, 0, 0, 0, 0, 0x00000000, bhvStartingPoint),
		TERRAIN(slides_area_13_collision),
		MACRO_OBJECTS(slides_area_13_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(2, slides_area_2),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_STAR, -1642, -308, 74, 0, 0, 0, (0x02 << 24), bhvStar),
		TERRAIN(slides_area_2_collision),
		MACRO_OBJECTS(slides_area_2_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(3, slides_area_3),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BAR, -1289, -1283, 21, 0, 0, 0, (40 << 16) | (230 << 8), bhvMovingBar),
		OBJECT(MODEL_BAR, -939, -1283, 21, 0, 0, 0, (80 << 16) | (230 << 8), bhvMovingBar),
		OBJECT(MODEL_BAR, -589, -1283, 21, 0, 0, 0, (120 << 16) | (230 << 8), bhvMovingBar),
		OBJECT(MODEL_STAR, 983, 362, 74, 0, 0, 0, (0x03 << 24), bhvStar),
		TERRAIN(slides_area_3_collision),
		MACRO_OBJECTS(slides_area_3_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(4, slides_area_4),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_GOOMBA, -384, -741, 33, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_GOOMBA, 221, -741, 33, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_STAR, 561, 854, 74, 0, 0, 0, (0x04 << 24), bhvStar),
		TERRAIN(slides_area_4_collision),
		MACRO_OBJECTS(slides_area_4_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(5, slides_area_5),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_DONUT1, -1728, -439, 19, 0, 0, 0, 0x00000000, bhvDonut1),
		OBJECT(MODEL_DONUT2, -1, -439, 19, 0, 0, 0, 0x00000000, bhvDonut2),
		OBJECT(MODEL_DONUT3, 1752, -439, 19, 0, 0, 0, 0x00000000, bhvDonut3),
		OBJECT(MODEL_STAR, 1753, -441, 33, 0, 0, 0, (0x05 << 24), bhvStar),
		TERRAIN(slides_area_5_collision),
		MACRO_OBJECTS(slides_area_5_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(6, slides_area_6),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_CORRELATION, 99, -747, 54, 0, 0, 0, (10 << 16), bhvCorrelation),
		OBJECT(MODEL_STAR, -1798, 114, 33, 0, 0, 0, (0x06 << 24), bhvStar),
		TERRAIN(slides_area_6_collision),
		MACRO_OBJECTS(slides_area_6_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(7, slides_area_7),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BAR, -1796, -1529, 21, 0, 0, 0, (50 << 16) | (254 << 8), bhvMovingBar),
		OBJECT(MODEL_BAR, -1151, -1529, 21, 0, 0, 0, (80 << 16) | (230 << 8), bhvMovingBar),
		OBJECT(MODEL_BAR, -618, -1529, 21, 0, 0, 0, (110 << 16) | (240 << 8), bhvMovingBar),
		OBJECT(MODEL_STAR, 2206, 125, 33, 0, 0, 0, (0x07 << 24), bhvStar),
		TERRAIN(slides_area_7_collision),
		MACRO_OBJECTS(slides_area_7_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(8, slides_area_8),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_CHECKPOINT_FLAG, 1530, -432, 69, 0, 0, 0, 0x00000000, bhvCheckpointFlag),
		OBJECT(MODEL_BAR, -1433, -1387, 21, 0, 0, 0, (120 << 16) | (230 << 8) | (100), bhvMovingBarYellow),
		OBJECT(MODEL_BAR, -1133, 420, 21, 0, 0, 180, (120 << 16) | (230 << 8) | (30), bhvMovingBarRed),
		OBJECT(MODEL_BAR, -833, -1387, 21, 0, 0, 0, (120 << 16) | (230 << 8) | (80), bhvMovingBarYellow),
		OBJECT(MODEL_BAR, -233, -1387, 21, 0, 0, 0, (120 << 16) | (230 << 8) | (160), bhvMovingBarYellow),
		OBJECT(MODEL_BAR, 367, -1387, 21, 0, 0, 0, (120 << 16) | (230 << 8) | (180), bhvMovingBarYellow),
		OBJECT(MODEL_BAR, 967, -1387, 21, 0, 0, 0, (120 << 16) | (230 << 8) | (254), bhvMovingBarYellow),
		OBJECT(MODEL_BAR, -533, 420, 21, 0, 0, 180, (120 << 16) | (230 << 8) | (60), bhvMovingBarRed),
		OBJECT(MODEL_BAR, 67, 420, 21, 0, 0, 180, (120 << 16) | (230 << 8) | (76), bhvMovingBarRed),
		OBJECT(MODEL_BAR, 667, 420, 21, 0, 0, 180, (120 << 16) | (230 << 8) | (130), bhvMovingBarRed),
		OBJECT(MODEL_BAR, 1267, 420, 21, 0, 0, 180, (120 << 16) | (230 << 8) | (230), bhvMovingBarRed),
		OBJECT(MODEL_STAR, -1978, 318, 33, 0, 0, 0, (0x08 << 24), bhvStar),
		TERRAIN(slides_area_8_collision),
		MACRO_OBJECTS(slides_area_8_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(9, slides_area_9),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BAR, -1125, -1388, 21, 0, 0, 0, (120 << 16) | (254 << 8), bhvMovingBar),
		OBJECT(MODEL_BAR, -518, -1388, 21, 0, 0, 0, (130 << 16) | (254 << 8), bhvMovingBarRed),
		OBJECT(MODEL_BAR, 53, -1388, 21, 0, 0, 0, (150 << 16) | (240 << 8) | (100), bhvMovingBar),
		OBJECT(MODEL_STAR, 1820, 920, 33, 0, 0, 0, (0x09 << 24), bhvStar),
		TERRAIN(slides_area_9_collision),
		MACRO_OBJECTS(slides_area_9_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, -1278, -94, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
