#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "error.h"
#include "conversion.h"
#include "clientA.h"
#include "common.h"
#include "copy.h"



int main(int argc, char *argv[])
{
    struct options opts;
    options_init(&opts);
    parse_arguments(argc, argv, &opts);
    options_process(&opts);
    copy(opts.fd_in, opts.server_socket, 1024);
    cleanup(&opts);
    return EXIT_SUCCESS;
}


static void options_init(struct options *opts)
{
    memset(opts, 0, sizeof(struct options));
    opts->fd_in    = STDIN_FILENO;
    opts->fd_out   = STDOUT_FILENO;
    opts->port_out = DEFAULT_PORT;
}


static void parse_arguments(int argc, char *argv[], struct options *opts)
{
    int c;

    while((c = getopt(argc, argv, ":s:p:")) != -1)   // NOLINT(concurrency-mt-unsafe)
    {
        switch(c)
        {
            case 's':
            {
                opts->ip_out = optarg;
                break;
            }
            case 'p':
            {
                opts->port_out = parse_port(optarg,
                                            10); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                break;
            }
            case ':':
            {
                fatal_message(__FILE__, __func__, __LINE__, "\"Option requires an operand\"", 5); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                break;
            }
            case '?':
            {
                fatal_message(__FILE__, __func__, __LINE__, "Unknown",
                              6); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            }
            default:
            {
                assert("should not get here");
            };
        }
    }

    if(optind < argc) {
        int i = 0;
        if (strcmp(argv[optind + i], "*.txt") == 0) {
            get_file_list(opts);
        }
        else {
            while (argv[optind + i] != NULL){
                opts->file_arr[i] = malloc(sizeof(char) * 20);
                strcpy(opts->file_arr[i],  argv[optind + i]);
                i++;
            }
        }

        opts->file_count = i;
    }
}


static void options_process(struct options *opts) {
    ssize_t server_connection_test_fd;
    char message[28];
    message[27] = '\0';
    if(opts->file_name)
    {
        opts->fd_in = open(opts->file_name, O_RDONLY);

        if(opts->fd_in == -1)
        {
            fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
        }
    }


    if(opts->ip_out)
    {
        int result;
        struct sockaddr_in server_addr;

        opts->server_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(opts->server_socket == -1)
        {
            fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(opts->port_out);
        server_addr.sin_addr.s_addr = inet_addr(opts->ip_out);

        if(server_addr.sin_addr.s_addr ==  (in_addr_t)-1)
        {
            fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
        }

        result = connect(opts->server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

        if(result == -1)
        {
            fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
        }
        server_connection_test_fd = read(opts->server_socket, message, sizeof(message));
        if(server_connection_test_fd == -1) {
            printf("You are not connected to server\n");
        }
        printf("SERVER: %s \n", message);
    }
}


static void cleanup(const struct options *opts)
{
    if(opts->ip_out)
    {
        close(opts->fd_out);
    }
    for (int i = 0; i < opts->file_count; i++) {
        free(opts->file_arr[i]);
    }
}


void get_file_list(struct options *opts) {

    DIR *directory_buffer;
    struct dirent *files;
    directory_buffer = opendir ("./");
    int i = 0;
    if (directory_buffer != NULL) {
        while ((files = readdir(directory_buffer))!=NULL) {
            unsigned long length = strlen(files->d_name);
            if (strncmp(files->d_name + length - 4, ".txt", 4) == 0) {
                opts->file_arr[i] = files->d_name;
                i++;
            }
        }
        (void) closedir (directory_buffer);
    }
}
