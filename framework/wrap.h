#ifndef WRAP_H_INCLUDED
#define WRAP_H_INCLUDED

#undef UNICODE

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <direct.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace framework
{

char *wrap(std::vector<std::string> *args, std::vector<std::string> *types, void *fp);
//TODO: this is a stopgap function
void call(char *cmd, std::unordered_map<std::string, std::string> *dllMap);
bool load(std::string module, std::unordered_map<std::string, std::string> *dllMap);

} //end namespace framework

#endif//WRAP_H_INCLUDED