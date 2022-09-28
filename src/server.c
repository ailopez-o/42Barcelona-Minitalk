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
#include "../lib/libft/libft.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define MAX_NUM_CLIENTS 50

typedef struct s_client{
	char	*buffer;
	char	byte;
	char	num_bit;
	int		pid;
}	t_client;

t_client	clients[MAX_NUM_CLIENTS];

int	new_client(int pid)
{
	int	i;

	i = 0;
	while (i < MAX_NUM_CLIENTS)
	{
		if (clients[i].pid == 0)
		{
			clients[i].pid = pid;
			return (i);
		}
		i++;
	}
	return (-1);
}

int	get_client(int pid)
{
	int	i;

	i = 0;
	while (i < MAX_NUM_CLIENTS)
	{
		if (clients[i].pid == pid)
			return (i);
		i++;
	}
	return (new_client(pid));
}

void	init_clients(void)
{
	int	i;

	i = 0;
	while (i < MAX_NUM_CLIENTS)
	{
		clients[i].pid = 0;
		clients[i].num_bit = 0;
		clients[i].buffer = ft_strdup("");
		i++;
	}
}

void	free_clients(void)
{
	int	i;

	i = 0;
	while (i < MAX_NUM_CLIENTS)
	{
		free(clients[i].buffer);
		i++;
	}
}

void signal_recived(int sig, siginfo_t *si, void *uap)
{
	int		id_client;
	char	temp[2];
	char *new_buffer;
	
	id_client = get_client(si->si_pid);	
	clients[id_client].num_bit++;
	clients[id_client].byte |= (sig == SIGUSR2);
	if (clients[id_client].num_bit == 8)
	{
		temp[0] = clients[id_client].byte;
		temp[1] = '\0';
		new_buffer = ft_strjoin(clients[id_client].buffer, temp);
		free (clients[id_client].buffer);
		clients[id_client].buffer = new_buffer;
		if(clients[id_client].byte == '\0')
			ft_putstr_fd(clients[id_client].buffer, 1);
		clients[id_client].byte = 0;
		clients[id_client].num_bit = 0;
	}
	else
		clients[id_client].byte <<= 1;
		
    printf ("signal ");
    if ( si->si_signo == SIGUSR2)
         printf ("[SIGUSR2]");
    if ( si->si_signo == SIGUSR1)
         printf ("[SIGUSR1]");
     printf (" from PID: %d\n", si->si_pid);     
  //printf("signal %d received with siginfo_t:\n", sig);
  //printf("\tsignal number: si_signo=%d\n", si->si_signo);
  //printf("\terror number: si_errno=%d\n", si->si_errno);
  //printf("\tsignal code: si_code=%d\n", si->si_code);
  //printf("\tsignal value union: si_value, sival_int=%d\n", si->si_value.sival_int);
  //printf("\tsignal value union: si_value, sival_ptr=%p\n", si->si_value.sival_ptr);
  //printf("\tsending process ID: si_pid=%d\n", si->si_pid);
  //printf("\tsending process user's ID: si_uid=%d\n", si->si_uid);
  //printf("\tfaulting instruction at: si_addr=%p\n", si->si_addr);
  //printf("\texit value or signal: si_status=%d\n", si->si_status);
  //printf("\tband event for SIGPOLL: si_band=%ld\n", si->si_band);
  printf("\n");

}

int main (void)
{
    /* sigaction is a struct describing a signal handler. It contains:
	   - A signal handler function
	   
	   - A signal mask which specifies which signals should be blocked
         while the signal handler function is running. If the signal
         handler returns normally, the original signal mask will be
         restored. To understand signal masks, see sigprocmask.c
	   
	   - A set of flags.
	*/   
    struct sigaction signal;
	/* Specify the signal handler function to be called when one of
	 * the specified signals occur. */
    signal.sa_sigaction = signal_recived;
	/* Add all of the signals to the signal mask set. This means that
	   all signals will be blocked (i.e., delayed) while our signal
	   handler function is running. The original signal mask will be
	   restored when our signal handler exits normally. */
	sigfillset(&signal.sa_mask);
 	/* If the signal handler gets run in the middle of some function
	 * calls (such as open(), read(), or write()) and the signal
	 * handler returns normally, there are couple of options: return a
	 * failure code EINTR from those functions, or try to resume the
	 * function. With SA_RESTART, the function is resumed instead of
	 * returning an error. */
	signal.sa_flags = SA_RESTART;

	init_clients();

    /* Add our signals handler */
    sigaction(SIGUSR2, &signal, NULL);
    sigaction(SIGUSR1, &signal, NULL);

	clients[0].byte |= (1);
	clients[0].byte <<= 1;	
	clients[0].byte |= (0);
	clients[0].byte <<= 1;
	clients[0].byte |= (1);
	clients[0].byte <<= 1;				

    printf("\nServer PID [%d]\n\n", getpid());
    while(1);
}