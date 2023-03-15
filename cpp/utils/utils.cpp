#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

#include "../server/server.h"

int getCountOfSubstr(std::string str, std::string substr) {
    int occurrences = 0;
    std::string::size_type pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++occurrences;
        pos += substr.length();
    }
    return occurrences;
}

Config parseConfig(const std::string &path) {
    Config conf;

    std::ifstream configurationOfServer(path);
    int buf1, buf2;
    std::string buf3;

    configurationOfServer >> buf1;
    configurationOfServer >> buf2;
    configurationOfServer >> buf3;
    conf.cpuLimit = buf1;
    conf.threadsLimit = buf2;
    conf.documentRoot = buf3;

    std::cout << "[LOG] CPU limit: " << conf.cpuLimit << std::endl
              << "[LOG] Count thread: " << conf.threadsLimit << std::endl
              << "[LOG] Root directory: " << conf.documentRoot << std::endl;

    configurationOfServer.close();

    return conf;
}

HTTPRequest parseHTTP(std::string request) {
    HTTPRequest parsedRequest;
    std::stringstream ss(request);

    ss >> parsedRequest.method;
    ss >> parsedRequest.path;
    ss >> parsedRequest.protocol;

    // std::cout << "[LOG] Parsed data: " << parsedRequest.method << " " << parsedRequest.path << " " << parsedRequest.protocol << std::endl;
    return parsedRequest;
}

std::string encodeUrl(std::string url) {
    std::string encodedUrl;
    int charCode;
    for (int i = 0; i < url.size(); i++) {
        if (url[i] == '%') {
            std::istringstream(url.substr(i + 1, 2)) >> std::hex >> charCode;
            encodedUrl += static_cast<char>(charCode);
            i += 2;
        } else {
            encodedUrl += url[i];
        }
    }

    if (encodedUrl.find('?') != std::string::npos) {
        encodedUrl.erase(encodedUrl.find('?'));
    }
    // std::cout << "[LOG] Encoded url: " << encodedUrl << std::endl;
    return encodedUrl;
}

std::string findFile(std::string pathToFile, int &code) {
    pathToFile = ".." + pathToFile;
    std::string result;
    bool isIndexHTML = false;
    if (pathToFile.back() == '/') {
        if (pathToFile.find(".html") != std::string::npos) {
            code = NOT_FOUND;
            return result;
        }
        isIndexHTML = true;
        pathToFile += "index.html";
    }

    if (getCountOfSubstr(pathToFile, "/../") > 3) {
        code = NOT_FOUND;
        return result;
    }

    std::ifstream file(pathToFile, std::ios_base::binary);

    if (!file.is_open()) {
        std::cout << "[ERROR] Cannot open file: " << pathToFile << std::endl;
        code = NOT_FOUND;
        if (isIndexHTML) {
            code = FORBIDDEN;
        }
        return result;
    }

    std::stringstream fileStream;

    fileStream << file.rdbuf();

    result = fileStream.str();
    file.close();

    return result;
}

std::string getContentType(const std::string fileName) {
    std::string extension = std::filesystem::path(fileName).extension();
    if (extension == ".html") {
        return "text/html";
    } else if (extension == ".js") {
        return "application/javascript";
    } else if (extension == ".css") {
        return "text/css";
    } else if (extension == ".jpg" || extension == ".jpeg") {
        return "image/jpeg";
    } else if (extension == ".png") {
        return "image/png";
    } else if (extension == ".gif") {
        return "image/gif";
    } else if (extension == ".svg") {
        return "image/svg+xml";
    } else if (extension == ".swf") {
        return "application/x-shockwave-flash";
    }
    return "text/plain";
}

std::string getCodeDescription(int code) {
    switch (code) {
        case OK:
            return "OK";
        case METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case FORBIDDEN:
            return "Forbidden";
        default:
            return "Not Found";
    }
}

std::string getStringFromHTTPResponse(HTTPResponse response) {
    std::string httpResponse = response.protocol + " " + std::to_string(response.code) + " " +
                               getCodeDescription(response.code) + "\r\n" +
                               "Version: HTTP/1.1\r\n" +
                               "Connection: " + response.connection + "\r\n" +
                               "Server: " + response.server + "\r\n";
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    httpResponse += "Date: " + std::string(ctime(&time));

    if (response.code != OK) {
        httpResponse += "\r\n\r\n";
        return httpResponse;
    }
    httpResponse += "Content-Type: " + response.contentType + "\r\n";
    httpResponse += "Content-Length: " + std::to_string(response.contentLength) + "\r\n\r\n" + response.data;

    return httpResponse;
}
