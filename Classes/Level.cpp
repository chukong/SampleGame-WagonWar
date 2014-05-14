//
//  Level.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-13.
//
//

#include "Level.h"
USING_NS_CC;

Level* Level::create(const std::string &filename)
{
    auto lvl = new Level();

    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);
    auto sp =Sprite::create(filename);
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    //ret->setDefaultSprite(sp);
    Size size = sp->getContentSize();
    auto ret = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGB5A1);
    lvl->addChild(ret);
    lvl->setRT(ret);
    
    //** register post update after the drawing is done
    auto listener = EventListenerCustom::create(Director::EVENT_AFTER_DRAW, CC_CALLBACK_0(Level::postUpdate, lvl));
    ret->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, ret);
    
    //** need to draw the default sprite
    ret->begin();
    sp->setPosition(Point(size/2));
    sp->visit();
    ret->end();
    Director::getInstance()->getRenderer()->render();
        
        
        auto p = ShaderCache::getInstance()->getProgram("ShaderPositionTextureColorAlphaTest_NoMV");
        ret->getSprite()->setShaderProgram(p);
        lvl->autorelease();
        
    return lvl;
}

void Level::postUpdate()
{
    
}

Size Level::getContentSize()
{
    return _rt->getSprite()->getContentSize();
}