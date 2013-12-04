#ifndef WRAP_H_INCLUDED
#define WRAP_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

char *wrap(std::vector<std::string> *args, std::vector<std::string> *types, void *fp);

#endif//WRAP_H_INCLUDED