//
//  LoadingScene.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "LoadingScene.h"
#include "GameLayer.h"


void * LoadingScene::sceneWithTargetScene(TargetScenes targetScene)
{
    CCScene *scene = CCScene::create();
   LoadingScene*ls=  new LoadingScene();
    ls->initWithTargetScene(targetScene);
    ls->autorelease();
    scene->addChild(ls);
    return scene;
}

bool  LoadingScene::initWithTargetScene(TargetScenes targetScene)
{
    if (!CCLayer::init()) {
        return false;
    }
    
    _targetScene=targetScene;
    CCLabelTTF *label = CCLabelTTF::create("Loading...", "Marker Felt", 64);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    label->setPosition(ccp(size.width*0.5, size.height*0.5));
    addChild(label);
    this->schedule(schedule_selector(LoadingScene::update), 1/60.0f);
    return true;
}

void LoadingScene::update(CCTime delta)
{
    this->unscheduleAllSelectors();
    switch (_targetScene) {
        case TargetSceneFirstScene:
            CCDirector::sharedDirector()->replaceScene(GameLayer::scenee());
            break;
        case TargetSceneOtherScene:
            ;
            break;
            
        default:
            break;
    }

}

