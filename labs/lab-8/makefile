# Variables
COMPILER = gcc
C_FLAGS = -m32
CLIENT_DIR = CLIENT
CLIENT_BIN = client.bin
SERVER_DIR = SERVER
SERVER_BIN = server.bin
SHARED = util

# Copy directory (arg 1) to another dir (arg 2)
define copy_dir
	cp -r $(1) $(2)
endef

# Recursively remove directory (arg 1) from another dir (arg 2)
define rm_dir
	rm -rf $(2)/$(1)
endef

client:
	$(call copy_dir,$(SHARED),$(CLIENT_DIR)); \
	cd $(CLIENT_DIR); \
	$(COMPILER) -o $(CLIENT_BIN) $(C_FLAGS) util/*.[ch] lcmd/*.c lcmd.c client.c; \
	cd ..
	
server:
	$(call copy_dir,$(SHARED),$(SERVER_DIR)); \
	cd $(SERVER_DIR); \
	$(COMPILER) -o $(SERVER_BIN) $(C_FLAGS) util/*.[ch] cmd/*.c cmd.c server.c; \
	cd ..

both:
	$(MAKE) client && $(MAKE) server

clean_client:
	$(call rm_dir,$(SHARED),$(CLIENT_DIR)); \
	rm -f $(CLIENT_DIR)/$(CLIENT_BIN)

clean_server:
	$(call rm_dir,$(SHARED),$(SERVER_DIR)); \
	rm -f $(SERVER_DIR)/$(SERVER_BIN)

clean:
	$(MAKE) clean_client && $(MAKE) clean_server

run_client: client
	@echo "usage: make run_client servername=[insert server name] port=[insert port name]"; \
	cd $(CLIENT_DIR) && ./$(CLIENT_BIN) $(servername) $(port)

run_server: server
	cd $(SERVER_DIR) && ./$(SERVER_BIN)
