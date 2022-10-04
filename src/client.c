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

/*
	This function does not do anything, is just to recive the ACK signal
	from the server that takes out the usleep() of send_byte
*/

void	clean_semaforo(int sig, siginfo_t *si, void *uap)
{
	if (sig == SIGUSR1)
		return ;
}

	/* 
		This funcion rotate 8 times the bits of byte, and send the correspondig 
		signal depending if the lowest bit is 1 o 0. Then set a uslepp like a 
		TimeOut of ACK signal of the server. The funcion will continue if a ACK 
		signal from the server is recived or the usleep is over
	*/

void	send_byte(char byte, int pid)
{
	int	i;
	int	kill_response;

	i = 0;
	ft_putchar_fd(byte, 1);
	while (i < 8)
	{
		if (byte & 0x80)
			kill (pid, SIGUSR2);
		else
			kill (pid, SIGUSR1);
		pause();
		if (kill_response < 0)
		{
			ft_putstr_fd("Signal error", 2);
			exit(-1);
		}			
		byte <<= 1;
		i++;
	//	usleep(10000);
	}
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
	while (*str)
		send_byte(*(str++), server_pid);
	if (!gnl)
		send_byte(*str, server_pid);
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
	return (bytes_send);
}

int	main(int argv, char **argc)
{
	int					server_pid;	
	struct sigaction	signal;
	int					bytes_send;

	signal.sa_sigaction = clean_semaforo;
	sigfillset(&signal.sa_mask);
	signal.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &signal, NULL);
	server_pid = ft_atoi(argc[1]);
	if (argv == 4 && argc[2][0] == '-' && argc[2][1] == 'g')
		bytes_send = send_gnl (argc[3], server_pid);
	else if (argv == 3)
		bytes_send = send_string (argc[2], server_pid, 0);
	else
	{
		ft_putstr_fd("Invalid arguments", 2);
		return (-1);
	}
	ft_printf("\n\n 📟 Sended %d bytes to PID [%d].\n\n", \
	bytes_send, server_pid);
	return (0);
}
