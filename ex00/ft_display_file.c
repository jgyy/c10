/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:38:51 by jegoh             #+#    #+#             */
/*   Updated: 2024/10/11 23:39:59 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

void ft_putstr(char *str)
{
    while (*str)
        write(1, str++, 1);
}

int main(int argc, char **argv)
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    if (argc < 2)
    {
        ft_putstr("File name missing.\n");
        return (1);
    }
    if (argc > 2)
    {
        ft_putstr("Too many arguments.\n");
        return (1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        ft_putstr("Cannot read file.\n");
        return (1);
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        write(1, buffer, bytes_read);
    }

    if (bytes_read == -1)
    {
        ft_putstr("Cannot read file.\n");
        close(fd);
        return (1);
    }

    close(fd);
    return (0);
}
