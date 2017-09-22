#ifndef BD_DB_H_
#define BD_DB_H_

#include <stdlib.h>
#include "bd.h"

// pointer to db handle
void *g_bd_db;

int bd_DbConnect(const char *username, const char *password);
void bd_DbFree();

int bd_DbGetBookmark(bd_bookmark_t *bookmark);
int bd_DbSaveBookmark(bd_bookmark_t *bookmark);
int bd_DbGetTags(bd_tag_t **tags, char *rx, size_t count);
int bd_DbGetBookmarks(bd_bookmark_t **bookmarks, char *rx, size_t count);

int db_dbSetBookmark(void *bookmark, int argc, char **argv, char **cols);
#endif // BD_DB_H_
