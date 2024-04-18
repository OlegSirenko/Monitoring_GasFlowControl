//
// Created by tehnokrat on 4/15/24.
//

#include "SocketModule.h"

SocketModule::SocketModule(const std::string &serverIP, int serverPort) {
    // Initialize the GameNetworkingSockets library
    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
    {
        fprintf(stderr, "GameNetworkingSockets_Init failed.  %s", errMsg);
        exit(1);
    }

    // Create a new GameNetworkingSockets interface
    m_pInterface = SteamNetworkingSockets();

    // Set up a connection to the server
    SteamNetworkingIPAddr serverAddr{};
    serverAddr.Clear();
    serverAddr.ParseString(serverIP.c_str());
    serverAddr.m_port = serverPort;

    HSteamNetConnection hConn = m_pInterface->ConnectByIPAddress(serverAddr, 0, nullptr);
    if (hConn == k_HSteamNetConnection_Invalid)
    {
        fprintf(stderr, "Failed to create connection");
        exit(1);
    }
}

SocketModule::~SocketModule() = default;

std::vector<double> SocketModule::receiveData() {
    std::vector<double> recieved_data = {};
    return recieved_data;
}
