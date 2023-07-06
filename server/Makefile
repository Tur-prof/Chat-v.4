SRC = BadLoginMessage.h BadLoginRegistr.h Demo_perms.h User.h User.cpp Message.h Message.cpp Chat.h Chat.cpp main.cpp

server : $(SRC)
	g++ -std=c++17 -o server $(SRC)

install:
	install ./server /usr/local/bin

uninstall:
	rm - rf /usr/local/bin/server
