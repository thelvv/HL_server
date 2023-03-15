#pragma once

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

const int MAX_MESSAGE_SIZE = 10000;
const int DEFAULT_CODE = 0;

const std::string configPath = "../server.conf";
const std::string filePath = "../httptest/../";

int threadCounter = 0;
std::mutex counterLock;

enum supportedCodes {
    OK = 200,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405
};

struct Config {
    int threadsLimit;
    std::string documentRoot;
};

struct HTTPRequest {
    std::string method;
    std::string path;
    std::string protocol;
};

struct HTTPResponse {
    std::string protocol = "HTTP/1.1";
    int code = DEFAULT_CODE;
    std::string server = "cppserver";
    int contentLength;
    std::string data;
    std::string contentType;
    std::string connection = "close";
};

class Server {

public:
    Server(Config conf, struct sockaddr_in &saun);

    ~Server();

    int InitServer(struct sockaddr_in &saun);

    void WaitingForAccept();

private:
    int CreateWorkers();

    void ReadData(int addressConnnected);

    void Response(int addressConnnected, HTTPRequest message);

    int _socket;
    std::vector<int> sockets;
    Config _conf;
    int countOfThreadsAlive = 0;
};
