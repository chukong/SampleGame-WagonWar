#pragma once

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
public:
    void lazyInit();

    float visibleWidth;
    float visibleHeight;
    float visibleOriginX;
    float visibleOriginY;

    Point left;
    Point right;
    Point top;
    Point bottom;
    Point center;
    Point leftTop;
    Point rightTop;
    Point leftBottom;
    Point rightBottom;
};

extern VisibleRect g_visibleRect;
