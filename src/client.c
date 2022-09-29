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

int	semaforo;

void clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	//semaforo = 0;
	//write(1, ".", 1);
}

int	send_byte(int byte, int pid)
{
	int i;

	char	temp[2];

	// temp[0] = byte;
	// temp[1] = '\0';
	// ft_putstr_fd("Caracter [", 1);
	// ft_putstr_fd(temp, 1);
	// ft_putstr_fd("]\n", 1);	

	i = 0;
	while (i < 8)
	{
		semaforo = 1;
		if (byte & 0x80)
			kill (pid, SIGUSR2);
		else
			kill (pid, SIGUSR1);
		byte <<= 1;
		i++;
		//while (semaforo)
		//pause();
		usleep(200);
	}
	return (0);
}


int main (int argv, char **argc)
{
	int	i;
	int server_pid;
    struct sigaction signal;	

	if (argv != 3)
		return (0);

    //signal.sa_sigaction = clean_semaforo;
	//sigfillset(&signal.sa_mask);
	//signal.sa_flags = SA_RESTART;
    //sigaction(SIGUSR1, &signal, NULL);

	server_pid = ft_atoi(argc[1]);
	while	(argc[2][i])
		send_byte(argc[2][i++], server_pid);
	send_byte(argc[2][i], server_pid);
    printf("\nSend [%s] to PID[%d]\n", argc[2], server_pid);
	return (0);
}

