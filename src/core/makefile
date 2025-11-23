.PHONY: lib clean

CORE_SRC = aes.c key_expansion.c Sbox_gen.c
CORE_OBJ = $(patsubst %.c, $(BUILD_DIR)/obj_files/%.o, $(CORE_SRC))

$(BUILD_DIR)/obj_files/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


lib: $(CORE_OBJ)
	ar rcs $(LIB_CORE) $(CORE_OBJ)

clean:
	rm -f $(CORE_OBJ) $(LIB_CORE)

