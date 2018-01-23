#include "MainMenu.h"
#include "Final/C3SButton.h"
#include "cocostudio/CocoStudio.h"
#include "AnimationAction.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

int MainMenu::level = 0; //靜態變數初始值

Scene* MainMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

MainMenu::~MainMenu()
{
	AnimationCache::destroyInstance();  // 釋放 AnimationCache 取得的資源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("rungame.plist");
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("MainMenu.csb");
	addChild(rootNode);

	//變數初始值
	settingVisible = false;
	easyisSelect = normalisSelect = hardisSelect = false;
	level = 1; //沒有選關卡

	//讀取物件
	_black_bg = dynamic_cast<Sprite*>(rootNode->getChildByName("black_bg"));
	_settingWindow = dynamic_cast<Sprite*>(rootNode->getChildByName("setting_window"));

	// Button -------------------------------------------------------------------------------------
	//Type 1
	auto BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_start")); //開始鈕
	Point loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_startbtn = C3SButton::create();
	_startbtn->setButtonInfo(NORMAL_BTN, "btn_start_0.png", "btn_start_2.png", "btn_start_1.png", loc);
	_startbtn->setScale(0.65f);
	this->addChild(_startbtn, 2);
	
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_setting")); //設定鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_settingbtn = C3SButton::create();
	_settingbtn->setButtonInfo(NORMAL_BTN, "btn_setting_0.png", "btn_setting_2.png", "btn_setting_1.png", loc);
	_settingbtn->setScale(0.65f);
	this->addChild(_settingbtn, 2);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_exit")); //離開鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_exitbtn = C3SButton::create();
	_exitbtn->setButtonInfo(NORMAL_BTN, "btn_exit_0.png", "btn_exit_2.png", "btn_exit_1.png", loc);
	_exitbtn->setScale(0.65f);
	this->addChild(_exitbtn, 2);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_okay")); //OK鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_okaybtn = C3SButton::create();
	_okaybtn->setButtonInfo(NORMAL_BTN, "btn_okay_0.png", "btn_okay_2.png", "btn_okay_1.png", loc);
	_okaybtn->setScale(0.73f);
	_okaybtn->setVisible(false);
	this->addChild(_okaybtn, 2);

	//Type 2
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_easy")); //EASY鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_easybtn = C3SButton::create();
	_easybtn->setButtonInfo(SELECT_BTN, "btn_easy_0.png", "btn_easy_2.png", "btn_easy_1.png", loc);
	_easybtn->setScale(0.45f);
	_easybtn->setVisible(false);
	this->addChild(_easybtn, 2);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_normal")); //NORMAL鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_normalbtn = C3SButton::create();
	_normalbtn->setButtonInfo(SELECT_BTN, "btn_normal_0.png", "btn_normal_2.png", "btn_normal_1.png", loc);
	_normalbtn->setScale(0.45f);
	_normalbtn->setVisible(false);
	this->addChild(_normalbtn, 2);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_hard")); //HARD鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_hardbtn = C3SButton::create();
	_hardbtn->setButtonInfo(SELECT_BTN, "btn_hard_0.png", "btn_hard_2.png", "btn_hard_1.png", loc);
	_hardbtn->setScale(0.45f);
	_hardbtn->setVisible(false);
	this->addChild(_hardbtn, 2);
	//---------------------------------------------------------------------------------------------	

	//讀入圖檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rungame.plist");

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(MainMenu::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(MainMenu::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(MainMenu::doStep));
	return true;
}

void MainMenu::doStep(float dt)
{

}

bool MainMenu::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	_settingbtn->onTouchBegan(touchLoc);
	_okaybtn->onTouchBegan(touchLoc);
	_startbtn->onTouchBegan(touchLoc);
	_exitbtn->onTouchBegan(touchLoc);
	_easybtn->onTouchBegan(touchLoc);
	_normalbtn->onTouchBegan(touchLoc);
	_hardbtn->onTouchBegan(touchLoc);
	return true;
}

void  MainMenu::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();
	_settingbtn->onTouchMoved(touchLoc);
	_okaybtn->onTouchMoved(touchLoc);
	_startbtn->onTouchMoved(touchLoc);
	_exitbtn->onTouchMoved(touchLoc);
	_easybtn->onTouchMoved(touchLoc);
	_normalbtn->onTouchMoved(touchLoc);
	_hardbtn->onTouchMoved(touchLoc);
}

void  MainMenu::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();
	if (_settingbtn->onTouchEnded(touchLoc) || _okaybtn->onTouchEnded(touchLoc)) { //設定&OK btn
		//隱藏3個選項
		_settingbtn->setVisible(settingVisible); 
		_startbtn->setVisible(settingVisible);
		_exitbtn->setVisible(settingVisible);

		//顯示設定視窗
		_okaybtn->setVisible(!settingVisible);
		_easybtn->setVisible(!settingVisible);
		_normalbtn->setVisible(!settingVisible);
		_hardbtn->setVisible(!settingVisible);
		settingVisible = !settingVisible;
		_settingWindow->setVisible(settingVisible);
		_black_bg->setVisible(settingVisible);
	}
	if (_startbtn->onTouchEnded(touchLoc)) { //開始鈕
		auto scene01 = AnimationAction::createScene(); //切換場景
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene01, Color3B(255, 255, 255)));
	}
	if (_exitbtn->onTouchEnded(touchLoc)) { //離開鈕
		unscheduleAllCallbacks();  // 將所有註冊在 Schedule 中的程序都取消
		Director::getInstance()->end(); // 終止 Director 的執行
	}
	if (_easybtn->onTouchEnded(touchLoc)) { //level btn
		_normalbtn->setEnable(true);
		_hardbtn->setEnable(true);
		level = 1;
	}
	if (_normalbtn->onTouchEnded(touchLoc)) {
		_easybtn->setEnable(true);
		_hardbtn->setEnable(true);
		level = 2;
	}
	if (_hardbtn->onTouchEnded(touchLoc)) {
		_easybtn->setEnable(true);
		_normalbtn->setEnable(true);
		level = 3;
	}
}