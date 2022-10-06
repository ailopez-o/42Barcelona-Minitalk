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
#include <fcntl.h>
#include <time.h>

int	g_num_ack;

/*
	This function does not do anything, is just to recive the ACK signal
	from the server that takes out the usleep() of send_byte
*/

void	clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	(void)uap;
	(void)si;
	if (sig == SIGUSR1)
		g_num_ack++;
}

	/* 
		This funcion rotate 8 times the bits of byte, and send the correspondig 
		signal depending if the lowest bit is 1 o 0. Then set a uslepp like a 
		TimeOut of ACK signal of the server. The funcion will continue if a ACK 
		signal from the server is recived or the usleep is over
	*/

void	send_byte(char byte, int pid)
{
	int			i;
	int			kill_response;
	int			signal;
	static int	num_bytes;

	i = 0;
	while (i < 8)
	{
		if (byte & 0x80)
			signal = SIGUSR2;
		else
			signal = SIGUSR1;
		kill_response = kill(pid, signal);
		if (kill_response < 0)
		{
			ft_putstr_fd("Signal error", 2);
			exit(-1);
		}
		pause();
		byte <<= 1;
		i++;
	}
	num_bytes++;
	ft_printf("\r\e[1;34mSending [%d] bytes\e[0m", num_bytes);
}

	/* 
	This function calls send_byte() for each byte in the string str
	If gnl==1 will not send null caracter to avoid the server think is the 
	end of the transmision in each file og get_next_line
	*/

int	send_string(char *str, int server_pid, int gnl)
{
	int	size;

	size = ft_strlen (str);
	if (!gnl)
		g_num_ack = 0;
	while (*str)
		send_byte(*(str++), server_pid);
	if (!gnl)
	{
		send_byte(*str, server_pid);
		size++;
	}
	return (size);
}

/*
	This function extract the lines of a fd and send to
	send_string with gnl=1
*/

int	send_gnl(char *path, int server_pid)
{
	int		fd;
	char	*line;
	int		bytes_send;

	g_num_ack = 0;
	fd = open(path, O_RDONLY);
	if (fd < 1)
	{
		ft_putstr_fd("Invalid path", 2);
		exit(-1);
	}
	bytes_send = 0;
	line = get_next_line(fd);
	while (line)
	{
		bytes_send += send_string (line, server_pid, 1);
		free(line);
		line = get_next_line(fd);
	}
	send_byte(0, server_pid);
	bytes_send++;
	return (bytes_send);
}

int	main(int argv, char **argc)
{
	int					server_pid;	
	struct sigaction	signal;
	int					bytes_send;

	if (argv == 3 || argv == 4)
	{
		signal.sa_sigaction = clean_semaforo;
		sigfillset(&signal.sa_mask);
		signal.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &signal, NULL);
		server_pid = ft_atoi(argc[1]);
		if (argv == 4 && argc[2][0] == '-' && argc[2][1] == 'g')
			bytes_send = send_gnl (argc[3], server_pid);
		else
			bytes_send = send_string (argc[2], server_pid, 0);
		ft_printf("\n\n 📟 Sended %d bytes to PID [%d].\n", \
		bytes_send, server_pid);
		ft_printf("\n 🔰 Recived %d bytes ACK from PID [%d].\n\n", \
		g_num_ack / 8, server_pid);
	}
	else
		ft_putstr_fd("Invalid arguments", 2);
	return (0);
}
