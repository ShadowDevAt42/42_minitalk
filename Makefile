NAME = client server
BONUS_NAME = client_bonus server_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes

# Couleurs ANSI
GREEN = \x1b[32m
BLUE = \x1b[34m
YELLOW = \x1b[33m
RED = \x1b[31m
RESET = \x1b[0m
BOLD = \x1b[1m
CYAN = \x1b[36m

SRC_DIR = srcs
BONUS_DIR = $(SRC_DIR)/bonus
OBJ_DIR = objs
OBJ_BONUS_DIR = objs_bonus

SRC_CLIENT = $(SRC_DIR)/client.c $(SRC_DIR)/utils.c
SRC_SERVER = $(SRC_DIR)/server.c $(SRC_DIR)/utils.c

BONUS_SRC_CLIENT = $(BONUS_DIR)/client_bonus.c \
					$(BONUS_DIR)/client_bonus_utils.c \
					$(BONUS_DIR)/utils_bonus.c \
					$(BONUS_DIR)/utils_bonus2.c

BONUS_SRC_SERVER = $(BONUS_DIR)/server_bonus.c \
					$(BONUS_DIR)/utils_bonus.c \
					$(BONUS_DIR)/utils_bonus2.c

OBJ_CLIENT = $(SRC_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_SERVER = $(SRC_SERVER:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ_CLIENT = $(BONUS_SRC_CLIENT:$(BONUS_DIR)/%.c=$(OBJ_BONUS_DIR)/%.o)
BONUS_OBJ_SERVER = $(BONUS_SRC_SERVER:$(BONUS_DIR)/%.c=$(OBJ_BONUS_DIR)/%.o)

define show_progress
	@printf "$(BLUE)⟦ Compilation"
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf " ⟧$(RESET)\n"
endef

define show_success
	@printf "\n$(GREEN)⟦ ✨ Compilation terminée ✨ ⟧$(RESET)\n\n"
endef

define show_signal_animation
	@printf "$(YELLOW)➜ Client $(CYAN)→ $(YELLOW)Server: $(CYAN)Transmission$(RESET)\n"
	@printf "$(BLUE)[      ] 0%%$(RESET)\r"
	@sleep 0.3
	@printf "$(BLUE)[▓▓    ] 25%%$(RESET)\r"
	@sleep 0.3
	@printf "$(BLUE)[▓▓▓▓  ] 50%%$(RESET)\r"
	@sleep 0.3
	@printf "$(GREEN)[▓▓▓▓ ] 75%%$(RESET)\r"
	@sleep 0.3
	@printf "$(BLUE)[▓▓▓▓▓▓] 100%%$(RESET)\n"
endef

define show_bonus_animation
	@printf "$(YELLOW)➜ Client $(CYAN)⚡️ $(YELLOW)Server: $(CYAN)Transmission sécurisée$(RESET)\n"
	@printf "$(GREEN)[              ] 0%%$(RESET)\r"
	@sleep 0.2
	@printf "$(GREEN)[▓▓▓           ] 25%%$(RESET)\r"
	@sleep 0.2
	@printf "$(GREEN)[▓▓▓▓▓▓        ] 50%%$(RESET)\r"
	@sleep 0.2
	@printf "$(GREEN)[▓▓▓▓▓▓▓▓▓     ] 75%%$(RESET)\r"
	@sleep 0.2
	@printf "$(GREEN)[▓▓▓▓▓▓▓▓▓▓▓▓▓▓] 100%%$(RESET)\n"
endef

all: $(NAME)
	$(show_success)

bonus: $(BONUS_NAME)
	$(show_success)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_BONUS_DIR):
	@mkdir -p $(OBJ_BONUS_DIR)

client: $(OBJ_DIR) $(OBJ_CLIENT)
	$(show_signal_animation)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_CLIENT)
	@printf "$(GREEN)✓ Client compilé avec succès$(RESET)\n"

server: $(OBJ_DIR) $(OBJ_SERVER)
	$(show_signal_animation)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_SERVER)
	@printf "$(GREEN)✓ Server compilé avec succès$(RESET)\n"

client_bonus: $(OBJ_BONUS_DIR) $(BONUS_OBJ_CLIENT)
	$(show_bonus_animation)
	@$(CC) $(CFLAGS) -o $@ $(BONUS_OBJ_CLIENT)
	@printf "$(GREEN)✓ Client bonus compilé avec succès$(RESET)\n"

server_bonus: $(OBJ_BONUS_DIR) $(BONUS_OBJ_SERVER)
	$(show_bonus_animation)
	@$(CC) $(CFLAGS) -o $@ $(BONUS_OBJ_SERVER)
	@printf "$(GREEN)✓ Server bonus compilé avec succès$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(show_progress)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(show_progress)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "$(RED)⟦ 🧹 Nettoyage"
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf " ⟧$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@printf "$(GREEN)✓ Nettoyage terminé$(RESET)\n"

clean_bonus:
	@printf "$(RED)⟦ 🧹 Nettoyage bonus"
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf " ⟧$(RESET)\n"
	@rm -rf $(OBJ_BONUS_DIR)
	@printf "$(GREEN)✓ Nettoyage terminé$(RESET)\n"

fclean: clean
	@printf "$(RED)⟦ 🗑️  Suppression"
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf " ⟧$(RESET)\n"
	@rm -f $(NAME)
	@printf "$(GREEN)✓ Suppression terminée$(RESET)\n"

fclean_bonus: clean_bonus
	@printf "$(RED)⟦ 🗑️  Suppression bonus"
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf "."
	@sleep 0.2
	@printf " ⟧$(RESET)\n"
	@rm -f $(BONUS_NAME)
	@printf "$(GREEN)✓ Suppression terminée$(RESET)\n"

re: fclean all

bonus_re: fclean_bonus bonus

.PHONY: all bonus clean clean_bonus fclean fclean_bonus re bonus_re
