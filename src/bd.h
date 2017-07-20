#ifndef BD_H_
#define BD_H_

#include <stdint.h>
#include <uuid/uuid.h>
#include <basedir.h>

#define BD_URL_MAXLENGTH 512
#define BD_NAME_MAXLENGTH 256
#define BD_DESCRIPTION_MAXLENGTH 4096
#define BD_TAGS_MAXCOUNT 16
#define BD_TAGS_CACHESIZE 1024
#define BD_SERIALIZED_MAXLENGTH 2048

/***
 * 
 * \short storage state of any object
 *
 */

enum bd_storestate {
	BD_NEW,
	BD_MODIFIED,
	BD_SAVED
};


/***
 *
 * \short identifiers for supported database drivers
 *
 */
enum bd_db_type {
	BD_SQLITE
};

/***
 *
 * \short which database driver is currently being used
 *
 */
enum bd_db_type g_bd_db_used;

/***
 *
 * \short taxonomy tag data type 
 *
 */
typedef struct { 
	uint32_t id;
	char* content;
} bd_tag_t;

/***
 *
 * \short bookmark data type
 *
 */
typedef struct {
	uuid_t id;
	char localpath[256];
	char *url;
	char *name;
	char *description;
	bd_tag_t **tags;
	size_t tags_count;
	enum bd_storestate storestate;
} bd_bookmark_t;

// global vars
int g_bd_init;
xdgHandle *g_bd_xdg;
char *g_bd_datapath;

bd_tag_t *g_bd_tag_cache;
int g_bd_tag_cache_size;
int g_bd_tag_cache_count;

// public 
void bd_Init(int tagcachesize);
bd_bookmark_t* bd_NewBookmark(bd_bookmark_t *bookmark, char *path);
int bd_SetUrl(bd_bookmark_t *bookmark, char *url);
int bd_SetName(bd_bookmark_t *bookmark, char *name);
int bd_AddTag(bd_bookmark_t *bookmark, char *tag);
int bd_Save(bd_bookmark_t *bookmark);

// private
int bd_checkPath(char *path);
int bd_initTags(bd_tag_t **tags);
int bd_getTag(bd_tag_t *tag, char *content);
int bd_cacheTag(bd_tag_t *tag, char *content);
char *bd_serialize(bd_bookmark_t *bookmark, char *data);

/***
 * \fn bd_bookmark_t* bd_NewBookmark(bd_bookmark_t* bookmark)
 * \short initialize and allocate memory for a new bookmark
 *
 * this does not by default allocate memory to anything else than the url
 * adding name, description or tags will auto-allocate memory calltime
 *
 */

#endif // BD_H_
