#include <stdio.h>

#include "bookmarkd.h"

int main(int argc, char** argv) {
	bd_bookmark_t bookmark;
	bd_NewBookmark(&bookmark);
	bd_SetUrl(&bookmark, "http.//www.rotten.com");
	bd_SetName(&bookmark, "deadbeef");
	printf("url: %s\nname: %s\n", bookmark.url, bookmark.name);
	return 0;
}
