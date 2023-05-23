# Generated by github.com/da0x/mmpp
# Binary:
xg: .obj .obj/main.cpp.o .obj/xg.cpp.o
	g++ -std=c++20 -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer .obj/main.cpp.o .obj/xg.cpp.o -o xg

.obj:
	mkdir .obj

.obj/main.cpp.o: main.cpp xg.hpp
	g++ -std=c++20 -I/opt/homebrew/include -o .obj/main.cpp.o -c main.cpp

.obj/xg.cpp.o: xg.cpp xg.hpp
	g++ -std=c++20 -I/opt/homebrew/include -o .obj/xg.cpp.o -c xg.cpp

run: xg
	./xg

clean:
	rm -rfv .obj

install:
	sudo cp -v xg /usr/local/bin/

uninstall:
	sudo rm -v /usr/local/bin/xg
