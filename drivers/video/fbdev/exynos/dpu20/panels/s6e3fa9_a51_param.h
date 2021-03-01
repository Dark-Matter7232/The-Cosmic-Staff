#ifndef __S6E3FA9_PARAM_H__
#define __S6E3FA9_PARAM_H__

#include <linux/types.h>
#include <linux/kernel.h>

#define EXTEND_BRIGHTNESS	365
#define UI_MAX_BRIGHTNESS	255
#define UI_DEFAULT_BRIGHTNESS	128

#define NORMAL_TEMPERATURE	25	/* 25 degrees Celsius */

#define ACL_CMD_CNT				((u16)ARRAY_SIZE(SEQ_ACL_OFF))
#define HBM_CMD_CNT				((u16)ARRAY_SIZE(SEQ_HBM_OFF))
#define ELVSS_CMD_CNT				((u16)ARRAY_SIZE(SEQ_ELVSS_SET))

#define LDI_REG_BRIGHTNESS			0x51
#define LDI_REG_ID				0x04
#define LDI_REG_COORDINATE			0xA1
#define LDI_REG_DATE				LDI_REG_COORDINATE
#define LDI_REG_MANUFACTURE_INFO		LDI_REG_COORDINATE
#define LDI_REG_MANUFACTURE_INFO_CELL_ID	LDI_REG_COORDINATE
#define LDI_REG_CHIP_ID				0xD6

/* len is read length */
#define LDI_LEN_ID				3
#define LDI_LEN_COORDINATE			4
#define LDI_LEN_DATE				7
#define LDI_LEN_MANUFACTURE_INFO		4
#define LDI_LEN_MANUFACTURE_INFO_CELL_ID	16
#define LDI_LEN_CHIP_ID				5

/* offset is position including addr, not only para */
#define LDI_OFFSET_ACL		1			/* 55h 1st para */
#define LDI_OFFSET_HBM		1			/* 53h 1st para */
#define LDI_OFFSET_ELVSS_1	3			/* B5h 3rd para ELVSS interpolation */
#define LDI_OFFSET_ELVSS_2	1			/* B5h 1st para TSET */

#define LDI_GPARA_COORDINATE			0	/* A1h 1st Para: x, y */
#define LDI_GPARA_DATE				4	/* A1h 5th Para: [D7:D4]: Year */
#define LDI_GPARA_MANUFACTURE_INFO		11	/* A1h 12th Para ~ 15th Para */
#define LDI_GPARA_MANUFACTURE_INFO_CELL_ID	15	/* A1h 16th Para ~ 31th Para */

struct bit_info {
	unsigned int reg;
	unsigned int len;
	char **print;
	unsigned int expect;
	unsigned int offset;
	unsigned int g_para;
	unsigned int invert;
	unsigned int mask;
	unsigned int result;
};

enum {
	LDI_BIT_ENUM_05,	LDI_BIT_ENUM_RDNUMPE = LDI_BIT_ENUM_05,
	LDI_BIT_ENUM_0A,	LDI_BIT_ENUM_RDDPM = LDI_BIT_ENUM_0A,
	LDI_BIT_ENUM_0E,	LDI_BIT_ENUM_RDDSM = LDI_BIT_ENUM_0E,
	LDI_BIT_ENUM_0F,	LDI_BIT_ENUM_RDDSDR = LDI_BIT_ENUM_0F,
	LDI_BIT_ENUM_EE,	LDI_BIT_ENUM_ESDERR = LDI_BIT_ENUM_EE,
	LDI_BIT_ENUM_MAX
};

static char *LDI_BIT_DESC_05[BITS_PER_BYTE] = {
	[0 ... 6] = "number of corrupted packets",
	[7] = "overflow on number of corrupted packets",
};

static char *LDI_BIT_DESC_0A[BITS_PER_BYTE] = {
	[2] = "Display is Off",
	[7] = "Booster has a fault",
};

static char *LDI_BIT_DESC_0E[BITS_PER_BYTE] = {
	[0] = "Error on DSI",
};

static char *LDI_BIT_DESC_0F[BITS_PER_BYTE] = {
	[7] = "Register Loading Detection",
};

static char *LDI_BIT_DESC_EE[BITS_PER_BYTE] = {
	[2] = "VLIN3 error",
	[3] = "ELVDD error",
	[6] = "VLIN1 error",
};

