#ifndef __CRUNNER_H__
#define __CRUNNER_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "SimpleAudioEngine.h"

#define RUNNERCOLOR1 cocos2d::Color3B(209,73,63) //RED
#define RUNNERCOLOR2 cocos2d::Color3B(200,209,63) //YELLOW
#define RUNNERCOLOR3 cocos2d::Color3B(75,155,77) //GREEN
#define RUNNERCOLOR4 cocos2d::Color3B(82,131,151); //BLUE

using namespace cocos2d;

class CRunner
{
private:

	unsigned int _eid; //sound

	Node   *_runnerRoot;
	Node   *_normalFace;
	Node   *_happyFace;
	Node   *_depressedFace;
	Node   *_jumpPart;
	Sprite *_body;
	Sprite *_shadow;
	Action *_action;

	Rect	_RunnerRect;
	Point	_RunnerLoc;

public:
	CRunner(const char *csbname, Layer &parent);
	~CRunner();

	cocos2d::Size	_bodySize;

	float getPositionX();
	float getPositionY();
	void setPosition(const Point &pos);
	void setPosition(const float x, const float y);
	void setColor(const Color3B& color);
	void setScale(const float scale);
	void setAnimation(const char *plistFile);
	void setVisible(const bool visible);
	void runAction();
	void stopAllActions();
	void setjumpTag(int tag); //for jump part

	//表情設定
	void setNormalFace();
	void setHappyFace();
	void setDepressedFace();

	//碰撞
	bool playerCollision(cocos2d::Point inPos);
	float getBodySize();
};

#endif