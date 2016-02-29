CC=gcc
CFLAGS=-W -Wall -ansi
LDFLAGS= -lMLV -lm
DEBUG=yes

INCLUDE_DIR=include
OBJ_DIR=obj
SRC_DIR=src
EXEC_DIR=bin


ifeq ($(DEBUG), yes)
	CFLAGS+= -g
endif


all: $(OBJ_DIR) $(EXEC_DIR) $(EXEC_DIR)/mydevin


$(OBJ_DIR):
	mkdir obj

$(EXEC_DIR):
	mkdir bin

$(EXEC_DIR)/mydevin: $(OBJ_DIR)/Main.o $(OBJ_DIR)/DBManager.o $(OBJ_DIR)/GUI.o $(OBJ_DIR)/Population.o $(OBJ_DIR)/Question.o $(OBJ_DIR)/Character.o $(OBJ_DIR)/Stream.o $(OBJ_DIR)/String.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/Main.o: $(SRC_DIR)/Main.c
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

$(OBJ_DIR)/DBManager.o: $(SRC_DIR)/DBManager.c $(INCLUDE_DIR)/DBManager.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

$(OBJ_DIR)/GUI.o: $(SRC_DIR)/GUI.c $(INCLUDE_DIR)/GUI.h $(INCLUDE_DIR)/GUI_components.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

$(OBJ_DIR)/Population.o: $(SRC_DIR)/Population.c $(INCLUDE_DIR)/Population.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)
	
$(OBJ_DIR)/Question.o: $(SRC_DIR)/Question.c $(INCLUDE_DIR)/Question.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)
	
$(OBJ_DIR)/Character.o: $(SRC_DIR)/Character.c $(INCLUDE_DIR)/Character.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

$(OBJ_DIR)/Stream.o: $(SRC_DIR)/Stream.c $(INCLUDE_DIR)/Stream.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)
	
$(OBJ_DIR)/String.o: $(SRC_DIR)/String.c $(INCLUDE_DIR)/String.h
	$(CC) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

clean: 
	rm -rf $(OBJ_DIR)

mrproper: clean
	rm -rf $(EXEC_DIR)