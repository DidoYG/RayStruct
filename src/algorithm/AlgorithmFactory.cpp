// src/Algorithm/AlgorithmFactory.cpp
#include "../../include/algorithm/AlgorithmFactory.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>

// Namespace for internal linkage
namespace {
// Structure to hold information about loaded custom libraries
struct CustomLibraryInfo {
    void* handle = nullptr;
    using CreateFn = Algorithm* (*)();
    CreateFn create = nullptr;
    bool ownsFile = false;
};

// Map to store loaded custom libraries
std::unordered_map<std::string, CustomLibraryInfo>& customLibraries() {
    static std::unordered_map<std::string, CustomLibraryInfo> libraries;
    return libraries;
}

// Helper function to determine if the factory should own the library file
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

// Instantiates built-in algorithms or dlopens a custom shared object on demand.
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
            // Load custom algorithm from shared library
            if (customLibraryPath.empty()) {
                std::cerr << "No custom algorithm library path provided." << std::endl;
                return nullptr;
            }
            {
                // Check if the library is already loaded
                auto& libraries = customLibraries();
                auto it = libraries.find(customLibraryPath);
                if (it == libraries.end()) {
                    // Load the shared library
                    CustomLibraryInfo info;
                    info.handle = dlopen(customLibraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
                    if (!info.handle) {
                        std::cerr << "dlopen failed for '" << customLibraryPath << "': " << dlerror() << std::endl;
                        return nullptr;
                    }

                    // Locate the factory function symbol
                    dlerror(); // clear any existing error state

                    // Assume the factory function is named "createAlgorithm"
                    info.create = reinterpret_cast<CustomLibraryInfo::CreateFn>(dlsym(info.handle, "createAlgorithm"));
                    const char* symbolError = dlerror();
                    if (symbolError != nullptr) {
                        std::cerr << "dlsym failed to locate createAlgorithm in '" << customLibraryPath << "': " << symbolError << std::endl;
                        dlclose(info.handle);
                        return nullptr;
                    }
                    
                    // Store the loaded library info
                    info.ownsFile = shouldOwnLibraryFile(customLibraryPath);
                    it = libraries.emplace(customLibraryPath, info).first;
                }

                // Use the factory function to create an instance
                if (!it->second.create) {
                    std::cerr << "Custom library '" << customLibraryPath << "' did not provide a valid createAlgorithm symbol." << std::endl;
                    return nullptr;
                }

                // Create and return the algorithm instance
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

// Clears every custom library handle and optionally deletes temporary .so files.
void AlgorithmFactory::cleanupCustomLibraries() {
    // Unload all custom libraries and delete files if owned
    auto& libraries = customLibraries();

    // Iterate and clean up
    for (auto& entry : libraries) {

        // Unload the shared library
        const std::string& path = entry.first;
        CustomLibraryInfo& info = entry.second;

        // Close the library handle
        if (info.handle) {
            dlclose(info.handle);
            info.handle = nullptr;
        }

        // Delete the library file if owned
        if (info.ownsFile) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
            if (ec) {
                std::cerr << "Warning: failed to remove custom library '" << path << "': " << ec.message() << std::endl;
            }
        }
    }

    // Clear the map
    libraries.clear();
}
