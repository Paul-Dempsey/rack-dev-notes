// Copyright © 2025 Paul Chase Dempsey
// See license in hot-svg.hpp
#include "hot-svg.hpp"
namespace hot_svg {

Cache svg_cache;

// nearly identical to Rack's cache
std::shared_ptr<::rack::window::Svg> Cache::load(const std::string & path)
{
    const auto & pair = svgs.find(path);
    if (pair != svgs.end()) {
        return pair->second;
    }

    std::shared_ptr<::rack::window::Svg> svg;
    try {
        svg = std::make_shared<::rack::window::Svg>();
        svg->loadFile(path);
    } catch (::rack::Exception& e) {
        WARN("%s", e.what());
        svg = nullptr;
    }
    svgs[path] = svg;
    return svg;
}

// Reload all SVGs in the cache
// For changes to be visible in Rack, you must also send a dirty event to affected module widgets
void Cache::reload()
{
    for (auto pair: svgs) {
        try {
            pair.second->loadFile(pair.first);
        } catch (::rack::Exception& e) {
            WARN("%s", e.what());
        }
    }
}

}
