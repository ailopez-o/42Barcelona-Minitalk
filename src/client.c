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
#include "../lib/mylib/inc/ft_printf.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

void	clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	if (sig == SIGUSR1)
		return ;
}

int	send_byte(char byte, int pid)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if (byte & 0x80)
			kill (pid, SIGUSR2);
		else
			kill (pid, SIGUSR1);
		byte <<= 1;
		i++;
		usleep(10000);
	}
	return (0);
}

void	print_resume(char *str, int pid, int ms)
{
	ft_printf("\n\n 📟 Sended %d bytes to PID [%d] in %d ms.\n\n", \
	ft_strlen(str), pid, ms);
}

int	main(int argv, char **argc)
{
	int					i;
	clock_t				t;
	double				time;
	int					server_pid;	
	struct sigaction	signal;	

	if (argv != 3)
		return (0);
	signal.sa_sigaction = clean_semaforo;
	sigfillset(&signal.sa_mask);
	signal.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &signal, NULL);
	server_pid = ft_atoi(argc[1]);
	t = clock();
	i = 0;
	while (argc[2][i])
		send_byte(argc[2][i++], server_pid);
	send_byte(argc[2][i], server_pid);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;
	print_resume (argc[2], server_pid, time * 1000);
	return (0);
}
