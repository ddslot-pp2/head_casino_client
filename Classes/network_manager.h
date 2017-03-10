#ifndef __NETWORK_MANAGER_H
#define __NETWORK_MANAGER_H

#define ASIO_STANDALONE

#include <functional>

#include "network/asio.hpp"
#include "network/asio/io_service.hpp"

using asio::ip::tcp;

class network_manager
{
public:
    network_manager();
    ~network_manager();

    static network_manager& instance()
    {
        static network_manager instance;
        return instance;
    }

    void do_connect(std::string host, std::string port, std::function<void(bool)> on_connected);

private:

    asio::io_service io_service_;
    tcp::socket socket_;
};

#endif
