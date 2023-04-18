PROG=mountd
OBJS=main.o lib/log.o lib/sys.o lib/autofs.o lib/mount.o lib/timer.o lib/signal.o lib/ucix.o lib/led.o lib/fs.o lib/ucix.o

LDFLAGS?=
LDFLAGS+=-ldl -luci

CFLAGS?=
CFLAGS+= -Wall

all: mountd 

mountd: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f lib/*.o *.o $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) -c  $^ -o $@
