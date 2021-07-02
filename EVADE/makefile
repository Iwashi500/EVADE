OBJS1	=	source/server/main_s.o source/server/system/system.o source/server/system/shot.o source/server/system/hit.o source/server/set_s/set_s.o source/server/system/support.o source/kadai/server_net.o source/kadai/server_command.o
OBJS2	=	source/client/main_c.o source/client/window/window.o source/client/set_c/set_c.o source/client/event/event.o source/kadai/client_net.o source/kadai/client_command.o 
OBJS3	=	main_s.o system.o shot.o hit.o set_s.o support.o server_net.o server_command.o
OBJS4	=	main_c.o window.o set_c.o event.o client_net.o client_command.o 
TARGET1	=	server
TARGET2	=	client
CFLAGS	=	-c -DNDEBUG -g


.c.o:
	gcc $(CFLAGS) $<

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	gcc -o $(TARGET1) $(OBJS3) -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -L/usr/local/lib -I/usr/local/include/SDL2 -Wl,-rpath,/usr/local/lib
$(TARGET2): $(OBJS2)
	gcc -o $(TARGET2) $(OBJS4) -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -L/usr/local/lib -I/usr/local/include/SDL2 -Wl,-rpath,/usr/local/lib

clean:
	rm *.o $(TARGET1) $(TARGET2)


set_s.o: source/server/set_s/set_s.c game.h
hit.o: source/server/system/hit.c game.h
shot.o: source/server/system/shot.c game.h
system.o: source/server/system/system.c game.h
support.o: source/server/system/support.c game.h
main_s.o: source/server/main_s game.h 

event.o: source/client/event/event.c game.h
set_c.o: source/client/set_c/set_c.c game.h
window.o: source/client/window/window.c game.h
main_c.o: source/client/main_c.c game.h

source/server_net.o: source/kadai/server_net.c source/kadai/server_common.h source/kadai/server_func.h source/kadai/common.h
source/server_command.o: source/kadai/server_command.c source/kadai/server_common.h source/kadai/server_func.h source/kadai/common.h
source/client_net.o: source/kadai/client_net.c source/kadai/client_func.h source/kadai/common.h
source/client_command.o: source/kadai/client_command.c source/kadai/client_func.h source/kadai/common.h
