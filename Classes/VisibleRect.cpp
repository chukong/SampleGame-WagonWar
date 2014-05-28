#include "VisibleRect.h"

VisibleRect g_visibleRect;

void VisibleRect::lazyInit()
{
    // no lazy init
    // Useful if we change the resolution in runtime
    auto visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    visibleWidth = visibleRect.size.width;
    visibleHeight = visibleRect.size.height;
    visibleOriginX = visibleRect.origin.x;
    visibleOriginY = visibleRect.origin.y;

    center.x = visibleOriginX + visibleWidth / 2;
    center.y = visibleOriginY + visibleHeight / 2;

    left.x = visibleOriginX;
    left.y = center.y;

    right.x = visibleOriginX + visibleWidth;
    right.y = center.y;

    top.x = center.x;
    top.y = visibleOriginY + visibleHeight;

    bottom.x = center.x;
    bottom.y = visibleOriginY;

    leftTop.x = left.x;
    leftTop.y = top.y;

    rightTop.x = right.x;
    rightTop.y = top.y;

    leftBottom.x = left.x;
    leftBottom.y = bottom.y;

    rightBottom.x = right.x;
    rightBottom.y = bottom.y;
}

