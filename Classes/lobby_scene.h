#ifndef __LOBBY_SCENE_H
#define __LOBBY_SCENE_H

#include "cocos2d.h"
#include "network_manager.h"

class lobby_scene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void on_connected(bool result);
    void on_disconnected();


    virtual void update(float ms) override;

    // 패킷 관련
    void process_packet();

    void process_LOBBY_CS_LOG_IN(const LOBBY::SC_LOG_IN& read);


    // implement the "static create()" method manually
    CREATE_FUNC(lobby_scene);
};

#endif // __HELLOWORLD_SCENE_H__
