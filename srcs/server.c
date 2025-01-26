/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 19:21:49 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:42:06 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Configuration des standards de compatibilité système
 * 
 * Les définitions en début de fichier sont cruciales pour assurer
 * la portabilité et l'accès aux fonctionnalités système avancées :
 * 
 * 1. _POSIX_C_SOURCE 199309L :
 *    Cette définition active les fonctionnalités POSIX.1b de 1993, essentielles
 *    pour notre serveur. Elle nous donne accès à :
 *    - La structure sigaction pour une gestion avancée des signaux
 *    - Le type sig_atomic_t pour des opérations atomiques
 *    - Les fonctionnalités temps réel de POSIX
 *    La valeur 199309L indique spécifiquement la version de septembre 1993.
 * 
 * 2. _XOPEN_SOURCE 700 :
 *    Cette définition nous donne accès aux extensions X/Open (XSI) :
 *    - Fonctionnalités UNIX avancées
 *    - APIs système étendues
 *    - Compatibilité accrue entre systèmes
 *    La valeur 700 correspond à la version 7 des spécifications UNIX.
 * 
 * 3. _DEFAULT_SOURCE :
 *    Active les définitions GNU par défaut et assure :
 *    - La compatibilité avec les systèmes modernes
 *    - L'accès aux extensions BSD
 *    - Le remplacement de l'ancien _BSD_SOURCE
 * 
 * Ces définitions doivent précéder tout include pour configurer correctement
 * l'environnement de compilation et garantir l'accès aux fonctionnalités
 * nécessaires au serveur.
 */
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include "minitalk.h"

/**
 * @brief Variable globale pour la synchronisation de la réception
 * 
 * Cette variable joue un rôle central dans la synchronisation entre
 * le gestionnaire de signaux et la boucle principale du serveur.
 * 
 * Caractéristiques essentielles :
 * - volatile : Empêche les optimisations du compilateur qui pourraient
 *             masquer les modifications de la variable
 * - sig_atomic_t : Garantit que les opérations de lecture/écriture sont
 *                  atomiques, évitant les conditions de course
 * - Initialisation à 0 : État initial "pas de données reçues"
 * 
 * Cette variable permet une communication sûre entre le gestionnaire
 * de signaux (qui la modifie) et la boucle principale (qui la lit).
 */
static volatile sig_atomic_t	g_data = 0;

/**
 * @brief Gère le traitement final d'un caractère reconstruit
 * @param c Caractère complet reconstruit à partir des bits reçus
 * @param client_pid Pointeur vers le PID du client émetteur
 * @param i Pointeur vers le compteur de bits
 * 
 * Cette fonction représente la dernière étape du processus de réception,
 * intervenant une fois qu'un caractère complet a été reconstruit.
 * Elle implémente une logique sophistiquée de fin de transmission :
 * 
 * Pour un caractère normal :
 * - Affiche le caractère immédiatement
 * - Réinitialise le compteur de bits pour le prochain caractère
 * 
 * Pour le caractère nul (fin de message) :
 * - Marque la fin visuelle du message avec un saut de ligne
 * - Envoie un signal SIGUSR2 au client pour confirmer la réception complète
 * - Réinitialise le PID client pour accepter une nouvelle connexion
 * 
 * La fonction est conçue pour être :
 * - Réentrante : Sûre en cas d'appels concurrents
 * - Efficace : Opérations minimales
 * - Robuste : Gestion appropriée des états
 */
static void	ft_handle_char(unsigned char c, pid_t *client_pid, int *i)
{
	*i = 0;
	if (!c)
	{
		ft_putchar('\n');
		kill(*client_pid, SIGUSR2);
		*client_pid = 0;
		return ;
	}
	ft_putchar(c);
}

/**
 * @brief Gestionnaire principal des signaux du serveur
 * @param sig Signal reçu (SIGUSR1 ou SIGUSR2)
 * @param info Structure contenant les informations détaillées du signal
 * @param context Contexte d'exécution (non utilisé)
 * 
 * Cette fonction est le cœur du serveur Minitalk, implémentant un protocole
 * sophistiqué de reconstruction de caractères à partir de signaux binaires.
 * 
 * Architecture de la reconstruction :
 * 
 * 1. Gestion de l'état :
 *    - Variables statiques pour préserver l'état entre les appels
 *    - Détection automatique des nouveaux clients
 *    - Maintien du contexte de reconstruction
 * 
 * 2. Protocole de reconstruction :
 *    - Décalage à gauche du buffer (c << 1)
 *    - Ajout du nouveau bit si SIGUSR2 (c | 1)
 *    - Comptage précis des bits reçus
 *    - Traitement après 8 bits complets
 * 
 * 3. Mécanisme de confirmation :
 *    - Envoi d'acquittement pour chaque bit (SIGUSR1)
 *    - Activation du flag global pour la boucle principale
 * 
 * Aspects de sécurité :
 * - Protection contre les signaux non sollicités
 * - Maintien de la cohérence des données
 * - Gestion robuste des états
 * 
 * La fonction utilise des variables statiques pour maintenir l'état :
 * - i : Position du bit en cours (0-7)
 * - c : Caractère en cours de reconstruction
 * - client_pid : Identifiant du client actif
 */
static void	ft_receive(int sig, siginfo_t *info, void *context)
{
	static int				i = 0;
	static unsigned char	c = 0;
	static pid_t			client_pid = 0;

	(void)context;
	if (!client_pid)
		client_pid = info->si_pid;
	c = c << 1;
	if (sig == SIGUSR2)
		c = c | 1;
	if (++i == 8)
		ft_handle_char(c, &client_pid, &i);
	if (client_pid)
	{
		kill(client_pid, SIGUSR1);
		g_data = 1;
	}
}

/**
 * @brief Point d'entrée principal du serveur Minitalk
 * 
 * Cette fonction met en place et exécute le serveur de communication
 * basé sur les signaux UNIX. Elle implémente une architecture 
 * événementielle robuste.
 * 
 * Séquence d'initialisation :
 * 
 * 1. Configuration de base :
 *    - Récupération et affichage du PID
 *    - Le PID est crucial car les clients en ont besoin pour
 *      établir la communication
 * 
 * 2. Configuration des signaux :
 *    - Initialisation de la structure sigaction
 *    - Configuration du masque de signaux
 *    - Activation du mode SA_SIGINFO pour des informations détaillées
 *    - Installation des handlers pour SIGUSR1 et SIGUSR2
 * 
 * 3. Boucle principale :
 *    - Attente économe avec pause()
 *    - Surveillance du flag global
 *    - Réinitialisation du flag après traitement
 * 
 * Gestion d'erreur :
 * - Vérification de l'installation des handlers
 * - Message d'erreur explicite en cas d'échec
 * - Terminaison propre si l'initialisation échoue
 * 
 * La boucle principale utilise une approche économe en ressources :
 * - pause() suspend le processus jusqu'au prochain signal
 * - Le flag g_data coordonne le traitement des données
 * - Architecture non bloquante et réactive
 */
int	main(void)
{
	struct sigaction	sa;

	ft_putnbr(getpid());
	write(1, "\n", 1);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = ft_receive;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(2, "Error: sigaction failed\n", 23);
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
