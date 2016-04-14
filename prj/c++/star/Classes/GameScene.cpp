#include "GameScene.h"
#include "InfoWrap.h"
#include "StarMatrix.h"

USING_NS_CC;

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // info wrap
	auto infoWrap = InfoWrap::create(visibleSize.width, visibleSize.height - visibleSize.width);
	infoWrap->setAnchorPoint(Vec2(0, 0));
	infoWrap->setPosition(Vec2(origin.x, origin.y + visibleSize.width));
	this->addChild(infoWrap);
    // game wrap
	auto starMatrix = StarMatrix::create(visibleSize.width, visibleSize.width);
	starMatrix->setAnchorPoint(Vec2(0, 0));
	starMatrix->setPosition(Vec2(origin.x, origin.y));
	this->addChild(starMatrix);
    return true;
}
