/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ailopez- <ailopez-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 18:21:42 by ailopez-          #+#    #+#             */
/*   Updated: 2022/09/28 18:21:44 by ailopez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/libft/libft.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int	send_byte(int byte, int pid)
{
	int i;
/*
	char	temp[2];

	temp[0] = byte;
	temp[1] = '\0';
	ft_putstr_fd("Caracter [", 1);
	ft_putstr_fd(temp, 1);
	ft_putstr_fd("]\n", 1);	
*/
	i = 0;
	while (i < 8)
	{
		if (byte & 0x80)
		{
			kill (pid, SIGUSR2);
			//ft_putstr_fd("bit1\n", 1);
		}
		else
		{
			kill (pid, SIGUSR1);
			//ft_putstr_fd("bit0\n", 1);
		}
		byte <<= 1;
		i++;
		usleep(500);
	}
	return (0);
}


int main (int argv, char **argc)
{
	int	i;
	int server_pid;

	if (argv != 3)
		return (0);

	server_pid = ft_atoi(argc[1]);

	while	(argc[2][i])
		send_byte(argc[2][i++], server_pid);
	send_byte(argc[2][i], server_pid);
    printf("\nSend [%s] to PID[%d]\n", argc[2], server_pid);
	return (0);
}

