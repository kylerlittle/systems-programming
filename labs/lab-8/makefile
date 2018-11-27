# Variables
COMPILER = gcc
C_FLAGS = -m32
CLIENT_BIN = client.bin
SERVER_BIN = server.bin

client:
	$(COMPILER) -o $(CLIENT_BIN) $(C_FLAGS) util/*.[ch] lcmd/*.c lcmd.c client.c

server:
	$(COMPILER) -o $(SERVER_BIN) $(C_FLAGS) util/*.[ch] cmd/*.c cmd.c server.c

both:
	$(MAKE) client && $(MAKE) server

clean:
	rm -rf $(CLIENT_BIN) $(SERVER_BIN)
