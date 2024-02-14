/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:58 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/14 04:35:58 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"
#include "Client.hpp"

/*
	steps:
	1. create socket
	2. bind socket to address
	3. listen for connections
	4. accept connections
	5. send/receive data
	6. close sockets
*/


int Utils::runServer(char **av)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol
	if (serverSocket < 0)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "Error setting socket options\n";
		return 1;
	}
	int port = std::stoi(av[1]);

	struct sockaddr_in serverAddr;// IPv4 address structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;		 // IPv4
	serverAddr.sin_port = htons(port);		 // Port, converted to network byte order
	serverAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY is accept any address
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
	{
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocket, 1) < 0) // 5 is the number of connections that can be waiting while the process is handling a particular connection
	{
		perror("Error listening on socket");
		exit(EXIT_FAILURE);
	}
	std::cout << "Server listening on port " << port << std::endl;
	// Accept incoming connections
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket < 0)
	{
		perror("Error accepting connection");
		exit(EXIT_FAILURE);
	}
	int bytesRead = 1;
	Client client;
	while (bytesRead > 0)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		bytesRead = recv(clientSocket, buffer, sizeof(buffer) -1, 0);
		if (bytesRead < 0)
		{
			perror("Error reading from socket");
			exit(EXIT_FAILURE);
		}

		try
		{
			client.parseRequest(buffer, av[2]);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			exit(1);
		}
	}
	close(serverSocket);
	close(clientSocket);
	return 0;
}
