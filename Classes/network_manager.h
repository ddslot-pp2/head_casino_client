#ifndef __NETWORK_MANAGER_H
#define __NETWORK_MANAGER_H

#define ASIO_STANDALONE

#include <functional>

#include "network/asio.hpp"
#include "network/asio/io_service.hpp"
#include <queue>
#include <array>

using asio::ip::tcp;

class network_manager
{
public:
    using packet_size_type = unsigned short;

    static constexpr unsigned short max_packet_size = 8000;
    static constexpr unsigned short packet_buf_size = 8096;

    using packet_buffer_type = std::shared_ptr<std::array<char, packet_buf_size>>;

    struct packet_info
    {
        unsigned short cereal_size;
        packet_buffer_type buffer;
    };

    network_manager();
    ~network_manager();

    static network_manager& instance()
    {
        static network_manager instance;
        return instance;
    }

    void do_connect(std::string host, std::string port, std::function<void(bool)> on_connected);
    void do_read_header();
    void do_read_body();

    std::queue<packet_info> q;

private:

    asio::io_service io_service_;
    tcp::socket socket_;

    packet_buffer_type receive_buffer_;

    packet_size_type receive_size_  = 0;
};

#endif
