//
//  mapLayer.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "mapLayer.h"


#define  _Scale  CCDirector::sharedDirector()->getContentScaleFactor()
#define _MaxEnemy 10


typedef enum
{
    ChildTagSpriteBatch = 100

}ChildTag;


bool mapLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    //创建A *寻路算法对象
    _aAlgorithm = new CKAStar();
    this->reloadTileMapByName("tileMap1.tmx");
    return true;
}

void mapLayer::reloadTileMapByName(const char *tileMapName)
{
   //清理
    this->removeAllChildrenWithCleanup(true);
    
    //加载地图
    tileMap =NULL;
    tileMap=CCTMXTiledMap::create(tileMapName);
    tileMap->setAnchorPoint(CCPointZero);
    this->addChild(tileMap, -1);
    //初始化敌人移动的起点和终点
    
    CCTMXObjectGroup * objectLayer = tileMap->objectGroupNamed("objects");
    CCDictionary *startdic = objectLayer->objectNamed("startPos");
    _startLocation = CCPointZero;
   // CCObject *obj=startdic->objectForKey("x");
   // CCString *st=(CCString *)obj;
    _startLocation.x = atof(((CCString *)startdic->objectForKey("x"))->m_sString.c_str());
    _startLocation.y = atof(((CCString *)startdic->objectForKey("y"))->m_sString.c_str());
   
    CCDictionary *enddic = objectLayer->objectNamed("endPos");
    _endLocation=CCPointZero;
    _endLocation.x =atof(((CCString *)enddic->objectForKey("x"))->m_sString.c_str());
    _endLocation.y =atof(((CCString *)enddic->objectForKey("y"))->m_sString.c_str());
    
    if (!_arrTowers) {
        _arrTowers =new CCArray();
    }else{
        _arrTowers->removeAllObjects();
    }
    //根据A* 寻路算法获得敌人移动的路径点
    this->getEnemyWayPoints();
    
    //创建敌人
    this->createEnemys();
    
}


void mapLayer::createEnemys()
{
    if (!_arrEnemys) {
        _arrEnemys = new CCArray();
    }else{
        _arrEnemys ->removeAllObjects();
    }
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("images.plist");
    CCSpriteBatchNode *bachNode = CCSpriteBatchNode::create("images.pvr.ccz");
    this->addChild(bachNode, 0, ChildTagSpriteBatch);
    for (int i=0; i<_MaxEnemy; i++) {
        CCString *spriteName = CCString::createWithFormat("p%d.png",1);
        EnemySprite *enemy = EnemySprite::spriteWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteName->m_sString.c_str()));
        enemy->setVisible(false);
        enemy->setScale(_Scale);
        enemy->setAnchorPoint(ccp(0.5, 0.5));
        enemy->setPosition(_startLocation);
        enemy->m_speed=CCRANDOM_0_1()*80+60;
        bachNode->addChild(enemy);
       // addChild(enemy);
        _arrEnemys->addObject(enemy);
    }
    this->schedule(schedule_selector(mapLayer::updateEnemy), .8f);
    
}
void mapLayer::getEnemyWayPoints()
{
    this->wayPoints=NULL;
    this->wayPoints=_aAlgorithm->getWayPointsWithStartPoint(this->tilePosFromLocationAndtileMap(_startLocation, tileMap),
                                                           this->tilePosFromLocationAndtileMap(_endLocation, tileMap),
                                                            ^bool(cocos2d::CCPoint po) {
                                                                return this->isPassableAtTilePos(po);
                                                            });
    
    this->wayPointsForLocation=NULL;
    this->wayPointsForLocation=this->getLocationWayPointsFromTilePoints(wayPoints);
    
    
}

//将tile路径点转换到gl坐标
CKWayPoints * mapLayer::getLocationWayPointsFromTilePoints(CKWayPoints *tilePoints)
{
    CKWayPoints *locationPoints = CKWayPoints::wayPoint();//[CKWayPoints wayPoints];
    for (int i=0; i<tilePoints->count(); i++) {
        CCPoint p=tilePoints->pointAtIndex(i)->_point;
        CKPoint *lp=CKPoint::pointWithCCPoint(this->locationForTilePos(p));
        locationPoints->addPoint(lp);
    }
    return locationPoints;
}


void mapLayer::addTower(TowerSprite * tower ,CCPoint pos)
{
    CCPoint location = this->locationForTilePos(pos);
    tower->setPosition(location);
    this->addChild(tower, 1);
    _arrTowers->addObject(tower);
}
void mapLayer::updateEnemy()
{
    CCSpriteBatchNode *batch = (CCSpriteBatchNode *)this->getChildByTag(ChildTagSpriteBatch);
    int count=batch->getChildrenCount();
    if (batch) {
        static int movedCount=0;
        static bool allowMove = true;
        CCArray *childArr=batch->getChildren();
        for (int n=0; n<count; n++) {
            EnemySprite *enemy = (EnemySprite *)childArr->objectAtIndex(n);
            if (!enemy->isVisible()&&allowMove) {
                enemy->setVisible(true);
                if (++movedCount>=count) {
                    allowMove =false;
                    }
                    enemy->moveWithWayPoints(this->wayPointsForLocation, ^{
                        enemy->setVisible(false);
                        if (--movedCount==0) {
                            allowMove=true;
                             this->getEnemyWayPoints();
                        }
                    });
                    break;
            }
        }
    }
}

