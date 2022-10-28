#include "conversion.h"
#include "copy.h"
#include "error.h"
#include "server.h"
#include "common.h"
#include <sys/time.h>
#include <net/if.h>


#define BUF_SIZE 1024
#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct options_server opts;

    options_init_server(&opts);
    parse_arguments_server(argc, argv, &opts);
    options_process_server(&opts);
    cleanup_server(&opts);
    return EXIT_SUCCESS;
}


static void options_init_server(struct options_server *opts) {
    memset(opts, 0, sizeof(struct options_server));
    opts->port_in = DEFAULT_PORT;
    opts->fd_out = STDOUT_FILENO;
}


static void parse_arguments_server(int argc, char *argv[], struct options_server *opts)
{
    int c;

    while((c = getopt(argc, argv, ":p:")) != -1)   // NOLINT(concurrency-mt-unsafe)
    {
        switch(c)
        {
            case 'p':
            {
                opts->port_in = parse_port(optarg, 10); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                break;
            }
            case ':':
            {
                fatal_message(__FILE__, __func__ , __LINE__, "\"Option requires an operand\"", 5); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                break;
            }
            case '?':
            {
                fatal_message(__FILE__, __func__ , __LINE__, "Unknown", 6); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
            }
            default:
            {
                assert("should not get here");
            };
        }
    }
}


static void options_process_server(struct options_server *opts)
{
    char message[27] = "You are connected to server";
    message[27] = '\0';


    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    int option = TRUE;
    fd_set readfds;
    int max_sd, sd, activity, new_socket, valread;
    char buffer[1024];
    ssize_t received_bytes = 0;

    for (int i = 0; i < BACKLOG; i++) {
        opts->client_socket[i] = 0;
    }


    opts->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(opts->server_socket == -1) {
        printf( "socket() ERROR\n");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(opts->port_in);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(server_address.sin_addr.s_addr ==  (in_addr_t) -1) {
        fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
    }

    option = 1;
    setsockopt(opts->server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));


    if (bind(opts->server_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) == -1) {
        printf("bind() ERROR\n");
        exit(1);
    }


    if (listen(opts->server_socket, BACKLOG) == -1) {
        printf("listen() ERROR\n");
        exit(1);
    }

    client_address_size = sizeof(client_address);

    while (TRUE) {
        FD_ZERO((&readfds));
        FD_SET(opts->server_socket, &readfds);
        max_sd = opts->server_socket;

        for (int i = 0; i < BACKLOG; i++) {
            sd = opts->client_socket[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) printf("select error\n");

        if (FD_ISSET(opts->server_socket, &readfds)) {
            if ((new_socket = accept(opts->server_socket, (struct sockaddr *) &client_address,
                                     &client_address_size)) < 0) {
                perror("accept: ");
                exit(EXIT_FAILURE);
            }

            printf("====== [New Client Connect] ======\n"
                   "Socket fd : %d\n"
                   "ip : %s\n"
                   "port : %d\n", new_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            if (send(new_socket, message, strlen(message), 0) != strlen(message)) perror("send : ");

            for (int i = 0; i < 5; i++) {
                if (opts->client_socket[i] == 0) {
                    opts->client_socket[i] = new_socket;
                    opts->active_sd = new_socket;
                    printf("Successfully added in client socket array: [%d]\n", i);
                    break;
                }
            }

            copy(opts->client_socket[0], opts->fd_out, 1024);

        }

        for (int i = 0; i < 5; i++) {
            sd = opts->client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1024)) == 0) {
                    getpeername(sd, (struct sockaddr *) &client_address, &client_address_size);
                    printf("DISCONNECTED [ ip : %s ]\n", inet_ntoa(client_address.sin_addr));
                    close(sd);
                    opts->client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                }
            }
        }
    }
}


static void cleanup_server(const struct options_server *opts) {
    close(opts->server_socket);
}
