#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bookmarkd.h"
#include "dbifc.h"

bd_bookmark_t* bd_NewBookmark(bd_bookmark_t* bookmark) {
	bookmark->url = malloc(sizeof(char) * BD_URL_MAXLENGTH);
	if (bookmark->url == 0x0) {
		return NULL;
	}
	bookmark->tags_count = 0;
	bookmark->name = 0x0;
	bookmark->description =0x0;
	bookmark->storestate = BD_NEW;
	
	return bookmark;
}

int bd_SetUrl(bd_bookmark_t *bookmark, char *url) {
	bookmark->url = strcpy(bookmark->url, url);
	if (bookmark->url == 0x0) {
		return -1;	
	}
	return strlen(bookmark->url);
}

int bd_SetName(bd_bookmark_t *bookmark, char *name) {
	if (bookmark->name == 0x0) {
		bookmark->name = malloc(sizeof(char) * BD_NAME_MAXLENGTH);
		if (bookmark->name == 0x0) {
			return -1;
		}
	}
	strcpy(bookmark->name, name);
	if (bookmark->name == 0x0) {
		return -1;
	}
	return strlen(bookmark->name);
}

int bd_AddTag(bd_bookmark_t *bookmark, char *tag) {
	if (bookmark->tags == 0x0) {
		bd_tag_t **tags = 0x0;
		if (bd_initTags(tags) < 0) {
			return -1;
		}
		bookmark->tags = tags;
	}
	if (bookmark->tags_count == BD_TAGS_MAXCOUNT) {
		return -1;
	}
       	bd_cacheTag(*(bookmark->tags + bookmark->tags_count), tag);
	bookmark->tags_count++;
	return 0;
}

int bd_initTags(bd_tag_t **tags) {
	tags = malloc(sizeof(bd_tag_t*) * BD_TAGS_MAXCOUNT);
	if (tags == 0x0) {
		return -1;
	}
	return 0;
}

int bd_cacheTag(bd_tag_t *tag, char *content) {
	char rx[strlen(content) + 2];
	sprintf(rx, "^%s$", content);
	return bd_DbGetTags(&tag, rx, 1);
}