static struct bit_info ldi_bit_info_list[LDI_BIT_ENUM_MAX] = {
	[LDI_BIT_ENUM_05] = {0x05, 1, LDI_BIT_DESC_05, 0x00, },
	[LDI_BIT_ENUM_0A] = {0x0A, 1, LDI_BIT_DESC_0A, 0x9C, .invert = (BIT(2) | BIT(7)), },
	[LDI_BIT_ENUM_0E] = {0x0E, 1, LDI_BIT_DESC_0E, 0x80, },
	[LDI_BIT_ENUM_0F] = {0x0F, 1, LDI_BIT_DESC_0F, 0xC0, .invert = (BIT(7)), },
	[LDI_BIT_ENUM_EE] = {0xEE, 1, LDI_BIT_DESC_EE, 0xC0, },
};

#if defined(CONFIG_DISPLAY_USE_INFO)
#define LDI_LEN_RDNUMPE		1		/* DPUI_KEY_PNDSIE: Read Number of the Errors on DSI */
#define LDI_PNDSIE_MASK		(GENMASK(7, 0))

/*
 * ESD_ERROR[0] = MIPI DSI error is occurred by ESD.
 * ESD_ERROR[1] = HS CLK lane error is occurred by ESD.
 * ESD_ERROR[2] = VLIN3 error is occurred by ESD.
 * ESD_ERROR[3] = ELVDD error is occurred by ESD.
 * ESD_ERROR[4] = CHECK_SUM error is occurred by ESD.
 * ESD_ERROR[5] = Internal HSYNC error is occurred by ESD.
 * ESD_ERROR[6] = VLIN1 error is occurred by ESD
 */
#define LDI_LEN_ESDERR		1		/* DPUI_KEY_PNELVDE, DPUI_KEY_PNVLI1E, DPUI_KEY_PNVLO3E, DPUI_KEY_PNESDE */
#define LDI_PNELVDE_MASK	(BIT(3))	/* ELVDD error */
#define LDI_PNVLI1E_MASK	(BIT(6))	/* VLIN1 error */
#define LDI_PNVLO3E_MASK	(BIT(2))	/* VLIN3 error */
#define LDI_PNESDE_MASK		(BIT(2) | BIT(3) | BIT(6))

#define LDI_LEN_RDDSDR		1		/* DPUI_KEY_PNSDRE: Read Display Self-Diagnostic Result */
#define LDI_PNSDRE_MASK		(BIT(7))	/* D7: REG_DET: Register Loading Detection */
#endif

struct lcd_seq_info {
	unsigned char	*cmd;
	unsigned int	len;
	unsigned int	sleep;
};

static unsigned char SEQ_SLEEP_OUT[] = {
	0x11
};

static unsigned char SEQ_SLEEP_IN[] = {
	0x10
};

static unsigned char SEQ_DISPLAY_ON[] = {
	0x29
};

static unsigned char SEQ_DISPLAY_OFF[] = {
	0x28
};

static unsigned char SEQ_TEST_KEY_ON_F0[] = {
	0xF0,
	0x5A, 0x5A
};

static unsigned char SEQ_TEST_KEY_OFF_F0[] = {
	0xF0,
	0xA5, 0xA5
};

static unsigned char SEQ_TE_ON[] = {
	0x35,
	0x00, 0x00
};

static unsigned char SEQ_PAGE_ADDR_SET_2A[] = {
	0x2A,
	0x00, 0x00, 0x04, 0x37
};

static unsigned char SEQ_PAGE_ADDR_SET_2B[] = {
	0x2B,
	0x00, 0x00, 0x09, 0x5F
};

static unsigned char SEQ_TSP_VSYNC_ON[] = {
	0xB9,
	0x00, 0x00, 0x14, 0x18, 0x00, 0x00, 0x00, 0x10
};

static unsigned char SEQ_ERR_FG_ENABLE[] = {
	0xE5,
	0x13
};

static unsigned char SEQ_ERR_FG_SET[] = {
	0xED,
	0x00, 0x4C, 0x40	/* 3rd 0x40 : default high */
};

static unsigned char SEQ_PCD_SET_DET_LOW[] = {
	0xCC,
	0x5C, 0x50	/* 1st 0x5C: default high, 2nd 0x50: Disable SW RESET */
};

