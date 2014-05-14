//
//  MaskSprite.h
//  TankMultiplayer
//
//  Created by WuHao on 14-5-5.
//
//

#ifndef __TankMultiplayer__MaskSprite__
#define __TankMultiplayer__MaskSprite__

#include "cocos2d.h"


class MaskSprite : public cocos2d::Sprite
{
public:
    void setContent(const std::string& filename);
    cocos2d::Texture2D * getContent();
    bool initShader();
    
    static MaskSprite* create(const std::string& filename, const std::string& filename2);
protected:
    cocos2d::Texture2D * _content;
    
    virtual void draw(cocos2d::Renderer* renderer, const kmMat4 &transform, bool transformUpdated);
    void onDraw(const kmMat4 &transform, bool transformUpdated);
    virtual void buildCustomUniforms();
    virtual void setCustomUniforms();
    
    
    cocos2d::CustomCommand _customCommand;
    GLuint _texture0Location;
    GLuint _texture1Location;
};


#endif /* defined(__TankMultiplayer__MaskSprite__) */
