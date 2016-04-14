#include "Launch.h"
#include "GameScene.h"

USING_NS_CC;

Scene* Launch::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Launch::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Launch::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemFont::create(" X ", CC_CALLBACK_1(Launch::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
    auto layer = LayerColor::create(Color4B(200, 200, 200, 100), visibleSize.width, visibleSize.height);
    this->addChild(layer, -1);
    auto label = Label::createWithSystemFont("Pop Star", "Verdana-Bold", 20);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(label, 1, "star");
    auto scaleTo = ScaleTo::create(1, 4);
    auto contentSize = label->getContentSize();
    auto moveBy = MoveBy::create(1, Vec2(0, contentSize.height));
    auto fun = CallFunc::create(CC_CALLBACK_0(Launch::switchScene, this));
    auto actions = Sequence::create(scaleTo, moveBy, fun, NULL);
    label->runAction(actions);
    
    return true;
}

void Launch::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Launch::switchScene()
{
    auto menuItem = MenuItemFont::create("start", [this](Ref* pSender){
        Director::getInstance()->replaceScene(GameScene::create());
    });
    menuItem->setColor(Color3B(219, 43, 59));
    auto menu = Menu::create(menuItem, NULL);
    auto label = this->getChildByName("star");
    label->setVisible(false);
    menu->alignItemsVertically();
    this->addChild(menu, 1);
}
