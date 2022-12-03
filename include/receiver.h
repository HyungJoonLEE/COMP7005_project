#ifndef PROJECT_RECEIVER_H
#define PROJECT_RECEIVER_H

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BACKLOG 5
#define TRUE 1


struct options
{
    in_port_t port_in;
    int receiver_socket;
    int client_socket[2];
    int client_count;
    char file_name[20];
};


/**
 * Initialize default option for client.
 * Memory setting for server option struct to 0 and DEFAULT port number saved.
 *
 * @param opts client struct settings
 */
static void options_init_receiver(struct options *opts);


/**
 * Parse input from server IP, server port, server download directory are included.
 * Set the server option struct after parsing.
 * If there is no input, it will use DEFAULT value.
 *
 * @param argc number of argument
 * @param argv server's input
 * @param opts server option struct settings
 */
static void parse_arguments_receiver(int argc, char *argv[], struct options *opts);


/**
 * Initialize network settings in server from protocol, ip, port and wait for client connection.
 *
 * @param opts client option struct settings
 */
static void options_process_receiver(struct options *opts);


/**
 * Free variables that were used for client option struct setting.
 *
 * @param opts
 */
static void cleanup(const struct options *opts);


static int options_process(struct options *opts);


void add_new_client(struct options *opts, int client_socket, struct sockaddr_in *newcliaddr);
int get_max_socket_number(struct options *opts);
void remove_client(struct options *opts, int client_socket);
bool data_receive_rate_process(struct options *opts);
bool ack_receive_rate_process(struct options *opts);

#endif //PROJECT_RECEIVER_H
