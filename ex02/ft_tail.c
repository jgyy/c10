/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tail.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegoh <jegoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 00:07:22 by jegoh             #+#    #+#             */
/*   Updated: 2024/10/12 00:23:35 by jegoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#define BUFFER_SIZE 4096

void display_error(char *program_name, char *file_name, char *error_msg) {
    fprintf(stderr, "%s: %s: %s\n", basename(program_name), file_name, error_msg);
}

void tail_file(int fd, off_t n_bytes, int is_stdin) {
    char buffer[BUFFER_SIZE];
    off_t file_size = 0;
    off_t offset;

    if (n_bytes == 0)
        return;

    if (!is_stdin) {
        file_size = lseek(fd, 0, SEEK_END);
        if (file_size == -1) {
            perror("Error getting file size");
            return;
        }

        offset = (n_bytes >= file_size) ? 0 : file_size - n_bytes;

        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("Error setting file pointer");
            return;
        }
    } else {
        char *stdin_buffer = NULL;
        size_t stdin_size = 0;
        ssize_t stdin_read;
        
        while ((stdin_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
            char *temp = realloc(stdin_buffer, stdin_size + stdin_read);
            if (temp == NULL) {
                perror("Error allocating memory");
                free(stdin_buffer);
                return;
            }
            stdin_buffer = temp;
            memcpy(stdin_buffer + stdin_size, buffer, stdin_read);
            stdin_size += stdin_read;
        }

        if (stdin_read == -1) {
            perror("Error reading from stdin");
            free(stdin_buffer);
            return;
        }

        offset = (n_bytes >= (off_t)stdin_size) ? 0 : (off_t)stdin_size - n_bytes;

        write(STDOUT_FILENO, stdin_buffer + offset, stdin_size - offset);
        free(stdin_buffer);
        return;
    }

    off_t bytes_to_read = (n_bytes < file_size) ? n_bytes : file_size;
    off_t bytes_read = 0;
    while (bytes_read < bytes_to_read) {
        ssize_t read_result = read(fd, buffer, 
                                   (bytes_to_read - bytes_read < BUFFER_SIZE) ? 
                                   (size_t)(bytes_to_read - bytes_read) : BUFFER_SIZE);
        if (read_result <= 0) {
            if (read_result < 0) {
                perror("Error reading file");
            }
            break;
        }
        write(STDOUT_FILENO, buffer, read_result);
        bytes_read += read_result;
    }
}

int main(int argc, char *argv[]) {
    int opt;
    off_t n_bytes = 0;
    int c_option_provided = 0;

    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
            case 'c': {
                c_option_provided = 1;
                long long temp = atoll(optarg);
                if (temp < 0) {
                    fprintf(stderr, "%s: invalid number of bytes: '%s'\n", basename(argv[0]), optarg);
                    return 1;
                }
                n_bytes = (off_t)temp;
                break;
            }
            default:
                fprintf(stderr, "Usage: %s [-c num_bytes] [file ...]\n", basename(argv[0]));
                return 1;
        }
    }

    if (!c_option_provided && n_bytes == 0 && optind < argc) {
        n_bytes = 800;
    }

    if (optind == argc) {
        tail_file(STDIN_FILENO, n_bytes, 1);
    } else {
        for (int i = optind; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                display_error(argv[0], argv[i], strerror(errno));
                continue;
            }

            if (argc - optind > 1) {
                printf("==> %s <==\n", argv[i]);
            }

            tail_file(fd, n_bytes, 0);
            close(fd);

            if (i < argc - 1) {
                printf("\n");
            }
        }
    }

    return 0;
}
