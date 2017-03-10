#include "network_manager.h"

network_manager::network_manager() : socket_(io_service_)
{
    
}

network_manager::~network_manager()
{

}

void network_manager::do_connect(std::string host, std::string port, std::function<void(bool)> on_connected)
{
    tcp::resolver resolver(io_service_);
    auto endpoint_iterator = resolver.resolve({ host, port });

    asio::async_connect(socket_, endpoint_iterator,
        [this, &on_connected](std::error_code ec, tcp::resolver::iterator)
    {
        if (ec)
        {
            on_connected(false);
            //do_read_header();   
        }

        on_connected(true);
        //do_read_header();
    });
}
