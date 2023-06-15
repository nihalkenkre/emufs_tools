CC=gcc

BUILD=build

COPY_SRC=copy/src
COPY_BUILD=copy/build

DIR_SRC=dir/src
DIR_BUILD=dir/build

TEST_SRC=test/src
TEST_BUILD=test/build

.PHONY: all copy dir test

all: always copy dir test
	cp $(COPY_BUILD)/copy $(BUILD)/emufs_copy
	cp $(DIR_BUILD)/dir $(BUILD)/emufs_dir
	cp $(TEST_BUILD)/read $(BUILD)/test_read

copy: $(COPY_BUILD)/copy

$(COPY_BUILD)/copy: copy-always
	$(CC) -g $(COPY_SRC)/copy.c -o $(COPY_BUILD)/copy

dir: $(DIR_BUILD)/dir

$(DIR_BUILD)/dir: dir-always
	$(CC) -g $(DIR_SRC)/dir.c -o $(DIR_BUILD)/dir

test: $(TEST_BUILD)/read

$(TEST_BUILD)/read: test-always
	$(CC) -g $(TEST_SRC)/read.c -o $(TEST_BUILD)/read

always: copy-always test-always
	mkdir -p build

copy-always:
	mkdir -p copy/build

dir-always:
	mkdir -p dir/build

test-always:
	mkdir -p test/build

clean-all: clean clean-copy clean-dir clean-test

clean:
	rm -fr build/*

clean-copy:
	rm -fr $(COPY_BUILD)/*

clean-dir:
	rm -fr $(DIR_BUILD)/*

clean-test:
	rm -fr $(TEST_BUILD)/*
