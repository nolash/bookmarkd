#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <uuid/uuid.h>

#include "config.h"
#include "db.h"

int bd_DbConnect(const char *username, const char *password) {
	char dbfile[1024];
	int new = 0;
	struct stat sbuf;

	strcpy(dbfile, g_bd_config.datadir);
	strcat(dbfile, "/idx");
	if (stat(dbfile, &sbuf) < 0) {
		new = 1;
	}
	if (sqlite3_open(dbfile, ((sqlite3**)&g_bd_db)) != SQLITE_OK) {
		return -1;
	}
	if (new) {
		int err;
		char *errstr;
		err = sqlite3_exec(g_bd_db,
			"CREATE TABLE tags ( \
			id INTEGER PRIMARY KEY ASC, \
			content VARCHAR(255) \
			); \
			CREATE TABLE bookmarks ( \
			id INTEGER PRIMARY KEY ASC, \
			uuid char(36), \
			local_path VARCHAR(1024), \
			bzz_hash varchar(64) \
			); \
			CREATE TABLE tag_bookmark ( \
			tag_id INT UNSIGNED NOT NULL, \
			bookmark_id INT UNSIGNED NOT NULL \
			);",
			NULL,
			NULL,
			&errstr);
		if (err != SQLITE_OK) {
			return -1;
		}
	}
				
	g_bd_db_type = BD_SQLITE;
	return 0;
}

void bd_DbFree() {
	sqlite3_close(g_bd_db);
}

int bd_DbGetBookmark(bd_bookmark_t *bookmark) {
	int err;
	sqlite3_stmt *st;
	char *t;
	char uuid[BD_UUID_LENGTH];

	t = "SELECT local_path, bzz_hash FROM bookmarks WHERE uuid = :UUID";
		
	err = sqlite3_prepare_v2((void*)g_bd_db,
		t,
		strlen(t),
		&st,
		NULL);
	if (err != SQLITE_OK) {
		return -1;
	}
	uuid_unparse_lower(bookmark->id, uuid);
	sqlite3_bind_text(st, 1, uuid, BD_UUID_LENGTH, NULL);
	if (err != SQLITE_OK) {
		return -1;
	}
	err = sqlite3_step(st);
	if (err != SQLITE_DONE) {
		return -1;
	}
	t = (char*)sqlite3_column_text(st, 0);
	if (t == NULL) {
		return 0;
	}
	strcpy(bookmark->local_path, t);
	sqlite3_finalize(st);

	return 1;
}

int bd_DbSaveBookmark(bd_bookmark_t *bookmark) {
	int err;
	sqlite3_stmt *st;
	char *sql;
	char uuid[BD_UUID_LENGTH];

	err = bd_DbGetBookmark(bookmark);
	if (err > 0) {
		return 0;
	} else if (err < 0) {
		return -1;
	}	

	sql = "INSERT INTO bookmarks (uuid, local_path) VALUES (:UUID, :LOCALPATH)";
		
	err = sqlite3_prepare_v2(g_bd_db,
		sql,
		BD_UUID_LENGTH + BD_PATH_MAXLENGTH + 64,
		&st,
		NULL);
	if (err != SQLITE_OK) {
		return -1;
	}
	uuid_unparse_lower(bookmark->id, uuid);
	sqlite3_bind_text(st, 1, uuid, BD_UUID_LENGTH, NULL);
	if (err != SQLITE_OK) {
		return -1;
	}
	sqlite3_bind_text(st, 2, bookmark->local_path, -1, NULL);
	if (err != SQLITE_OK) {
		return -1;
	}
	err = sqlite3_step(st);
	if (err != SQLITE_DONE) {
		return -1;
	}
	sqlite3_finalize(st);
	return 0;
}

int bd_DbGetTags(bd_tag_t **tags, char *rx, size_t count) {
	return 0;
}
