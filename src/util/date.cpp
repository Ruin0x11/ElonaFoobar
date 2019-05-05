#include "date.hpp"
#include <cstdio>

namespace lib
{

boost::optional<Date> Date::parse_iso8601(const std::string& str)
{
    Date date{};

    int result = sscanf(
        str.c_str(),
        "%d-%d-%dT%d:%d:%fZ",
        &date.year,
        &date.month,
        &date.day,
        &date.hour,
        &date.month,
        &date.second);

    if (result == EOF)
    {
        return boost::none;
    }

    return date;
}

} // namespace lib
