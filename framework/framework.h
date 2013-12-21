#ifndef FRAMEWORK_H_INCLUDED
#define FRAMEWORK_H_INCLUDED

#include "wrap.h"

namespace framework
{

bool init(char *namesp);

bool erl_connect(char *server, char *port);
bool erl_send(const char *str);

bool process();

} //end namespace framework


#endif//FRAMWORK_H_INCLUDED