static unsigned char SEQ_PARTIAL_UPDATE[] = {
	0xC2,
	0x1B, 0x41, 0xB0, 0x0E, 0x00, 0x1E, 0x5A, 0x1E,
	0x1E
};

static unsigned char SEQ_FFC_SET[] = {
	0xC5,
	0x0D, 0x10, 0x64, 0x1E, 0x15, 0x05, 0x00, 0x23,
	0x50
};

static unsigned char SEQ_ELVSS_SET[] = {
	0xB5,
	0x19,	/* 1st: TSET */
	0x8D,
	0x16,	/* 3rd: ELVSS return */
};

static unsigned char SEQ_HBM_ON[] = {
	0x53,
	0xE8,
};

static unsigned char SEQ_HBM_OFF[] = {
	0x53,
	0x28,
};

static unsigned char SEQ_HBM_ON_DIMMING_OFF[] = {
	0x53,
	0xE0,
};

static unsigned char SEQ_HBM_OFF_DIMMING_OFF[] = {
	0x53,
	0x20,
};

static unsigned char SEQ_ACL_OFF[] = {
	0x55,
	0x00
};

static unsigned char SEQ_ACL_08P[] = {
	0x55,
	0x01
};

static unsigned char SEQ_ACL_15P[] = {
	0x55,
	0x03
};

#if defined(CONFIG_EXYNOS_DOZE)
enum {
	ALPM_OFF,
	ALPM_ON_LOW,	/* ALPM 2 NIT */
	HLPM_ON_LOW,	/* HLPM 2 NIT */
	ALPM_ON_HIGH,	/* ALPM 60 NIT */
	HLPM_ON_HIGH,	/* HLPM 60 NIT */
	ALPM_MODE_MAX
};

enum {
	AOD_MODE_OFF,
	AOD_MODE_ALPM,
	AOD_MODE_HLPM,
	AOD_MODE_MAX
};

enum {
	AOD_HLPM_OFF,
	AOD_HLPM_02_NIT,
	AOD_HLPM_10_NIT,
	AOD_HLPM_30_NIT,
	AOD_HLPM_60_NIT,
	AOD_HLPM_STATE_MAX
};

static const char *AOD_HLPM_STATE_NAME[AOD_HLPM_STATE_MAX] = {
	"HLPM_OFF",
	"HLPM_02_NIT",
	"HLPM_10_NIT",
	"HLPM_30_NIT",
	"HLPM_60_NIT",
};

static unsigned int lpm_old_table[ALPM_MODE_MAX] = {
	AOD_HLPM_OFF,
	AOD_HLPM_02_NIT,
	AOD_HLPM_02_NIT,
	AOD_HLPM_60_NIT,
	AOD_HLPM_60_NIT,
};

static unsigned int lpm_brightness_table[EXTEND_BRIGHTNESS + 1] = {
	[0 ... 39]			= AOD_HLPM_02_NIT,
	[40 ... 70]			= AOD_HLPM_10_NIT,
	[71 ... 93]			= AOD_HLPM_30_NIT,
	[94 ... EXTEND_BRIGHTNESS]	= AOD_HLPM_60_NIT,
};

static unsigned char SEQ_HLPM_NO_PULSE_1[] = {
	0xB0,
	0x0D, 0xB5
};

static unsigned char SEQ_HLPM_NO_PULSE_2[] = {
	0xB5,
	0x80
};

static unsigned char SEQ_HLPM_AOR_60[] = {
	0xBB,
	0x09, 0x0C, 0x0A, 0x18
};

static unsigned char SEQ_HLPM_AOR_30[] = {
	0xBB,
	0x09, 0x0C, 0x5A, 0x84
};

static unsigned char SEQ_HLPM_AOR_10[] = {
	0xBB,
	0x09, 0x0C, 0x8A, 0xD8
};

static unsigned char SEQ_HLPM_ON_H[] = {
	0x53,
	0x22
};

static unsigned char SEQ_HLPM_ON_L[] = {
	0x53,
	0x23
};

static unsigned char SEQ_HLPM_OFF[] = {
	0x53,
	0x20
};

static unsigned char SEQ_NORMAL_NO_PULSE_1[] = {
	0xB0,
	0x0A, 0xB5
};

