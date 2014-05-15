//
//  Helper.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-12.
//
//

#include "Helper.h"
#include "bullet.h"
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

void CollisionCheckNode::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(CollisionCheckNode::checkCollision, this);
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&_customCommand);
}
void CollisionCheckNode::checkCollision()
{
    auto offset = getGameLayer()->getPosition();
    auto aabb2 = _level->getRT()->getBoundingBox();
    for(Node* bullet : _bullets->getChildren())
    {
        Bullet* b = dynamic_cast<Bullet*>(bullet);
        auto aabb1 = b->getBoundingBox();
        if(aabb1.intersectsRect(aabb2))
        {
            
            
            Point pos = b->getPosition()+offset;
            int radius =b->getConfig()->radius;
            int bufferSize =pow(radius*2,2);
            
            Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
            
            glReadPixels(pos.x-radius, pos.y-radius, radius*2, radius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            bool coll = false;
            for(int i = 0; i < bufferSize; i++)
            {
                if(buffer[i].a>0)
                {
                    coll = true;
                    break;
                }
            }
            free(buffer);
            if(coll)
                b->setColor(Color3B::BLUE);
            else
                b->setColor(Color3B::YELLOW);
        }
        else{
            b->setColor(Color3B::WHITE);
            b->runAction(RemoveSelf::create());
        }
    }
}