#
# Makefile
# © datsi UPM 2003. Francisco Rosales <frosal@fi.upm.es>
#

CFLAGS = -Wall
LDFLAGS += -lpthread

EJEMPLOS = \
	PC_PLs_sem LE_PLs_sem P_PPs_sem C_PPs_sem PC_PLs_mc LE_PLs_mc L_PPs_cf E_PPs_cf \
	PC_PIPE PIPELINE mk_FIFO P_PPs_FIFO C_PPs_FIFO TCP_c TCP_s UDP_c UDP_s

all : $(EJEMPLOS)

#LE_PLs_sem :
#	${CC} LE_PLs_sem.c -o LE_PLs_sem -lpthread
#

test : test_input test_FIFO test_TCP

test_input :
	@echo "### Creando fichero de pruebas..."
	-echo "Un resultado correcto deberá mostrar" > test_input
	-echo "este texto con caracteres mayusculas." >> test_input

FIFOs : mk_FIFO P_PPs_FIFO C_PPs_FIFO test_FIFO

test_FIFO : test_input FIFOs
	@echo "### Creando una FIFO..."
	-./mk_FIFO FIFO
	@echo "### Arrancando un consumidor..."
	-./C_PPs_FIFO FIFO > test_output &
	@echo "### Arrancando un productor..."
	-./P_PPs_FIFO FIFO < test_input
	cat test_output

TCPs : TCP_c TCP_s

test_TCP : test_input TCPs
	@echo "### Arrancando servidor TCP..."
	-TIMEOUT=10 ./TCP_s &
	@echo "### Arrancando un cliente TCP..."
	-TIMEOUT=3 ./TCP_c localhost < test_input &
	@echo "### Arrancando otro cliente TCP..."
	-TIMEOUT=5  ./TCP_c localhost < test_input &
	@echo "### Arrancando otro más cliente TCP..."
	-TIMEOUT=15 ./TCP_c localhost < test_input &
	@echo "### Esperando a que terminen..."
	sleep 20

clean:
	rm -f test_input
	rm -f mk_FIFO P_PPs_FIFO C_PPs_FIFO FIFO
	rm -f TCP_c TCP_s
	rm -f UDP_c UDP_s
	-rm -f $(EJEMPLOS)

