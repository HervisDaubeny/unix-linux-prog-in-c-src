PROGS=	addresses \
	getaddrinfo \
	getbyname \
	gethostbyaddr \
	gethostbyname \
	getnameinfo \
	hname-resolver \
	passive

all: $(PROGS)

addresses: addresses.o
	$(CC) -o $@ $^

getaddrinfo: getaddrinfo.o
	$(CC) -o $@ $^

getbyname: getbyname.o
	$(CC) -o $@ $^

gethostbyaddr: gethostbyaddr.o
	$(CC) -o $@ $^

gethostbyname: gethostbyname.o
	$(CC) -o $@ $^

getnameinfo: getnameinfo.o
	$(CC) -o $@ $^

hname-resolver: hname-resolver.o
	$(CC) -o $@ $^

passive: passive.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
