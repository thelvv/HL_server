#pragma once

#include <string>
#include "../server/server.h"

Config parseConfig(const std::string& path);

HTTPRequest parseHTTP(std::string request);

std::string getStringFromHTTPResponse(HTTPResponse);

std::string encodeUrl(std::string url);

std::string findFile(std::string pathToFile, int &code);

std::string getContentType(std::string fileName);

std::string getCodeDescription(int code);

int getCountOfSubstr(std::string str, std::string substr);
