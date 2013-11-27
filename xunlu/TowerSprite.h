//
//  TowerSprite.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__TowerSprite__
#define __xunlu__TowerSprite__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class TowerSprite: public CCSprite
{
public:
    float m_attack;
    float m_speed;
    float m_range;
    void reset();
};

#endif /* defined(__xunlu__TowerSprite__) */
