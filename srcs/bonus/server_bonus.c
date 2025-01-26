/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:38:07 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:46:12 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Définitions des standards POSIX pour assurer la portabilité */
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include "bonus.h"

/**
 * @brief Variable globale pour la synchronisation de la réception des données
 * 
 * Cette variable volatile de type sig_atomic_t joue un rôle crucial dans
 * la synchronisation entre le gestionnaire de signaux et la boucle principale.
 * Son état indique si de nouvelles données ont été reçues et
 * doivent être traitées.
 * 
 * Caractéristiques importantes :
 * - volatile : Empêche l'optimisation qui masquerait les changements
 * - sig_atomic_t : Garantit des opérations atomiques
 * - Initialisation à 0 : État initial "pas de données en attente"
 */
static volatile sig_atomic_t	g_data = 0;

/**
 * @brief Gère le traitement d'un caractère complet et les statistiques associées
 * @param c           Caractère reconstruit à partir des bits reçus
 * @param client_pid  Pointeur vers le PID du client actif
 * @param i          Pointeur vers le compteur de bits du caractère en cours
 * @param stats      Pointeur vers la structure des statistiques de transmission
 * 
 * Cette fonction est appelée chaque fois qu'un caractère complet
 * (8 bits) est reçu.
 * Elle gère deux scénarios principaux :
 * 
 * 1. Réception du caractère nul (fin de message) :
 *    - Affiche un saut de ligne
 *    - Imprime les statistiques finales
 *    - Envoie un signal de confirmation au client (SIGUSR2)
 *    - Réinitialise les compteurs pour la prochaine transmission
 * 
 * 2. Réception d'un caractère normal :
 *    - Affiche le caractère
 *    - Met à jour le compteur de caractères
 * 
 * @note Cette fonction est critique pour maintenir la cohérence des statistiques
 *       et assurer une communication fiable avec le client.
 */
static void	ft_handle_char_bonus(unsigned char c, pid_t *client_pid,
				int *i, t_stats *stats)
{
	*i = 0;
	if (!c)
	{
		ft_putchar_bonus('\n');
		ft_print_stats(stats);
		kill(*client_pid, SIGUSR2);
		stats->chars_received = 0;
		stats->bits_received = 0;
		*client_pid = 0;
		return ;
	}
	ft_putchar_bonus(c);
	stats->chars_received++;
}

/**
 * @brief Initialise une nouvelle session client avec affichage visuel
 * @param stats      Pointeur vers la structure des statistiques
 * @param client_pid PID du nouveau client connecté
 * 
 * Cette fonction assure un accueil visuel et informatif pour chaque
 * nouvelle connexion client. Elle :
 * 
 * 1. Enregistre le PID du nouveau client
 * 2. Affiche un message d'accueil formaté et coloré
 *    - Utilise une flèche distinctive (ARROW_MARK)
 *    - Affiche le PID du client en jaune
 *    - Réinitialise la couleur après l'affichage
 * 
 * L'utilisation des couleurs améliore la lisibilité et permet de
 * distinguer facilement les différents types d'événements dans les logs.
 */
static void	ft_handle_new_client(t_stats *stats, pid_t client_pid)
{
	stats->client_pid = client_pid;
	ft_putstr_bonus(COLOR_YELLOW);
	ft_putstr_bonus(ARROW_MARK " Nouvelle connexion client (PID: ");
	ft_putnbr_bonus(stats->client_pid);
	ft_putstr_bonus(")\n");
	ft_putstr_bonus(COLOR_RESET);
}

