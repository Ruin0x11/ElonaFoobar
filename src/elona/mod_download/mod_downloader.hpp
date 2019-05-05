#pragma once

#include <string>
#include <vector>
#include <json.hpp>
#include "../../util/either.hpp"
#include "network_mod_description.hpp"

namespace elona
{

using json = nlohmann::json;

/**
 * Interface for retrieving mods in the mod list screen.
 */
class ModDownloader
{
public:
    virtual NetworkResult<NetworkModList> list(int page) = 0;

    virtual NetworkResult<NetworkModList> search(
        const std::string& query,
        int page) = 0;
};

} // namespace elona