static unsigned char SEQ_NORMAL_NO_PULSE_2[] = {
	0xB5,
	0x00
};

static struct lcd_seq_info LCD_SEQ_HLPM_60_NIT[] = {
	{SEQ_TEST_KEY_ON_F0, ARRAY_SIZE(SEQ_TEST_KEY_ON_F0) },
	{SEQ_HLPM_NO_PULSE_1, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_1) },
	{SEQ_HLPM_NO_PULSE_2, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_2) },
	{SEQ_HLPM_AOR_60, ARRAY_SIZE(SEQ_HLPM_AOR_60) },
	{SEQ_HLPM_ON_H, ARRAY_SIZE(SEQ_HLPM_ON_H), 1},
	{SEQ_TEST_KEY_OFF_F0, ARRAY_SIZE(SEQ_TEST_KEY_OFF_F0) },
};

static struct lcd_seq_info LCD_SEQ_HLPM_30_NIT[] = {
	{SEQ_TEST_KEY_ON_F0, ARRAY_SIZE(SEQ_TEST_KEY_ON_F0) },
	{SEQ_HLPM_NO_PULSE_1, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_1) },
	{SEQ_HLPM_NO_PULSE_2, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_2) },
	{SEQ_HLPM_AOR_30, ARRAY_SIZE(SEQ_HLPM_AOR_30) },
	{SEQ_HLPM_ON_H, ARRAY_SIZE(SEQ_HLPM_ON_H), 1},
	{SEQ_TEST_KEY_OFF_F0, ARRAY_SIZE(SEQ_TEST_KEY_OFF_F0) },
};

static struct lcd_seq_info LCD_SEQ_HLPM_10_NIT[] = {
	{SEQ_TEST_KEY_ON_F0, ARRAY_SIZE(SEQ_TEST_KEY_ON_F0) },
	{SEQ_HLPM_NO_PULSE_1, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_1) },
	{SEQ_HLPM_NO_PULSE_2, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_2) },
	{SEQ_HLPM_AOR_10, ARRAY_SIZE(SEQ_HLPM_AOR_10) },
	{SEQ_HLPM_ON_H, ARRAY_SIZE(SEQ_HLPM_ON_H), 1},
	{SEQ_TEST_KEY_OFF_F0, ARRAY_SIZE(SEQ_TEST_KEY_OFF_F0) },
};

static struct lcd_seq_info LCD_SEQ_HLPM_02_NIT[] = {
	{SEQ_TEST_KEY_ON_F0, ARRAY_SIZE(SEQ_TEST_KEY_ON_F0) },
	{SEQ_HLPM_NO_PULSE_1, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_1) },
	{SEQ_HLPM_NO_PULSE_2, ARRAY_SIZE(SEQ_HLPM_NO_PULSE_2) },
	{SEQ_HLPM_ON_L, ARRAY_SIZE(SEQ_HLPM_ON_L), 1},
	{SEQ_TEST_KEY_OFF_F0, ARRAY_SIZE(SEQ_TEST_KEY_OFF_F0) },
};

static struct lcd_seq_info LCD_SEQ_HLPM_OFF[] = {
	{SEQ_TEST_KEY_ON_F0, ARRAY_SIZE(SEQ_TEST_KEY_ON_F0) },
	{SEQ_NORMAL_NO_PULSE_1, ARRAY_SIZE(SEQ_NORMAL_NO_PULSE_1) },
	{SEQ_NORMAL_NO_PULSE_2, ARRAY_SIZE(SEQ_NORMAL_NO_PULSE_2) },
	{SEQ_HLPM_OFF, ARRAY_SIZE(SEQ_HLPM_OFF), 1},
	{SEQ_TEST_KEY_OFF_F0, ARRAY_SIZE(SEQ_TEST_KEY_OFF_F0) },
};
#endif

static unsigned char SEQ_XTALK_B0[] = {
	0xB0,
	0x1C
};

static unsigned char SEQ_XTALK_ON[] = {
	0xD9,
	0x60
};

static unsigned char SEQ_XTALK_OFF[] = {
	0xD9,
	0xC0
};

#if defined(CONFIG_SEC_FACTORY)
static unsigned char SEQ_FD_ON[] = {
	0xB5,
	0x40	/* FD enable */
};

