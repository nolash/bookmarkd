BUILDDIR=../build
all:
	$(CC) -Wall -g -o $(BUILDDIR)/db_sqlite.o -c db_sqlite.c
	$(CC) -Wall -g -Iext/include -Iinclude -o $(BUILDDIR)/tests.o -c tests.c 
	$(CC) -Wall -g -Iext/include -o $(BUILDDIR)/config.o -c config.c
	$(CC) -Wall -g -o $(BUILDDIR)/bd.o -c bd.c
	$(CC) -Wall -o $(BUILDDIR)/tests $(BUILDDIR)/tests.o $(BUILDDIR)/bd.o $(BUILDDIR)/db.o $(BUILDDIR)/db_sqlite.o $(BUILDDIR)/config.o -luuid -lxdg-basedir -lsqlite3 $(BUILDDIR)/libinih.a
