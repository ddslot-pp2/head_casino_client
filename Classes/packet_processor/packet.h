#ifndef __PACKET_H
#define __PACKET_H

#include <vector>
#include <map>



namespace LOBBY
{
    struct CS_LOG_IN //: public base_packet
    {
        int x;
        std::vector<int> v;
        std::map<int, int> m;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(x, v, m); // serialize things by passing them to the archive
        }
    };

    struct CS_CONNECT //: public base_packet
    {
        int x;
        std::vector<int> v;
        std::map<int, int> m;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(x, v, m); // serialize things by passing them to the archive
        }
    };

    struct SC_CONNECT //: public base_packet
    {
        int x;
        std::vector<int> v;
        std::map<int, int> m;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(x, v, m); // serialize things by passing them to the archive
        }
    };
}

namespace GAME
{
    struct CS_MOVE_CHARACTER //: public base_packet
    {
        int x;
        std::vector<int> v;
        std::map<int, int> m;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(x, v, m); // serialize things by passing them to the archive
        }
    };
}

namespace SKILL
{
    struct CS_CAST_SKILL //: public base_packet
    {
        int x;
        std::vector<int> v;
        std::map<int, int> m;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(x, v, m); // serialize things by passing them to the archive
        }
    };
}


#endif
