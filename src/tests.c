#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <errno.h>

#include "bd.h"
#include "ini.h"
#include "config.h"

#define TEST_URL "http://www.rotten.com"
#define TEST_NAME "deadbeef"
#define TEST_SERIALIZED "BEGIN:BOOKMARK\nURL:http://www.rotten.com\nNAME:deadbeef\nEND:BOOKMARK\n"

int testCreateBookmark(bd_bookmark_t *bookmark) {
	char data[BD_SERIALIZED_MAXLENGTH];
	char *uuidstr = malloc(sizeof(char) * 128);
	bd_Init(0);

	// check and set up datadir
	bd_checkPath(g_bd_config.datadir);
	
	bd_NewBookmark(bookmark, "one/two/three/");
	bd_SetUrl(bookmark, TEST_URL);
	bd_SetName(bookmark, TEST_NAME);
	uuid_unparse_lower(bookmark->id, uuidstr);
	bd_serialize(bookmark, data);
	if (strcmp(TEST_SERIALIZED, data)) {
		fprintf(stderr, "bookmark serialization fail");
		return -1;
	}
	if (bd_Save(bookmark) < 0) {
		fprintf(stderr, "save fail: %s\n", strerror(errno));
		return -1;
	}
	bd_Destroy();
	return 0;
}

int checkDataDir(char *datadir) {
	char defaultdatadir[256];

	xdgInitHandle(g_bd_xdg);
	strcpy(defaultdatadir, xdgDataHome(g_bd_xdg));
	strcat(defaultdatadir, "/bookmarkd");
	fprintf(stderr, "xdg path %s ... %d\n", defaultdatadir, bd_checkPath(defaultdatadir));
	fprintf(stderr, "configpath %s ... %d\n", datadir, bd_checkPath(datadir));

	return 0;
}

int main(int argc, char** argv) {
	bd_bookmark_t bookmark;
	bd_Config("../test/config");
	if (testCreateBookmark(&bookmark) < 0) {
		return 1;
	}
	return 0;
}
