#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <sys/stat.h>
#include <unistd.h> // getuid, getgid

#include "bd.h"
#include "db.h"
#include "error.h"
#include "config.h"

void bd_Init(int tagcachesize) {
	g_bd_init = 0;
	g_bd_tag_cache_size = BD_TAGS_CACHESIZE;
	if (tagcachesize > 0) {
		 g_bd_tag_cache_size = tagcachesize;
	}
	g_bd_tag_cache_count = 0;
	g_bd_tag_cache = malloc(g_bd_tag_cache_size * sizeof(bd_tag_t));
	if (errno > 0) {
		fprintf(stderr, "malloc for tag cache fail: %s", strerror(errno));
		return;
	}
	if (bd_DbConnect(NULL, NULL) < 0) {
		fprintf(stderr, "failed to open db: %s", strerror(errno));
	}
	g_bd_init = 1;
}

/***
 * \todo check path string overflow
 */
int bd_checkPath(char *path) {
	int ok = 0;
	struct stat statbuf;
	if (strrchr(path, '/') != path + strlen(path) - 1) {
		strcat(path, "/");
	}
	if (stat(path, &statbuf) == - 1) {
		return BD_CFG_DATADIR_NONE;
	}
	if (!(statbuf.st_mode & S_IFDIR)) {
		return BD_CFG_DATADIR_NODIR;
	}
	if (statbuf.st_mode & S_IWOTH) {
		ok = 1;
	}
	if (statbuf.st_mode & S_IWGRP && getgid() == statbuf.st_gid) {
		ok = 1;
	}
	if (statbuf.st_mode & S_IWUSR && getuid() == statbuf.st_uid) {
		ok = 1;
	}
	if (!ok) {
		return BD_CFG_DATADIR_ACCESS;	
	}
	strcpy(g_bd_config.datadir, path);
	return 0;
}

bd_bookmark_t* bd_NewBookmark(bd_bookmark_t* bookmark, char *path) {
	bookmark->url = malloc(sizeof(char) * BD_URL_MAXLENGTH);
	if (bookmark->url == 0x0) {
		return NULL;
	}
	strcpy(bookmark->local_path, path);
	bookmark->tags_count = 0;
	bookmark->name = 0x0;
	bookmark->description =0x0;
	bookmark->storestate = BD_NEW;
	uuid_generate(bookmark->id);
	
	return bookmark;
}

/***
 * Saves the specified bookmark
 */
int bd_Save(bd_bookmark_t *bookmark) {
	FILE *f;
	size_t c;
	char resultpath[1024];
	char data[BD_SERIALIZED_MAXLENGTH];
	char *tmp;
	struct stat sbuf;

	strcpy(resultpath, g_bd_config.datadir);
	tmp = malloc(sizeof(bookmark->local_path));
	strcpy(tmp, bookmark->local_path);

	// create path recursively if it doesnt exist
	while ((tmp = strtok(tmp, "/")) != NULL) {
		strcat(resultpath, tmp);
		strcat(resultpath, "/");
		if (stat(resultpath, &sbuf) < 0) {
			if ((mkdir(resultpath, S_IRWXU)) < 0) {
				return -1;
			}
		}
		tmp = NULL;
	}
	free(tmp);

	// append new uuid
	uuid_unparse_lower(bookmark->id, resultpath + strlen(resultpath));

	// write the serialized bookmark
	bd_serialize(bookmark, data);

	f = fopen(resultpath, "w");
	if (f == NULL) {
		return -1;
	}

	c = fwrite(data, sizeof(char), strlen(data), f);
	fclose(f);

	// check if the write count matches the data
	if (c < strlen(data)) {
		remove(resultpath);
		return -1;
	}
	bd_DbSaveBookmark(bookmark);
	bookmark->storestate = BD_SAVED;
	
	return 0;
}

/***
 * format for file storage of bookmarks
 * based on vcf format
 *
 * TODO: check overflow
 */
char* bd_serialize(bd_bookmark_t *bookmark, char *data) {
	sprintf(data, "BEGIN:BOOKMARK\nURL:%s\nNAME:%s\nEND:BOOKMARK\n", bookmark->url, bookmark->name);
	return data;
}

/*** 
 * set the url field of a bookmark
 */
int bd_SetUrl(bd_bookmark_t *bookmark, char *url) {
	bookmark->url = strcpy(bookmark->url, url);
	if (bookmark->url == 0x0) {
		return -1;	
	}
	bookmark->storestate = BD_MODIFIED;
	return strlen(bookmark->url);
}

/***
 * set the name field of a bookmark
 */
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
	bookmark->storestate = BD_MODIFIED;
	return strlen(bookmark->name);
}

/***
 * add a tag to a bookmark
 */
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
	bookmark->storestate = BD_MODIFIED;
	return 0;
}

int bd_initTags(bd_tag_t **tags) {
	tags = malloc(sizeof(bd_tag_t*) * BD_TAGS_MAXCOUNT);
	if (tags == 0x0) {
		return -1;
	}
	return 0;
}

int bd_getTag(bd_tag_t *tag, char *content) {
	bd_tag_t **tags = NULL;
	char rx[strlen(content) + 2];
	sprintf(rx, "^%s$", content);
	if (bd_DbGetTags(tags, rx, 1) == 0) {
		
	}
	return 0;
}

int bd_cacheTag(bd_tag_t *tag, char *content) {
	return 0;
}

void bd_Destroy() {
	bd_DbFree();
}
