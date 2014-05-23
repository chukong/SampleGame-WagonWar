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

bool Helper::isInCircle(int index, int radius)
{
    //find out x and y;
    int x = index/(radius*2);
    int y = index%(radius*2);
    return pow(x-radius,2) + pow(y-radius, 2) <= radius*radius;
}
float Helper::getAngleFromIndex(int index, int radius)
{
    int x = index/(radius*2);
    int y = index%(radius*2);
    if(x == y == radius)
        return -999;
    
    return atan2f(radius-y, radius-x);
}
Point Helper::getPointFromIndex(int index, int radius)
{
    return Point(index/(radius*2),index%(radius*2));
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
        bool coll = false;
        Bullet* b = dynamic_cast<Bullet*>(bullet);
        auto aabb1 = b->getBoundingBox();
        if(aabb2.intersectsRect(aabb1))
        {
            //check if we are colliding with a player
            for(Node* player : _players->getChildren())
            {
                TestNode* p = dynamic_cast<TestNode*>(player);
                if(b->getPosition().getDistance(p->getPosition())< b->getConfig()->radius + p->radius)
                {
                    b->explode();
                    log("collided with player");
                    coll = true;
                    break;
                }
            }
            if(coll)
                return;
            
            
            //Point pos(Director::getInstance()->convertToGL(b->getPosition()));
            Point pos(b->getPosition()+offset);
            int radius =b->getConfig()->radius;
            int bufferSize =pow(radius*2,2);
            
            Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
            
            glReadPixels(int (pos.x-radius*0.833984), int (pos.y-radius*0.833984), int(radius*2*0.833984), int(radius*2*0.833984), GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            
            for(int i = 0; i < bufferSize; i++)
            {
                if(buffer[i].a>0 && Helper::isInCircle(i, radius))
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
    
    for(Node* player : _players->getChildren())
    {
        TestNode* p = dynamic_cast<TestNode*>(player);
        if(p->airborn)
        {
            Point pos(p->getPosition()+offset);
            int radius =p->radius;
            int bufferSize =pow(radius*2,2);
            Color4B *buffer = (Color4B*)malloc(sizeof(Color4B)*bufferSize);
            glReadPixels(pos.x-radius, pos.y-radius, radius*2, radius*2, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            for(int i = 0; i < bufferSize; i++)
            {
                //TODO:: need to pass circle check
                if(buffer[i].a>0 && Helper::isInCircle(i, radius)  )
                {
                    p->airborn = false;
                    p->setLastPos(p->getPosition());
                    break;
                }
            }
            free(buffer);
        }
    }
}


TestNode* TestNode::create()
{
    auto sp = Sprite::create("testnode.png");
    TestNode* ret = new TestNode;
    if(sp)
    {
        ret->radius = ret->getContentSize().width/2;
        sp->setFlippedX(true);
        ret->addChild(sp);
        sp->setPosition(0, 40);
        ret->autorelease();
        ret->radius = 20;
        
        auto drawN = DrawNode::create();
        drawN ->drawDot(Point::ZERO, ret->radius, Color4F(0,1,0,0.5));
        ret->addChild(drawN);
        
        ret->gunPoint = Node::create();
        ret->addChild(ret->gunPoint);
        ret->gunPoint->setPosition(43, 45);
        
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
