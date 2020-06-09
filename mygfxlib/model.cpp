//
// Created by ripopov on 6/7/2020.
//

#include "model.h"

#include <filesystem>
#include <iostream>

namespace gfx {

Model::Model(const std::string& model_obj_file) {
    std::string warn;
    std::string err;

    std::filesystem::path model_path(model_obj_file);
    auto parent_dir = model_path.parent_path();

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_obj_file.c_str(),
                                parent_dir.string().c_str());

    if (!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error("Can't open model " + model_obj_file);
    }

    for (const auto& mt : materials) {
        if (!mt.diffuse_texname.empty()) {
            if (diffuse_textures.contains(mt.diffuse_texname)) {
                mat_diff_tex.push_back(&diffuse_textures.at(mt.diffuse_texname));
            } else {
                diffuse_textures.emplace(mt.diffuse_texname, (parent_dir / mt.diffuse_texname).string().c_str());
            }
        } else {
            mat_diff_tex.push_back(nullptr);
        }
    }
}

} // namespace gfx