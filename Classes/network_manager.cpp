#include "network_manager.h"
#include "ui/UIDeprecated.h"
#include <memory>
#include <type_traits>
#include <functional>

network_manager::network_manager() : socket_(io_service_)
{

}

network_manager::~network_manager()
{

}

void network_manager::do_connect(std::string host, std::string port, std::function<void(bool)> on_connected)
{
    io_service_.reset();
    tcp::resolver resolver(io_service_);
    auto endpoint_iterator = resolver.resolve({ host, port });

    //asio::error_code ec;
    //socket_.set_option(asio::socket_base::reuse_address(true), ec);

    asio::async_connect(socket_, endpoint_iterator,
        [this, on_connected](std::error_code ec, tcp::resolver::iterator it)
    {
        if (ec)
        {
            on_connected(false);
            return;
        }

        //socket2_ = std::move(socket_);
        on_connected(true);
        do_read_header();
    });
    
           
    std::thread t([this] {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //asio::error_code ec;
        io_service_.run();

        //CCLOG("ec:%d\n", ec);
        CCLOG("finish io_service thread\n");
    });
    t.detach();
         
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
             
                disconnected();
                return;
            }

            do_read_body();
        }
        else
        {
             disconnected();   
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
            disconnected();
        }
    });
  
}

void network_manager::io_service_run()
{
    io_service_.run();
}

void network_manager::stop()
{
    io_service_.stop();
}

void network_manager::disconnected()
{
    asio::error_code ec;
    //socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close(ec);
    stop();

    if(on_disconnected_) on_disconnected_();
}

void network_manager::set_on_disconnected(std::function<void()> on_disconnected)
{
    on_disconnected_ = on_disconnected;
}

