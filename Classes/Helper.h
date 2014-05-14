//
//  Helper.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-12.
//
//

#ifndef __TankMultiplayer__Helper__
#define __TankMultiplayer__Helper__

#include "cocos2d.h"

class Helper
{
public:
    static void removeAfter(cocos2d::Node* node, float seconds);
    
};

class DepthOn : public cocos2d::Node
{
public:
    void onDraw();
    virtual void visit(cocos2d::Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    CREATE_FUNC(DepthOn);
protected:
    cocos2d::CustomCommand _customCommand;
};

class DepthOff : public cocos2d::Node
{
public:
    void onDraw();
    virtual void visit(cocos2d::Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    CREATE_FUNC(DepthOff);
protected:
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__TankMultiplayer__Helper__) */
