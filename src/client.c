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

int server_pid;
int	signal_timeouts;

void clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	if (sig == SIGUSR1)
		return;
}

int	send_byte(char byte, int pid)
{
	int i;

	//ft_putchar_fd(byte, 1);
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
	float errors;

	errors = signal_timeouts * 100;
	errors = errors / (ft_strlen(str) * 8);

	//ft_printf("\n\n Sended %d bytes to PID [%s]\n\n", 27654, "Hola");


	printf("\n\n Sended %d bytes to PID [%d] in %d ms with %f%% timeouts \n\n", ft_strlen(str), pid, ms, errors);
}


int main (int argv, char **argc)
{
	int	i;
	clock_t	t;
	double time;
    struct sigaction signal;	

	if (argv != 3)
		return (0);
    signal.sa_sigaction = clean_semaforo;
	sigfillset(&signal.sa_mask);
	signal.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &signal, NULL);
	server_pid = ft_atoi(argc[1]);
	t = clock();
	i = 0;
	signal_timeouts = 0;
	while	(argc[2][i])
		send_byte(argc[2][i++], server_pid);
	send_byte(argc[2][i], server_pid);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;	
	print_resume (argc[2], server_pid, time * 1000);
	return (0);
}

