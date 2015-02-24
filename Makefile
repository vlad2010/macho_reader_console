CC=gcc
LD=$(CC)

SDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk


LDFLAGS=-arch i386
CFLAGS=-I $(SDK) -arch i386
#LDFLAGS=
#CFLAGS=-I $(SDK)

SRC=macho_reader

all: $(SRC)


$(SRC): $(SRC).o
		$(LD) $(LDFLAGS) -o $@ $^
		
#%.o: %.m
#		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
		
%.o: %.c
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
		
#%.o: %.cpp
#		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm $(SRC)
	rm $(SRC).o


