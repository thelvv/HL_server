#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "server.h"
#include "../utils/utils.h"

Server::Server(Config conf, struct sockaddr_in &saun) {
    _conf = conf;
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        perror("socket");
    }

    int result = bind(this->_socket, (struct sockaddr *) &saun, sizeof(saun));
    if (result < 0) {
        perror("bind");
    }
};

int Server::InitServer(struct sockaddr_in &saun) {
    int result = listen(this->_socket, this->_conf.threadsLimit);
    if (result < 0) {
        perror("listen");
        return -1;
    }

    std::cout << "server started" << std::endl;
    return 0;
};

void Server::WaitingForAccept() {
    while (true) {
        struct sockaddr_un addrConnected;
        socklen_t len = sizeof(addrConnected);

        int clientAddress = accept(this->_socket, (struct sockaddr *) &addrConnected, &len);
        if (clientAddress < 0) {
            perror("accept");
        } else {
            ReadData(clientAddress);
        }
    }
};

void Server::ReadData(int clientAddress) {
    std::string recievedData(MAX_MESSAGE_SIZE, 0);
    int result = recv(clientAddress, &recievedData[0], MAX_MESSAGE_SIZE, 0);
    if (result < 0) {
        perror("recv");
    }

    counterLock.lock();
    if (threadCounter < this->_conf.threadsLimit) {
        HTTPRequest request = parseHTTP(recievedData);
        std::thread clientRequestThread(&Server::Response, this, clientAddress, request);
        clientRequestThread.detach();
        threadCounter++;
    }
    counterLock.unlock();
    // std::cout << "threads: " << threadCounter << std::endl;
};

void Server::Response(int clientAddress, HTTPRequest clientRequest) {
    HTTPResponse response;
    clientRequest.path = encodeUrl(clientRequest.path);
    int error = 0;
    if (clientRequest.method == "GET" || clientRequest.method == "HEAD") {

        if (clientRequest.path.find(filePath) != std::string::npos) {
            response.code = NOT_FOUND;
        } else {
            std::string message = findFile(clientRequest.path, response.code);
            if (response.code == NOT_FOUND) {
                std::cout << "[ERROR] File does not exist: " << clientRequest.path << std::endl;
            } else if (response.code == DEFAULT_CODE) {
                response.code = OK;
                response.contentLength = message.length();
                if (clientRequest.method != "HEAD") {
                    response.data = message;
                }
                // std::cout << "[LOG] File was read: " << clientRequest.path << std::endl;
            }
            response.contentType = getContentType(clientRequest.path);
        }
    } else {
        response.code = METHOD_NOT_ALLOWED;
    }

    /*
    std::cout << "[LOG] File was read: " << clientRequest.path << std::endl;
    std::cout << "[LOG] Response code: " << response.code << std::endl;
    */

    std::string responseData = getStringFromHTTPResponse(response);
    if (send(clientAddress, &responseData[0], responseData.size(), 0) < 0) {
        perror("send");
    }

    close(clientAddress);

    counterLock.lock();
    threadCounter--;
    counterLock.unlock();
}

Server::~Server() {
    close(_socket);
}
