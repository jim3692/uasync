OUT_DIR?=build

prepare:
	mkdir -p $(OUT_DIR)

build: prepare uasync.cpp test.cpp
	clang++ -DUASYNC_LOGGER -g -o $(OUT_DIR)/test -x c++ uasync.cpp test.cpp

run: build
	gdb -q -ex "run" -ex="set confirm off" -ex "quit" $(OUT_DIR)/test
