#pragma once
#ifndef __COBSTACLE_H__
#define __COBSTACLE_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

#define SMALL_ROCK 0
#define BIG_ROCK 1

#define TREE_ONE 0
#define TREE_TWO 1

#define BG_TRACK 4
#define UPPER_TRACK 3
#define MIDDLE_TRACK 2
#define LOWER_TRACK 1

USING_NS_CC;

class CObstacle : public cocos2d::Node
{
private:
	cocos2d::Node   *_objRoot;
	cocos2d::Sprite *_rockPic;
	cocos2d::MoveBy   *_Move;
	cocos2d::CallFunc *_moveCallback;
	cocos2d::Sprite *_objPos; //�����m

	int _iType; //��ê������
	int _iBGType; //�I��(��)����
	float _fScale;	// �Y��j�p

public:
	CObstacle(const int track);
	~CObstacle();

	CObstacle *link; //for link list
	cocos2d::Size	_obstacleSize;

	bool _bVisible; // �O�_���
	bool _bMoveDone; // �ʵe����
	void moveActionFinished(); // CallBack �禡

	int getType(); //���o���Y����
	void setVisible(bool bVis);
	bool getVisible();
	void setScale(float scale);
	float getPositionX();
	float getPositionY();
	void pause(); //for animation
	void resume();

	void setColor(); //for test
};

#endif // __COBSTACLE_H__