/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:43:38 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/25 20:47:12 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>

// Codes ANSI pour les couleurs
# define COLOR_GREEN "\033[1;32m"
# define COLOR_BLUE "\033[1;34m"
# define COLOR_YELLOW "\033[1;33m"
# define COLOR_RED "\033[1;31m"
# define COLOR_RESET "\033[0m"

// Emojis et symboles
# define CHECK_MARK "✓ "
# define CROSS_MARK "✗ "
# define ARROW_MARK "→ "

/**
 * @brief Structure pour les statistiques de transmission
 */
typedef struct s_stats
{
	size_t	chars_received;	/* Nombre de caractères reçus */
	size_t	bits_received;	/* Nombre total de bits reçus */
	pid_t	client_pid;		/* PID du client actuel */
	int		verbose_mode;	/* Mode verbeux activé/désactivé */
}	t_stats;

// Fonctions utilitaires
void	ft_putchar_bonus(char c);
void	ft_putstr_bonus(const char *str);
void	ft_putnbr_bonus(int n);
int		ft_atoi_bonus(const char *str);
void	ft_print_stats(t_stats *stats);
void	ft_print_colored(const char *msg, const char *color);

// Fonctions client bonus
void	ft_send_bit_bonus(pid_t pid, int bit_val, int verbose);
void	ft_send_char_bonus(pid_t pid, unsigned char c, int verbose);
void	ft_send_message_bonus(pid_t pid, const char *message, int verbose);

#endif
