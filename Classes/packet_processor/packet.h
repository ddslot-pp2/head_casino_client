#ifndef __PACKET_H
#define __PACKET_H

#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace LOBBY
{
    struct CS_LOG_IN
    {

        std::string username;
        std::string password;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(username, password); // serialize things by passing them to the archive
        }
    };

    struct SC_LOG_IN
    {
        bool result;
        std::string error_code;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(result, error_code)
                ; // serialize things by passing them to the archive
        }
    };

    struct CS_CONNECT
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

    struct SC_CONNECT
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
    struct CS_MOVE_CHARACTER
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
    struct CS_CAST_SKILL
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
