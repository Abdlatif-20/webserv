/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:58 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/10 00:01:14 by aben-nei         ###   ########.fr       */
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

int runServer(char *av)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, default protocol
	if (serverSocket < 0)
	{
		std::cerr << "Error creating socket\n";
		return 1;
	}
	int port = std::stoi(av);
	
	struct sockaddr_in serverAddr;// IPv4 address structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;		 // IPv4
	serverAddr.sin_port = htons(port);		 // Port, converted to network byte order
	serverAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY is accept any address
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
	{
		std::cerr << "Error binding socket" << std::endl;
		return 1;
	}
	if (listen(serverSocket, 1) < 0) // 5 is the number of connections that can be waiting while the process is handling a particular connection
	{
		std::cerr << "Error listening on socket" << std::endl;
		return 1;
	}
	std::cout << "Server listening on port " << port << std::endl;
	// Accept incoming connections
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket < 0)
	{
		std::cerr << "Error accepting connection\n";
		return 1;
	}
	int bytesRead = 1;
	Client client;
	while (bytesRead > 0)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead < 0)
		{
			std::cerr << "Error receiving data" << std::endl;
			return 1;
		}

		try
		{
			client.parseRequest(buffer);
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
