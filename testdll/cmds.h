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

void echo(char *arg);
void kill(char *arg);
void dump(char *arg);
void call(char *arg);
}

#endif // CMDS_H_INCLUDED
