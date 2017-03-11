#ifndef __NETWORK_MANAGER_H
#define __NETWORK_MANAGER_H

#define ASIO_STANDALONE

#include <functional>
#include <queue>
#include <array>
#include <thread>
#include <utility>
#include <mutex>

#include "network/asio.hpp"
#include "network/asio/io_service.hpp"

#include "packet_processor/opcode.h"
#include "packet_processor/packet.h"

#include "cereal_v1.2.2/include/cereal/cereal.hpp"
#include "cereal_v1.2.2/include/cereal/archives/binary.hpp"
#include "cereal_v1.2.2/include/cereal/archives/portable_binary.hpp"
#include "cereal_v1.2.2/include/cereal/types/map.hpp"
#include "cereal_v1.2.2/include/cereal/types/vector.hpp"

using asio::ip::tcp;
using namespace packet;

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
    void do_write();

    template <typename Opcode, typename Packet>
    void send_packet(Opcode opcode, Packet packet)
    {
        send_lock_.lock();
        auto send_buf = std::make_shared<std::array<char, packet_buf_size>>();

        auto written = 0;

        std::stringstream os;
        {
            cereal::BinaryOutputArchive ar(os);
            ar(packet);
        }

        auto byte_packet = os.str();
        written = byte_packet.size();
        if (written)
        {
            
            auto head_size = written + sizeof(unsigned short) + sizeof(unsigned short);

            std::memcpy(send_buf->data(), &head_size, sizeof(unsigned short));
            std::memcpy(send_buf->data() + sizeof(unsigned short), &opcode, sizeof(unsigned short));
            std::memcpy(send_buf->data() + sizeof(unsigned short) * 2, byte_packet.data(), written);

            asio::async_write(socket_,
                asio::buffer(send_buf->data(),
                    head_size),
                [this, send_buf](std::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
		           send_lock_.unlock();
		           
                }
                else
                {
                    send_lock_.unlock();
                    disconnected();
                }
            });

        }
    }

    void disconnected();
    void set_on_disconnected(std::function<void()> on_disconnected);

    void io_service_run();
    void stop();

    std::queue<packet_info> q;

private:
    std::mutex send_lock_;

    asio::io_service io_service_;

    //std::shared_ptr<tcp::socket> socket_;
    tcp::socket socket_;

    packet_buffer_type receive_buffer_;

    packet_size_type receive_size_ = 0;

    std::function<void()> on_disconnected_;
};

#endif
