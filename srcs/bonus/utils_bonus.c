/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:43:51 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/26 00:31:41 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

/**
 * @brief Écrit un caractère unique sur la sortie standard
 * @param c Le caractère à écrire
 * 
 * Cette fonction constitue la base de notre système d'affichage. Elle utilise
 * l'appel système write() pour écrire un seul caractère sur la sortie standard
 * (file descriptor 1). 
 * 
 * Caractéristiques importantes :
 * - Utilise write() plutôt que putchar() pour une meilleure portabilité
 * - Écrit exactement 1 octet à chaque appel
 * - Ne vérifie pas les erreurs pour rester simple et rapide
 * 
 * Exemple d'utilisation :
 * ft_putchar_bonus('A'); // Affiche le caractère 'A'
 * 
 * @note Cette fonction est utilisée comme brique de base par les autres
 *       fonctions d'affichage de la bibliothèque.
 */
void	ft_putchar_bonus(char c)
{
	write(1, &c, 1);
}

/**
 * @brief Affiche une chaîne de caractères sur la sortie standard
 * @param str Pointeur vers la chaîne à afficher
 * 
 * Cette fonction parcourt une chaîne de caractères et affiche chaque
 * caractère individuellement en utilisant ft_putchar_bonus(). Elle s'arrête
 * automatiquement lorsqu'elle rencontre le caractère nul terminal ('\0').
 * 
 * Fonctionnement détaillé :
 * 1. Vérifie si la chaîne n'est pas NULL (implicitement)
 * 2. Parcourt la chaîne jusqu'au caractère nul
 * 3. Affiche chaque caractère avec ft_putchar_bonus
 * 4. Utilise l'opérateur post-incrémentation pour une syntaxe concise
 * 
 * Points d'attention :
 * - La chaîne doit être correctement terminée par '\0'
 * - Aucune vérification de NULL n'est effectuée
 * - L'utilisation de post-incrémentation permet d'avoir un code compact
 * 
 * Exemple d'utilisation :
 * ft_putstr_bonus("Hello"); // Affiche "Hello"
 */
void	ft_putstr_bonus(const char *str)
{
	while (*str)
		ft_putchar_bonus(*str++);
}

/**
 * @brief Convertit et affiche un nombre entier sur la sortie standard
 * @param n Le nombre entier à afficher
 * 
 * Cette fonction réalise une conversion complexe d'un entier en caractères
 * pour l'affichage. Elle gère tous les cas spéciaux et utilise une approche
 * récursive élégante pour décomposer le nombre.
 * 
 * Algorithme détaillé :
 * 1. Conversion en long pour gérer INT_MIN sans débordement
 * 2. Gestion du signe négatif :
 *    - Affiche le '-'
 *    - Convertit le nombre en positif
 * 3. Décomposition récursive pour les nombres > 9 :
 *    - Divise par 10 pour obtenir les chiffres de gauche
 *    - Utilise modulo 10 pour obtenir le chiffre de droite
 * 4. Conversion finale en caractère ASCII (+ '0')
 * 
 * Caractéristiques importantes :
 * - Gère correctement les nombres négatifs
 * - Peut afficher INT_MIN sans problème grâce au type long
 * - Approche récursive pour une décomposition naturelle
 * - Pas de buffer temporaire nécessaire
 * 
 * Exemples d'utilisation :
 * ft_putnbr_bonus(42);    // Affiche "42"
 * ft_putnbr_bonus(-123);  // Affiche "-123"
 * ft_putnbr_bonus(0);     // Affiche "0"
 * ft_putnbr_bonus(INT_MIN); // Affiche "-2147483648"
 * 
 * @note La récursion s'arrête naturellement quand le nombre
 *       devient inférieur à 10
 */
void	ft_putnbr_bonus(int n)
{
	char	c;
	long	nb;

	nb = n;
	if (nb < 0)
	{
		ft_putchar_bonus('-');
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr_bonus(nb / 10);
		ft_putnbr_bonus(nb % 10);
	}
	else
	{
		c = nb + '0';
		ft_putchar_bonus(c);
	}
}
