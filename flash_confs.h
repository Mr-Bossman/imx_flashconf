#ifndef __FLASH_CONFIGS__
#define __FLASH_CONFIGS__
	typedef struct _flash_configs {
		flexspi_nor_config_t config;
		char name[32];
	} flash_configs;
	const flash_configs * get_flash_confs();
#endif /* __FLASH_CONFIGS__*/