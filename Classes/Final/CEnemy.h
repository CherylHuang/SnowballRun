#pragma once
#ifndef __CENEMY_H__
#define __CENEMY_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d;

class CEnemy : public cocos2d::Node
{
private:
	Node   *_objRoot;
	ActionTimeline *_objAction;
	Sprite *_SnowMan;
	Node   *_snowballPos;
	MoveBy *_Move;
	RepeatForever *_MoveAni; //重複動畫
	CallFunc *_fadeoutCallback;

	bool _bVisible; // 是否顯示

public:
	CEnemy(const int trackPos);
	~CEnemy();

	Sprite *_snowballPic1;
	Sprite *_snowballPic2;
	Sprite *_snowballPic3;

	int _iballs; //丟出雪球數
	int _ithrow; //最後丟出的球
	bool _bFadeOutDone; //淡出

	bool throwSnowball();
	bool resetSnowBall();
	bool balloverwindow(); //偵測雪球是否超出視窗
	void snowmanFadeOut(); //雪人消失
	void ActionFinished(); // CallBack 函式

	void setVisible(bool bVis); //全部顯示
	void setSnowballVisible(int i, bool bVis); //雪球顯示
	void pause(); //for animation
	void resume();

	//跟隨主角
	void slideUp();
	void slideDown();

	//雪球碰撞
	float getSnowballX(int i); //引數 : 第幾顆雪球
	float getSnowballY();
	float getSnowballwidth();
};

#endif // __CENEMY_H__