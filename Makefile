CC = gcc
SRC_DIR = src
BIN_DIR = bin

READER_SRC = $(SRC_DIR)/shm_reader.c
WRITER_SRC = $(SRC_DIR)/shm_writer.c

READER_BIN = $(BIN_DIR)/reader
WRITER_BIN = $(BIN_DIR)/writer

all: $(READER_BIN) $(WRITER_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(READER_BIN): $(READER_SRC) | $(BIN_DIR)
	$(CC) $(READER_SRC) -o $(READER_BIN)

$(WRITER_BIN): $(WRITER_SRC) | $(BIN_DIR)
	$(CC) $(WRITER_SRC) -o $(WRITER_BIN)

clean:
	rm -rf $(BIN_DIR)