//
//  CKAStar.h
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#ifndef __xunlu__CKAStar__
#define __xunlu__CKAStar__

#include <iostream>


#include "cocos2d.h"
using namespace cocos2d;


class CKPoint:public CCNode
{
    
private:
 
  
public:
    CKPoint *_parentPoint;
     const char* description();
    inline float getfScore(){ return  _hScore+_gScore;}

  static  CKPoint * pointWithCCPoint(CCPoint point);
    void  initWithCGPoint(CCPoint point);

    bool isEqual(CKPoint *object);
    
    CCPoint _point;
  

    float _hScore;
    float _gScore;
};



class CKWayPoints:public CCNode
{
public:

    CREATE_FUNC(CKWayPoints);
    virtual bool init();
    CCArray * _points;
//
    inline int count(){ if(_points)
    {
        return _points->count();
    }else{
        return 0;
    };}
   
    
    int indexOfPoint(CKPoint *point);
    
  static  CKWayPoints* wayPoint();
    void addPoint(CKPoint *point,bool sort);
    void addPoint(CKPoint *point);
    void replacePointsFromIndex(int index,CKWayPoints *newPoints);
    CKPoint *pointIfExistAtCCPoint(CCPoint cgpoint);
    CKPoint *pointIfExist(CKPoint *point);
    CKPoint *lastPoint();
    CKPoint *pointAtIndex(int index);
    void removeAllPoints();
    void removeLastPoint();
    void removePointAtIndex(int index);
    void removePoint(CKPoint *point);
    void insertPoint(CKPoint *point,int index);
    
    
};

class CKAStar:public CCNode
{
    void reset();
public:
    
    CKWayPoints * _open;
    CKWayPoints * _close;
    
    CKPoint *_startPoint;
    CKPoint *_endPoint;
    CKWayPoints *_path;
    //CCPoint sp,CCPoint ep,
    
    CKWayPoints * getWayPointsWithStartPoint(CCPoint sp,CCPoint ep,bool (^isUnimpeded)(CCPoint po));
//    -(CKWayPoints*)nearbyWalkablePointsForPosition:(CKPoint*)position UnimpededBlock:(BOOL(^)(CGPoint po))isUnimpeded
    CKWayPoints * nearbyWalkablePointsForPosition(CKPoint * position,bool (^isUnimpeded)(CCPoint po));
    int getHValueForCGPoint(CCPoint p);

    
};
#endif /* defined(__xunlu__CKAStar__) */
