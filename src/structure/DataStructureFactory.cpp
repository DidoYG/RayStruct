// src/structure/DataStructureFactory.cpp
#include "../../include/structure/DataStructureFactory.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>

// Namespace for internal linkage
namespace {
// Structure to hold info about loaded custom data structure libraries
struct CustomDSLibraryInfo {
    void* handle = nullptr;
    using CreateFn = DataStructure* (*)();
    CreateFn create = nullptr;
    bool ownsFile = false;
};

// Map to store loaded custom DS libraries
std::unordered_map<std::string, CustomDSLibraryInfo>& customDSLibraries() {
    static std::unordered_map<std::string, CustomDSLibraryInfo> libraries;
    return libraries;
}

// Helper: determine if the factory should delete the file after unloading
bool shouldOwnDSLibraryFile(const std::string& pathStr) {
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
    return parent == "structure";
}
} // namespace

// Factory method to create data structure instances based on the provided type
DataStructure* DataStructureFactory::createDataStructure(DataStructureEnum type, const std::string& customLibraryPath) {
    switch (type) {
        case DataStructureEnum::LIST:
            return new ListStructure();
        case DataStructureEnum::HEAP:
            return new HeapStructure();
        case DataStructureEnum::GRAPH:
            return new GraphStructure();
        case DataStructureEnum::CUSTOM:
            if (customLibraryPath.empty()) {
                std::cerr << "No custom data structure library path provided." << std::endl;
                return nullptr;
            }

            {
                // Check if already loaded
                auto& libraries = customDSLibraries();
                auto it = libraries.find(customLibraryPath);
                if (it == libraries.end()) {
                    // Load the shared object
                    CustomDSLibraryInfo info;
                    info.handle = dlopen(customLibraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
                    if (!info.handle) {
                        std::cerr << "dlopen failed for '" << customLibraryPath
                                  << "': " << dlerror() << std::endl;
                        return nullptr;
                    }

                    // Clear previous dl errors
                    dlerror();

                    // Lookup factory symbol
                    info.create = reinterpret_cast<CustomDSLibraryInfo::CreateFn>(
                        dlsym(info.handle, "createDataStructure"));
                    const char* symbolError = dlerror();
                    if (symbolError != nullptr) {
                        std::cerr << "dlsym failed to locate createDataStructure in '"
                                  << customLibraryPath << "': " << symbolError << std::endl;
                        dlclose(info.handle);
                        return nullptr;
                    }

                    // Determine ownership
                    info.ownsFile = shouldOwnDSLibraryFile(customLibraryPath);
                    it = libraries.emplace(customLibraryPath, info).first;
                }

                // Use factory function
                if (!it->second.create) {
                    std::cerr << "Custom library '" << customLibraryPath
                              << "' did not provide a valid createDataStructure symbol." << std::endl;
                    return nullptr;
                }

                DataStructure* instance = it->second.create();
                if (!instance) {
                    std::cerr << "Custom data structure factory returned nullptr from '"
                              << customLibraryPath << "'." << std::endl;
                }
                return instance;
            }
        default:
            return nullptr;
    }
}

// Cleanup method to unload custom libraries and delete owned files
void DataStructureFactory::cleanupCustomLibraries() {
    auto& libraries = customDSLibraries();

    for (auto& entry : libraries) {
        const std::string& path = entry.first;
        CustomDSLibraryInfo& info = entry.second;

        if (info.handle) {
            dlclose(info.handle);
            info.handle = nullptr;
        }

        if (info.ownsFile) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
            if (ec) {
                std::cerr << "Warning: failed to remove custom data structure library '"
                          << path << "': " << ec.message() << std::endl;
            }
        }
    }

    libraries.clear();
}
