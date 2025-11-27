.PHONY: binary clean
ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common -fno-omit-frame-pointer

CLI_SRC    = cli.c
CLI_OBJ    = $(BUILD_DIR)/obj_files/cli.o
CLI_BINARY = $(BUILD_DIR)/joeyaes

# compile step: .c → .o
$(CLI_OBJ): $(CLI_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# "make binary" builds the executable
binary: $(CLI_BINARY)

# link step: .o → executable
$(CLI_BINARY): $(CLI_OBJ) $(LIBS)
	$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(CLI_OBJ) $(CLI_BINARY)
