#ifndef __STAR_SPRITE_H__
#define __STAR_SPRITE_H__

#include "cocos2d.h"

class Star: public cocos2d::LayerColor
{
    enum Color {
        RED = 0,
        BLUE,
        YELLOW,
        GRAY,
        GREEN
    };
public:
    static Star* create(int color, int width, int height);
    static cocos2d::Color3B makeColor(int color);
private:
    int m_nColor;
};

#endif
