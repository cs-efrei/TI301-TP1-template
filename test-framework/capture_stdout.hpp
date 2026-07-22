// Classe de capture robuste et portable (Linux + Windows/MinGW).
// Redirige le descripteur de fichier stdout (fd 1) vers un fichier temporaire,
// ce qui capture aussi bien printf() (C) que std::cout (C++), contrairement
// à une simple redirection de streambuf qui ne voit pas printf().

#ifndef CAPTURE_STDOUT_HPP
#define CAPTURE_STDOUT_HPP "__CAPTURE__STDOUT_HPP"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
	#include <io.h>
	#include <windows.h>
	#define dup _dup
	#define dup2 _dup2
	#define fileno _fileno
	#define close _close
	#define getpid _getpid
#else
	#include <unistd.h>
#endif

using namespace std;


class StdoutRedirector
{
	public:
    StdoutRedirector()
	{
        fflush(stdout);

        old_stdout_fd = dup(fileno(stdout));
        if (old_stdout_fd == -1)
		{
            throw std::runtime_error("StdoutRedirector: dup(stdout) a echoue");
        }

        // IMPORTANT: on n'utilise PAS tmpfile() : sous Windows/MinGW, tmpfile()
        // tente de creer le fichier dans le repertoire racine (C:\) et echoue
        // silencieusement (retourne NULL) a cause des permissions, ce qui
        // provoquait une capture vide sans aucune erreur visible.
        tmp_filename = makeTempFilename();
        tmp_file = fopen(tmp_filename.c_str(), "w+b");
		
        if (!tmp_file)
		{
            close(old_stdout_fd);
            throw std::runtime_error("StdoutRedirector: impossible de creer le fichier temporaire: " + tmp_filename);
        }

        if (dup2(fileno(tmp_file), fileno(stdout)) == -1)
		{
            fclose(tmp_file);
            remove(tmp_filename.c_str());
            close(old_stdout_fd);
            throw std::runtime_error("StdoutRedirector: dup2 vers stdout a echoue");
        }
    }

    std::string getContent()
	{
        fflush(stdout);
        dup2(old_stdout_fd, fileno(stdout)); // restaure stdout AVANT de lire
        close(old_stdout_fd);

        fseek(tmp_file, 0, SEEK_SET);
        std::string result;
        char buffer[4096];
        size_t n;
        while ((n = fread(buffer, 1, sizeof(buffer), tmp_file)) > 0)
		{
            result.append(buffer, n);
        }
        fclose(tmp_file);
        remove(tmp_filename.c_str());
        restored = true;
        return result;
    }

    ~StdoutRedirector()
	{
        // Filet de securite si getContent() n'a pas ete appele (ex: exception/REQUIRE qui echoue)
        if (!restored)
		{
            fflush(stdout);
            dup2(old_stdout_fd, fileno(stdout));
            close(old_stdout_fd);
            if (tmp_file) fclose(tmp_file);
            remove(tmp_filename.c_str());
        }
    }

    // Non copiable (gere un descripteur de fichier)
    StdoutRedirector(const StdoutRedirector&) = delete;
    StdoutRedirector& operator=(const StdoutRedirector&) = delete;

	private:
    
	static std::string getTempDir() {

#ifdef _WIN32
        char buf[MAX_PATH];
        DWORD len = GetTempPathA(MAX_PATH, buf);
        if (len == 0 || len > MAX_PATH) return ".\\";
        return std::string(buf, len);
#else
        const char* d = getenv("TMPDIR");
        if (d) return std::string(d) + "/";
        return "/tmp/";
#endif
    }

    static std::string makeTempFilename()
	{
        static int counter = 0;
        std::string dir = getTempDir();
        int pid = getpid();
        return dir + "capture_" + std::to_string(pid) + "_" + std::to_string(counter++) + ".tmp";
    }

    int old_stdout_fd;
    FILE* tmp_file;
    std::string tmp_filename;
    bool restored = false;
};

#endif