// src/Algorithm/AlgorithmFactory.cpp
#include "../../include/algorithm/AlgorithmFactory.hpp"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>

// Namespace for internal linkage
namespace {
// Platform specific shared library helpers
#ifdef _WIN32
using LibraryHandle = HMODULE;

std::string windowsErrorMessage() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return "Unknown error";
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

    std::string message = "Unknown error";
    if (size && messageBuffer) {
        message.assign(messageBuffer, size);
        while (!message.empty() && (message.back() == '\r' || message.back() == '\n')) {
            message.pop_back();
        }
    }

    if (messageBuffer) {
        LocalFree(messageBuffer);
    }

    return message;
}
#else
using LibraryHandle = void*;
#endif

LibraryHandle openLibrary(const std::string& path, std::string& error) {
    error.clear();
#ifdef _WIN32
    LibraryHandle handle = LoadLibraryA(path.c_str());
    if (!handle) {
        error = windowsErrorMessage();
    }
    return handle;
#else
    LibraryHandle handle = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        const char* err = dlerror();
        error = err ? err : "Unknown error";
    }
    return handle;
#endif
}

void* loadSymbol(LibraryHandle handle, const char* name, std::string& error) {
    error.clear();
#ifdef _WIN32
    FARPROC symbol = GetProcAddress(handle, name);
    if (!symbol) {
        error = windowsErrorMessage();
        return nullptr;
    }
    return reinterpret_cast<void*>(symbol);
#else
    dlerror(); // clear any existing error state
    void* symbol = dlsym(handle, name);
    const char* err = dlerror();
    if (err != nullptr) {
        error = err;
        return nullptr;
    }
    return symbol;
#endif
}

void closeLibrary(LibraryHandle handle) {
    if (!handle) {
        return;
    }
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

// Structure to hold information about loaded custom libraries
struct CustomLibraryInfo {
    LibraryHandle handle = nullptr;
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

    const std::string extension = path.extension().string();
    if (extension != ".so" && extension != ".dll") {
        return false;
    }

    auto filename = path.filename().string();
    if (filename.find(std::string("_custom") + extension) == std::string::npos) {
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
                    std::string loadError;
                    info.handle = openLibrary(customLibraryPath, loadError);
                    if (!info.handle) {
                        std::cerr << "Failed to load custom algorithm library '"
                                  << customLibraryPath << "': " << loadError << std::endl;
                        return nullptr;
                    }

                    // Locate the factory function symbol
                    std::string symbolError;
                    info.create = reinterpret_cast<CustomLibraryInfo::CreateFn>(
                        loadSymbol(info.handle, "createAlgorithm", symbolError));
                    if (!info.create) {
                        std::cerr << "Failed to locate createAlgorithm in '" << customLibraryPath
                                  << "': " << symbolError << std::endl;
                        closeLibrary(info.handle);
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
            closeLibrary(info.handle);
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
