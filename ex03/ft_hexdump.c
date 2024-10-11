/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 00:26:54 by jegoh             #+#    #+#             */
/*   Updated: 2024/10/12 00:36:00 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#define BUFFER_SIZE 16

void print_hex(unsigned char *buffer, size_t size, size_t offset) {
    printf("%08lx  ", offset);

    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        if (i < size)
            printf("%02x ", buffer[i]);
        else
            printf("   ");
        if (i == 7)
            printf(" ");
    }

    printf("|");
    for (size_t i = 0; i < size; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126)
            printf("%c", buffer[i]);
        else
            printf(".");
    }
    printf("|");
    printf("\n");
}

void hexdump(int fd) {
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t offset = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        print_hex(buffer, bytes_read, offset);
        offset += bytes_read;
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        exit(1);
    }

    if (offset > 0) {
        printf("%08lx\n", offset);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        hexdump(STDIN_FILENO);
        return 0;
    }

    int first_file = 1;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "%s: %s: %s\n", basename(argv[0]), argv[i], strerror(errno));
            continue;
        }

        if (argc > 2) {
            if (!first_file) {
                printf("\n");
            }
            printf("\n%s:\n", argv[i]);
            first_file = 0;
        }

        hexdump(fd);
        close(fd);
    }

    return 0;
}
