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

    auto aabb2 = getBulletLayer()->getBoundingBox();
    Point offset(aabb2.origin+getGameLayer()->getPosition());
    aabb2.origin = Point::ZERO;
    //log("bullet layer aabb %f, %f", aabb2.origin.x, aabb2.origin.y);
    for(Node* bullet : _bullets->getChildren())
    {
        Bullet* b = dynamic_cast<Bullet*>(bullet);
        auto aabb1 = b->getBoundingBox();
        if(aabb2.intersectsRect(aabb1))
        {
            //Point pos(Director::getInstance()->convertToGL(b->getPosition()));
            Point pos(b->getPosition()+offset);
            int radius =b->getConfig()->radius;
            int bufferSize =pow(radius*2,2);
            
            Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
            
            glReadPixels(pos.x-radius, pos.y-radius, radius*2, radius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            bool coll = false;
            for(int i = 0; i < bufferSize; i++)
            {
                if(buffer[i].a>0)
                {
                    b->explode();
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
            //log("layer  aabb %f, %f, %f, %f", aabb2.origin.x, aabb2.origin.y, aabb2.size.width, aabb2.size.height);
            //log("bullet aabb %f, %f, %f, %f", aabb1.origin.x, aabb1.origin.y, aabb1.size.width, aabb1.size.height);
            
            b->setColor(Color3B::WHITE);
            b->runAction(RemoveSelf::create());
        }
    }
}