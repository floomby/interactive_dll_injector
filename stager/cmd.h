#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

/// The command registration/calling interface
/// is far from being final

#include <windows.h>

#include "../common/piper.h"

namespace cmd
{
void init(piper *pipe);

void add(char cmd, void(*func)(const char *));
void call(char cmd, const char *arg);

void load(const char *arg);
}

#endif // CMD_H_INCLUDED
