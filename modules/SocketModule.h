//
// Created by tehnokrat on 4/15/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_SOCKETMODULE_H
#define IMGUI_GASFLOWCONTROLL_SOCKETMODULE_H

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <vector>
#include <string>


class SocketModule {
public:
    SocketModule(const std::string& serverIP, int serverPort);
    ~SocketModule();
    static std::vector<double> receiveData();
private:
    ISteamNetworkingSockets* m_pInterface{};
};


#endif //IMGUI_GASFLOWCONTROLL_SOCKETMODULE_H
