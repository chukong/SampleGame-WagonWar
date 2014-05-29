#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

typedef std::function<void(Ref*)> LoadResourceCallback;

class LoadResourceLayer : public LayerColor
{
public:
    static LoadResourceLayer* create(const LoadResourceCallback& callback);

    bool init(const LoadResourceCallback& callback);

    void addImage(const std::string& file);
    void addPlist(const std::string& plistFile,const std::string& imageFile);

    void startLoad();

    virtual void update(float delta);

    void loadingCallBack(Texture2D* tex);

    void loadPlists();
    
private:
    LoadResourceLayer();

    std::vector<std::string> _images;
    std::vector<std::string> _plists;

    LoadResourceCallback _onFinish;
    ui::LoadingBar* _loadingBar;

    float _targetValue;
    float _currValue;
    int _speed;

    int _resourceAmount;
    int _finish;
};
