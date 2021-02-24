USER= isatanner

#Targets
PutCGI= /var/www/html/class/csc3004/$(USER)/cgi-bin/bibleajax.cgi
PutHTML= /var/www/html/class/csc3004/$(USER)/bibleajax.html

#Compiler
CC= g++
CFLAGS= -g -std=c++11

all: $(PutCGI) $(PutHTML)

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

# Bible Ajax Object
bibleajax.o: bibleajax.cpp Ref.h Verse.h Bible.h
	$(CC) $(CFLAGS) -c bibleajax.cpp

# Bible Ajax CGI
bibleajax.cgi: bibleajax.o Ref.o Verse.o Bible.o
	$(CC) $(CFLAGS) -o bibleajax.cgi bibleajax.o Ref.o Verse.o Bible.o -lcgicc

# Deploy
$(PutCGI): bibleajax.cgi
	cp bibleajax.cgi $(PutCGI)
	chmod 755 $(PutCGI)

$(PutHTML): bibleajax.html
	cp bibleajax.html $(PutHTML)

# "make clean" will clean up by removing intermediate files
clean:
	rm -f *.o core bibleajax.cgi
