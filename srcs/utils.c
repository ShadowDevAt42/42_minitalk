/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 19:21:46 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:45:24 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/**
 * @brief Détermine si un caractère est un espace selon la norme C
 * @param c Le caractère à analyser
 * @return 1 si c'est un espace, 0 sinon
 * 
 * Cette fonction implémente une vérification complète des caractères
 * d'espacement selon la norme C. La notion d'espace en C est plus large
 * que le simple caractère espace et inclut tous les caractères de
 * formatage horizontal et vertical.
 * 
 * Caractères considérés comme espaces :
 * - Espace standard (ASCII 32) : Séparateur visible
 * - Tabulation horizontale '\t' : Alignement et indentation
 * - Nouvelle ligne '\n' : Séparation de lignes
 * - Tabulation verticale '\v' : Rarement utilisé en pratique
 * - Saut de page '\f' : Hérité des imprimantes
 * - Retour chariot '\r' : Utilisé dans certains systèmes
 * 
 * L'approche avec || (OU logique) permet une évaluation efficace
 * car elle s'arrête dès qu'une condition est vraie.
 * 
 * Cette fonction est particulièrement utile dans ft_atoi pour
 * normaliser l'entrée en ignorant les espaces initiaux, suivant
 * ainsi le comportement standard de atoi().
 */
static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

/**
 * @brief Convertit une chaîne de caractères en entier avec gestion complète
 * @param str La chaîne à convertir
 * @return La valeur entière correspondante
 * 
 * Cette fonction reproduit le comportement de atoi() standard avec
 * une approche méthodique de la conversion. Elle suit un algorithme
 * en plusieurs étapes pour assurer une conversion correcte :
 * 
 * Phase 1 : Préparation
 * - Ignore tous les caractères d'espacement initiaux
 * - Utilise ft_isspace pour une définition cohérente des espaces
 * 
 * Phase 2 : Détection du signe
 * - Reconnaît les symboles + et -
 * - Le signe - inverse la valeur finale
 * - Plusieurs signes sont traités (seul le premier compte)
 * 
 * Phase 3 : Conversion numérique
 * - Parcourt les chiffres séquentiellement
 * - Calcule la valeur en base 10 de manière itérative
 * - S'arrête au premier caractère non numérique
 * 
 * Exemple de fonctionnement :
 * "  -123abc" → ignore espaces → détecte - → convertit 123 → retourne -123
 * 
 * Limitations connues :
 * - Pas de détection de dépassement
 * - Ne gère pas les bases autres que décimale
 * - Ne signale pas les erreurs de format
 */
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
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

/**
 * @brief Écrit un caractère sur la sortie standard de manière sûre
 * @param c Le caractère à écrire (transmis comme unsigned char)
 * 
 * Cette fonction fondamentale utilise l'appel système write() plutôt
 * que putchar() pour plusieurs raisons importantes :
 * 
 * 1. Contrôle direct :
 *    - Pas d'intermédiaire de buffering
 *    - Écriture immédiate garantie
 *    - Comportement prévisible
 * 
 * 2. Sécurité :
 *    - Utilise unsigned char pour éviter les problèmes de signe
 *    - Écrit exactement un octet
 *    - Pas de conversion de format
 * 
 * 3. Performance :
 *    - Appel système direct
 *    - Pas de surcharge liée au buffering
 * 
 * Le descripteur 1 correspond à stdout (sortie standard).
 * Cette fonction est utilisée comme base pour toutes les
 * autres fonctions d'affichage.
 */
void	ft_putchar(unsigned char c)
{
	write(1, &c, 1);
}

/**
 * @brief Gère la conversion des nombres négatifs pour l'affichage
 * @param n Pointeur vers le nombre à traiter
 * @param is_negative Pointeur vers l'indicateur de négativité
 * 
 * Cette fonction utilitaire encapsule la logique complexe de
 * gestion des nombres négatifs pour ft_putnbr. Elle utilise
 * des pointeurs pour modifier les valeurs d'origine :
 * 
 * 1. Détection de la négativité :
 *    - Vérifie si le nombre est négatif
 *    - Active le drapeau correspondant
 * 
 * 2. Traitement :
 *    - Affiche le signe moins
 *    - Convertit en positif pour le traitement
 * 
 * 3. Marquage :
 *    - Enregistre l'état de négativité
 *    - Permet un traitement spécial ultérieur
 * 
 * Cette approche modulaire simplifie la gestion des cas
 * négatifs dans la fonction principale d'affichage.
 */
static void	ft_handle_negative(long *n, int *is_negative)
{
	if (*n < 0)
	{
		ft_putchar('-');
		*n *= -1;
		*is_negative = 1;
	}
}

/**
 * @brief Convertit et affiche un nombre entier avec gestion complète des cas
 * @param n Le nombre à afficher
 * 
 * Cette fonction implémente un système complet d'affichage de nombres
 * entiers avec une gestion sophistiquée des cas particuliers :
 * 
 * 1. Architecture générale :
 *    - Conversion type pour gérer INT_MIN
 *    - Approche récursive pour décomposition
 *    - Gestion séparée de la négativité
 * 
 * 2. Cas spéciaux :
 *    - Traitement spécifique de -2147483648
 *    - Gestion du zéro
 *    - Support des nombres négatifs
 * 
 * 3. Algorithme de conversion :
 *    - Décomposition récursive décimale
 *    - Conversion caractère par caractère
 *    - Maintien de l'ordre correct
 * 
 * La récursion permet une décomposition élégante :
 * 123 → (12, '3') → (1, '2') → ('1')
 * 
 * L'utilisation de long permet de gérer INT_MIN sans
 * problème de dépassement lors de la conversion.
 */
void	ft_putnbr(int n)
{
	char	c;
	int		is_negative;
	long	nb;

	is_negative = 0;
	nb = n;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	ft_handle_negative(&nb, &is_negative);
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putnbr(nb % 10);
	}
	else
	{
		c = nb + '0';
		ft_putchar(c);
	}
}
