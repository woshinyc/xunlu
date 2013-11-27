//
//  TowerHudLayer.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "TowerHudLayer.h"
#include "TowerSprite.h"

static TowerHudLayer *_towerHud;
TowerHudLayer *TowerHudLayer::towerHudLayer()
{
    if (_towerHud==0) {
        _towerHud = (TowerHudLayer *)TowerHudLayer::create();
    }
    return _towerHud;
}

bool TowerHudLayer::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setAnchorPoint(CCPointZero);
        towerTag =0;
        
        _towers= CCArray::create();
        for (int i=0; i<4; i++) {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("images.plist");
            TowerSprite *tower = NULL;
            switch (i) {
                case 0:
                    tower = (TowerSprite *)CCSprite::createWithSpriteFrameName("en1.png");
                    break;
                case 1:
                    tower = (TowerSprite *)CCSprite::createWithSpriteFrameName("en1r.png");
                    break;
                case 2:
                    tower = (TowerSprite *)CCSprite::createWithSpriteFrameName("en6.png");
                    break;
                case 3:
                    tower = (TowerSprite *)CCSprite::createWithSpriteFrameName("en7.png");
                    break;
                    
                default:
                    break;
            }
            if (!tower) {
                continue;
            }
            float scale = 1.0;
            tower->setAnchorPoint(ccp(0.5,0.5));
            tower->setScale(scale);
            tower->setPosition(ccp(winSize.width-tower->getContentSize().width*scale/2.0-5, 35+i*(tower->getContentSize().height*scale+7)));
            tower->setOpacity(170);
            tower->setTag(i+1);
            tower->m_speed=(i+1)*2;
            tower->m_attack =1;
            this->addChild(tower);
            
        }
        
    }

    return true;
}

int TowerHudLayer::touchAtPoint(CCPoint point)
{
    for (int n=0; n<this->getChildren()->count(); n++) {
        TowerSprite *sprite = (TowerSprite *)this->getChildren()->objectAtIndex(n);
        if (sprite->boundingBox().containsPoint(point)) {
            sprite->setOpacity(255);
            towerTag = sprite->getTag();
            return towerTag;
        }
    }
    return 0;
}

void TowerHudLayer::touchEnded()
{
    TowerSprite *sprite = (TowerSprite *)this->getChildByTag(towerTag);
    if (sprite) {
        sprite->setOpacity(170);
    }
}