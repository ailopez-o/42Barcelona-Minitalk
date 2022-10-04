/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ailopez- <ailopez-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 18:21:52 by ailopez-          #+#    #+#             */
/*   Updated: 2022/09/28 18:21:53 by ailopez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/mylib/inc/ft_printf.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

typedef struct s_client{
	char	byte;
	char	num_bit;
	int		pid;
	int		bytes_recived;
}	t_client;

	/* 
	Sigaction is a struct describing a signal handler. It contains:
		- A signal handler function
		- A signal mask which specifies which signals should be blocked
		 while the signal handler function is running. If the signal
		 handler returns normally, the original signal mask will be
		 restored. To understand signal masks, see sigprocmask.c
		- A set of flags.
	
	Specify the signal handler function to be called when one of
	the specified signals occur.

		>> signal.sa_sigaction = signal_recived;

	Add all of the signals to the signal mask set. This means that
	all signals will be blocked (i.e., delayed) while our signal
	handler function is running. The original signal mask will be
	restored when our signal handler exits normally.
	
		>> sigfillset(&signal.sa_mask);

 	If the signal handler gets run in the middle of some function
	calls (such as open(), read(), or write()) and the signal
	handler returns normally, there are couple of options: return a
	failure code EINTR from those functions, or try to resume the
	function. With SA_RESTART, the function is resumed instead of
	returning an error. 
	
		>> signal.sa_flags = SA_RESTART;

	*/

void	print_msj(int bytes_recived, int pid, double t_start)
{
	double	dbl_time;
	int		time;

	t_start = clock() - t_start;
	dbl_time = ((double)t_start) / CLOCKS_PER_SEC;
	time = (int)(dbl_time * 1000);
	ft_printf("\n\n 📟 Message finished from PID[%d]\n", pid);
	ft_printf(" 🔸 %d bytes recived in %d ms.\n\n", bytes_recived, time);
}

void	reset_client(t_client *client, int pid, clock_t *t_start)
{
	ft_printf("\n\n 📥 Starting new message from PID [%d]\n\n", pid);
	client->pid = pid;
	client->num_bit = 0;
	client->bytes_recived = 0;
	*t_start = clock();
}

/*
	This function checks the signal recived and push into the byte the
	new bit recived. Once the byte is recived is printed in consule
*/

void	signal_recived(int sig, siginfo_t *si, void *uap)
{
	static t_client	client;
	static clock_t	t_start;
	double			time;

	if (client.pid != si->si_pid && si->si_pid != 0)
		reset_client (&client, si->si_pid, &t_start);
	client.num_bit++;
	client.byte |= (sig == SIGUSR2);
	if (client.num_bit == 8)
	{
		ft_putchar_fd(client.byte, 1);
		if (client.byte == 0)
			print_msj(client.bytes_recived, client.pid, t_start);
		client.bytes_recived++;
		client.byte = 0;
		client.num_bit = 0;
	}
	else
		client.byte <<= 1;
	usleep(40);
	kill (client.pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	signal;

	signal.sa_sigaction = signal_recived;
	sigfillset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	sigaction(SIGUSR2, &signal, NULL);
	sigaction(SIGUSR1, &signal, NULL);
	ft_printf("\nServer PID [%d]\n\n", getpid());
	while (1)
	{
		pause();
	}
}
