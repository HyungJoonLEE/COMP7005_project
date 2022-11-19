#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "sender.h"
#include "conversion.h"
#include "error.h"


const char *INPUT_EXIT = "exit";


int main(int argc, char *argv[]) {
    int max_socket_num; // IMPORTANT Don't forget to set +1
    unsigned int expected_ack = 0;
    char buffer[256] = {0};
    char response[256] = {0};
    ssize_t rx_len;
    fd_set read_fds;
    int flag = 0;

    struct timeval timeout;
    // receive time out config
    // Set 1 ms timeout counter
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;


    struct options opts;
    options_init(&opts);
    parse_arguments(argc, argv, &opts);
    opts.proxy_socket = options_process(&opts);
    if (opts.proxy_socket == -1) {
        printf("Connect() fail");
    }

    max_socket_num = opts.proxy_socket;

    FD_ZERO(&read_fds);
    while (1) {
        FD_SET(0, &read_fds);
        FD_SET(opts.proxy_socket, &read_fds);
        if (select(max_socket_num + 1, &read_fds, NULL, NULL, &timeout) < 0) {
            printf("select fail");
            exit(1);
        }

//        if (FD_ISSET(opts.proxy_socket, &read_fds)) {
//            ssize_t received_data_size;
//            if ((received_data_size = read(opts.proxy_socket, buffer, sizeof(buffer))) > 0) {
//                buffer[received_data_size] = '\0';
//                printf("%s\n", buffer);
//            }
//        }

        if (FD_ISSET(0, &read_fds)) {
            if (fgets(buffer, sizeof(buffer), stdin)) {
                if (strstr(buffer, INPUT_EXIT) != NULL) {
                    printf("Exit from the server");
                    close(opts.proxy_socket);
                    exit(0);
                }

                if (strstr(buffer, "start") != NULL) {
                    send_file(&opts);
                }

                write(opts.proxy_socket, buffer, sizeof(buffer));
                expected_ack += strlen(buffer) + 1;
            }
        }

        if (FD_ISSET(opts.proxy_socket, &read_fds)) {
            read(opts.proxy_socket, response, sizeof(response));
            printf("[ proxy ] : %s\n", response);
            memset(buffer, 0, sizeof(char) * 256);
            memset(response, 0, sizeof(char) * 256);
        }
        else {
            write(opts.proxy_socket, buffer, sizeof(buffer));
            expected_ack += strlen(buffer) + 1;
        }

    }
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


static int options_process(struct options *opts) {
    ssize_t server_connection_test_fd;
    int result;
    char message[50] = {0};
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
        struct sockaddr_in server_addr;

        opts->proxy_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(opts->proxy_socket == -1)
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

        result = connect(opts->proxy_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

        if(result == -1)
        {
            fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
        }
        server_connection_test_fd = read(opts->proxy_socket, message, sizeof(message));
        if(server_connection_test_fd == -1) {
            printf("You are not connected to server\n");
        }
        printf("%s \n", message);
    }
    return opts->proxy_socket;
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


void send_file(struct options *opts) {
    char buffer[256];
    char response[256];
    unsigned int expected_ack = 0;

    // Start to send file(s)
    for (int i = 0; i < opts->file_count; i++) {
        FILE *file;
        ssize_t file_size, current_size = 0;

        // Send server - file size of <filename>.txt
        file = fopen(opts->file_arr[i], "rb");
        fseek(file, 0, SEEK_END);
        file_size = (int) ftell(file);
        fseek(file, 0, SEEK_SET);


        // Send proxy - read <filename>.txt with 256 bytes and send buffer
        while (current_size != file_size) {
            size_t fp_size = fread(buffer, 1, 256, file);
            buffer[fp_size] = '\0';
            current_size += fp_size;

            write(opts->proxy_socket, buffer, sizeof(buffer));
            read(opts->proxy_socket, response, sizeof(response));
//            expected_ack += strlen(buffer);
//            while (1) {
//                read(opts->proxy_socket, response, sizeof(response));
//                if (expected_ack == (unsigned int) atoi(response)) {
//                    printf("%u  %s\n", expected_ack, response);
//                    memset(response, 0, sizeof(char) * 256);
//                    break;
//                }
//            }
            memset(buffer, 0, sizeof(char) * 256);
        }
        fclose(file);
    }
}


