/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-tria <fdi-tria@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 19:21:40 by fdi-tria          #+#    #+#             */
/*   Updated: 2025/01/25 19:21:41 by fdi-tria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>

// Utils
void	ft_putchar(unsigned char c);
void	ft_putnbr(int n);
int		ft_atoi(const char *str);

// Client
void	ft_send_char(pid_t pid, unsigned char c);

#endif
