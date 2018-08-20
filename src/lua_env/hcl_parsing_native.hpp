#pragma once
#include <string>
#include "../thirdparty/sol2/sol.hpp"

namespace elona
{
namespace lua
{

/**
 * Parses an HCL file using the native microhcl library. This results
 * in a ~4x speedup over parsing HCL in pure Lua.
 *
 * @param filename The name of an HCL file.
 *
 * @return A Lua table containing the parsed data.
 */
sol::table parse_hcl_native(const std::string &filename);

} // namespace lua
} // namespace elona
