const Collision donut4_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(80),
	COL_VERTEX(-145, 447, 136),
	COL_VERTEX(-276, 380, 136),
	COL_VERTEX(-154, 212, 136),
	COL_VERTEX(-81, 249, 136),
	COL_VERTEX(0, 470, 136),
	COL_VERTEX(0, 262, 136),
	COL_VERTEX(145, 447, 136),
	COL_VERTEX(81, 249, 136),
	COL_VERTEX(276, 380, 136),
	COL_VERTEX(154, 212, 136),
	COL_VERTEX(380, 276, 136),
	COL_VERTEX(212, 154, 136),
	COL_VERTEX(-145, -447, 136),
	COL_VERTEX(0, -470, 136),
	COL_VERTEX(0, -262, 136),
	COL_VERTEX(-81, -249, 136),
	COL_VERTEX(-276, -380, 136),
	COL_VERTEX(-154, -212, 136),
	COL_VERTEX(-380, -276, 136),
	COL_VERTEX(-212, -154, 136),
	COL_VERTEX(-447, -145, 136),
	COL_VERTEX(-249, -81, 136),
	COL_VERTEX(-470, 0, 136),
	COL_VERTEX(-262, 0, 136),
	COL_VERTEX(-447, 145, 136),
	COL_VERTEX(-249, 81, 136),
	COL_VERTEX(-380, 276, 136),
	COL_VERTEX(-212, 154, 136),
	COL_VERTEX(-249, -81, -272),
	COL_VERTEX(-262, 0, -272),
	COL_VERTEX(-212, -154, -272),
	COL_VERTEX(0, 470, -272),
	COL_VERTEX(-145, 447, -272),
	COL_VERTEX(-154, -212, -272),
	COL_VERTEX(-276, 380, -272),
	COL_VERTEX(-81, -249, -272),
	COL_VERTEX(-380, 276, -272),
	COL_VERTEX(0, -262, -272),
	COL_VERTEX(-447, 145, -272),
	COL_VERTEX(380, 276, -272),
	COL_VERTEX(276, 380, -272),
	COL_VERTEX(-470, 0, -272),
	COL_VERTEX(145, 447, -272),
	COL_VERTEX(-447, -145, -272),
	COL_VERTEX(-380, -276, -272),
	COL_VERTEX(-276, -380, -272),
	COL_VERTEX(-145, -447, -272),
	COL_VERTEX(0, -470, -272),
	COL_VERTEX(-249, 81, -272),
	COL_VERTEX(447, 145, 136),
	COL_VERTEX(249, 81, 136),
	COL_VERTEX(470, 0, 136),
	COL_VERTEX(262, 0, 136),
	COL_VERTEX(447, -145, 136),
	COL_VERTEX(249, -81, 136),
	COL_VERTEX(380, -276, 136),
	COL_VERTEX(212, -154, 136),
	COL_VERTEX(276, -380, 136),
	COL_VERTEX(154, -212, 136),
	COL_VERTEX(145, -447, 136),
	COL_VERTEX(81, -249, 136),
	COL_VERTEX(145, -447, -272),
	COL_VERTEX(276, -380, -272),
	COL_VERTEX(81, 249, -272),
	COL_VERTEX(154, 212, -272),
	COL_VERTEX(380, -276, -272),
	COL_VERTEX(0, 262, -272),
	COL_VERTEX(447, -145, -272),
	COL_VERTEX(470, 0, -272),
	COL_VERTEX(447, 145, -272),
	COL_VERTEX(81, -249, -272),
	COL_VERTEX(154, -212, -272),
	COL_VERTEX(212, -154, -272),
	COL_VERTEX(249, -81, -272),
	COL_VERTEX(-81, 249, -272),
	COL_VERTEX(262, 0, -272),
	COL_VERTEX(-154, 212, -272),
	COL_VERTEX(249, 81, -272),
	COL_VERTEX(-212, 154, -272),
	COL_VERTEX(212, 154, -272),
	COL_TRI_INIT(SURFACE_HARD_NOT_SLIPPERY, 64),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(4, 0, 3),
	COL_TRI(4, 3, 5),
	COL_TRI(6, 4, 5),
	COL_TRI(6, 5, 7),
	COL_TRI(8, 6, 7),
	COL_TRI(8, 7, 9),
	COL_TRI(10, 8, 9),
	COL_TRI(10, 9, 11),
	COL_TRI(12, 13, 14),
	COL_TRI(12, 14, 15),
	COL_TRI(16, 12, 15),
	COL_TRI(16, 15, 17),
	COL_TRI(18, 16, 17),
	COL_TRI(18, 17, 19),
	COL_TRI(20, 18, 19),
	COL_TRI(20, 19, 21),
	COL_TRI(22, 20, 21),
	COL_TRI(22, 21, 23),
	COL_TRI(24, 22, 23),
	COL_TRI(24, 23, 25),
	COL_TRI(26, 24, 25),
	COL_TRI(26, 25, 27),
	COL_TRI(1, 26, 27),
	COL_TRI(1, 27, 2),
	COL_TRI(23, 21, 28),
	COL_TRI(23, 28, 29),
	COL_TRI(21, 19, 30),
	COL_TRI(21, 30, 28),
	COL_TRI(0, 4, 31),
	COL_TRI(0, 31, 32),
	COL_TRI(19, 17, 33),
	COL_TRI(19, 33, 30),
	COL_TRI(1, 0, 32),
	COL_TRI(1, 32, 34),
	COL_TRI(17, 15, 35),
	COL_TRI(17, 35, 33),
	COL_TRI(26, 1, 34),
	COL_TRI(26, 34, 36),
	COL_TRI(15, 14, 37),
	COL_TRI(15, 37, 35),
	COL_TRI(24, 26, 36),
	COL_TRI(24, 36, 38),
	COL_TRI(8, 10, 39),
	COL_TRI(8, 39, 40),
	COL_TRI(22, 24, 38),
	COL_TRI(22, 38, 41),
	COL_TRI(6, 8, 40),
	COL_TRI(6, 40, 42),
	COL_TRI(20, 22, 41),
	COL_TRI(20, 41, 43),
	COL_TRI(4, 6, 42),
	COL_TRI(4, 42, 31),
	COL_TRI(18, 20, 43),
	COL_TRI(18, 43, 44),
	COL_TRI(16, 18, 44),
	COL_TRI(16, 44, 45),
	COL_TRI(12, 16, 45),
	COL_TRI(12, 45, 46),
	COL_TRI(13, 12, 46),
	COL_TRI(13, 46, 47),
	COL_TRI(25, 23, 29),
	COL_TRI(25, 29, 48),
	COL_TRI_INIT(SURFACE_RED_BAR, 56),
	COL_TRI(49, 10, 11),
	COL_TRI(49, 11, 50),
	COL_TRI(51, 49, 50),
	COL_TRI(51, 50, 52),
	COL_TRI(53, 51, 52),
	COL_TRI(53, 52, 54),
	COL_TRI(55, 53, 54),
	COL_TRI(55, 54, 56),
	COL_TRI(57, 55, 56),
	COL_TRI(57, 56, 58),
	COL_TRI(59, 57, 58),
	COL_TRI(59, 58, 60),
	COL_TRI(13, 59, 60),
	COL_TRI(13, 60, 14),
	COL_TRI(57, 59, 61),
	COL_TRI(57, 61, 62),
	COL_TRI(9, 7, 63),
	COL_TRI(9, 63, 64),
	COL_TRI(55, 57, 62),
	COL_TRI(55, 62, 65),
	COL_TRI(7, 5, 66),
	COL_TRI(7, 66, 63),
	COL_TRI(53, 55, 65),
	COL_TRI(53, 65, 67),
	COL_TRI(51, 53, 67),
	COL_TRI(51, 67, 68),
	COL_TRI(49, 51, 68),
	COL_TRI(49, 68, 69),
	COL_TRI(10, 49, 69),
	COL_TRI(10, 69, 39),
	COL_TRI(14, 60, 70),
	COL_TRI(14, 70, 37),
	COL_TRI(60, 58, 71),
	COL_TRI(60, 71, 70),
	COL_TRI(58, 56, 72),
	COL_TRI(58, 72, 71),
	COL_TRI(56, 54, 73),
	COL_TRI(56, 73, 72),
	COL_TRI(5, 3, 74),
	COL_TRI(5, 74, 66),
	COL_TRI(54, 52, 75),
	COL_TRI(54, 75, 73),
	COL_TRI(3, 2, 76),
	COL_TRI(3, 76, 74),
	COL_TRI(52, 50, 77),
	COL_TRI(52, 77, 75),
	COL_TRI(2, 27, 78),
	COL_TRI(2, 78, 76),
	COL_TRI(50, 11, 79),
	COL_TRI(50, 79, 77),
	COL_TRI(27, 25, 48),
	COL_TRI(27, 48, 78),
	COL_TRI(59, 13, 47),
	COL_TRI(59, 47, 61),
	COL_TRI(11, 9, 64),
	COL_TRI(11, 64, 79),
	COL_TRI_STOP(),
	COL_END()
};