.PHONY: binary clean

CLI_SRC = cli.c
CLI_OBJ = $(patsubst %.c, $(BUILD_DIR)/obj_files/%.o, $(CLI_SRC))

$(CLI_OBJ): $(CLI_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

binary: $(CLI_BINARY)

$(CLI_BINARY): $(CLI_OBJ) $(LIBS)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(CLI_OBJ) $(CLI_BINARY)


