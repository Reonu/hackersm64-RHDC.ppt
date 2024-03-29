static const s16 dudeguy_anim_talking_generic_values[] = {
	0x0000, 0xFFF1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xFF6E, 0xFE14, 0xFC77, 0xFB1C, 0xFA8B, 0xFB1C, 0xFC77, 0xFE14, 
	0xFF6E, 0x0000, 0xEE34, 0xEE3F, 0xEE51, 0xEE5C, 0xEE5B, 0xEE58, 0xEE5B, 
	0xEE5C, 0xEE51, 0xEE3F, 0xEE34, 0x0000, 0xFF82, 0xFE58, 0xFCF4, 0xFBCA, 
	0xFB4D, 0xFBCA, 0xFCF4, 0xFE58, 0xFF82, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFC13, 0xFCB5, 0x0953, 
	0x07BC, 0x0161, 0xF1AB, 0xFB54, 0x0522, 0x032C, 0xFFFF, 0xFFFF, 0x0000, 
	0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x02B7, 0xFE3A, 0x0AA8, 
	0xFCFC, 0xFF65, 0xEFF4, 0xFA97, 0xFFD5, 0x011C, 
};

static const u16 dudeguy_anim_talking_generic_indices[] = {
	0x0001, 0x0000, 0x0001, 0x0001, 0x0001, 0x0002, 0x0001, 0x0003, 0x0001, 
	0x0004, 0x0001, 0x0005, 0x0001, 0x0006, 0x0001, 0x0007, 0x0001, 0x0008, 
	0x000B, 0x0009, 0x000B, 0x0014, 0x000B, 0x001F, 0x0001, 0x002A, 0x0001, 
	0x002B, 0x0001, 0x002C, 0x0001, 0x002D, 0x0001, 0x002E, 0x0001, 0x002F, 
	0x0001, 0x0030, 0x0001, 0x0031, 0x0001, 0x0032, 0x0001, 0x0033, 0x0001, 
	0x0034, 0x0001, 0x0035, 0x0001, 0x0036, 0x0001, 0x0037, 0x0001, 0x0038, 
	0x0001, 0x0039, 0x0001, 0x003A, 0x0001, 0x003B, 0x0001, 0x003C, 0x0001, 
	0x003D, 0x0001, 0x003E, 0x0001, 0x003F, 0x0001, 0x0040, 0x0001, 0x0041, 
	0x0001, 0x0042, 0x0001, 0x0043, 0x0001, 0x0044, 0x0001, 0x0045, 0x0001, 
	0x0046, 0x0001, 0x0047, 0x0001, 0x0048, 0x0001, 0x0049, 0x0001, 0x004A, 
	0x0001, 0x004B, 0x0001, 0x004C, 0x0001, 0x004D, 
};

static const struct Animation dudeguy_anim_talking_generic = {
	0,
	0,
	0,
	0,
	10,
	ANIMINDEX_NUMPARTS(dudeguy_anim_talking_generic_indices),
	dudeguy_anim_talking_generic_values,
	dudeguy_anim_talking_generic_indices,
	0,
};

