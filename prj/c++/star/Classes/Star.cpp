#include "Star.h"

USING_NS_CC;

Star* Star::create(int color, int width, int height)
{
    Star* ret = new Star();
    Color4B c(Star::makeColor(color));
    if (ret && ret->initWithColor(c, width, height))
    {
        ret->m_nColor = color;
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Color3B Star::makeColor(int color)
{
    switch (color)
    {
    case RED:
        return Color3B(205, 92, 92);
    case BLUE:
        return Color3B(70, 130, 180);
    case YELLOW:
        return Color3B(238, 238, 0);
    case GRAY:
        return Color3B(156, 156, 156);
    case GREEN:
    default:
        return Color3B(46, 139, 87);
    }
}
