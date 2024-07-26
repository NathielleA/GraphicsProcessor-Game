obj-m +=  kernel/gpp_data_bus.o

TARGET_DIR = $(PWD)/kernel
BUILD_DIR = $(PWD)/build
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

# All *.c files.
SOURCES =  GraphSync.c  NewSprites.c  GameVisualization.c main.c 

# All *.h files.
HEADERS =  utils/types.h  GraphSync.h  NewSprites.h GameVisualization.h

all: build kernel

build: build/dir build/main

build/dir:
	@mkdir -p build

build/main: build/GraphSync.o build/main.o build/NewSprites.o build/GameVisualization.o
	@gcc build/GraphSync.o build/main.o build/NewSprites.o  build/GameVisualization.o  -o build/main -lpthread

build/GraphSync.o:  GraphSync.c  GraphSync.h  NewSprites.c  NewSprites.h  utils/ui.h  utils/types.h
	@gcc -c  GraphSync.c -o build/GraphSync.o -Isrc -Iutils

build/NewSprites.o:  NewSprites.c NewSprites.h utils/ui.h  utils/types.h
	@gcc -c  NewSprites.c -o build/NewSprites.o -Isrc -Iutils

build/GameVisualization.o:  GameVisualization.c GameVisualization.h GraphSync.c  GraphSync.h utils/ui.h  utils/types.h
	@gcc -c  GameVisualization.c -o build/GameVisualization.o -Isrc -Iutils

build/main.o:  main.c  GraphSync.h NewSprites.h  GameVisualization.h  utils/ui.h  utils/types.h
	@gcc -c  main.c -o build/main.o -Isrc -Iutils

run: build 
	@sudo ./build/main

kernel:
	make -C $(KERNEL_DIR) M=$(TARGET_DIR) modules

clean: 
	make -C $(KERNEL_DIR) M=$(TARGET_DIR) clean
	@sudo rm -r build

load: 
	@sudo insmod $(TARGET_DIR)/gpp_data_bus.ko

unload:
	@sudo rmmod gpp_data_bus

IGNORE_FILES_FORMAT =
HEADERS_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(HEADERS))
SOURCES_FORMAT = $(filter-out $(IGNORE_FILES_FORMAT), $(SOURCES))

format:
	@clang-format -i -style=llvm $(HEADERS_FORMAT) $(SOURCES_FORMAT)

.PHONY: format unload load clean kernel build all
