#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network_manager.h"


#include "network/asio/io_service.hpp"

#include "cereal_v1.2.2/include/cereal/archives/binary.hpp"
#include "cereal_v1.2.2/include/cereal/archives/portable_binary.hpp"
#include "cereal_v1.2.2/include/cereal/types/map.hpp"
#include "cereal_v1.2.2/include/cereal/types/vector.hpp"

#include <sstream>
#include <array>
#include <vector>
#include <map>

enum { max_length = 1024 };

struct CS_LOG_IN
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

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void on_connected(bool r);

    void process_packet();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
