# the compiler: gcc for C program, define as g++ for C++
CC = gcc
CXX = g++

BUILD_DIR = build
IMGUI_DIR = imgui

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
LINKING = -lglut -lGL -lGLU

IMGUI_SOURCES_DIRTY := $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_SOURCES_NO_PREFIX := $(subst $(IMGUI_DIR)/,,$(IMGUI_SOURCES_DIRTY))
IMGUI_SOURCES := $(IMGUI_SOURCES_NO_PREFIX:.cpp=)
IMGUI_OBJS := $(IMGUI_SOURCES:%=$(BUILD_DIR)/imgui/%.o)

all: imgui square spline

imgui: $(IMGUI_OBJS)

$(BUILD_DIR)/imgui/%.o: $(IMGUI_DIR)/%.cpp | build_imgui
	$(CXX) $(CFLAGS) -o $@ -c $< 

test:
	@echo IMGUI_OBJS: $(IMGUI_OBJS)

square: imgui quadrado_imgui.cpp
	$(CXX) $(CFLAGS) -o square quadrado_imgui.cpp build/imgui/*.o $(LINKING)

square-run: imgui square
	./square

spline: imgui spline_imgui.cpp
	$(CXX) $(CFLAGS) -o spline spline_imgui.cpp build/imgui/*.o $(LINKING)

spline-run: imgui spline
	./spline

build_imgui: build
	mkdir build/imgui -p

build:
	mkdir build -p

imgui: $(IMGUI_OBJS) build

clean:
	$(RM) $(BUILD_DIR) -rf
