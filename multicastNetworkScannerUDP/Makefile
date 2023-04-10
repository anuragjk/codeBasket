CC=gcc
CFLAGS=-I.
DEPS = 
OBJ =  
OBJ_DIR = ./obj
MKDIR = mkdir

prereq: 
	$(MKDIR) -p $(OBJ_DIR)

$(OBJ_DIR)/listner.o: listener.c $(DEPS)
	$(CC) -c -o $(OBJ_DIR)/listner.o listner.c $(CFLAGS)

listner: $(OBJ_DIR)/listener.o
	$(CC) -o listner $(OBJ_DIR)/listner.o $(CFLAGS)

$(OBJ_DIR)/sender.o: sender.c $(DEPS)
	$(CC) -c -o $(OBJ_DIR)/sender.o sender.c $(CFLAGS)

sender: $(OBJ_DIR)/sender.o
	$(CC) -o sender $(OBJ_DIR)/sender.o $(CFLAGS)

all: prereq listener sender

clean: 
	$(RM) -rf $(OBJ_DIR) listener sender

