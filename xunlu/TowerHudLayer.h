//
//  TowerHudLayer.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__TowerHudLayer__
#define __xunlu__TowerHudLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class TowerHudLayer:public CCLayer
{
public:
    CCArray *_towers;
    int towerTag;
   static TowerHudLayer *towerHudLayer();
    int touchAtPoint(CCPoint point);
    void touchEnded();
    
      virtual bool init();
    CREATE_FUNC(TowerHudLayer);
};
#endif /* defined(__xunlu__TowerHudLayer__) */
