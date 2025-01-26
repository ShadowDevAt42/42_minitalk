/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:30:32 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:27:53 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

extern volatile sig_atomic_t	g_signal_received;

/**
 * @brief Système de debug visuel pour l'envoi des bits
 * @param pid    PID du serveur destinataire
 * (non utilisé mais conservé pour cohérence)
 * @param bit_val Valeur du bit en cours d'envoi (0 ou 1)
 * 
 * Cette fonction améliore la lisibilité du debugging en affichant en temps réel
 * les bits envoyés avec un formatage coloré. Elle est particulièrement utile
 * pendant le développement et le débogage pour suivre le flux de données.
 * 
 * Note: Le paramètre pid est conservé pour maintenir une signature cohérente
 * avec les autres fonctions, même s'il n'est pas utilisé ici.
 */
static void	ft_send_bit_verbose(pid_t pid, int bit_val)
{
	(void)pid;
	ft_putstr_bonus(COLOR_BLUE);
	ft_putstr_bonus("Envoi bit: ");
	ft_putchar_bonus('0' + bit_val);
	ft_putchar_bonus('\n');
	ft_putstr_bonus(COLOR_RESET);
}

/**
 * @brief Mécanisme d'envoi des signaux pour la transmission des bits
 * @param pid     PID du processus serveur destinataire
 * @param bit_val Valeur du bit à transmettre (0 ou 1)
 * 
 * Implémente le protocole de communication bit par bit :
 * - SIGUSR2 représente un bit à 1
 * - SIGUSR1 représente un bit à 0
 * 
 * La fonction intègre une gestion d'erreur robuste :
 * - Vérifie la réussite de chaque envoi de signal
 * - Termine proprement le programme en cas d'échec
 * - Affiche un message d'erreur explicite
 * 
 * @note En cas d'échec d'envoi, le programme se termine avec le code 1
 */
static void	ft_send_bit_signal(pid_t pid, int bit_val)
{
	if (bit_val == 1)
	{
		if (kill(pid, SIGUSR2) == -1)
		{
			ft_print_colored("Erreur: Échec de l'envoi du signal", COLOR_RED);
			exit(1);
		}
	}
	else if (kill(pid, SIGUSR1) == -1)
	{
		ft_print_colored("Erreur: Échec de l'envoi du signal", COLOR_RED);
		exit(1);
	}
}

/**
 * @brief Orchestre la transmission complète d'un bit avec
 * gestion des acquittements
 * @param pid     PID du serveur destinataire
 * @param bit_val Bit à transmettre (0 ou 1)
 * @param verbose Active le mode debug avec affichage détaillé
 * 
 * Cette fonction implémente le protocole complet de transmission d'un bit :
 * 1. Réinitialisation du flag de réception (g_signal_received)
 * 2. Affichage debug si le mode verbose est activé
 * 3. Envoi effectif du signal
 * 4. Attente active de l'acquittement du serveur
 * 
 * Le système d'acquittement garantit la fiabilité de la transmission :
 * - Chaque bit envoyé doit être confirmé par le serveur
 * - Le client attend en pause jusqu'à la réception de la confirmation
 * 
 * @note Cette fonction est thread-safe grâce à l'utilisation de sig_atomic_t
 */
void	ft_send_bit_bonus(pid_t pid, int bit_val, int verbose)
{
	g_signal_received = 0;
	if (verbose)
		ft_send_bit_verbose(pid, bit_val);
	ft_send_bit_signal(pid, bit_val);
	while (!g_signal_received)
		pause();
}

/**
 * @brief Décompose et transmet un caractère bit par bit
 * @param pid     PID du serveur destinataire
 * @param c       Caractère à transmettre
 * @param verbose Mode debug avec affichage détaillé
 * 
 * Processus de transmission d'un caractère :
 * 1. Décomposition en 8 bits, du plus significatif au moins significatif
 * 2. Utilisation d'opérations bit à bit pour l'extraction : (c >> bit) & 1
 * 3. Transmission séquentielle de chaque bit
 * 
 * L'ordre de transmission (MSB first) est crucial pour la reconstruction
 * correcte du caractère côté serveur.
 * 
 * @note Chaque caractère nécessite exactement 8 transmissions de bits
 */
void	ft_send_char_bonus(pid_t pid, unsigned char c, int verbose)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		ft_send_bit_bonus(pid, (c >> bit) & 1, verbose);
		bit--;
	}
}

/**
 * @brief Gère la transmission complète d'une chaîne de caractères
 * @param pid     PID du serveur destinataire
 * @param message Chaîne à transmettre
 * @param verbose Active le mode debug
 * 
 * Cette fonction orchestre le processus complet de transmission :
 * 1. Notification du début de transmission
 * 2. Envoi séquentiel de chaque caractère
 * 3. Transmission du caractère nul terminal ('\0')
 * 4. Attente de la confirmation finale
 * 
 * Particularités importantes :
 * - Le '\0' terminal est explicitement transmis pour marquer la fin
 * - La fonction attend indéfiniment la confirmation finale du serveur
 * - L'affichage est coloré pour une meilleure lisibilité
 * 
 * @note La fonction ne retourne jamais normalement, elle attend
 *       une interruption par signal pour terminer
 */
void	ft_send_message_bonus(pid_t pid, const char *message, int verbose)
{
	int	i;

	i = 0;
	ft_print_colored("Début de la transmission...", COLOR_BLUE);
	while (message[i])
	{
		ft_send_char_bonus(pid, message[i], verbose);
		i++;
	}
	ft_send_char_bonus(pid, '\0', verbose);
	while (1)
		pause();
}
