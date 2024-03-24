static const s16 arm_anim_arm_raise_values[] = {
	0x0000, 0xFFF1, 0x0000, 0x0000, 0x0000, 0x0000, 0xFCBC, 0xFCB8, 0xFCBB, 
	0xFCE8, 0xFD62, 0xFE3D, 0xFF68, 0x00B3, 0x01DC, 0x02AD, 0x02FA, 0xFF98, 
	0xFFE8, 0x00C7, 0x0215, 0x03A9, 0x0548, 0x06B2, 0x07C0, 0x086A, 0x08C1, 
	0x08DB, 0xFD46, 0xFEA7, 0x0279, 0x084A, 0x0F9A, 0x17C5, 0x2004, 0x2786, 
	0x2D90, 0x318D, 0x32FE, 0x108B, 0x1001, 0x0E94, 0x0C8F, 0x0A3E, 0x07EC, 
	0x05E7, 0x047B, 0x03F1, 0x00E2, 0x00DD, 0x00CD, 0x00B6, 0x0098, 0x0079, 
	0x005C, 0x0047, 0x003F, 0x00FE, 0x00F4, 0x00DB, 0x00B9, 0x0092, 0x006E, 
	0x0050, 0x003B, 0x0034, 0xFFFF, 0xFF23, 0xFCEA, 0xF9DE, 0xF68B, 0xF37F, 
	0xF147, 0xF06B, 0x0000, 0xFFF1, 0xFFCA, 0xFF94, 0xFF56, 0xFF1B, 0xFEEF, 
	0xFEDE, 0xFFFF, 0xFFF5, 0xFFDB, 0xFFBB, 0xFF9D, 0xFF87, 0xFF79, 0xFF74, 
	0x131D, 0xFF09, 0x0131, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 
	0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
	0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	0x0000, 0xFFFF, 0x0000, 0xFFFF, 
};

static const u16 arm_anim_arm_raise_indices[] = {
	0x0001, 0x0000, 0x0001, 0x0001, 0x0001, 0x0002, 0x0001, 0x0003, 0x0001, 
	0x0004, 0x0001, 0x0005, 0x000B, 0x0006, 0x000B, 0x0011, 0x000B, 0x001C, 
	0x0009, 0x0027, 0x0009, 0x0030, 0x0009, 0x0039, 0x0008, 0x0042, 0x0008, 
	0x004A, 0x0008, 0x0052, 0x0001, 0x005A, 0x0001, 0x005B, 0x0001, 0x005C, 
	0x000A, 0x005D, 0x0008, 0x0067, 0x000A, 0x006F, 
};

static const struct Animation arm_anim_arm_raise = {
	1,
	0,
	0,
	0,
	10,
	ANIMINDEX_NUMPARTS(arm_anim_arm_raise_indices),
	arm_anim_arm_raise_values,
	arm_anim_arm_raise_indices,
	0,
};
