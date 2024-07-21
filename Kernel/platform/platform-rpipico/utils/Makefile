ROOT=../../../..
FUZIX_ROOT=$(ROOT)
USERCPU=armm0
VERBOSE=1
include $(FUZIX_ROOT)/Target/rules.$(USERCPU)

UTILS = picoctl.c picogpio.c
UTILSBIN = $(UTILS:.c=)

.PHONY: all clean

all: $(UTILSBIN) picoaudio

clean:
	rm -f *.o
	rm -f $(UTILSBIN)
	rm -f progbase.h
	rm -f ps.c
	rm -f ./audio/*.o
	rm -f ./audio/picoaudio

# ./audio/picoaudio: picoaudio.o buzzer.o
# 	$(LINKER) $(CRT0) $^ -o $@ $(LINKER_OPT) $(LINKER_TAIL)

picoaudio:
	$(CC) $(CFLAGS) $(CCOPTS) -c ./audio/picoaudio.c -o ./audio/picoaudio.o
	$(CC) $(CFLAGS) $(CCOPTS) -c ./audio/buzzer.c -o ./audio/buzzer.o
	$(LINKER) $(CRT0) audio/picoaudio.o audio/buzzer.o -o audio/picoaudio $(LINKER_OPT) $(LINKER_TAIL)

$(UTILSBIN): %: %.o
	$(LINKER) $(CRT0) $^ -o $@ $(LINKER_OPT) $(LINKER_TAIL)

%.o: %.c
	$(CC) $(CFLAGS) $(CCOPTS) -c $<
