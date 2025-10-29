// src/Algorithm/AlgorithmFactory.cpp
#include "../../include/algorithm/AlgorithmFactory.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>

namespace {
struct CustomLibraryInfo {
    void* handle = nullptr;
    using CreateFn = Algorithm* (*)();
    CreateFn create = nullptr;
    bool ownsFile = false;
};

std::unordered_map<std::string, CustomLibraryInfo>& customLibraries() {
    static std::unordered_map<std::string, CustomLibraryInfo> libraries;
    return libraries;
}

bool shouldOwnLibraryFile(const std::string& pathStr) {
    std::error_code ec;
    std::filesystem::path path(pathStr);
    if (!std::filesystem::exists(path, ec)) {
        return false;
    }
    if (path.extension() != ".so") {
        return false;
    }

    auto filename = path.filename().string();
    if (filename.find("_custom.so") == std::string::npos) {
        return false;
    }

    auto parent = path.parent_path().filename();
    return parent == "algorithm";
}
} // namespace

Algorithm* AlgorithmFactory::createAlgorithm(AlgorithmEnum type, const std::string& customLibraryPath) {
    switch (type) {
        case AlgorithmEnum::INSERTION_SORT: 
            return new InsertionSort();
        case AlgorithmEnum::MERGE_SORT: 
            return new MergeSort();
        case AlgorithmEnum::HEAP_BUILD: 
            return new HeapBuild();
        case AlgorithmEnum::HEAP_SELECTION:
            return new HeapSelection();
        case AlgorithmEnum::A_STAR: 
            return new AStar();
        case AlgorithmEnum::PRIMS: 
            return new Prims();
        case AlgorithmEnum::CUSTOM:
            if (customLibraryPath.empty()) {
                std::cerr << "No custom algorithm library path provided." << std::endl;
                return nullptr;
            }
            {
                auto& libraries = customLibraries();
                auto it = libraries.find(customLibraryPath);
                if (it == libraries.end()) {
                    CustomLibraryInfo info;
                    info.handle = dlopen(customLibraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
                    if (!info.handle) {
                        std::cerr << "dlopen failed for '" << customLibraryPath << "': " << dlerror() << std::endl;
                        return nullptr;
                    }

                    dlerror(); // clear any existing error state
                    info.create = reinterpret_cast<CustomLibraryInfo::CreateFn>(dlsym(info.handle, "createAlgorithm"));
                    const char* symbolError = dlerror();
                    if (symbolError != nullptr) {
                        std::cerr << "dlsym failed to locate createAlgorithm in '" << customLibraryPath << "': " << symbolError << std::endl;
                        dlclose(info.handle);
                        return nullptr;
                    }

                    info.ownsFile = shouldOwnLibraryFile(customLibraryPath);
                    it = libraries.emplace(customLibraryPath, info).first;
                }

                if (!it->second.create) {
                    std::cerr << "Custom library '" << customLibraryPath << "' did not provide a valid createAlgorithm symbol." << std::endl;
                    return nullptr;
                }

                Algorithm* instance = it->second.create();
                if (!instance) {
                    std::cerr << "Custom algorithm factory returned nullptr from '" << customLibraryPath << "'." << std::endl;
                }
                return instance;
            }
        default:
            return nullptr;
    }
}

void AlgorithmFactory::cleanupCustomLibraries() {
    auto& libraries = customLibraries();
    for (auto& entry : libraries) {
        const std::string& path = entry.first;
        CustomLibraryInfo& info = entry.second;

        if (info.handle) {
            dlclose(info.handle);
            info.handle = nullptr;
        }

        if (info.ownsFile) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
            if (ec) {
                std::cerr << "Warning: failed to remove custom library '" << path << "': " << ec.message() << std::endl;
            }
        }
    }

    libraries.clear();
}