/**
 * @brief Gestionnaire principal des signaux avec reconstruction des caractères
 * @param sig      Signal reçu (SIGUSR1 ou SIGUSR2)
 * @param info     Structure contenant les informations du signal
 * @param context  Contexte d'exécution (non utilisé)
 * 
 * Cette fonction implémente un protocole sophistiqué de reconstruction
 * des caractères à partir des signaux reçus bit par bit. Elle maintient
 * plusieurs états persistants entre les appels :
 * 
 * - i : Position du bit en cours dans le caractère (0-7)
 * - c : Caractère en cours de reconstruction
 * - stats : Statistiques globales de la session
 * 
 * Processus de reconstruction :
 * 1. Détection d'un nouveau client si nécessaire
 * 2. Décalage à gauche du caractère en construction
 * 3. Ajout du nouveau bit (1 pour SIGUSR2, 0 pour SIGUSR1)
 * 4. Mise à jour des statistiques
 * 5. Traitement du caractère complet après 8 bits
 * 6. Envoi de l'acquittement au client
 * 
 * La fonction utilise g_data pour signaler à la boucle principale
 * qu'un nouveau bit a été reçu et traité.
 */
static void	ft_receive_bonus(int sig, siginfo_t *info, void *context)
{
	static int				i = 0;
	static unsigned char	c = 0;
	static t_stats			stats = {0, 0, 0, 0};

	(void)context;
	if (!stats.client_pid)
		ft_handle_new_client(&stats, info->si_pid);
	c = c << 1;
	if (sig == SIGUSR2)
		c = c | 1;
	stats.bits_received++;
	if (++i == 8)
		ft_handle_char_bonus(c, &stats.client_pid, &i, &stats);
	if (stats.client_pid)
	{
		kill(stats.client_pid, SIGUSR1);
		g_data = 1;
	}
}

/**
 * @brief Configure le système avancé de gestion des signaux
 * @param sa Pointeur vers la structure de configuration sigaction
 * @return 1 en cas de succès, 0 en cas d'échec
 * 
 * Cette fonction établit une configuration robuste pour le traitement
 * des signaux SIGUSR1 et SIGUSR2. Elle utilise sigaction pour une
 * gestion moderne et fiable des signaux avec les caractéristiques suivantes :
 * 
 * 1. Masque de signaux vide pour éviter le blocage
 * 2. Handler avec informations étendues (SA_SIGINFO)
 * 3. Configuration identique pour les deux signaux
 * 
 * La configuration est vérifiée pour chaque signal avec gestion
 * d'erreur appropriée.
 */
static int	ft_setup_signals(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sa->sa_sigaction = ft_receive_bonus;
	sa->sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, sa, NULL) == -1)
		return (0);
	if (sigaction(SIGUSR2, sa, NULL) == -1)
		return (0);
	return (1);
}

/**
 * @brief Point d'entrée du serveur Minitalk avec fonctionnalités avancées
 * 
 * Cette fonction implémente la boucle principale du serveur avec :
 * 
 * 1. Initialisation :
 *    - Récupération et affichage du PID
 *    - Configuration des gestionnaires de signaux
 *    - Messages de démarrage colorés
 * 
 * 2. Boucle de service :
 *    - Attente active économe (pause)
 *    - Traitement des données reçues
 *    - Réinitialisation du flag de données
 * 
 * Le serveur utilise un système de couleurs pour améliorer la lisibilité :
 * - Vert : Messages de démarrage
 * - Bleu : Informations techniques (PID)
 * - Jaune : États d'attente
 * - Rouge : Messages d'erreur
 * 
 * @return 0 en cas de succès, 1 en cas d'erreur d'initialisation
 */
int	main(void)
{
	struct sigaction		sa;
	pid_t					pid;

	pid = getpid();
	ft_print_colored("🚀 Serveur Minitalk Bonus démarré", COLOR_GREEN);
	ft_putstr_bonus(COLOR_BLUE);
	ft_putstr_bonus("PID: ");
	ft_putnbr_bonus(pid);
	ft_putchar_bonus('\n');
	ft_print_colored("En attente de messages...", COLOR_YELLOW);
	if (!ft_setup_signals(&sa))
	{
		ft_print_colored("Erreur: Configuration des signaux échouée",
			COLOR_RED);
		return (1);
	}
	while (1)
	{
		while (!g_data)
			pause();
		g_data = 0;
	}
	return (0);
}
