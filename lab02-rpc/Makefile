# SYSR - Lab 02 RPC
# Build with: make
# Requires:   libnsl (rpcgen / ONC-RPC runtime)
#
# Regenerate stubs from IDL:
#   rpcgen -a calcul.x
#   (overwrites calcul.h, calcul_xdr.c, calcul_clnt.c, calcul_svc.c)

CC      = gcc
CFLAGS  = -Wall -g
LDLIBS  = -lnsl

GENERATED = calcul.h calcul_xdr.c calcul_clnt.c calcul_svc.c

all: server client

# ---- generated stubs (can be rebuilt with rpcgen) ----
calcul_xdr.o: calcul_xdr.c calcul.h
	$(CC) $(CFLAGS) -c calcul_xdr.c

calcul_clnt.o: calcul_clnt.c calcul.h
	$(CC) $(CFLAGS) -c calcul_clnt.c

calcul_svc.o: calcul_svc.c calcul.h
	$(CC) $(CFLAGS) -c calcul_svc.c

# ---- user code ----
calcul_server.o: calcul_server.c calcul.h
	$(CC) $(CFLAGS) -c calcul_server.c

calcul_client.o: calcul_client.c calcul.h
	$(CC) $(CFLAGS) -c calcul_client.c

# ---- link ----
server: calcul_svc.o calcul_server.o calcul_xdr.o
	$(CC) $(CFLAGS) -o server $^ $(LDLIBS)

client: calcul_clnt.o calcul_client.o calcul_xdr.o
	$(CC) $(CFLAGS) -o client $^ $(LDLIBS)

regen:
	rpcgen -a calcul.x

clean:
	rm -f *.o server client

.PHONY: all regen clean
