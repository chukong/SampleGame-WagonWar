#include "AppDelegate.h"
#include "GameScene.h"
#include "Configuration.h"
#include "MainScreenScene.h"
#include "WagonSelect.h"


//#include "MainScreenScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        int height, width;
        height = 640;
        width = height*(960.0/640.0);
        
        glview = GLView::createWithRect("TankMultiplayer", Rect(0, 0, 960, 640));
        
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::FIXED_HEIGHT);
    log("%f, scale", director->getContentScaleFactor());
    log("%f, scale2", director->getWinSize().width/ director->getOpenGLView()->getFrameSize().width  );
    // turn on display FPS
//    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    g_gameConfig.lazyInit();
    
    FileUtils::getInstance()->addSearchPath("sfx");
    FileUtils::getInstance()->addSearchPath("particles");
    FileUtils::getInstance()->addSearchPath("fonts");

    
    // create a scene. it's an autorelease object
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   auto scene = MainScreenScene::createScene(false);
#else
   auto scene = GameScene::createScene();
#endif
    
    //auto scene = MainScreenScene::createScene(false);
    //auto scene = WagonSelect::createScene(FIRST_TURN);

    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
