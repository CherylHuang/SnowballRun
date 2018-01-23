#pragma once
#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "Final/C3SButton.h"

class MainMenu : public cocos2d::Layer
{
private:
	bool settingVisible;
	bool easyisSelect;
	bool normalisSelect;
	bool hardisSelect;
	cocos2d::Sprite *_black_bg; //黑幕遮屏
	cocos2d::Sprite *_settingWindow;
	C3SButton *_startbtn;
	C3SButton *_settingbtn;
	C3SButton *_exitbtn;
	C3SButton *_okaybtn;
	
	//level btn
	C3SButton *_easybtn;
	C3SButton *_normalbtn;
	C3SButton *_hardbtn;

public:
	~MainMenu();
	static cocos2d::Scene* createScene();
	virtual bool init();
	void doStep(float dt);
	static int level; //紀錄選擇關卡

	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

	CREATE_FUNC(MainMenu);
};
#endif