BUILDDIR=../build
all:
	$(CC) -Wall -g -o $(BUILDDIR)/bddb.o -c bddb.c
	$(CC) -Wall -g -Iext/include -Iinclude -o $(BUILDDIR)/tests.o -c tests.c 
	$(CC) -Wall -g -Iext/include -o $(BUILDDIR)/bdconfig.o -c bdconfig.c
	$(CC) -Wall -g -o $(BUILDDIR)/bd.o -c bd.c
	$(CC) -Wall -o $(BUILDDIR)/tests $(BUILDDIR)/tests.o $(BUILDDIR)/bd.o $(BUILDDIR)/bddb.o $(BUILDDIR)/bdconfig.o -luuid -lxdg-basedir $(BUILDDIR)/libinih.a
