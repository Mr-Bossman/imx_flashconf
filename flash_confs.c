// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 */

#include "flash-defs.h"
#include "flash_confs.h"
static const flash_configs configs[] = {
	{ .config = {
		.memConfig = {
			.tag              = FLEXSPI_CFG_BLK_TAG,
			.version          = FLEXSPI_CFG_BLK_VERSION,
			.readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackFromDqsPad,
			.csHoldTime       = 3u,
			.csSetupTime      = 3u,
			.sflashPadType    = kSerialFlash_4Pads,
			.serialClkFreq    = kFlexSpiSerialClk_100MHz,
			.sflashA1Size     = 8u * 1024u * 1024u,
			.lookupTable = {
				// Read LUTs
				FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18),
				FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06, READ_SDR, FLEXSPI_4PAD, 0x04),
			},
		},
		.pageSize           = 256u,
		.sectorSize         = 4u * 1024u,
		.blockSize          = 64u * 1024u,
		.isUniformBlockSize = false,
	}, .name = "imxrt1060" },
	{}
};

const flash_configs * get_flash_confs(){
	return configs;
}