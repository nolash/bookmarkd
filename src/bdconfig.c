#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ini.h"
#include "bdconfig.h"

int bd_configHandler(void *user, const char *section, const char *name, const char *value) {
	printf("sec %s name %s\n", section, name);
	if (!strcmp(section, "global") && !strcmp(name, "datadir")) {
		g_bd_config.datadir = strdup(value);
	} else if (!strcmp(section, "global") && !strcmp(name, "editor")) {
		g_bd_config.editor = strdup(value);
	}
	return 1;
}

int bd_Config(const char *configdir) {
	g_bd_config.datadir = 0x0;
	g_bd_config.editor = 0x0;
	if (ini_parse("../tmp/config", bd_configHandler, &g_bd_config) < 0) {
		fprintf(stderr, "can't parse config file");
		return 1;
	}
	return 0;
}

void bd_FreeConfig() {
	if (g_bd_config.datadir != 0x0) {
		free(g_bd_config.datadir);
	}
	if (g_bd_config.editor != 0x0) {
		free(g_bd_config.editor);
	}	
}
