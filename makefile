# Makefile pour le programme

# Nom du compilateur
CC=gcc

# Options de compilation
CFLAGS=-Wall -Wextra -g

# Liste des fichiers sources
SRCS=main.c fonctions.c

# Liste des fichiers objets générés
OBJS=$(SRCS:.c=.o)

# Nom de l'exécutable généré
EXEC=my_program

# Règle de compilation
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Règle de nettoyage
clean:
	rm -f $(EXEC) $(OBJS)

