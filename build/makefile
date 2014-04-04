PRODUCT_NAME=discemoneSensor
PROJECT_LOC=/Users/mauricio/Documents/development/discemoneSensor/src/
PROJECT_LIB_LOC=/Users/mauricio/Documents/development/furSwarmMember/
VPATH=$(PROJECT_LOC)
OBJECT_DIR=./

DFLAGS=-DTEENSY

APP_CPP_SOURCES=main.cpp discemoneSensor.cpp
APP_OBJECTS=$(addprefix $(OBJECT_DIR),$(APP_CPP_SOURCES:.cpp=.o))

include $(PROJECT_LIB_LOC)/build/common/arm_common_dev.txt

default: build 

build: discemoneSensor_copy $(OBJECT_DIR)$(PRODUCT_NAME).hex
	@echo
	@echo Listing:
	@echo
	ls -l $(PRODUCT_NAME).hex 
	@echo
	@echo Size:
	@echo
	$(BIN)/arm-none-eabi-size $(PRODUCT_NAME).hex

$(OBJECT_DIR)$(PRODUCT_NAME).hex: $(OBJECT_DIR)$(PRODUCT_NAME).elf
	$(OBJCPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $(OBJECT_DIR)$(PRODUCT_NAME).eep
	$(OBJCPY) -O ihex -R .eeprom $< $@

$(OBJECT_DIR)$(PRODUCT_NAME).elf: $(APP_OBJECTS)
	$(CC) -Os -Wl,--gc-sections $(MCU) -mthumb -T$(CORES)/mk20dx128.ld -o $@ $^ $(PROJECT_LIB_LOC)/build/lib/$(BOARD)/$(LIBNAME) -L. -larm_cortexM4l_math -lm

discemoneSensor_copy: $(PROJECT_LOC)/discemoneSensor.cpp

$(PROJECT_LOC)/discemoneSensor.cpp: discemoneSensor.ino
	@echo Copy ino...
	cp $(PROJECT_LOC)discemoneSensor.ino $(PROJECT_LOC)discemoneSensor.cpp

$(OBJECT_DIR)%.o : %.c
	$(CC) $< $(CFLAGS) $(DFLAGS) $(INCLUDE) -c -o $@

$(OBJECT_DIR)%.o : %.cpp
	$(CXX) $< $(CXXFLAGS) $(DFLAGS) $(INCLUDE) -c -o $@

clean:
    #Clear previous...
	rm -v $(APP_OBJECTS)
	rm -v *.d
	rm -v *.eep
	rm -v *.elf
	rm -v *.hex


