BLUE=\033[0;34m
LIGHT_BLUE=\033[0;94m
YELLOW=\033[0;33m
GREEN=\033[0;32m
RED=\033[0;31m
RESET=\033[0m

NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I../ -I.

LIBFT_DIR = ../
LIBFT = $(LIBFT_DIR)libft.a

SRCS = \
	ft_printf.c specifiers.c str_wrappers.c \
	num_wrappers.c hex_wrappers.c ptr_wrappers.c \
	errors.c

OBJS = $(SRCS:%.c=%.o)

all: start_build $(NAME) end_build

start_build:
	@echo "▶ Run Build $(NAME)"
	@echo "$(shell date +%H:%M:%S) ${YELLOW}🟡 Starting libftprintf compilation...${RESET}"

end_build:
	@echo "$(shell date +%H:%M:%S) ${GREEN}🟢 libftprintf build finished successfully.${RESET}"

$(NAME): $(OBJS)
	@echo "$(shell date +%H:%M:%S) ${LIGHT_BLUE}	Building $(NAME) from objects...${RESET}"
	@echo "$(shell date +%H:%M:%S) ${LIGHT_BLUE}	Using libft from $(LIBFT)...${RESET}"
	@cp $(LIBFT) $(NAME) || (echo "$(shell date +%H:%M:%S) ${RED}🔴 Failed to copy libft.a.${RESET}"; exit 1)
	@ar rcs $(NAME) $(OBJS) || (echo "$(shell date +%H:%M:%S) ${RED}🔴 Archiving failed.${RESET}"; exit 1)

%.o: %.c
	@echo "$(shell date +%H:%M:%S) ${BLUE}	Compiling $< ...${RESET}"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ || (echo "$(shell date +%H:%M:%S) ${RED}🔴 Compile failed on $<.${RESET}"; exit 1)

clean:
	@echo "▶ Run Clean"
	@echo "$(shell date +%H:%M:%S) ${BLUE}🟡 Cleaning object files...${RESET}"
	@rm -f $(OBJS)
	@echo "$(shell date +%H:%M:%S) ${GREEN}🟢 Clean done.${RESET}"

fclean: clean
	@echo "▶ Run Fclean"
	@echo "$(shell date +%H:%M:%S) ${BLUE}🟡 Removing archive...${RESET}"
	@rm -f $(NAME)
	@echo "$(shell date +%H:%M:%S) ${GREEN}🟢 Fclean done.${RESET}"

re: fclean all

.PHONY: all clean fclean re start_build end_build
