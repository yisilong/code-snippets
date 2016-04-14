#ifndef __INFO_WRAP_H__
#define __INFO_WRAP_H__

#include "cocos2d.h"

class InfoWrap: public cocos2d::Node
{
public:
	static InfoWrap* create(int width, int height);
	virtual bool init(int width, int height);
};

#endif
