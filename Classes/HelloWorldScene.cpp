#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

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
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("res/ui/ss1.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    using namespace std::placeholders;
    network_manager::instance().do_connect("127.0.0.1", "3000", std::bind(&HelloWorld::on_connected, this, _1));

    
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

    
    return true;
}

void HelloWorld::on_connected(bool r)
{
    bool result = true;
    if (result)
    {
        CCLOG("successfully connected\n");
    }
    else
    {
        CCLOG("fail to connect\n");
    }
}

void HelloWorld::process_packet()
{
    while (!network_manager::instance().q.empty())
    {
        auto packet_info = network_manager::instance().q.front();

        auto opcode = reinterpret_cast<unsigned short>(packet_info.buffer->data());

        std::stringstream is(std::string(packet_info.buffer->data() + sizeof(unsigned short), packet_info.cereal_size));

        cereal::BinaryInputArchive ir(is); // Create an input archive
        CS_LOG_IN read;
        try
        {
            ir(read);
        }
        catch (...)
        {
            // log error
        }
    

        network_manager::instance().q.pop();
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
