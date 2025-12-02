// src/structure/DataStructureFactory.cpp
#include "../../include/structure/DataStructureFactory.hpp"
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
        // Trim trailing newlines inserted by FormatMessage
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
    dlerror(); // clear previous errors
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

// Structure to hold info about loaded custom data structure libraries
struct CustomDSLibraryInfo {
    LibraryHandle handle = nullptr;
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

    const std::string extension = path.extension().string();
    if (extension != ".so" && extension != ".dll") {
        return false;
    }

    auto filename = path.filename().string();
    if (filename.find(std::string("_custom") + extension) == std::string::npos) {
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
                // Checks if already loaded
                auto& libraries = customDSLibraries();
                auto it = libraries.find(customLibraryPath);
                if (it == libraries.end()) {
                    // Load the shared object
                    CustomDSLibraryInfo info;
                    std::string loadError;
                    info.handle = openLibrary(customLibraryPath, loadError);
                    if (!info.handle) {
                        std::cerr << "Failed to load custom data structure library '"
                                  << customLibraryPath << "': " << loadError << std::endl;
                        return nullptr;
                    }

                    // Lookup factory symbol
                    std::string symbolError;
                    info.create = reinterpret_cast<CustomDSLibraryInfo::CreateFn>(
                        loadSymbol(info.handle, "createDataStructure", symbolError));
                    if (!info.create) {
                        std::cerr << "Failed to locate createDataStructure in '"
                                  << customLibraryPath << "': " << symbolError << std::endl;
                        closeLibrary(info.handle);
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

// Method to clear custom libraries and delete owned files
void DataStructureFactory::cleanupCustomLibraries() {
    auto& libraries = customDSLibraries();

    for (auto& entry : libraries) {
        const std::string& path = entry.first;
        CustomDSLibraryInfo& info = entry.second;

        if (info.handle) {
            closeLibrary(info.handle);
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
