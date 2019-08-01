#ifndef MULTIPLAYER_SERVER_SCANER_H
#define MULTIPLAYER_SERVER_SCANER_H

#include <functional>
#include "multiplayer_server.h"

//Class to find all servers that have the correct version number. Creates a big nice list.
class ServerScanner : public Updatable
{
    int server_port;
    sf::UdpSocket socket;
    sf::Clock broadcast_clock;

public:
    struct ServerInfo
    {
        sf::IpAddress address;
        int port;
        string name;

        sf::Clock timeout_clock;
    };
private:
    std::vector<struct ServerInfo> server_list;
    int version_number;
    constexpr static float BroadcastTimeout = 2.0f;
    constexpr static float ServerTimeout = 30.0f;
    
    string master_server_url;
    sf::Mutex server_list_mutex;
    sf::Thread master_server_scan_thread;
    
    std::function<void(sf::IpAddress, string)> newServerCallback;
    std::function<void(sf::IpAddress)> removedServerCallback;
public:

    ServerScanner(int version_number, int server_port = defaultServerPort);
    virtual ~ServerScanner();

    virtual void update(float delta) override;
    void addCallbacks(std::function<void(sf::IpAddress, string)> newServerCallback, std::function<void(sf::IpAddress)> removedServerCallback);
    
    void scanLocalNetwork();
    void scanMasterServer(string url);

    std::vector<ServerInfo> getServerList();

private:
    void masterServerScanThread();
    
    void updateServerEntry(sf::IpAddress address, int port, string name);
};

#endif//MULTIPLAYER_SERVER_SCANER_H
