//
//  MaskSprite.cpp
//  TankMultiplayer
//
//  Created by WuHao on 14-5-5.
//
//

#include "MaskSprite.h"
USING_NS_CC;

MaskSprite *MaskSprite::create(const std::string& filename, const std::string& filename2)
{
    MaskSprite *sprite = new MaskSprite();
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::I8);
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename2);
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    if(texture && sprite->initWithFile(filename) && sprite->initShader())
    {
        sprite->autorelease();
        sprite->_content = texture;

        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


bool MaskSprite::initShader()
{
    auto scache = ShaderCache::getInstance();
    auto p = scache->getProgram("masked");
    if(!p)
    {
        
        GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
                                                                         FileUtils::getInstance()->fullPathForFilename("masked.fsh").c_str())->getCString();
        if(!*fragSource)
        {
            return false;
        }
        p = new GLProgram();
        p->initWithByteArrays(ccPositionTextureColor_vert, fragSource);
        scache->addProgram(p, "masked");
    }
    setShaderProgram(p);
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    
    p->link();
    
    p->updateUniforms();
    
    buildCustomUniforms();

    return true;
}

void MaskSprite::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(MaskSprite::onDraw, this, transform, transformUpdated);
    cocos2d::Director::getInstance()->getRenderer()->addCommand(&_customCommand);
}

void MaskSprite::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    auto shader = getShaderProgram();
    shader->use();
    shader->setUniformsForBuiltins(transform);
    
    setCustomUniforms();
    
    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    GL::bindTexture2D( getTexture()->getName());
    
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, getTexture()->getName());
    //getShaderProgram()->setUniformLocationWith1i(_texture2, 0);
    
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _content->getName());
    glUniform1f(_texture1Location, 1);
    
    //
    // Attributes
    //
#define kQuadSize sizeof(_quad.bl)
    size_t offset = (size_t)&_quad;
    
    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glActiveTexture(GL_TEXTURE0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
}
void MaskSprite::buildCustomUniforms()
{
    _texture1Location= getShaderProgram()->getUniformLocation("texture1");
    getShaderProgram()->use();
    
    // Since sample most probably won't change, set it to 0 now.
    getShaderProgram()->setUniformLocationWith1i(_texture1Location, 1);
}

void MaskSprite::setCustomUniforms()
{
    //auto program = getShaderProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTexture()->getName());
    glUniform1f(_texture0Location, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _content->getName());
    glUniform1f(_texture1Location, 1);
}