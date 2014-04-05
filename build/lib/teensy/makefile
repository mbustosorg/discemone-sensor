include ../../common/arm_common.txt

LIB_CPP_SOURCES=SPI.cpp EEPROM.cpp Adafruit_WS2801.cpp XBee.cpp Wire.cpp Adafruit_SSD1306.cpp Adafruit_GFX.cpp LPD8806.cpp TinyGPS.cpp
CORE_C_SOURCES=nonstd.c analog.c eeprom.c keylayouts.c mk20dx128.c pins_teensy.c \
serial1.c serial2.c serial3.c touch.c usb_desc.c usb_dev.c usb_keyboard.c \
usb_mem.c usb_serial.c yield.c
CORE_CPP_SOURCES=IPAddress.cpp HardwareSerial1.cpp HardwareSerial2.cpp HardwareSerial3.cpp \
Print.cpp Stream.cpp WMath.cpp WString.cpp
OBJECTS=$(CORE_C_SOURCES:.c=.c.o) $(CORE_CPP_SOURCES:.cpp=.cpp.o) $(LIB_CPP_SOURCES:.cpp=.cpp.o)
COBJECTS=$(addprefix $(OBJECT_DIR),$(OBJECTS))
VPATH = $(LIBRARIES)SPI $(LIBRARIES)EEPROM $(LIBRARIES)adafruitLED $(LIBRARIES)XBee $(LIBRARIES)Wire $(LIBRARIES)adafruitOLED $(LIBRARIES)adafruitGFX $(LIBRARIES)adafruitStrip $(LIBRARIES)TinyGPS

default: $(COBJECTS)
	$(AR) rcs $(LIBNAME) $^

$(OBJECT_DIR)%.c.o : $(CORES)%.c
	$(CC) $< $(CFLAGS) $(INCLUDE) -c -o $@

$(OBJECT_DIR)%.cpp.o : $(CORES)%.cpp 
	$(CXX) $< $(CXXFLAGS) $(INCLUDE) -c -o $@

$(OBJECT_DIR)%.cpp.o : %.cpp
	$(CXX) $< $(CXXFLAGS) $(INCLUDE) -c -o $@

clean:
	@echo -n Clear previous...
	rm -v *.d
	rm -v *.o
	rm -v *.a


