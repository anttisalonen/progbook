default: all

.PHONY: all

BOOKPWD = $(shell readlink -f ${PWD}/../..)

all: $(BOOKPWD)/tmp/bus
	cd $(BOOKPWD)/tmp; PATH=$(PATH):$(BOOKPWD)/material/bus:$(BOOKPWD)/tmp $(PWD)/test.sh
	cd $(BOOKPWD)/tmp; PATH=$(PATH):$(BOOKPWD)/material/bus:$(BOOKPWD)/tmp $(PWD)/test2.sh

$(BOOKPWD)/tmp/bus: sdl_app.cpp main.cpp
	g++ -Wall -g3 -O2 $(shell sdl2-config --cflags --libs) -lSDL2_ttf -o $(BOOKPWD)/tmp/bus sdl_app.cpp main.cpp
