#include "lobby_scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* lobby_scene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = lobby_scene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool lobby_scene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(lobby_scene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
        origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("res/lobby/ui/lobby_bg.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    // 버튼 생성
    game_start_button_ = Button::create("res/lobby/ui/start0.png", "res/lobby/ui/start1.png", "res/lobby/ui/start1.png");
    game_start_button_->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    game_start_button_->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            break;
        case ui::Widget::TouchEventType::ENDED:

            network_manager::instance().do_connect("192.168.25.57", "3000", std::bind(&lobby_scene::on_connected, this, _1));

          
            CCLOG("Button 1 clicked\n");
            game_start_button_->setBright(false);
            game_start_button_->setTouchEnabled(false);
            break;

        default:
            break;
        }
    });
    this->addChild(game_start_button_, 1);
    game_start_button_->setBright(false);
    game_start_button_->setTouchEnabled(false);

    // 네트워크 셋팅
    network_manager::instance().set_on_disconnected(std::bind(&lobby_scene::on_disconnected, this));
    network_manager::instance().do_connect("192.168.25.57", "3000", std::bind(&lobby_scene::on_connected, this, _1));

    

    /*
    CS_LOG_IN write;
    write.x = 20;
    write.v.push_back(10);
    write.m[0] = 20;
    write.m[1] = 20;

    std::stringstream os;
    {
    cereal::BinaryOutputArchive ar(os);
    ar(write);
    }

    auto str = os.str();
    auto size = str.size();

    CCLOG("size: %d\n", size);
    */
    this->scheduleUpdate();

    return true;
}

void lobby_scene::on_connected(bool result)
{
    if (result)
    {
        CCLOG("successfully connected\n");
    }
    else
    {
        CCLOG("fail to connect\n");
    }
}

void lobby_scene::on_disconnected()
{
    game_start_button_->setBright(true);
    game_start_button_->setTouchEnabled(true);
    CCLOG("disconnected\n");
}

void lobby_scene::update(float ms)
{
    process_packet();
}

void lobby_scene::process_packet()
{
    while (!network_manager::instance().q.empty())
    {
        auto packet_info = network_manager::instance().q.front();

        auto opcode = *reinterpret_cast<unsigned short*>(packet_info.buffer->data());

        std::stringstream is(std::string(packet_info.buffer->data() + sizeof(unsigned short), packet_info.cereal_size));

        cereal::BinaryInputArchive ir(is); // Create an input archive

        try
        {
            if (opcode == SC_LOG_IN)
            {
                LOBBY::SC_LOG_IN read;
                ir(read);
            }
            else
            {

            }
        }
        catch (...)
        {
            CCLOG("exception cated due to parsing packet!!\n");
        }

        network_manager::instance().q.pop();
    }
}

void lobby_scene::menuCloseCallback(Ref* pSender)
{
    network_manager::instance().stop();
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void lobby_scene::process_LOBBY_CS_LOG_IN(const LOBBY::SC_LOG_IN& read)
{
    if (!read.result)
    {
        CCLOG("error code: %s\n", read.error_code.c_str());
    }

}
