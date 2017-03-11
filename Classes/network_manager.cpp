#include "network_manager.h"
#include "ui/UIDeprecated.h"
#include <memory>
#include <type_traits>

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
        [this, on_connected](std::error_code ec, tcp::resolver::iterator it)
    {
        if (ec)
        {
            on_connected(false);
            return;
        }

        on_connected(true);
        do_read_header();
    });
}

void network_manager::do_read_header()
{
    asio::async_read(socket_,
        asio::buffer(&receive_size_, sizeof(receive_size_)),
        [this](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            if (receive_size_ == 0 || receive_size_ > network_manager::max_packet_size)
            {
                socket_.close();
                return;
            }

            do_read_body();
        }
        else
        {
            socket_.close();
        }
    });
}

void network_manager::do_read_body()
{
    receive_buffer_ = std::make_shared<std::array<char, packet_buf_size>>();

    asio::async_read(socket_,
        asio::buffer(receive_buffer_->data(), receive_size_ - sizeof(receive_size_)),
        [this](std::error_code ec, std::size_t)
    {
        if (!ec)
        {
            packet_info info;
                                                     // head             // opcode
            info.cereal_size = receive_size_ - sizeof(receive_size_) - sizeof(unsigned short);
            info.buffer = std::move(receive_buffer_);

            q.push(std::move(info));
            //packet_handler_(self, std::move(receive_buffer_), receive_size_ - sizeof(receive_size_));
            do_read_header();
            // opcode + body
        }
        else
        {
            socket_.close();
        }
    });
  
}

void network_manager::io_service_run()
{
    io_service_.run();
}
