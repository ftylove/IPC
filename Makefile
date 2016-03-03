all:client server open
client:client.o bank.o
	cc -o client client.o bank.o
server:server.o bank.o
	cc -o server server.o bank.o
open:open.o bank.o dao.o
	cc -o open open.o bank.o dao.o
client.o:client.c
	cc -c client.c
server.o:server.c
	cc -c server.c
open.o:open.c
	cc -c open.c
bank.o:bank.c
	cc -c bank.c
dao.o:dao.c
	cc -c dao.c
clean:
	rm -r client server open *.o *.dat
