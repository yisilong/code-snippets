#include "InfoWrap.h"

USING_NS_CC;

InfoWrap* InfoWrap::create(int width, int height)
{
	InfoWrap* ret = new InfoWrap();
	if (ret && ret->init(width, height))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool InfoWrap::init(int width, int height)
{
	if (!Node::init())
	{
		return false;
	}
	this->setContentSize(Size(width, height));
	this->setColor(Color3B(255, 0, 0));
	return true;
}