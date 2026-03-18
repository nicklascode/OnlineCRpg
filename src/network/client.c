
#define NOGDI
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "client.h"
#include "packet.h"
#include "network.h"
#include "../util/logger.h"
#include "../network/nettypes.h"
#include "../network/global_net.h"

Client client;

void client_init() {
	// Initialize Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a socket and connect to the server
	client.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	client.address.sin_family = AF_INET;
	client.address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost for now
	client.address.sin_port = htons(PORT);

	   if (connect(client.socket, (SOCKADDR*)&client.address, sizeof(client.address)) == SOCKET_ERROR) {
		   ERROR_LOG("Failed to connect to server");
	   } else {
		   DEBUG_LOG("Connected to server");
		   // Set client socket to non-blocking
		   u_long mode = 1;
		   ioctlsocket(client.socket, FIONBIO, &mode);
	   }
}

void client_handle_packets() {
	static int greeting_sent = 0;
	if (client.socket == INVALID_SOCKET) {
		ERROR_LOG("Client socket is invalid");
		return;
	}
	if (!greeting_sent) {
		DEBUG_LOG("Sending greeting");
		PlayerDataPacket greeting;
		memset(&greeting, 0, sizeof(PlayerDataPacket));
		strcpy(greeting.username, "ClientUser");
		greeting.client_id = -1;

		Packet packet;
		serialize_packet(&greeting, Greeting, &packet, sizeof(PlayerDataPacket));
		send_packet(client.socket, &packet);
		DEBUG_LOG("Sent greeting packet");
		greeting_sent = 1;
	}

	// Receive packets from server
	Packet recv_packet;
	while (1) {
		int received = receive_packet(client.socket, &recv_packet);
		if (received == 1) {
			DEBUG_LOG("Received packet from server, id: %d", recv_packet.id);
			switch (recv_packet.id) {
				case Greeting: {
					PlayerDataPacket reply;
					deserialize_packet(&recv_packet, &reply, sizeof(PlayerDataPacket));
					// TODO Handle a list of players
					break;
				}
				case 0:
				default:
					ERROR_LOG("Received unknown packet ID: %d", recv_packet.id);
					break;
			}
		} else if (received == -1) {
			// No data available, break loop
			break;
		} else if (received == 0) {
            ERROR_LOG("Server disconnected");
            client_shutdown();
            break;
		}
	}
}

void client_shutdown() {
	DEBUG_LOG("Disconnected from server");
	if (client.socket != INVALID_SOCKET) {
		closesocket(client.socket);
		client.socket = INVALID_SOCKET;
	}
	// Mark as disconnected so network_update() stops calling client_handle_packets
	extern Network global_network;
	global_network.isConnected = 0;
	WSACleanup();
}
