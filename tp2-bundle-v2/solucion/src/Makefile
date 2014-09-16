ASM = nasm
DBG = gdb
CFLAGS64 = -ggdb -Wall -std=c99 -pedantic -m64
ASMFLAGS64 = -felf64 -g -F dwarf
CFLAGS   = $(CFLAGS64)
ASMFLAGS = $(ASMFLAGS64)


UBUNTU = $(shell lsb_release -sd)

OPENCV_LIB = `pkg-config --cflags --libs opencv`

BIN = tp2
BIN_DIR = ../bin



COBJS = tp2.o cli.o utils.o opencv_wrapper.o \
	cropflip_c.o cropflip.o \
	sierpinski_c.o sierpinski.o \
	mblur_c.o mblur.o \
	bandas_c.o bandas.o

ASMOBJS = cropflip_asm.o \
	sierpinski_asm.o \
	mblur_asm.o \
	bandas_asm.o

OBJS = $(COBJS) $(ASMOBJS)
# OBJS = $(COBJS) tmp.o

.PHONY: all clean

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(OPENCV_LIB) -lm
	cp $(BIN) $(BIN_DIR)/$(BIN)

opencv_wrapper.o: opencv_wrapper.c
	$(CC) $(CFLAGS) -c -o $@ $< $(OPENCV_LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -lm

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

all: $(BIN)

clean:
	rm -f $(OBJS)
	rm -f ./$(BIN)
	rm -f $(BIN_DIR)/$(BIN)


installopencv:
	sudo apt-get install libcv-dev libhighgui-dev libcvaux-dev libc6-dev
ifeq ($("${UBUNTU}") , $(filter , "${UBUNTU}", "Ubuntu 13.10" "Ubuntu 14.04"))
	sudo cp opencv1310.pc /usr/lib/pkgconfig/opencv.pc
endif



