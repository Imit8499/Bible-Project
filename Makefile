USER= isatanner

#Targets
PutCGI= /var/www/html/class/csc3004/$(USER)/cgi-bin/bibleajax.cgi
PutHTML= /var/www/html/class/csc3004/$(USER)/bibleajax.html


#Compiler
CC= g++
CFLAGS= -g -std=c++11 -Wall -Og -Wreorder

all: $(PutCGI) $(PutHTML) biblereader biblelookupserver

# Lookup Server program
biblelookupserver: biblelookupserver.o fifo.o Ref.o Verse.o Bible.o
	$(CC) $(CFLAGS) -o $@ $^

# Bible Ajax CGI
bibleajax.cgi: bibleajax.o Ref.o Verse.o Bible.o fifo.o biblelookupclient.o
	$(CC) $(CFLAGS) -o $@ $^ -lcgicc

# Main
biblereader: main.o Bible.o Ref.o Verse.o fifo.o biblelookupclient.o
	$(CC) $(CFLAGS) -o $@ $^ 

biblelookupserver.o: BibleLookupServer.cpp fifo.h Ref.h Verse.h Bible.h
	$(CC) $(CFLAGS) -c -o $@ $<

biblelookupclient.o: BibleLookupClient.cpp BibleLookupClient.h Bible.h Verse.h Ref.h fifo.h
	$(CC) $(CFLAGS) -c -o $@ $<

fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Verse Object
Verse.o :  Verse.cpp Ref.h Verse.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Ref Object
Ref.o : Ref.cpp Ref.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Bible Object
Bible.o : Bible.cpp Ref.h Verse.h Bible.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.cpp Ref.h Verse.h Bible.h BibleLookupClient.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Bible Ajax Object
bibleajax.o: bibleajax.cpp Ref.h Verse.h Bible.h logfile.h BibleLookupClient.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Deploy
$(PutCGI): bibleajax.cgi
	rm -f $(PutCGI)
	cp bibleajax.cgi $(PutCGI)
	chmod 755 $(PutCGI)

$(PutHTML): bibleajax.html
	cp bibleajax.html $(PutHTML)

# "make clean" will clean up by removing intermediate files
clean:
	rm -f *.o core bibleajax.cgi biblereader biblelookupserver
