#include "backend.hpp"


// To check if a Function is available in the Path Var
bool checkInPath(std::string command)
{

#ifdef _WIN32
    // ---------------- WINDOWS ----------------

    char buffer[32767];

    DWORD len = GetEnvironmentVariableA("PATH", buffer, sizeof(buffer));
    if (len == 0)
        return false;

    std::string path_env(buffer, len);
    auto paths = split(path_env, ';');

    DWORD ext_len = GetEnvironmentVariableA("PATHEXT", buffer, sizeof(buffer));
    if (ext_len == 0)
        return false;

    std::string pathext_env(buffer, ext_len);
    auto exts = split(pathext_env, ';');

    for (const auto& dir : paths)
    {
        // If command already has extension
        fs::path direct = fs::path(dir) / command;
        if (fs::exists(direct) && fs::is_regular_file(direct))
        {
            std::cout << command << " is "
                << fs::absolute(direct) << "\n";
            return true;
        }

        for (const auto& ext : exts)
        {
            fs::path full_path = fs::path(dir) / (command + ext);

            if (fs::exists(full_path) && fs::is_regular_file(full_path))
            {
                std::cout << command << " is "
                    << fs::absolute(full_path) << "\n";
                return true;
            }
        }
    }

    return false;

#else
    // ---------------- LINUX / MAC ----------------

    const char* path_env = std::getenv("PATH");
    if (!path_env)
        return false;

    auto paths = split(path_env, ':');

    for (const auto& dir : paths)
    {
        fs::path full_path = fs::path(dir) / command;

        if (fs::exists(full_path) &&
            fs::is_regular_file(full_path) &&
            access(full_path.c_str(), X_OK) == 0)
        {
            std::cout << command << " is "
                << fs::absolute(full_path).string() << "\n";
            return true;
        }
    }

    return false;

#endif
}