static unsigned char SEQ_FD_OFF[] = {
	0xB5,
	0x80	/* FD disable */
};

static unsigned char SEQ_GPARA_FD[] = {
	0xB0,
	0x0B, 0xB5
};
#endif

enum {
	ACL_STATUS_OFF,
	ACL_STATUS_08P,
	ACL_STATUS_15P,
	ACL_STATUS_MAX,
};

enum {
	TEMP_ABOVE_MINUS_00_DEGREE,	/* T > 0 */
	TEMP_ABOVE_MINUS_15_DEGREE,	/* -15 < T <= 0 */
	TEMP_BELOW_MINUS_15_DEGREE,	/* T <= -15 */
	TEMP_MAX
};

enum {
	HBM_STATUS_OFF,
	HBM_STATUS_ON,
	HBM_STATUS_MAX
};

enum {
	TRANS_DIMMING_OFF,
	TRANS_DIMMING_ON,
	TRANS_DIMMING_MAX
};

static unsigned char *HBM_TABLE[TRANS_DIMMING_MAX][HBM_STATUS_MAX] = {
	{SEQ_HBM_OFF_DIMMING_OFF, SEQ_HBM_ON_DIMMING_OFF},
	{SEQ_HBM_OFF, SEQ_HBM_ON}
};

static unsigned char *ACL_TABLE[ACL_STATUS_MAX] = {SEQ_ACL_OFF, SEQ_ACL_08P, SEQ_ACL_15P};

/* platform brightness <-> acl opr and percent */
static unsigned int brightness_opr_table[ACL_STATUS_MAX][EXTEND_BRIGHTNESS + 1] = {
	{
		[0 ... EXTEND_BRIGHTNESS]			= ACL_STATUS_OFF,
	}, {
		[0 ... UI_MAX_BRIGHTNESS]			= ACL_STATUS_15P,
		[UI_MAX_BRIGHTNESS + 1 ... EXTEND_BRIGHTNESS]	= ACL_STATUS_08P
	}
};

/* platform brightness <-> gamma level */
static unsigned int brightness_table[EXTEND_BRIGHTNESS + 1] = {
	3,
	6, 9, 12, 15, 18, 21, 24, 27, 30, 33,
	36, 39, 42, 45, 48, 52, 55, 59, 62, 66,
	69, 73, 77, 80, 84, 87, 91, 95, 98, 102,
	105, 109, 112, 116, 120, 123, 127, 130, 134, 138,
	141, 145, 148, 152, 156, 159, 163, 166, 170, 173,
	177, 181, 184, 188, 191, 195, 199, 202, 206, 209,
	213, 216, 220, 224, 227, 231, 234, 238, 242, 245,
	249, 252, 256, 259, 263, 267, 270, 274, 277, 281,
	285, 288, 292, 295, 299, 302, 306, 310, 313, 317,
	320, 324, 328, 331, 335, 338, 342, 345, 349, 353,
	356, 360, 363, 367, 371, 374, 378, 381, 385, 388,
	392, 396, 399, 403, 406, 410, 414, 417, 421, 424,
	428, 431, 435, 439, 442, 446, 449, 453, 457, 462, /* 128: 453 */
	466, 471, 475, 480, 484, 489, 493, 498, 502, 507,
	511, 516, 520, 525, 529, 534, 538, 543, 547, 552,
	556, 561, 565, 570, 574, 579, 583, 588, 592, 597,
	601, 606, 610, 615, 619, 624, 628, 633, 637, 641,
	646, 650, 655, 659, 664, 668, 673, 677, 682, 686,
	691, 695, 700, 704, 709, 713, 718, 722, 727, 731,
	736, 740, 745, 749, 754, 758, 763, 767, 772, 776,
	781, 785, 790, 794, 799, 803, 808, 812, 817, 821,
	826, 830, 834, 839, 843, 848, 852, 857, 861, 866,
	870, 875, 879, 884, 888, 893, 897, 902, 906, 911,
	915, 920, 924, 929, 933, 938, 942, 947, 951, 956,
	960, 965, 969, 974, 978, 983, 987, 992, 996, 1001,
	1005, 1010, 1014, 1018, 1023, 3, 7, 11, 15, 18, /* 255: 1023 */
	22, 26, 29, 33, 37, 40, 44, 48, 51, 55,
	59, 62, 66, 70, 73, 77, 81, 84, 88, 92,
	95, 99, 103, 107, 110, 114, 118, 121, 125, 129,
	132, 136, 140, 143, 147, 151, 154, 158, 162, 165,
	169, 173, 176, 180, 184, 187, 191, 195, 198, 202,
	206, 209, 213, 217, 220, 224, 228, 231, 235, 239,
	242, 246, 250, 253, 257, 261, 264, 268, 272, 275,
	279, 283, 286, 290, 294, 297, 301, 305, 309, 312,
	316, 320, 323, 327, 331, 334, 338, 342, 345, 349,
	353, 356, 360, 364, 367, 371, 375, 378, 382, 386,
	389, 393, 397, 400, 404,
};

