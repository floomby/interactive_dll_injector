#ifndef CMDS_H_INCLUDED
#define CMDS_H_INCLUDED

#include <cstdio>
#include <cstring>

#include <windows.h>

#include "include/piper.h"
#include "include/analyze.h"

namespace cmd
{
void init(piper *pipe);

void register_func(char cmd, void(*func)(char *));
void call_func(char cmd, char *arg);

void echo(char *arg);
void kill(char *arg);
void disasm(char *arg);
void call(char *arg);
}

#endif // CMDS_H_INCLUDED
