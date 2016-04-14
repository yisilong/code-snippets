#include "StarMatrix.h"
#include "Star.h"

USING_NS_CC;

bool StarMatrix::init(int width, int height)
{
    if (!Node::init())
    {
        return false;
    }
	m_nWidth = width;
    m_nHeight = height;
    if (!init_matrix())
    {
        return false;
    }
    
    return true;
}

StarMatrix* StarMatrix::create(int width, int height)
{
    StarMatrix* ret = new StarMatrix();
    if (ret && ret->init(width, height))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StarMatrix::init_matrix()
{
    // make matrix
    auto width = m_nWidth / 10;
    auto height = m_nHeight / 13;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i != 10; ++i) {
        for (int j = 0; j != 13; ++j) {
            auto t = random(0, 4);
            Star* child = Star::create(t, width, height);
            child->setAnchorPoint(Vec2(0, 0));
            child->setPosition(Vec2(i*width, j*height));
            this->addChild(child);
        }
    }
	// callback
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StarMatrix::OnTouchBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

bool StarMatrix::OnTouchBegin(Touch* touch, Event* event)
{
	auto target = event->getCurrentTarget();
	Point p = touch->getLocationInView();
	p = Director::getInstance()->convertToGL(p);
	log("x=%f y=%f, target=%0x, this=%0x", p.x, p.y, target, this);
	return true;
}