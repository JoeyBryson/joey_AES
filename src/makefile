.PHONY: all clean libs

libs:
	$(MAKE) -C core lib
	$(MAKE) -C file-utilities lib

all:
	$(MAKE) -C core lib
	$(MAKE) -C file-utilities lib
	$(MAKE) -C cli binary
	
clean:
	$(MAKE) -C core clean
	$(MAKE) -C file-utilities clean
	$(MAKE) -C cli clean