static unsigned int elvss_table[EXTEND_BRIGHTNESS + 1] = {
	[0 ... 255] = 0x16,
	[256 ... 256] = 0x1F,
	[257 ... 268] = 0x1E,
	[269 ... 282] = 0x1D,
	[283 ... 295] = 0x1C,
	[296 ... 309] = 0x1B,
	[310 ... 323] = 0x19,
	[324 ... 336] = 0x18,
	[337 ... 350] = 0x17,
	[351 ... EXTEND_BRIGHTNESS] = 0x16,
};

static unsigned int nit_table[EXTEND_BRIGHTNESS + 1] = {
	2,
	3, 4, 6, 7, 8, 9, 10, 12, 13, 14,
	15, 16, 18, 19, 20, 22, 23, 25, 26, 27,
	29, 30, 32, 33, 35, 36, 37, 39, 40, 42,
	43, 45, 46, 47, 49, 50, 52, 53, 55, 56,
	58, 59, 60, 62, 64, 65, 66, 68, 69, 70,
	72, 74, 75, 76, 78, 79, 81, 82, 84, 85,
	86, 88, 89, 91, 92, 94, 95, 97, 98, 99,
	101, 102, 104, 105, 107, 108, 109, 111, 112, 114,
	115, 117, 118, 119, 121, 122, 124, 125, 127, 128,
	130, 131, 133, 134, 136, 137, 138, 140, 141, 143,
	144, 146, 147, 148, 150, 151, 153, 154, 156, 157,
	158, 160, 161, 163, 164, 166, 167, 169, 170, 171,
	173, 174, 176, 177, 179, 180, 181, 183, 185, 187,
	188, 190, 192, 194, 196, 198, 200, 202, 203, 205,
	207, 209, 211, 213, 215, 217, 218, 220, 222, 224,
	226, 228, 230, 232, 233, 235, 237, 239, 241, 243,
	245, 247, 248, 250, 252, 254, 256, 258, 260, 261,
	263, 265, 267, 269, 271, 272, 274, 276, 278, 280,
	282, 284, 286, 287, 289, 291, 293, 295, 297, 299,
	301, 302, 304, 306, 308, 310, 312, 314, 316, 317,
	319, 321, 323, 325, 327, 329, 331, 332, 334, 336,
	338, 340, 341, 343, 345, 347, 349, 351, 353, 355,
	356, 358, 360, 362, 364, 366, 368, 370, 371, 373,
	375, 377, 379, 381, 383, 385, 386, 388, 390, 392,
	394, 396, 398, 400, 401, 403, 405, 407, 409, 411,
	413, 415, 416, 418, 420, 422, 423, 425, 427, 428,
	430, 432, 433, 435, 436, 438, 440, 441, 443, 444,
	446, 448, 449, 451, 452, 454, 456, 457, 459, 461,
	462, 464, 466, 468, 469, 471, 473, 474, 476, 477,
	479, 481, 482, 484, 485, 487, 489, 490, 492, 493,
	495, 497, 498, 500, 502, 503, 505, 507, 508, 510,
	512, 513, 515, 517, 518, 520, 522, 523, 525, 526,
	528, 530, 531, 533, 534, 536, 538, 539, 541, 543,
	544, 546, 547, 549, 551, 552, 554, 556, 558, 559,
	561, 563, 564, 566, 567, 569, 571, 572, 574, 575,
	577, 579, 580, 582, 584, 585, 587, 588, 590, 592,
	593, 595, 597, 598, 600,
};
#endif /* __S6E3FA9_PARAM_H__ */
