#ifndef __STAR_MATRIX_H__
#define __STAR_MATRIX_H__

#include "cocos2d.h"

class StarMatrix: public cocos2d::Node
{
public:
	virtual bool init(int width, int height);
	static StarMatrix* create(int width, int height);
private:
    bool init_matrix();
	bool OnTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    int m_nWidth;
    int m_nHeight;
};

#endif
