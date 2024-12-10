NAME = ft_malcolm

SRC = main.c \
	  utils.c \
	  address.c \
	  print.c



CFLAGS = -Wall -Wextra -Werror

all : $(NAME)
	
$(NAME): 
	gcc $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(SRC:.c=.o)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:
	all clean fclean re bonus