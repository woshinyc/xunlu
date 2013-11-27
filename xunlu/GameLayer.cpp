//
//  GameLayer.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "GameLayer.h"
#include "TowerHudLayer.h"




 bool GameLayer::init()
{

    
    if (CCLayer::init())
    {
          addMapLayer();
          addTowerHudLayer();
          this->schedule(schedule_selector(GameLayer::update));
        return true;
    }
    return false;
}

CCScene *GameLayer::scenee()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = new GameLayer();
    layer->init();
    scene->addChild(layer);
    return scene;
    
}

#pragma mark--加入触摸监测
void GameLayer::onEnter()
{
    setTouchEnabled(true);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCLayer::onEnter();
}

void GameLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
void GameLayer::addMapLayer()
{
    m_mapLayer = mapLayer::create();
    //左下角对齐
    m_mapLayer->setAnchorPoint(CCPointZero);
   // m_mapLayer->setScale(.6f);
    m_mapLayer->setPosition(ccp(0,-408));
    _tmpScale =m_mapLayer->getScale();
    _tmpPosition = m_mapLayer->getPosition();
    
    //支持缩放
   
    this->addChild(m_mapLayer);
    
}
void GameLayer::addTowerHudLayer()
{
    m_towerHudLayer = TowerHudLayer::towerHudLayer();
    this->addChild(m_towerHudLayer);
}


bool GameLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    
    _touchPoint = touch->getLocation();
    if (m_towerHudLayer->touchAtPoint(_touchPoint)) {
        _isTouchAtHudLayer = true;
        _newTower = NULL;
        switch (m_towerHudLayer->towerTag) {
            case 1:
                _newTower = (TowerSprite *)TowerSprite::createWithSpriteFrameName("en1.png");
                break;
            case 2:
                    _newTower = (TowerSprite *)TowerSprite::createWithSpriteFrameName("en1r.png");
                break;
            case 3:
                    _newTower = (TowerSprite *)TowerSprite::createWithSpriteFrameName("en6.png");
                break;
            case 4:
                    _newTower = (TowerSprite *)TowerSprite::createWithSpriteFrameName("en7.png");
                break;
                
            default:
                _newTower= NULL;
                break;
        }
        if (_newTower) {
            _newTower->reset();
            _newTower->m_speed = 2*m_towerHudLayer->towerTag;
             _newTower->m_attack = 5-m_towerHudLayer->towerTag;
             _newTower->m_range = 200*(0.75f+m_towerHudLayer->towerTag/4.0f);
            _newTower->setAnchorPoint(CCPointMake(0.5, 0.5));
            _newTower->setPosition(_touchPoint);
            this->addChild(_newTower);
            _newTower->setScale(1);
        }
    }else{
        _isTouchAtHudLayer=false;
    }
    
    
    
    return true;
}

void GameLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
        CCPoint newPoint = touch->getLocation();
    if (_isTouchAtHudLayer) {
        if (_newTower) {
            _newTower->setPosition(newPoint);
            CCPoint posAtMap = m_mapLayer->convertToNodeSpace(newPoint);
            CCPoint tilePos = m_mapLayer->tilePosFromLocation(posAtMap);
            
            //显示临时tower的攻击范围
            m_mapLayer->highlightRangeForTower(_newTower, tilePos);
        }
    }
    else{
        CCPoint newPos = ccpAdd(_tmpPosition, ccpSub(newPoint, _touchPoint));
        _tmpPosition = this->boundLayerPos(newPos);
//        CCLog("newPos .x= %f ,newPos.y=%f",newPos.x,newPos.y);
//        CCLog("x=%f y = %f",_tmpPosition.x,_tmpPosition.y);
        _touchPoint= newPoint;
    }
}

//防止移动后的点超出地图范围
CCPoint GameLayer::boundLayerPos(CCPoint newPos)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    newPos.x = MIN(newPos.x,0);
    newPos.x = MAX(newPos.x, -(m_mapLayer->tileMap->getContentSize().width*m_mapLayer->getScale()-winSize.width));
    newPos.y = MIN(newPos.y, 0);
    newPos.y = MAX(newPos.y, -(m_mapLayer->tileMap->getContentSize().height *m_mapLayer->getScale() - winSize.height));
    return newPos;
}

void GameLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    //当移动屏幕时不做处理
    if (!_isTouchAtHudLayer) {
        return;
    }
    //当添加新的tower时
    _isTouchAtHudLayer =false;
    CCPoint newPoint = touch->getLocation();
    //恢复被选中的按钮的透明度
    m_towerHudLayer->touchEnded();
    //取消显示攻击范围
    m_mapLayer->cancelHighlight();
    
    /*============================这个tower临时变量很重要==========================================
     _newTower是个全局指针,它一开始都是指向本次移动的新tower对象。但是这里的的计算量很大（有可能重新进行A＊寻路算法），
     在线程结束之前，有可能另一个touch发生了：此时_newTower将指向下一个新生成的tower。所以这里一开始就用一个临时变量保存
     本次touch时生成的tower，这样本次线程处理的永远都是本次生成的tower对象而不发生错误
     ===========================================================================================*/
    TowerSprite *tower = _newTower;
    tower->setVisible(false);
    
    
    //移动到towerHud的按钮图标上，表示放弃建造tower
    if (m_towerHudLayer->touchAtPoint(newPoint)) {
        tower->removeFromParentAndCleanup(true);
    }
    else{
        CCPoint posAtMap = m_mapLayer->convertToNodeSpace(newPoint);
        CCPoint tilePos = m_mapLayer->tilePosFromLocation(posAtMap);
        //如果该点可以建造tower,就把临时tower对象添加到map层
        if (m_mapLayer->isBuildableAtTilePos(tilePos)) {
            tower->retain();
            tower->removeFromParentAndCleanup(true);
            m_mapLayer->addTower(tower, tilePos);
            tower->setVisible(true);
            m_mapLayer->enemysMovingByNewWay();
            tower->release();
        }
        //否则就放弃并删除临时tower对象
        else{
            tower->removeFromParentAndCleanup(true);
        }
    }
}


void GameLayer::update()
{
     //CCLog("_tmpPosition .x=%f _tmpPosition.y=%f",_tmpPosition.x,_tmpPosition.y);
    if (_tmpPosition.equals(m_mapLayer->getPosition())==false) {
       
        m_mapLayer->setPosition(_tmpPosition);
    }
    if (m_mapLayer->getScale()!=_tmpScale) {
        m_mapLayer->setScale(_tmpScale);
    }
}