/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:24:07 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:26:10 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Définition des standards POSIX pour garantir la compatibilité */
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include "bonus.h"

/**
 * @brief Variable globale pour la synchronisation inter-processus
 * 
 * Cette variable volatile de type sig_atomic_t assure une synchronisation
 * thread-safe entre le gestionnaire de signaux et le programme principal.
 * Son utilisation est considérée comme une bonne pratique dans ce contexte car :
 * 
 * 1. volatile : Empêche l'optimisation du compilateur qui pourrait cacher
 *              les modifications de la variable
 * 2. sig_atomic_t : Garantit que les opérations de lecture/écriture sont
 *                   atomiques, évitant les conditions de course
 * 
 * Cette approche est recommandée par POSIX pour la communication entre
 * un gestionnaire de signal et le programme principal.
 */
volatile sig_atomic_t	g_signal_received;

/**
 * @brief Gestionnaire de signaux avancé pour le client
 * @param sig Numéro du signal reçu (SIGUSR1 ou SIGUSR2)
 * 
 * Implémente un protocole de communication bidirectionnel :
 * 
 * SIGUSR2 : Signal de fin de transmission
 * - Indique que le serveur a reçu et traité l'intégralité du message
 * - Affiche un message de confirmation en vert
 * - Termine le programme avec succès (code 0)
 * 
 * SIGUSR1 : Signal d'acquittement
 * - Confirme la réception d'un bit individuel
 * - Active le drapeau g_signal_received pour permettre l'envoi suivant
 * - Fait partie du mécanisme de contrôle de flux bit par bit
 * 
 * @note Cette fonction est appelée de manière asynchrone et doit donc
 *       rester aussi simple et rapide que possible
 */
static void	ft_sig_handler_bonus(int sig)
{
	if (sig == SIGUSR2)
	{
		ft_print_colored("Message reçu avec succès!", COLOR_GREEN);
		exit(0);
	}
	g_signal_received = 1;
}

/**
 * @brief Initialise le système de gestion des signaux
 * @return 1 en cas de succès, 0 en cas d'échec
 * 
 * Configuration avancée du gestionnaire de signaux avec sigaction :
 * 
 * 1. Création d'une structure sigaction personnalisée :
 *    - Définition du gestionnaire de signal
 *    - Initialisation du masque de signaux
 *    - Configuration des drapeaux de comportement
 * 
 * 2. Installation des gestionnaires pour :
 *    - SIGUSR1 : Acquittement de réception de bit
 *    - SIGUSR2 : Confirmation de fin de message
 * 
 * Cette approche moderne (sigaction vs signal()) offre :
 * - Une meilleure portabilité entre systèmes UNIX
 * - Un contrôle plus fin du comportement des signaux
 * - Une gestion plus sûre des interruptions
 */
static int	ft_init_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_sig_handler_bonus;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_print_colored("Erreur: Configuration signaux échouée", COLOR_RED);
		return (0);
	}
	return (1);
}

/**
 * @brief Analyse et valide les arguments du programme
 * @param argc    Nombre d'arguments reçus
 * @param argv    Tableau des arguments
 * @param verbose Pointeur vers le flag du mode verbeux
 * @param pid     Pointeur vers le PID du serveur destinataire
 * @return 1 si les arguments sont valides, 0 sinon
 * 
 * Validation complète des arguments avec les critères suivants :
 * 
 * Format attendu : ./client_bonus [pid] [message] [-v]
 * - [pid] : Identifiant du processus serveur (obligatoire)
 * - [message] : Texte à transmettre (obligatoire)
 * - [-v] : Active le mode verbeux (optionnel)
 * 
 * Règles de validation :
 * 1. Nombre d'arguments : minimum 3 (programme + pid + message)
 *                        maximum 4 (avec option -v)
 * 2. PID : Doit être un nombre positif non nul
 * 3. Option verbose : Doit être exactement "-v" si présente
 * 
 * En cas d'erreur :
 * - Affiche un message d'usage en rouge
 * - Retourne 0 pour indiquer l'échec
 */
static int	ft_check_args(int argc, char **argv, int *verbose, pid_t *pid)
{
	if (argc < 3 || argc > 4)
	{
		ft_print_colored("Usage: ./client_bonus [pid] [msg] [-v]", COLOR_RED);
		return (0);
	}
	*verbose = 0;
	if (argc == 4 && argv[3][0] == '-' && argv[3][1] == 'v')
		*verbose = 1;
	*pid = ft_atoi_bonus(argv[1]);
	if (*pid <= 0)
	{
		ft_print_colored("Erreur: PID invalide", COLOR_RED);
		return (0);
	}
	return (1);
}

/**
 * @brief Point d'entrée principal du programme client bonus
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @return 0 en cas de succès, 1 en cas d'erreur
 * 
 * Séquence d'exécution du client :
 * 1. Validation des arguments de la ligne de commande
 * 2. Initialisation du système de gestion des signaux
 * 3. Transmission du message au serveur
 * 
 * La fonction suit un modèle de gestion d'erreur strict :
 * - Vérifie chaque étape de l'initialisation
 * - Termine proprement en cas d'échec
 * - Utilise des codes de retour cohérents
 * 
 * @note Le programme ne termine pas normalement après l'envoi
 *       du message - il attend d'être interrompu par un signal
 */
int	main(int argc, char **argv)
{
	pid_t	pid;
	int		verbose;

	if (!ft_check_args(argc, argv, &verbose, &pid))
		return (1);
	if (!ft_init_signals())
		return (1);
	ft_send_message_bonus(pid, argv[2], verbose);
	return (0);
}
