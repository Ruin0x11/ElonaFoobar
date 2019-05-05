#pragma once

#include <string>
#include <vector>
#include "../../util/date.hpp"
#include "../lua_env/mod_manifest.hpp"
#include "../semver.hpp"

namespace elona
{


struct NetworkModDescription
{
    std::string id;
    std::string name;
    int downloads;
    semver::Version version;
    int version_downloads;
    std::string authors;
    std::string summary;
    std::string licenses;
    lib::Date updated_at;
    lua::ModManifest::Dependencies dependencies;
    std::string base_uri;
    std::string download_uri;
};


template <class T>
using NetworkResult = either::either<T, std::string>;

using NetworkModList = std::vector<NetworkModDescription>;


} // namespace elona
