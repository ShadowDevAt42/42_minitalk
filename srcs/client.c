/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 19:16:45 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:40:09 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Définitions des standards de compatibilité POSIX et UNIX
 * 
 * Ces trois définitions sont cruciales pour assurer la portabilité et
 * l'accès aux fonctionnalités système avancées :
 * 
 * 1. _POSIX_C_SOURCE 199309L :
 *    - Active les fonctionnalités POSIX.1b de 1993
 *    - Nécessaire pour sigaction, sig_atomic_t et autres fonctions temps réel
 *    - La valeur 199309L correspond à la version POSIX.1b de septembre 1993
 * 
 * 2. _XOPEN_SOURCE 700 :
 *    - Active les extensions X/Open (XSI)
 *    - La valeur 700 correspond à la version 7 de UNIX
 *    - Donne accès à des fonctionnalités UNIX avancées
 * 
 * 3. _DEFAULT_SOURCE :
 *    - Active les définitions par défaut de GNU
 *    - Remplace l'ancien _BSD_SOURCE
 *    - Assure la compatibilité avec les systèmes modernes
 * 
 * Ces définitions doivent être placées avant tout include pour
 * être effectives et configurer correctement l'environnement.
 */
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include "minitalk.h"

/**
 * @brief Variable globale pour la synchronisation inter-processus
 * 
 * Cette variable est essentielle au protocole de communication :
 * - volatile : Empêche l'optimisation qui masquerait les changements
 * - sig_atomic_t : Garantit des opérations atomiques
 * - Initialisation à 0 : État initial "pas de confirmation reçue"
 * 
 * Son utilisation est justifiée car c'est le seul moyen sûr de
 * communiquer entre un gestionnaire de signal et le programme principal.
 */
static volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief Gestionnaire de signaux pour le protocole client
 * @param sig Signal reçu (SIGUSR1 pour ACK, SIGUSR2 pour fin)
 * 
 * Implémente un protocole de confirmation à deux niveaux :
 * 
 * 1. Confirmation de bit (SIGUSR1) :
 *    - Active le drapeau g_signal_received
 *    - Permet la progression de la transmission
 * 
 * 2. Confirmation finale (SIGUSR2) :
 *    - Indique la réception complète du message
 *    - Affiche une confirmation
 *    - Termine proprement le programme
 * 
 * Aspects de sécurité :
 * - Utilisation de write() au lieu de printf() dans le handler
 * - Opérations minimales pour éviter les problèmes de réentrance
 * - Gestion atomique du drapeau de synchronisation
 */
static void	ft_sig_handler(int sig)
{
	if (sig == SIGUSR2)
	{
		write(1, "Message received by server\n", 26);
		exit(0);
	}
	g_signal_received = 1;
}

/**
 * @brief Transmet un bit unique avec confirmation
 * @param pid PID du serveur destinataire
 * @param bit_val Valeur du bit (0 ou 1)
 * 
 * Cette fonction implémente le protocole de base avec :
 * 
 * 1. Préparation :
 *    - Réinitialisation du drapeau de confirmation
 *    - Sélection du signal approprié
 * 
 * 2. Transmission :
 *    - SIGUSR2 pour bit 1
 *    - SIGUSR1 pour bit 0
 * 
 * 3. Confirmation :
 *    - Attente active avec pause()
 *    - Vérification du drapeau g_signal_received
 * 
 * Gestion d'erreur robuste :
 * - Vérification de chaque appel système
 * - Messages d'erreur explicites
 * - Terminaison propre en cas d'échec
 */
static void	ft_send_bit(pid_t pid, int bit_val)
{
	g_signal_received = 0;
	if (bit_val)
	{
		if (kill(pid, SIGUSR2) == -1)
		{
			write(2, "Error: Failed to send signal\n", 28);
			exit(1);
		}
	}
	else
	{
		if (kill(pid, SIGUSR1) == -1)
		{
			write(2, "Error: Failed to send signal\n", 28);
			exit(1);
		}
	}
	while (!g_signal_received)
		pause();
}

/**
 * @brief Décompose et transmet un caractère
 * @param pid PID du serveur destinataire
 * @param c Caractère à transmettre
 * 
 * Processus de transmission détaillé :
 * 1. Décomposition du caractère en 8 bits
 * 2. Transmission MSB first (bit 7 à 0)
 * 3. Utilisation d'opérations bit à bit optimisées
 * 
 * Particularités techniques :
 * - Utilisation de unsigned char pour éviter les problèmes de signe
 * - Ordre de transmission standardisé (MSB first)
 * - Attente de confirmation pour chaque bit
 */
void	ft_send_char(pid_t pid, unsigned char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		ft_send_bit(pid, (c >> bit) & 1);
		bit--;
	}
}

/**
 * @brief Orchestre la transmission complète d'un message
 * @param server_pid PID du serveur destinataire
 * @param message Message à transmettre
 * 
 * Protocole complet de transmission :
 * 1. Envoi séquentiel des caractères
 * 2. Transmission du caractère nul terminal
 * 3. Attente de la confirmation finale
 * 
 * Caractéristiques importantes :
 * - Transmission ordonnée et fiable
 * - Gestion de la fin de message
 * - Synchronisation bidirectionnelle
 * - Attente active économe (pause)
 */
static void	ft_send_message(pid_t server_pid, char *message)
{
	int	i;

	i = 0;
	while (message[i])
	{
		ft_send_char(server_pid, message[i]);
		i++;
	}
	ft_send_char(server_pid, '\0');
	while (1)
		pause();
}

/**
 * @brief Point d'entrée principal du client
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @return 0 en cas de succès, 1 en cas d'erreur
 * 
 * Séquence d'initialisation et d'exécution :
 * 
 * 1. Validation des arguments :
 *    - Vérification du nombre d'arguments
 *    - Validation du PID serveur
 * 
 * 2. Configuration des signaux :
 *    - Initialisation de sigaction
 *    - Configuration des handlers
 *    - Vérification des erreurs
 * 
 * 3. Transmission du message :
 *    - Appel à ft_send_message
 *    - Attente de la confirmation finale
 * 
 * Gestion d'erreur complète :
 * - Vérification de chaque étape
 * - Messages d'erreur explicites
 * - Codes de retour appropriés
 */
int	main(int argc, char **argv)
{
	struct sigaction	sa;
	pid_t				server_pid;

	if (argc != 3)
	{
		write(2, "Usage: ./client [server_pid] [message]\n", 38);
		return (1);
	}
	server_pid = ft_atoi(argv[1]);
	if (server_pid <= 0)
	{
		write(2, "Error: Invalid PID\n", 18);
		return (1);
	}
	sa.sa_handler = ft_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(2, "Error: sigaction failed\n", 23);
		return (1);
	}
	ft_send_message(server_pid, argv[2]);
	return (0);
}
