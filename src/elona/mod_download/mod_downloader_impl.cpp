#include "mod_downloader_impl.hpp"

#include <iostream>
#include <boost/network/protocol/http/client.hpp>

namespace elona
{

using namespace boost::network;

NetworkModDescription ModDownloaderImpl::_parse_mod(const json& j)
{
    NetworkModDescription mod;

    // clang-format off
    mod.id                = j["id"].get<std::string>();
    mod.name              = j["name"].get<std::string>();
    mod.downloads         = j["downloads"].get<int>();
    auto version_str      = j["version"].get<std::string>();
    mod.version           = semver::Version::parse(version_str).right();
    mod.version_downloads = j["version_downloads"].get<int>();
    mod.authors           = j["authors"].get<std::string>();
    mod.summary           = j["summary"].get<std::string>();
    // mod.licenses       = j["licenses"].get<std::string>();
    auto updated_at_str   = j["updated_at"].get<std::string>();
    mod.updated_at        = lib::Date::parse_iso8601(updated_at_str).get();
    mod.dependencies      = lua::ModManifest::Dependencies();
    mod.base_uri          = j["base_uri"].get<std::string>();
    mod.download_uri      = j["download_uri"].get<std::string>();
    // clang-format on

    return mod;
}

NetworkResult<NetworkModList> ModDownloaderImpl::list(int page)
{
    auto uri = _api_uri + "/api/v1/mods.json?page=" + std::to_string(page);

    std::string body_;
    try
    {
        http::client client;
        http::client::request request(uri);
        request << header("Connection", "close");
        http::client::response response = client.get(request);
        body_ = body(response);
    }
    catch (std::exception& e)
    {
        return NetworkResult<NetworkModList>::right_of(e.what());
    }

    auto resp = json::parse(body_);

    NetworkModList list;

    for (const auto& it : resp)
    {
        auto mod = _parse_mod(it);
        list.emplace_back(mod);
    }

    return NetworkResult<NetworkModList>::left_of(list);
}

NetworkResult<NetworkModList> ModDownloaderImpl::search(
    const std::string& query,
    int page)
{
    // TODO
    return list(page);
}


} // namespace elona
