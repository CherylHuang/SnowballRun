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
	RepeatForever *_MoveAni; //���ưʵe
	CallFunc *_fadeoutCallback;

	bool _bVisible; // �O�_���

public:
	CEnemy(const int trackPos);
	~CEnemy();

	Sprite *_snowballPic1;
	Sprite *_snowballPic2;
	Sprite *_snowballPic3;

	int _iballs; //��X���y��
	int _ithrow; //�̫��X���y
	bool _bFadeOutDone; //�H�X

	bool throwSnowball();
	bool resetSnowBall();
	bool balloverwindow(); //�������y�O�_�W�X����
	void snowmanFadeOut(); //���H����
	void ActionFinished(); // CallBack �禡

	void setVisible(bool bVis); //�������
	void setSnowballVisible(int i, bool bVis); //���y���
	void pause(); //for animation
	void resume();

	//���H�D��
	void slideUp();
	void slideDown();

	//���y�I��
	float getSnowballX(int i); //�޼� : �ĴX�����y
	float getSnowballY();
	float getSnowballwidth();
};

#endif // __CENEMY_H__