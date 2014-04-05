
TRGTS = sensor
LIBS = teensyLib

default: all

all: $(LIBS) $(TRGTS)

hex: $(TRGTS)

lib: $(LIBS)

teensyLib:
	cd ./build/lib/teensy ; make

sensor:
	cd ./build/sensor ; make

clean:
	cd ./build/sensor ; make clean
	cd ./build/lib/teensy ; make clean