void mapLayer::enemysMovingByNewWay()
{
    CCSpriteBatchNode *batch = (CCSpriteBatchNode*)this->getChildByTag(ChildTagSpriteBatch);
    if (batch) {
        EnemySprite *enemy=NULL;
        CCArray *array= batch->getChildren();
        for (int n=0; n<batch->getChildrenCount(); n++) {
            enemy=(EnemySprite *)array->objectAtIndex(n);
            if (enemy->isVisible()) {
                enemy->movingForNewWayPoints(this->wayPointsForLocation);
            }
        }
    }
}
CCPoint mapLayer::tilePosFromLocation(CCPoint location)
{
    return this->tilePosFromLocationAndtileMap(location, tileMap);
}
CCPoint mapLayer::tilePosFromLocationAndtileMap(CCPoint location ,CCTMXTiledMap *tileMap)
{
    CCPoint pos = CCPointZero;
    pos.x=(int)(location.x/tileMap->getTileSize().width);
    pos.y=(int)(tileMap->getMapSize().height - location.y/tileMap->getTileSize().height);
    CCLOG("ffffx=%f,y=%f",pos.x,pos.y);
    return pos;
}

CCPoint mapLayer::locationForTilePos(CCPoint pos)
{
    return this->locationForTilePostileMap(pos, tileMap);
}
CCPoint mapLayer::locationForTilePostileMap(CCPoint pos ,CCTMXTiledMap*tileMap)
{
    float x=(pos.x * tileMap->getTileSize().width + tileMap->getTileSize().width*0.5f);
    float tileHeight = tileMap->getTileSize().height;
    float y =tileHeight * tileMap->getMapSize().height-(tileHeight*pos.y + tileHeight/2.0f);
    CCPoint location = ccp(x,y);
    return location;
}
bool mapLayer::isPassableAtTilePos(CCPoint pos)
{
    if (pos.x<0 || pos.y<0 ||pos.x >= tileMap->getMapSize().width || pos.y >= tileMap->getMapSize().height) {
        return false;
    }
    CCTMXLayer *background = tileMap->layerNamed("background");
    int tileGID = background->tileGIDAt(pos);
    CCDictionary *dic = tileMap->propertiesForGID(tileGID);
    if (dic) {
        CCString *buildable= (CCString *)dic->valueForKey("buildable");
        if (atoi(buildable->m_sString.c_str())!=1) {
            return false;
        }
    }
    else{
        return false;
    }
    TowerSprite *tower;
    for (int n=0; n<_arrTowers->count(); n++) {
        tower=(TowerSprite *)_arrTowers->objectAtIndex(n);
        if (this->tilePosFromLocation(tower->getPosition()).equals(pos)) {
            return false;
        }
    }
 
    return true;
    
}
void mapLayer::highlightRangeForTower(TowerSprite *tower,CCPoint pos)
{
    this->highlightRangeForTowerAtTilePoswithColor(tower, pos, NULL);
}

void mapLayer::highlightRangeForTowerAtTilePoswithColor(TowerSprite*tower ,CCPoint pos,ccColor4B *color)
{
    CCPoint position = this->locationForTilePostileMap(pos, tileMap);
    if (!_highlightMask) {
        _highlightMask=CCSprite::create("Range.png");
        _highlightMask->setAnchorPoint(ccp(.5f,.5f));
        this->addChild(_highlightMask, -1);
    }
    if (color) {
        ccColor4B c4 = *color;
        _highlightMask->setColor(ccc3(c4.r, c4.g, c4.b));
        _highlightMask->setOpacity(c4.a);
    }
    _highlightMask->setScale(_Scale*tower->m_range*0.01);
    _highlightMask->setPosition(position);
}

bool mapLayer::isBuildableAtTilePos(CCPoint pos)
{
    if (!this->isPassableAtTilePos(pos)) {
        return false;
    }
    if (!wayPoints) {
        return false;
    }
    CKWayPoints *newPoints;
    int prevIndex;
    CCLOG("wayPoints->count() = %d",wayPoints->count());
    for (int i =0; i<wayPoints->count(); i++) {
        CKPoint *point = wayPoints->pointAtIndex(i);
        //如果建造的点刚好落在路径点上
        if (point->_point.equals(pos)) {
            //当为路径的起点或者终点时，返回NO
            if (i == wayPoints->count() -1 ||i==0) {
                return false;
            }
            //前一个路径点
            prevIndex= i -1;
            CKPoint *prev = wayPoints->pointAtIndex(prevIndex);
            newPoints=_aAlgorithm->getWayPointsWithStartPoint(prev->_point,
                                                              wayPoints->lastPoint()->_point,
                                                              ^bool(cocos2d::CCPoint po) {
                                                                  if (this->isPassableAtTilePos(po)) {
                                                                      if (!po.equals(pos)) {
                                                                          return true;
                                                                      }
                                                                  }
                                                                  return false;
                                                              });
            //如果没有新的路径，返回false
            if (!newPoints) {
                return false;
            }
            if (newPoints->count()==0) {
                return false;
            }
            //获取新的路径点
            wayPoints->replacePointsFromIndex(prevIndex, newPoints);
            this->wayPointsForLocation = NULL;
            this->wayPointsForLocation = this->getLocationWayPointsFromTilePoints(wayPoints);
            break;
        }
    }
    return true;
}

//取消tower的攻击范围的显示
void mapLayer::cancelHighlight()
{
    _highlightMask->removeFromParentAndCleanup(true);
    _highlightMask=NULL;

}
