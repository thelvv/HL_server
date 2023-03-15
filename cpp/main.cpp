#include <iostream>

#include "./server/server.h"
#include "utils/utils.h"

int main() {
    struct sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(4000);

    Server server(parseConfig(configPath), addr);
    server.InitServer(addr);
    server.WaitingForAccept();

    return 0;
}
