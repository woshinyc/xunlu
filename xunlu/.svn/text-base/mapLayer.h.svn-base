//
//  mapLayer.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__mapLayer__
#define __xunlu__mapLayer__

#include <iostream>
#include "CKAStar.h"
#include "EnemySprite.h"
#include "TowerSprite.h"
#include "cocos2d.h"

using namespace cocos2d;
class mapLayer:public CCLayer
{
public:
    CCPoint _startLocation;
    CCPoint _endLocation;
    
    CCArray *_arrEnemys;
    CCArray *_arrTowers;
    
    CKAStar *_aAlgorithm;
    
    CCSprite *_highlightMask;
    
    CKWayPoints *wayPoints;
    CKWayPoints *wayPointsForLocation;
    CCTMXTiledMap *tileMap;
    
    void addTower(TowerSprite * tower ,CCPoint pos);
    void enemysMovingByNewWay();
   
   // -(void)highlightRangeForTower:(TowerSprite*)tower AtTilePos:(CGPoint)pos withColor:(ccColor4B *)color
    void highlightRangeForTowerAtTilePoswithColor(TowerSprite*tower ,CCPoint pos,ccColor4B *color);
    void highlightRangeForTower(TowerSprite *tower,CCPoint pos);
    void cancelHighlight();
    bool isBuildableAtTilePos(CCPoint pos);
    bool isPassableAtTilePos(CCPoint pos);
    void reloadTileMapByName(const char *tileMapName);
    void getEnemyWayPoints();
    //创建敌人
    void createEnemys();
    void updateEnemy();//每隔一段时间出现一个敌人

    virtual bool init();
    CREATE_FUNC(mapLayer);
    
    
    
    //tileMap
    //从gl坐标获得tile坐标
    CCPoint tilePosFromLocation(CCPoint location);
    CCPoint tilePosFromLocationAndtileMap(CCPoint location ,CCTMXTiledMap *tileMap);
    
    //从tile坐标获得gl坐标
    CKWayPoints * getLocationWayPointsFromTilePoints(CKWayPoints *tilePoints);
    CCPoint locationForTilePos(CCPoint pos);
    CCPoint locationForTilePostileMap(CCPoint pos ,CCTMXTiledMap*tileMap);


};
#endif /* defined(__xunlu__mapLayer__) */
