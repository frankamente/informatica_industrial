ejemplos = salta entrada_num head_knocking prueba_laser mapas prueba_conexion pulsador\
          raton sonar velocidad velocidad1 mapa_cambiante

all: $(ejemplos)

clean: 
	rm $(ejemplos)

salta: salta.c api_grafi.o api_grafi.h
	gcc -o salta salta.c api_grafi.o -lrt -lpthread

entrada_num: entrada_num.c api_grafi.o api_grafi.h
	gcc -o entrada_num entrada_num.c api_grafi.o -lrt -lpthread

head_knocking: head_knocking.c api_grafi.o api_grafi.h
	gcc -o head_knocking head_knocking.c api_grafi.o -lrt -lpthread

prueba_laser: prueba_laser.c api_grafi.o api_grafi.h
	gcc -o prueba_laser prueba_laser.c api_grafi.o -lrt -lm -lpthread -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

mapas: mapas.c api_grafi.o api_grafi.h
	gcc -o mapas mapas.c api_grafi.o -lrt -lpthread

prueba_conexion: prueba_conexion.c api_grafi.o api_grafi.h
	gcc -o prueba_conexion prueba_conexion.c api_grafi.o -lrt -lpthread -Wno-pointer-to-int-cast

pulsador: pulsador.c api_grafi.o api_grafi.h
	gcc -o pulsador pulsador.c api_grafi.o -lrt -lpthread

raton: raton.c api_grafi.o api_grafi.h
	gcc -o raton raton.c api_grafi.o -lrt -lpthread

sonar: sonar.c api_grafi.o api_grafi.h
	gcc -o sonar sonar.c api_grafi.o -lrt -lpthread

mapa_cambiante: mapa_cambiante.c api_grafi.o api_grafi.h
	gcc -o mapa_cambiante mapa_cambiante.c api_grafi.o -lrt -lpthread

velocidad: velocidad.c api_grafi.o api_grafi.h
	gcc -o velocidad velocidad.c api_grafi.o -lrt -lpthread

velocidad1: velocidad1.c api_grafi.o api_grafi.h
	gcc -o velocidad1 velocidad1.c api_grafi.o -lrt -lm -lpthread

