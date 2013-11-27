//
//  GameLayer.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__GameLayer__
#define __xunlu__GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "TowerSprite.h"
#include "mapLayer.h"
#include "TowerHudLayer.h"
#include "cocos-ext.h"
using namespace cocos2d;
USING_NS_CC_EXT;
class GameLayer :public CCLayer
{
public:
    CCPoint _touchPoint;
    CCPoint _tmpPosition;
    float _tmpScale;
    bool _isTouchAtHudLayer;
    
    TowerSprite *_newTower;
    
    static CCScene *scenee();
    mapLayer *m_mapLayer;
    TowerHudLayer *m_towerHudLayer;
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    
    //--
    void addMapLayer();
    void addTowerHudLayer();
    
    //防止移动后的点超出地图范围
    CCPoint boundLayerPos(CCPoint newPos);
    
    //
    void update();
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch , CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch,CCEvent *event);

};
#endif /* defined(__xunlu__GameLayer__) */
