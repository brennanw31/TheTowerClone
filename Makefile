.PHONY: all clean

PROJECT_NAME       ?= game
RAYLIB_VERSION     ?= 4.2.0
RAYLIB_PATH        ?= C:\raylib\raylib

COMPILER_PATH      ?= C:/raylib/w64devkit/bin

PLATFORM           ?= PLATFORM_DESKTOP

DESTDIR ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

RAYLIB_LIBTYPE        ?= STATIC

BUILD_MODE            ?= DEBUG

USE_EXTERNAL_GLFW     ?= FALSE

USE_WAYLAND_DISPLAY   ?= FALSE

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
        export PATH := $(COMPILER_PATH):$(PATH)
    endif
endif

RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src
EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

CC = gcc

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),OSX)
        CC = clang
    endif
    ifeq ($(PLATFORM_OS),BSD)
        CC = clang
    endif
endif

MAKE = mingw32-make

CFLAGS += -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        CFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data -Wl,--subsystem,windows
    endif
endif

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        INCLUDE_PATHS += -I/usr/local/include
    endif
endif

LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    endif
    ifeq ($(USE_EXTERNAL_GLFW),TRUE)
        LDLIBS += -lglfw
    endif
endif

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS ?= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ifeq ($(PLATFORM),PLATFORM_ANDROID)
    MAKEFILE_PARAMS = -f Makefile.Android
    export PROJECT_NAME
    export SRC_DIR
else
    MAKEFILE_PARAMS = $(PROJECT_NAME)
endif

all:
	$(MAKE) $(MAKEFILE_PARAMS)

$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

clean:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		del *.o *.exe /s
    endif
    ifeq ($(PLATFORM_OS),LINUX)
	find -type f -executable | xargs file -i | grep -E 'x-object|x-archive|x-sharedlib|x-executable' | rev | cut -d ':' -f 2- | rev | xargs rm -fv
    endif
    ifeq ($(PLATFORM_OS),OSX)
		find . -type f -perm +ugo+x -delete
		rm -f *.o
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
	find . -type f -executable -delete
	rm -fv *.o
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	del *.o *.html *.js
endif
	@echo Cleaning done

