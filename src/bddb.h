#ifndef BD_DB_H_
#define BD_DB_H_

#include <stdlib.h>
#include "bd.h"

int bd_DbGetTags(bd_tag_t **tags, char *rx, size_t count);
int bd_DbGetBookmarks(bd_bookmark_t **bookmarks, char *rx, size_t count);

#endif // BD_DB_H_
