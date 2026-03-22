#include "network.h"
#include "client.h"
#include "server.h"
#include "nettypes.h"
#include "global_net.h"

extern Network global_network;

void network_init(NetworkMode mode) {
	global_network.mode = mode;
	global_network.isConnected = 0;
	if (mode == NETWORK_CLIENT) {
		client_init();
		global_network.isConnected = 1;
	} else if (mode == NETWORK_SERVER) {
		server_init();
		global_network.isConnected = 1;
	}else if (mode == NETWORK_HOST) {
		server_init();
		client_init();
		global_network.isConnected = 1;
		global_network.isHost = 1;
		global_network.local_client_id = 0; // Host is always clientId 0
	}
}

void network_update() {
	// DEBUG_LOG("network_update() called: mode=%d, isConnected=%d", global_network.mode, global_network.isConnected);
	if (global_network.mode == NETWORK_CLIENT && global_network.isConnected) {
		client_handle_packets();
	} else if (global_network.mode == NETWORK_SERVER && global_network.isConnected) {
		server_handle_packets();
	} else if (global_network.mode == NETWORK_HOST && global_network.isConnected) {
		server_handle_packets();
		client_handle_packets();
	}
}

void network_shutdown() {
	if (global_network.mode == NETWORK_CLIENT && global_network.isConnected) {
		client_shutdown();
		global_network.isConnected = 0;
	} else if (global_network.mode == NETWORK_SERVER && global_network.isConnected) {
		server_shutdown();
		global_network.isConnected = 0;
	} else if (global_network.mode == NETWORK_HOST && global_network.isConnected) {
		server_shutdown();
		client_shutdown();
		global_network.isConnected = 0;
	}
}