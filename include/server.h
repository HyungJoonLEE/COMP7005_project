#ifndef COMP_7005_PROJECT_SERVER_H
#define COMP_7005_PROJECT_SERVER_H

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>

#define BACKLOG 5
#define DEFAULT_PORT 4000


struct options
{
    in_port_t port_in;
    int server_socket;
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
static void options_init_server(struct options *opts);


/**
 * Parse input from server IP, server port, server download directory are included.
 * Set the server option struct after parsing.
 * If there is no input, it will use DEFAULT value.
 *
 * @param argc number of argument
 * @param argv server's input
 * @param opts server option struct settings
 */
static void parse_arguments_server(int argc, char *argv[], struct options *opts);


/**
 * Initialize network settings in server from protocol, ip, port and wait for client connection.
 *
 * @param opts client option struct settings
 */
static void options_process_server(struct options *opts);


/**
 * Free variables that were used for client option struct setting.
 *
 * @param opts
 */
static void cleanup(const struct options *opts);


void add_new_client(struct options *opts, int client_socket, struct sockaddr_in *newcliaddr);
int get_max_socket_number(struct options *opts); // 최대 소켓번호 찾기
void remove_client(struct options *opts, int client_socket); // 채팅 탈퇴 처리 함수
void error_handling(char *message);
#endif //COMP_7005_PROJECT_SERVER_H
