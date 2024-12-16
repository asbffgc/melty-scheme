CFLAGS=-Wall --std=c++17 -L/mingw64/bin -L/mingw64/lib
DLLFLAGS=-Wall --std=c++17
CC=g++
SRC=main.cpp src/*.cpp
PKG_LIB=`pkgconf --cflags --libs gtkmm-4.0`
OUT=out

# TODO: EXE only works inside mingw64, needs to be imported to visual studio
# TODO: to be able to be used within regular windows + then compiled, somehow
#
# TODO: update to also compile DLL
all: clean exe dll

exe: #*.o
	mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)/mltscm $(PKG_LIB) 

dll:
	mkdir -p $(OUT)
	$(CC) $(DLLFLAGS) dll_entry.cpp -shared -o $(OUT)/melty_scheme.dll -fPIC `pkg-config --cflags --libs guile-3.0`

# *.o: *.cpp *.c

clean:
	rm -rf $(OUT)
