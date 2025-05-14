#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <map>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    int port = 6667; // Default IRC port

    // Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    server_addr.sin_port = htons(port); // Convert the port to network byte order

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    // Accept a connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    std::map<int, std::string> client_buffers; // Moved outside loop
    char temp[512];
    bool should_exit = false;

    do {
        // Read without blocking (buffer may contain partial command)
        ssize_t bytes_read = recv(client_fd, temp, sizeof(temp), 0);
        
        if(bytes_read > 0) {
            // Append to persistent buffer
            client_buffers[client_fd].append(temp, bytes_read);
            
            // Process complete commands
            size_t crlf_pos;
            while((crlf_pos = client_buffers[client_fd].find("\r\n")) != std::string::npos) {
                std::string full_cmd = client_buffers[client_fd].substr(0, crlf_pos);
                
				std::cout << full_cmd << std::endl;
                // Command handling (case-sensitive)
				if(full_cmd == "exit") {
					std::cout << "Closing connection" << std::endl;

					// (A) Send final acknowledgment to client
					const char* response = "SERVER: Closing connection\r\n";
					send(client_fd, response, strlen(response), 0);

					// (B) Partial socket shutdown
					shutdown(client_fd, SHUT_WR);  // FIN sent to client

					// (C) Drain remaining data (prevents RST)
					// char drain_buffer[128];
					// while(recv(client_fd, drain_buffer, sizeof(drain_buffer), 0) > 0) {
					// 	; // Discard leftover data
					// }

					should_exit = true;
    				close(client_fd);
					break;
				}

                // Echo back to client (optional)
                send(client_fd, (full_cmd + "\r\n").c_str(), full_cmd.length() + 2, 0);
                
                // Remove processed command from buffer
                client_buffers[client_fd].erase(0, crlf_pos + 2);
            }
        }
        else if(bytes_read == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        else {
            perror("recv error");
            break;
        }
    } while(!should_exit);

    // Close the client and server sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
