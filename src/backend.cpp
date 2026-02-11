#include "backend.hpp"


bool hasExtension(const std::string& cmd)
{
    return cmd.find('.') != std::string::npos;
}


// To check if a Function is available in the Path Var
bool checkInPath(std::string command)
{
#ifdef _WIN32
    char fullPath[MAX_PATH];

    DWORD result = SearchPathA(
        NULL,                 // PATH durchsuchen
        command.c_str(),      // Datei (z.B. "git")
        ".exe",                 // PATHEXT benutzen
        MAX_PATH,
        fullPath,
        NULL
    );

    if (result > 0 && result < MAX_PATH)
    {
        return true;
    }

    return false;

#else

    const char* path_env = std::getenv("PATH");
    if (!path_env)
        return false;

    auto paths = split(path_env, ':');

    for (const auto& dir : paths)
    {
        fs::path full = fs::path(dir) / command;

        if (fs::exists(full) &&
            fs::is_regular_file(full) &&
            access(full.c_str(), X_OK) == 0)
        {
            return true;
        }
    }

    return false;

#endif
}


std::string getExecutablePath(std::string command)
{
#ifdef _WIN32
    char fullPath[MAX_PATH];
    DWORD result = SearchPathA(
        NULL,                 // PATH durchsuchen
        command.c_str(),      // Datei (z.B. "git")
        ".exe",                 // PATHEXT benutzen
        MAX_PATH,
        fullPath,
        NULL
    );
    if (result > 0 && result < MAX_PATH)
    {
        return std::string(fullPath);
    }
	return "";
#else
    const char* path_env = std::getenv("PATH");
    if (!path_env)
        return "";
    auto paths = split(path_env, ':');
    for (const auto& dir : paths)
    {
        fs::path full = fs::path(dir) / command;
        if (fs::exists(full) &&
            fs::is_regular_file(full) &&
            access(full.c_str(), X_OK) == 0)
        {
            return fs::absolute(full).string();
        }
    }
	return "";
#endif
}


bool runProcess(const std::vector<std::string>& args)
{
#ifdef _WIN32
    std::string commandLine;

    for (const auto& arg : args)
    {
        commandLine += "\"" + arg + "\" ";
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    BOOL success = CreateProcessA(
        NULL,
        commandLine.data(),  // mutable!
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (!success)
    {
        std::cerr << "CreateProcess failed\n";
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
#else
    pid_t pid = fork();

    if (pid == 0)  // Child
    {
        std::vector<char*> c_args;

        for (const auto& arg : args)
            c_args.push_back(const_cast<char*>(arg.c_str()));

        c_args.push_back(nullptr);

        execvp(c_args[0], c_args.data());

        perror("exec failed");
        exit(1);
    }
    else if (pid > 0)  // Parent
    {
        int status;
        waitpid(pid, &status, 0);
        return true;
    }
    else
    {
        perror("fork failed");
        return false;
    }
#endif
}


bool changeDirectory(const std::string& path)
{
#ifdef _WIN32
    if (_chdir(path.c_str()) != 0)
    {
        std::cerr << "cd failed\n";
        return false;
    }
    return true;
#else
    if (chdir(path.c_str()) != 0)
    {
        perror("cd failed");
        return false;
    }
    return true;
#endif
}