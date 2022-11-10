CC=clang++
LD=$(CC)

SDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk
CFLAGS=-I $(SDK)

SRC=macho_reader

all: $(SRC)

$(SRC): $(SRC).o
		$(LD) $(LDFLAGS) -o $@ $^
		
%.o: %.cpp
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
		
clean:
	rm $(SRC)
	rm $(SRC).o


