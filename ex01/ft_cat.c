/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:45:02 by jegoh             #+#    #+#             */
/*   Updated: 2024/10/11 23:55:28 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>

#define BUFFER_SIZE 4096

void display_error(char *filename) {
    char *base = basename(filename);
    write(2, "ft_cat: ", 8);
    write(2, base, strlen(base));
    write(2, ": ", 2);
    write(2, strerror(errno), strlen(strerror(errno)));
    write(2, "\n", 1);
}

int cat_file(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int last_char = '\n';

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (write(1, &buffer[i], 1) != 1) {
                return -1;
            }
            last_char = buffer[i];
        }
    }

    if (last_char != '\n') {
        if (write(1, "\n", 1) != 1) {
            return -1;
        }
    }

    return (bytes_read < 0) ? -1 : 0;
}

int main(int argc, char **argv) {
    int fd, error_occurred = 0;

    if (argc == 1) {
        error_occurred = (cat_file(0) == -1);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == '\0') {
                error_occurred |= (cat_file(0) == -1);
            } else {
                fd = open(argv[i], O_RDONLY);
                if (fd < 0) {
                    display_error(argv[i]);
                    error_occurred = 1;
                } else {
                    error_occurred |= (cat_file(fd) == -1);
                    close(fd);
                }
            }
        }
    }
    return error_occurred ? 1 : 0;
}
