//
//  CKAStar.cpp
//  xunlu
//
//  Created by Mac on 13-11-18.
//
//

#include "CKAStar.h"

#pragma mark -CKPoint
//--------------------CKPoint
CKPoint * CKPoint::pointWithCCPoint(CCPoint point)
{
    CKPoint *po=new CKPoint();
    po->initWithCGPoint(point);
    po->autorelease();
    return po;
}

void  CKPoint::initWithCGPoint(CCPoint point)
{
    _point=point;
    _hScore=0;
    _gScore=0;
    _parentPoint = NULL;

}

const char* CKPoint::description()
{
const char *strParent = "null";
    if (_parentPoint) {
       // strParent =
    }
    //return CCString::createWithFormat("CKPoint:%s, G:%.0f, H:%.0f, F:%.0f, _parentPoint:%s")
    return strParent;
}

bool CKPoint::isEqual(CKPoint *object)
{
    return _point.equals(object->_point);

}

#pragma mark -CKWayPoints
//----------------------------------CKWayPoints

CKWayPoints* CKWayPoints::wayPoint()
{
    CKWayPoints *wayPoint =CKWayPoints::create();
    return wayPoint;
}

 bool CKWayPoints::init()
{
    if (!CCNode::init()) {
        return false;
    }
    _points = new CCArray();//CCArray::create();
    return true;
}


CKPoint *CKWayPoints::pointAtIndex(int index)
{
    return (CKPoint *)_points->objectAtIndex(index);
}

void CKWayPoints::insertPoint(CKPoint *point, int index)
{
    _points->insertObject(point, index);
}

void CKWayPoints::addPoint(CKPoint *point,bool sort)
{
    if (sort) {
        int i;
        float cf;
        float f;
        for (i=0; i<_points->count(); i++) {
            CKPoint *curP = (CKPoint *)_points->objectAtIndex(i);
            f=point->getfScore();
            cf=curP->getfScore();
            if (f<cf) {
                _points->insertObject(point, i);
                break;
            }
            else if (f ==cf)
            {
                if (point->_gScore<curP->_gScore) {
                    _points->insertObject(point, i);
                    break;
                }
            }
        }
        if (i == _points->count()) {
            _points->addObject(point);
        }
    }
    else
    {
        _points->addObject(point);
    }
}


void CKWayPoints::addPoint(CKPoint *point)
{
    _points->addObject(point);
}

void CKWayPoints::replacePointsFromIndex(int index,CKWayPoints *newPoints)
{
    int c =_points->count();
    for (int i=index; i<c; i++) {
        _points->removeObjectAtIndex(index);
        
    }
    for (int i=0; i<newPoints->count(); i++) {
        _points->addObject(newPoints->pointAtIndex(i));
    }
}
void CKWayPoints::removeAllPoints()
{
    _points->removeAllObjects();
}
void CKWayPoints::removeLastPoint()
{
    _points->removeLastObject();
}
void CKWayPoints::removePointAtIndex(int index)
{
    _points->removeObjectAtIndex(index);
}
CKPoint * CKWayPoints::pointIfExistAtCCPoint(cocos2d::CCPoint cgpoint)
{
    for (int n=0;n<_points->count();n++) {
        CKPoint *p=(CKPoint *)_points->objectAtIndex(n);
        if (p->_point.equals(cgpoint)) {
            return p;
        }
    }
    return NULL;
}
CKPoint * CKWayPoints::lastPoint()
{
    return (CKPoint *)_points->lastObject();
}
void CKWayPoints::removePoint(CKPoint *point)
{
    for (int n=0; n<_points->count(); n++) {
        CKPoint *curPoint=(CKPoint *)_points->objectAtIndex(n);
        if (curPoint->isEqual(point)) {
            _points->removeObject(curPoint);
        }
    }
}
CKPoint *CKWayPoints::pointIfExist(CKPoint *point)
{
    for (int n=0; n<_points->count(); n++) {
        CKPoint *p=(CKPoint *)_points->objectAtIndex(n);
        if (p->isEqual(point)) {
            return p;
        }
    }
    return NULL;
}
int CKWayPoints::indexOfPoint(CKPoint *point)
{
    for (int n=0; n<_points->count(); n++) {
        CKPoint *p=(CKPoint *)_points->objectAtIndex(n);
        if (p->isEqual(point)) {
            return _points->indexOfObject(p);
        }
    }
    return -1;
}

#pragma mark -CKAStar
//----------------CKAStar

void CKAStar::reset()
{
    if (_open==NULL) {
        //_open->release();
        _open= CKWayPoints::wayPoint();
        
    }
    if (_close==NULL) {
         //  _close->release();
        _close =  CKWayPoints::wayPoint();
    }
    if (_path==NULL) {
       //  _path->release();
        _path=CKWayPoints::wayPoint();
    }
    if (_startPoint==NULL) {
       // _startPoint->release();
        _startPoint=NULL;
    }
    if (_endPoint==NULL) {
       // _endPoint->release();
        _endPoint = NULL;
    }
}

