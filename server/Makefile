OUT_FILE=server.out

MAIN_OBJECT_FILE=index.o
LIB_OBJECT_FILES=lib/rule.o
UTILS_OBJECT_FILES=utils/function-utils.o utils/list-utils.o utils/network-utils.o
COMMANDS_SHARED_OBJECT_FILES=commands/handle-add-rule.so commands/handle-list-rule.so commands/handle-delete-rule.so commands/handle-server-shutdown.so

# ---

all: program commands

# ---

program: $(MAIN_OBJECT_FILE) $(LIB_OBJECT_FILES) $(UTILS_OBJECT_FILES)
	gcc -o $(OUT_FILE) $(MAIN_OBJECT_FILE) $(LIB_OBJECT_FILES) $(UTILS_OBJECT_FILES)

# ---

index.o: index.c
	gcc -c -o index.o index.c

clean_index:
	rm -rf $(MAIN_OBJECT_FILE)

# ---

lib: $(LIB_OBJECT_FILES)

lib/%.o: lib/%.c
	gcc -c -o lib/$*.o lib/$*.c

clean_lib:
	rm -rf $(LIB_OBJECT_FILES)

# ---

utils: $(UTILS_OBJECT_FILES)

utils/%.o: utils/%.c
	gcc -c -o utils/$*.o utils/$*.c

clean_utils:
	rm -rf $(UTILS_OBJECT_FILES)

# ---

commands: $(COMMANDS_SHARED_OBJECT_FILES) $(LIB_OBJECT_FILES) $(UTILS_OBJECT_FILES)

commands/%.so: commands/%.c
	gcc -o commands/$*.so commands/$*.c $(LIB_OBJECT_FILES) $(UTILS_OBJECT_FILES) -fPIC -shared

clean_commands:
	rm -rf $(COMMANDS_SHARED_OBJECT_FILES)

# ---

clean: clean_index clean_utils clean_lib clean_commands