#pragma once
#include <string>

namespace elona
{

void netload(const std::string&);
int net_dl(const std::string&, const std::string&);
int net_dllist(const std::string&, int = 0);
int net_read(int = 0);
int net_send(const std::string&, int = 0);

}