CKWayPoints * CKAStar::getWayPointsWithStartPoint(CCPoint sp,CCPoint ep,bool (^isUnimpeded)(CCPoint po))
{
    reset();
    _startPoint=CKPoint::pointWithCCPoint(sp);
    _endPoint=CKPoint::pointWithCCPoint(ep);
    _open->addPoint(_startPoint);
    
    //CCLOG("ep.x=%f ,ep.y=%f",ep.x,ep.y);
    
    
    while (_open->count()>0) {
        CCLOG("_open->count() = %d",_open->count());
//        if (_open->count()==58) {
//            CCLOG("NOW");
//        }
        CKPoint *curPoint = _open->pointAtIndex(0);
       
//        if (_open->count()>2) {
//             CKPoint *curP= _open->pointAtIndex(1);
//            CKPoint *xx=_open->lastPoint();
//            CCLOG("x1 = %f ,y1=%f",curPoint->_point.x,curPoint->_point.y);
//            CCLOG("x2 = %f ,y2=%f",curP->_point.x,curP->_point.y);
//             CCLOG("x2 = %f ,y2=%f",xx->_point.x,xx->_point.y);
//        }
       
        _close->addPoint(curPoint);
        _open->removePointAtIndex(0);
        //找到路径，返回
        if (curPoint->isEqual(_endPoint)) {
            _path->addPoint(curPoint);
            
            while ((curPoint=curPoint->_parentPoint)) {
                _path->insertPoint(curPoint, 0);
            }
            break;
        }
        
        CKWayPoints *nearbyPoints=this->nearbyWalkablePointsForPosition(curPoint, isUnimpeded);
        CCLOG("aa=%d",nearbyPoints->count());
        for (int i=0; i<nearbyPoints->count(); i++) {
            CKPoint *nearP = nearbyPoints->pointAtIndex(i);
            //close表中已存在则忽略
            if (_close->pointIfExist(nearP)) {
                continue;
            }
            CKPoint *p=_open->pointIfExist(nearP);
            if (p!=NULL) {
                if (p->_gScore>nearP->_gScore) {
                    p->_gScore=nearP->_gScore;
                    p->_parentPoint = nearP->_parentPoint;
                    p->retain();
                    _open->removePoint(p);
                    _open->addPoint(p, true);
                    p->release();
                    
                }
            }
            //不在open表中则添加
            else {
                _open->addPoint(nearP, true);
            }
            
        }
        
    }
    return _path;
}
CKWayPoints * CKAStar::nearbyWalkablePointsForPosition(CKPoint * position,bool (^isUnimpeded)(CCPoint po))
{
    CCPoint p = position->_point;
    CCPoint tmpP;
    CKWayPoints *nearbyPoints=CKWayPoints::wayPoint();
    tmpP = CCPointMake(p.x-1, p.y);
    //left
    if (isUnimpeded(tmpP)) {
        CKPoint *point = CKPoint::pointWithCCPoint(tmpP);
        if (!_close->pointIfExistAtCCPoint(tmpP)) {
            point->_hScore=this->getHValueForCGPoint(tmpP);
            point->_gScore=position->_gScore+1;
           
            point->_parentPoint=position;
            nearbyPoints->addPoint(point);
        }
    }
    //right
    tmpP = CCPointMake(p.x+1, p.y);
    if (isUnimpeded(tmpP)) {
        CKPoint *point = CKPoint::pointWithCCPoint(tmpP);
        if (!_close->pointIfExistAtCCPoint(tmpP)) {
            point->_hScore=this->getHValueForCGPoint(tmpP);
            point->_gScore=position->_gScore+1;
            point->_parentPoint=position;
           
            nearbyPoints->addPoint(point);
        }
    }
    // upper
    tmpP=CCPointMake(p.x, p.y-1);
    if (isUnimpeded(tmpP)) {
        CKPoint *point = CKPoint::pointWithCCPoint(tmpP);
        if (!_close->pointIfExistAtCCPoint(tmpP)) {
            point->_hScore=this->getHValueForCGPoint(tmpP);
            point->_gScore=position->_gScore+1;
            point->_parentPoint=position;
            
            nearbyPoints->addPoint(point);
            
        }
    }
    //under
    tmpP = CCPointMake(p.x, p.y+1);
    if (isUnimpeded(tmpP)) {
        CKPoint *point = CKPoint::pointWithCCPoint(tmpP);
        if (!_close->pointIfExistAtCCPoint(tmpP)) {
            point->_hScore=this->getHValueForCGPoint(tmpP);
            point->_gScore=position->_gScore+1;
            point->_parentPoint=position;
        
            nearbyPoints->addPoint(point);
        }
    }
    return nearbyPoints;
}
int CKAStar::getHValueForCGPoint(CCPoint p)
{

    int h=abs(_endPoint->_point.x-p.x)+abs(_endPoint->_point.y-p.y);
    return h;
}