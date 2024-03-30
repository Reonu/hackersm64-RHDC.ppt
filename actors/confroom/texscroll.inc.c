void scroll_gfx_mat_confroom_sea_layer1() {
	Gfx *mat = segmented_to_virtual(mat_confroom_sea_layer1);


	shift_s_down(mat, 10, PACK_TILESIZE(0, 1));
	shift_s(mat, 15, PACK_TILESIZE(0, 1));
	shift_t(mat, 15, PACK_TILESIZE(0, 1));

};

void scroll_actor_geo_confroom() {
	scroll_gfx_mat_confroom_sea_layer1();
};
