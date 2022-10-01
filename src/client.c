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
#include "../lib/printf/inc/ft_printf.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

int	semaforo;
int server_pid;
int	num_ack;

void clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	if (sig == SIGUSR1)
	{
		semaforo = 0;
		num_ack ++;
		//write(1, ".", 1);
	}
}

int	send_byte(char byte, int pid)
{
	int i;
	int timeout;
	char	temp[2];

	// temp[0] = byte;
	// temp[1] = '\0';
	// ft_putstr_fd("Caracter [", 1);
	// ft_putstr_fd(temp, 1);
	// ft_putstr_fd("]\n", 1);	
	ft_putchar_fd(byte, 1);
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
		timeout = 2000;
		while (semaforo || timeout == 0)
		{
			timeout--;
			//pause();
		}
		//usleep(400);
	}

	return (0);
}

void	print_resume(char *str, int pid, int ms)
{
	ft_putstr_fd("\n\n 📟 Sended ", 1);
	ft_putstr_fd(ft_itoa(ft_strlen(str)), 1);
	ft_putstr_fd(" bytes to PID [" , 1);
	ft_putstr_fd(ft_itoa(server_pid), 1);
	ft_putstr_fd("] in " , 1);
	ft_putstr_fd(ft_itoa(ms), 1);
	ft_putstr_fd("ms \n\n" , 1);	
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
	while	(argc[2][i])
		send_byte(argc[2][i++], server_pid);
	send_byte(argc[2][i], server_pid);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;	
	print_resume (argc[2], server_pid, 0);
	return (0);
}

