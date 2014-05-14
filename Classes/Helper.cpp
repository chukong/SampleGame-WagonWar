//
//  Helper.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-12.
//
//

#include "Helper.h"
USING_NS_CC;

void Helper::removeAfter(Node* node, float seconds)
{
    auto call= CallFunc::create([node](){node->removeFromParent();});
    node->runAction(Sequence::create(
                                     DelayTime::create(seconds),
                                     call,
                                     nullptr));
}

void DepthOn::onDraw()
{
    glEnable(GL_DEPTH_TEST);
}

void DepthOn::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(DepthOn::onDraw, this);
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&_customCommand);
}

void DepthOff::onDraw()
{
    glDisable(GL_DEPTH_TEST);
}

void DepthOff::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(DepthOff::onDraw, this);
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&_customCommand);
}