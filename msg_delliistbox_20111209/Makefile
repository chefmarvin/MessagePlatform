DEMOSHOW = demoshow 
DEMOSHOWOBJS = demoshow.o

CC = armv4l-unknown-linux-gcc


CFLAGS = -I/linux2410/kernel/include -Wall -Wstrict-prototypes -Wno-trigraphs -Os -I/opt/host/armv4l/src/linux/include 
LIBS = -lminigui -lm -lpthread -lc

all:$(DEMOSHOW)
    
$(DEMOSHOW):$(DEMOSHOWOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(DEMOSHOWOBJS) $(LIBS) $(LDLIBS)
	cp -f $(DEMOSHOW) /home/nfs/$(DEMOSHOW)


clean:
	-rm -f $(DEMOSHOW) *.elf *.gdb *.o

    
    
