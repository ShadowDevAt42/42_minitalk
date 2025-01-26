/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:45:20 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:33:53 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

/**
 * @brief Affiche un rapport détaillé des statistiques de communication
 * @param stats Pointeur vers la structure contenant les statistiques
 * 
 * Cette fonction crée un rapport visuel élaboré des
 * statistiques de communication
 * en utilisant une mise en forme colorée et des symboles pour améliorer la
 * lisibilité. Le rapport présente trois métriques principales :
 * 
 * 1. PID du client émetteur
 * 2. Nombre total de caractères reçus
 * 3. Nombre total de bits traités
 * 
 * Format de présentation :
 * - Titre en bleu avec séparateurs
 * - Chaque métrique précédée d'une coche verte (CHECK_MARK)
 * - Retour à la couleur par défaut après chaque élément coloré
 * - Espacement vertical pour une meilleure lisibilité
 * 
 * Exemple de sortie :
 * === Statistiques de Réception ===
 * ✓ Message reçu du client PID: 12345
 * ✓ Caractères reçus : 42
 * ✓ Bits reçus : 336
 * 
 * @note Cette fonction utilise les constantes de couleur définies
 *       (COLOR_BLUE, COLOR_GREEN, COLOR_RESET) pour le formatage
 */
void	ft_print_stats(t_stats *stats)
{
	ft_putstr_bonus(COLOR_BLUE);
	ft_putstr_bonus("\n=== Statistiques de Réception ===\n");
	ft_putstr_bonus(COLOR_GREEN);
	ft_putstr_bonus(CHECK_MARK);
	ft_putstr_bonus(COLOR_RESET);
	ft_putstr_bonus(" Message reçu du client PID: ");
	ft_putnbr_bonus(stats->client_pid);
	ft_putchar_bonus('\n');
	ft_putstr_bonus(COLOR_GREEN);
	ft_putstr_bonus(CHECK_MARK);
	ft_putstr_bonus(COLOR_RESET);
	ft_putstr_bonus(" Caractères reçus : ");
	ft_putnbr_bonus(stats->chars_received);
	ft_putchar_bonus('\n');
	ft_putstr_bonus(COLOR_GREEN);
	ft_putstr_bonus(CHECK_MARK);
	ft_putstr_bonus(COLOR_RESET);
	ft_putstr_bonus(" Bits reçus : ");
	ft_putnbr_bonus(stats->bits_received);
	ft_putstr_bonus("\n\n");
	ft_putstr_bonus(COLOR_RESET);
}

/**
 * @brief Affiche un message avec une couleur spécifique
 * @param msg   Message à afficher
 * @param color Code de couleur ANSI à utiliser
 * 
 * Cette fonction encapsule la logique d'affichage coloré pour simplifier
 * l'interface utilisateur. Elle :
 * 
 * 1. Active la couleur spécifiée
 * 2. Affiche le message
 * 3. Réinitialise la couleur
 * 4. Ajoute un saut de ligne
 * 
 * Avantages de cette approche :
 * - Encapsulation de la logique de coloration
 * - Garantie de réinitialisation de la couleur
 * - Simplification du code appelant
 * - Cohérence dans l'affichage
 * 
 * Exemple d'utilisation :
 * ft_print_colored("Erreur!", COLOR_RED);
 * ft_print_colored("Succès", COLOR_GREEN);
 */
void	ft_print_colored(const char *msg, const char *color)
{
	ft_putstr_bonus(color);
	ft_putstr_bonus(msg);
	ft_putstr_bonus(COLOR_RESET);
	ft_putchar_bonus('\n');
}

/**
 * @brief Convertit une chaîne de caractères en entier
 * @param str Chaîne à convertir
 * @return   Valeur entière correspondante
 * 
 * Cette fonction implémente une version personnalisée de atoi() avec
 * une gestion robuste des différents cas. Elle suit un algorithme
 * en plusieurs étapes :
 * 
 * 1. Gestion des espaces blancs initiaux :
 *    - Espaces classiques ' '
 *    - Caractères de contrôle (\t, \n, \v, \f, \r)
 * 
 * 2. Traitement du signe :
 *    - '+' : conserve le signe positif (par défaut)
 *    - '-' : inverse le signe
 * 
 * 3. Conversion des chiffres :
 *    - Accumule les valeurs numériques
 *    - Multiplie le résultat par 10 à chaque itération
 *    - Ajoute le nouveau chiffre (après soustraction du code ASCII '0')
 * 
 * Caractéristiques importantes :
 * - Ignore les espaces blancs initiaux
 * - Gère correctement les signes + et -
 * - S'arrête au premier caractère non numérique
 * - Ne gère pas explicitement le dépassement de capacité
 * 
 * Exemples de comportement :
 * "  42"    -> 42
 * "-123"    -> -123
 * "+999"    -> 999
 * "abc"     -> 0
 * "  +42abc" -> 42
 * 
 * @note Cette implémentation diffère légèrement de l'atoi standard
 *       car elle ne gère pas tous les cas de dépassement
 */
int	ft_atoi_bonus(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}
