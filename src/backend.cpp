#include "backend.hpp"


bool hasExtension(const std::string& cmd)
{
    return cmd.find('.') != std::string::npos;
}


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
        fs::path base = fs::path(dir);

        // Wenn command bereits Extension hat
        if (hasExtension(command))
        {
            fs::path full = base / command;
            if (fs::exists(full) && fs::is_regular_file(full))
            {
                std::cout << command << " is "
                    << fs::absolute(full) << "\n";
                return true;
            }
        }
        else
        {
            for (const auto& ext : exts)
            {
                fs::path full = base / (command + ext);

                if (fs::exists(full) && fs::is_regular_file(full))
                {
                    std::cout << command << " is "
                        << fs::absolute(full) << "\n";
                    return true;
                }
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
