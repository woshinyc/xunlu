//
//  LoadingScene.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__LoadingScene__
#define __xunlu__LoadingScene__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
typedef enum
{
    TargetSceneINVALID = 0,
    TargetSceneFirstScene,
    TargetSceneOtherScene,
    TargetSceneMAX,
}TargetScenes;

class LoadingScene :CCLayer
{
public:
//    virtual bool init();
//    static cocos2d::CCScene *scene();
//    CREATE_FUNC(LoadingScene);
    
    void update(CCTime delta);
   static void * sceneWithTargetScene(TargetScenes targetScene);
    bool initWithTargetScene(TargetScenes targetScene);
    TargetScenes _targetScene;

};

#endif /* defined(__xunlu__LoadingScene__) */
