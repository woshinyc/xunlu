//
//  EnemySprite.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "EnemySprite.h"

typedef enum
{
    DirectionTypeUp = 1,
    DirectionTypeDown,
    DirectionTypeLeft,
    DirectionTypeRight

}DirectionType;


bool EnemySprite::initWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame)
{

	if (pSpriteFrame)
	{
		CCSprite::initWithSpriteFrame(pSpriteFrame);
		return true;
	}
	return false;
}
EnemySprite * EnemySprite::spriteWithSpriteFrame(CCSpriteFrame *spriteFrame)
{
    EnemySprite *enemy =new EnemySprite();
    enemy->initWithSpriteFrame(spriteFrame);
    enemy->reset();
    return enemy;
}

void EnemySprite::reset()
{
    m_speed = 100;
    m_blood = 0 ;
    this->setVisible(false);
    m_curPoint =CCPointZero;
    _direction = DirectionTypeRight;
    if (_wayPoints==NULL) {
        _wayPoints = new CCArray();
    }
//    if (_wayPoints!=NULL) {
//        _wayPoints->removeAllObjects();
//    }
}


void EnemySprite::movingForNewWayPoints(CKWayPoints *newPoints)
{
    int i = 0;
    int count = newPoints->count();
    for (; i<count; i++) {
        if (m_curPoint.equals(newPoints->pointAtIndex(i)->_point)) {
            break;
        }
    }
    if (i>=count) {
        return;
    }
    _wayPoints->removeAllObjects();
    for (; i<count; i++) {
        CKPoint *p = newPoints->pointAtIndex(i);
        _wayPoints->addObject(p);
    }
}
void EnemySprite::moveWithWayPoints(CKWayPoints * points,void(^finish)(void))
{
    
  
    _finishBlock =finish;
    if (!points) {
        return;
    }
    //设置position为起始点
    if (points->count() ==0) {
        return;
    }
    //保存要移动的路径点
    if (points->count() >1) {
        for (int i = 1; i<points->count(); i++) {
            CKPoint *p = points->pointAtIndex(i);
           
            _wayPoints->addObject(p);
        }
    }
    //第一个路径点
    if (_wayPoints->count()>0) {
        CKPoint *pp=(CKPoint *)_wayPoints->objectAtIndex(0);
        CCPoint firstP=pp->_point;
        CCLOG("firstPx = %f y=%f",firstP.x,firstP.y);
        this->moveTo(firstP);
    }
    
}

void EnemySprite::moveTo(CCPoint newPos)
{
    m_curPoint = newPos;
    CCPoint tmpPos;
    float xoffset = fabsf(newPos.x - this->getPosition().x);
    float yoffset = fabsf(newPos.y - this->getPosition().y);
    CCMoveTo *firstMove = NULL;
    CCMoveTo *secondMove = NULL;
    CCSequence *sequence = NULL;
    CCCallFuncND *funcFirst = NULL;
    CCCallFunc *funcSecond = CCCallFunc::create(this, callfunc_selector(EnemySprite::onCallSeconedMove));
    
    if (xoffset) {
        //CCTime duration = xoffset/m_speed;
        float duration= xoffset/m_speed;
        tmpPos = ccp(newPos.x,this->getPositionY());
        firstMove = CCMoveTo::create(duration, tmpPos);
    }
    if (yoffset) {
        float duration = yoffset/m_speed;
        secondMove = CCMoveTo::create(duration, newPos);
        static CCPoint pos;
        pos = newPos;
        funcFirst = CCCallFuncND::create(this, callfuncND_selector(EnemySprite::onCallFirstMoveandData), &pos);
    }
    if (firstMove && secondMove) {
        this->rotateFromPostoNewPos(this->getPosition(), tmpPos);
        sequence = CCSequence::create(firstMove,funcFirst,secondMove,funcSecond,NULL);
    }else if (firstMove)
    {
        this->rotateFromPostoNewPos(this->getPosition(), tmpPos);
        sequence =CCSequence::create(firstMove,funcSecond,NULL);
    
    }else if (secondMove){
        this->rotateFromPostoNewPos(this->getPosition(), newPos);
        sequence = CCSequence::create(secondMove,funcSecond,NULL);
    }
    
    if (sequence) {
        this->runAction(sequence);
    }
    
}
//完成了一次指定路径的移动
void EnemySprite::onCallSeconedMove()
{
    if (_wayPoints->count()>0) {
        _wayPoints->removeObjectAtIndex(0);
    }
    this->stopAllActions();
    if (_wayPoints->count()>0) {
        
         CKPoint *pp=(CKPoint *)_wayPoints->objectAtIndex(0);
        CCPoint newPos =pp->_point;
          //开始移动到下个指定路径
        this->moveTo(newPos);
    }
    else{
        if (_finishBlock) {
            _finishBlock();
        }
    }
}

//到达了本次指定路径的移动的中转点
void EnemySprite::onCallFirstMoveandData(CCNode *node,void* data)
{
    CCPoint *pos = (CCPoint *)data;
    this->rotateFromPostoNewPos(this->getPosition(), *pos);
}

void EnemySprite::rotateFromPostoNewPos(CCPoint pos,CCPoint newPos)
{
    CCPoint vectorPoint = ccpSub(newPos, pos);
    float angle = ccpToAngle(vectorPoint);
    float angleDegrees = 90 - CC_RADIANS_TO_DEGREES(angle);
    this->setRotation(angleDegrees);
}
