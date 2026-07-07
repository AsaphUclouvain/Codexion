CC=cc
NAME=codexion
CPPFLAGS=-I$(INCLUDE)
CFLAGS=#-Wall -Werror -Wextra -pthread #-g
INCLUDE=include
SRC_DIR=src
OBJ_DIR=obj
TEST_INPUT = 11 190 60 60 60 2 70 edf

SRC_RADICAL= main cleanup coder_action key_handler key_handler_utils \
	thread_handler time utils vars_initializer_1 vars_initializer_2 \
	worker ft_split input_parser input_parser_utils priority_queue \
	priority_queue_utils

SRC_FILES = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_RADICAL)))
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_RADICAL)))

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

debug_thread: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -fsanitize=thread -g $^ -o $(NAME)
	setarch $(uname -m) -R ./$(NAME) $(TEST_INPUT)

debug_leak: all
	valgrind -s --track-origins=yes --leak-check=full ./$(NAME) $(TEST_INPUT)

debug_memo: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -fsanitize=address $^ -o $(NAME)
	./$(NAME) $(TEST_INPUT)

test: all
	./$(NAME) $(TEST_INPUT)

run: all
	./$(NAME) $(TEST_INPUT)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all debug_leak debug_thread debug_memo run clean re