.PHONY: lib clean

UTIL_SRC = file_encrypt.c file_decrypt.c utils.c
UTIL_OBJ = $(patsubst %.c, $(BUILD_DIR)/obj_files/%.o, $(UTIL_SRC))

$(BUILD_DIR)/obj_files/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lib: $(LIB_UTIL)

$(LIB_UTIL): $(UTIL_OBJ) $(LIB_CORE)
	ar rcs $(LIB_UTIL) $(UTIL_OBJ)

clean:
	rm -f $(UTIL_OBJ) $(LIB_UTIL)

