CC=gcc

BUILD=build

COPY_SRC=copy/src
COPY_BUILD=copy/build

TEST_SRC=test/src
TEST_BUILD=test/build

.PHONY: all copy test

all: always copy test
	cp $(COPY_BUILD)/copy $(BUILD)/emufs_copy
	cp $(TEST_BUILD)/read $(BUILD)/test_read

copy: $(COPY_BUILD)/copy

$(COPY_BUILD)/copy: copy-always
	$(CC) -g $(COPY_SRC)/copy.c -o $(COPY_BUILD)/copy

test: $(TEST_BUILD)/read

$(TEST_BUILD)/read: test-always
	$(CC) -g $(TEST_SRC)/read.c -o $(TEST_BUILD)/read

always: copy-always test-always
	mkdir -p build

copy-always:
	mkdir -p copy/build

test-always:
	mkdir -p test/build

clean-all: clean clean-copy clean-test

clean:
	rm -fr build/*

clean-copy:
	rm -fr $(COPY_BUILD)/*

clean-test:
	rm -fr $(TEST_BUILD)/*
