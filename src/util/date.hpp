#pragma once
#include <boost/optional.hpp>

namespace lib
{

struct Date
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    float second;

    static boost::optional<Date> parse_iso8601(const std::string& str);
};

} // namespace lib
