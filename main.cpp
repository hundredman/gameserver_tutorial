#include "bootstraper.h"
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <iostream>

std::atomic<bool> g_stop_flag{false};

void signal_handler([[ maybe_unused ]] int signal)
{
    g_stop_flag = true;
    g_stop_flag.notify_all();
}

// Port used when none is given on the command line (10000-20000 range for the class server).
constexpr int DEFAULT_PORT = 10135;

int main(int argc, char* argv[])
{
    int port = DEFAULT_PORT;
    if (argc > 1)
    {
        port = std::atoi(argv[1]);
        if (port < 1 || port > 65535)
        {
            std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
            return 1;
        }
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGPIPE, SIG_IGN);

    Bootstraper bootstraper;
    if (bootstraper.run(port) == false)
        return 1;
    
    while (g_stop_flag == false) g_stop_flag.wait(false);
    bootstraper.stop();

    return 0;
}
