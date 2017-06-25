BUILDDIR=../build
all:
	$(CC) -Wall -g -Iinclude -o $(BUILDDIR)/dbifc.o -c dbifc.c
	$(CC) -Wall -g -Iinclude -o $(BUILDDIR)/tests.o -c tests.c
	$(CC) -Wall -g -Iinclude -o $(BUILDDIR)/bookmarkd.o -c bookmarkd.c
	$(CC) -Wall -o $(BUILDDIR)/tests $(BUILDDIR)/tests.o $(BUILDDIR)/bookmarkd.o $(BUILDDIR)/dbifc.o
