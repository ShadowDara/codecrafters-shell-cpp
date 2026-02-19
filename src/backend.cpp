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


bool runProcess(
    const std::vector<std::string>& args,
    bool redirect,
    bool redirectStderr,
    const std::string& filename)
{
#ifdef _WIN32
    std::string commandLine;
    for (const auto& arg : args)
        commandLine += "\"" + arg + "\" ";

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    HANDLE hFile = NULL;

    if (redirect || redirectStderr)
    {
        hFile = CreateFileA(
            filename.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE)
        {
            std::cerr << "File open failed\n";
            return false;
        }

        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = (redirect ? hFile : GetStdHandle(STD_OUTPUT_HANDLE));
        si.hStdError = (redirectStderr ? hFile : GetStdHandle(STD_ERROR_HANDLE));
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }

    BOOL success = CreateProcessA(
        NULL,
        commandLine.data(),
        NULL,
        NULL,
        TRUE, // Handle inheritance
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (!success)
    {
        std::cerr << "CreateProcess failed\n";
        if (hFile) CloseHandle(hFile);
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if (hFile) CloseHandle(hFile);

    return true;

#else
    pid_t pid = fork();

    if (pid == 0) // Child
    {
        if (redirect)
        {
            int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open failed"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            if (redirectStderr)
                dup2(fd, STDERR_FILENO); // stderr auch umleiten
            close(fd);
        }
        else if (redirectStderr)
        {
            int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open failed"); exit(1); }
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        std::vector<char*> c_args;
        for (auto& arg : args)
            c_args.push_back(const_cast<char*>(arg.c_str()));
        c_args.push_back(nullptr);

        execvp(c_args[0], c_args.data());
        perror("exec failed");
        exit(1);
    }
    else if (pid > 0) // Parent
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
#else
    if (chdir(path.c_str()) != 0)
#endif
    {
        std::cerr << "cd: " << path << ": ";
        perror("");
        return false;
    }
    return true;
}


bool changeToHome()
{
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    if (!home)
    {
        std::cerr << "Home not found\n";
        return false;
    }

#ifdef _WIN32
    if (_chdir(home) != 0)
#else
    if (chdir(home) != 0)
#endif
    {
        perror("cd");
        return false;
    }

    return true;
}
