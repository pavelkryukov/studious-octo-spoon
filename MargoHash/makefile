CC:=gcc
CFLAGS:= -c -O3 -Wall -std=c99 -pedantic -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

SRC_DIR:=source
BIN_DIR:=bin
OBJ_DIR:=obj

TEST_0:=$(OBJ_DIR)/test0.o
TEST_1:=$(OBJ_DIR)/test1.o
TEST_2:=$(OBJ_DIR)/test2.o
TEST_3:=$(OBJ_DIR)/test3.o

TEST:= $(TEST_0) $(TEST_1) $(TEST_2) $(TEST_3)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

hash%: $(OBJ_DIR)/hash%.o $(TEST)
	$(CC) $< $(TEST_0) -o $(BIN_DIR)/$@0
	$(CC) $< $(TEST_1) -o $(BIN_DIR)/$@1
	$(CC) $< $(TEST_2) -o $(BIN_DIR)/$@2
	$(CC) $< $(TEST_3) -o $(BIN_DIR)/$@3
    
all: build_dirs hash0 hash1 hash2 hash3      
    
build_dirs:
	mkdir $(BIN_DIR) -p
	mkdir $(OBJ_DIR) -p

clean:
	rm -rf $(BIN_DIR)  
	rm -rf $(OBJ_DIR)

.PHONY: clean
