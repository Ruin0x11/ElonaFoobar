#pragma once
#include "mod_downloader.hpp"

namespace elona
{

/**
 * Consumes the mod list API of the mod repository.
 *
 * @see https://www.github.com/Ruin0x11/foobar-mod-repo.
 */
class ModDownloaderImpl : ModDownloader
{
public:
    ModDownloaderImpl(std::string api_uri)
        : _api_uri(api_uri)
    {
    }

    NetworkResult<NetworkModList> list(int page) override;

    NetworkResult<NetworkModList> search(const std::string& query, int page)
        override;

private:
    NetworkModDescription _parse_mod(const json& j);

    std::string _api_uri;
};

} // namespace elona
