//
//  EnemySprite.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__EnemySprite__
#define __xunlu__EnemySprite__

#include <iostream>
#include "cocos2d.h"
#include "CKAStar.h"
using namespace std;
using namespace cocos2d;

class EnemySprite :public CCSprite
{
//正面的朝向
public:
    int _direction;
    //CCArray *_wayPoints;
    
    
    CCArray *_wayPoints=NULL;
   // vector<CCPoint > _wayPoints;
   void(^_finishBlock)(void);
    float _moveDuration;
    float m_speed;
    int m_blood;
    CCPoint m_curPoint;
    void reset();
    
     bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    static EnemySprite * spriteWithSpriteFrame(CCSpriteFrame *spriteFrame);
    void moveWithWayPoints(CKWayPoints * points,void(^finish)(void));
    void movingForNewWayPoints(CKWayPoints *newPoints);
    
    void moveTo(CCPoint newPos);
    
    //完成了一次指定路径的移动
    void onCallSeconedMove();

    //到达了本次指定路径的移动的中转点
    void onCallFirstMoveandData(CCNode *node,void* data);
    void rotateFromPostoNewPos(CCPoint pos,CCPoint newPos);
    
  
};
#endif /* defined(__xunlu__EnemySprite__) */
