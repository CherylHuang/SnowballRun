#include "cocostudio/CocoStudio.h"
#include "AnimationAction.h"
#include "MainMenu.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* AnimationAction::createScene()
{
    auto scene = Scene::create();
    auto layer = AnimationAction::create();
    scene->addChild(layer);
    return scene;
}

AnimationAction::~AnimationAction()
{
	if (_myAction != nullptr) _myAction->release();
	if (_myJump != nullptr) _myJump->release();
	if (_myJump2 != nullptr) _myJump2->release();
	if (_myJump3 != nullptr) _myJump3->release();
	if (_myJump4 != nullptr) _myJump4->release();
	if (_jumpCallback != nullptr) _jumpCallback->release();
	if (trackAni != nullptr) trackAni->release();
	if (_SlideUp != nullptr) _SlideUp->release();
	if (_SlideDown != nullptr) _SlideDown->release();
	if (_slideCallback != nullptr) _slideCallback->release();
	if(_runner != nullptr) delete _runner;
	if (_snowman != nullptr) delete  _snowman;
	if (_scoring != nullptr) delete _scoring;
	SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // 釋放音效檔

	//free link list
	deleteObstacle();

	AnimationCache::destroyInstance();  // 釋放 AnimationCache 取得的資源
	//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("rungame.plist");
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool AnimationAction::init()
{
	_myAction = nullptr;
	_myJump = nullptr;
	_jumpCallback = nullptr;
	trackAni = nullptr; //軌道動畫
	_SlideUp = nullptr;
	_SlideDown = nullptr;
	_slideCallback = nullptr;

	_level = MainMenu::level; //未選擇 預設為Lv1 
	//_level = 3;////////////////////////////////////////////////////
	_bStartGame = false;
	_bGameing = false;
	_bWin = false;
	_fcount = 0;
	_iCountdowntime = 3;
	_fpercent = 100;
	_fHP = 100;
	_pauseBtnPressed = false; //按按鈕時停止跳躍

	//slide
	_bMoving = false;
	_fBeganY = 0;
	_fEndY = 0;
	_bSlide = false;
	_iRunPosition = MIDDLE_TRACK;

	//obstacles
	_fcount_bgTree = 0;
	_bCollision = false;
	_bUp =_bMid = _bLow = 0;
	_col_up = nullptr;
	_col_mid = nullptr;
	_col_low = nullptr;

	//snowman
	_snowman = nullptr;
	_bSnowman_show = false;
	_fcount_snowman = 0;
	_fRandTime_snowman = 0;
	_bSnowballCol = false; //雪球碰撞

	//scoring
	_iScore = 0;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("AnimationAction.csb");
    addChild(rootNode);

	//讀取物件
	_black_bg = dynamic_cast<Sprite*>(rootNode->getChildByName("black_bg")); this->addChild(_black_bg, 2);
	_tutorial_lv1_1 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv1_1")); this->addChild(_tutorial_lv1_1, 2); //tutorial
	_tutorial_lv1_2 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv1_2")); this->addChild(_tutorial_lv1_2, 2);
	_tutorial_lv2 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv2")); this->addChild(_tutorial_lv2, 2);
	_tutorial_lv3 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv3")); this->addChild(_tutorial_lv3, 2);
	_loadingbar = dynamic_cast<Slider*>(rootNode->getChildByName("slider")); //進度條
	_window_win = dynamic_cast<Sprite*>(rootNode->getChildByName("window_win")); this->addChild(_window_win, 2); //game over window
	_window_lose = dynamic_cast<Sprite*>(rootNode->getChildByName("window_lose")); this->addChild(_window_lose, 2);
	_HPbar = dynamic_cast<LoadingBar*>(rootNode->getChildByName("hp_bar_bk")->getChildByName("hp_bar")); //血量條
	_sign = dynamic_cast<Sprite*>(rootNode->getChildByName("sign")); //倒數牌
	_countdownText = dynamic_cast<Text*>(rootNode->getChildByName("count_down"));

	//底圖圖層設定
	_HPbar_bg = dynamic_cast<ImageView*>(rootNode->getChildByName("hp_bar_bk"));
	_score_window = dynamic_cast<ImageView*>(rootNode->getChildByName("score_window"));
	this->addChild(_HPbar_bg, 1);
	this->addChild(_score_window, 1);

	//分數
	_gamingScoreText = dynamic_cast<Text*>(rootNode->getChildByName("score")); this->addChild(_gamingScoreText, 1);
	_winScoreText = dynamic_cast<Text*>(rootNode->getChildByName("window_win")->getChildByName("window_score"));
	_loseScoreText = dynamic_cast<Text*>(rootNode->getChildByName("window_lose")->getChildByName("window_score"));

	//進度條
	_loadingbar->setMaxPercent(100);
	_loadingbar->setPercent(100);

	//血量條
	_HPbar->setPercent(100);

	// Music -------------------------------------------------------------------------------------
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");	// 預先載入音效檔

	// Button -------------------------------------------------------------------------------------
	auto BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_start")); //開始鈕
	Point loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_startbtn = C3SButton::create();
	_startbtn->setButtonInfo(NORMAL_BTN, "btn_start_0.png", "btn_start_2.png", "btn_start_1.png", loc);
	_startbtn->setScale(0.58f);
	_startbtn->setVisible(true);
	this->addChild(_startbtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_next")); //下一頁鈕 (Lv1)
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_nextbtn = C3SButton::create();
	_nextbtn->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_1.png", loc);
	_nextbtn->setScale(0.447f);
	_nextbtn->setVisible(false); //未判定關卡先隱藏
	this->addChild(_nextbtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_pre")); //上一頁鈕 (Lv1)
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_prebtn = C3SButton::create();
	_prebtn->setButtonInfo(NORMAL_BTN, "btn_ previous_0.png", "btn_ previous_2.png", "btn_ previous_1.png", loc);
	_prebtn->setScale(0.447f);
	_prebtn->setVisible(false);
	this->addChild(_prebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_pause")); //暫停鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_pausebtn = C3SButton::create();
	_pausebtn->setButtonInfo(NORMAL_BTN, "btn_pause_0.png", "btn_pause_2.png", "btn_pause_1.png", loc);
	_pausebtn->setScale(0.41f);
	_pausebtn->setVisible(false);
	this->addChild(_pausebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_resume")); //繼續遊戲鈕
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_resumebtn = C3SButton::create();
	_resumebtn->setButtonInfo(NORMAL_BTN, "btn_play_0.png", "btn_play_2.png", "btn_play_1.png", loc);
	_resumebtn->setScale(0.5362f);
	_resumebtn->setVisible(false);
	this->addChild(_resumebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_retry")); //重新遊戲鈕
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn = C3SButton::create();
	_retrybtn->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn->setScale(0.5362f);
	_retrybtn->setVisible(false);
	this->addChild(_retrybtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_home")); //回首頁鈕
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn = C3SButton::create();
	_homebtn->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn->setScale(0.5362f);
	_homebtn->setVisible(false);
	this->addChild(_homebtn, 3);
	// ------------------------------------------------------------------------------------------------- 

	//win window Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_retry")); //重新遊戲鈕
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn_win = C3SButton::create();
	_retrybtn_win->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn_win->setScale(0.4987f);
	_retrybtn_win->setVisible(false);
	this->addChild(_retrybtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_option")); //排行鈕
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_rankingbtn_win = C3SButton::create();
	_rankingbtn_win->setButtonInfo(NORMAL_BTN, "btn_option_0.png", "btn_option_2.png", "btn_option_1.png", loc);
	_rankingbtn_win->setScale(0.4987f);
	_rankingbtn_win->setVisible(false);
	this->addChild(_rankingbtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_home")); //回首頁鈕
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn_win = C3SButton::create();
	_homebtn_win->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn_win->setScale(0.4987f);
	_homebtn_win->setVisible(false);
	this->addChild(_homebtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_next")); //下一關鈕
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_nextlevelbtn_win = C3SButton::create();
	_nextlevelbtn_win->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_3.png", loc);
	_nextlevelbtn_win->setScale(0.4987f);
	if (_level == 3) _nextlevelbtn_win->setEnable(false); //關卡為最後一關 不可點選
	_nextlevelbtn_win->setVisible(false);
	this->addChild(_nextlevelbtn_win, 3);
	// -------------------------------------------------------------------------------------------------

	//lose window Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_retry")); //重新遊戲鈕
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn_lose = C3SButton::create();
	_retrybtn_lose->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn_lose->setScale(0.4987f);
	_retrybtn_lose->setVisible(false);
	this->addChild(_retrybtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_option")); //排行鈕
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_rankingbtn_lose = C3SButton::create();
	_rankingbtn_lose->setButtonInfo(NORMAL_BTN, "btn_option_0.png", "btn_option_2.png", "btn_option_1.png", loc);
	_rankingbtn_lose->setScale(0.4987f);
	_rankingbtn_lose->setVisible(false);
	this->addChild(_rankingbtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_home")); //回首頁鈕
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn_lose = C3SButton::create();
	_homebtn_lose->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn_lose->setScale(0.4987f);
	_homebtn_lose->setVisible(false);
	this->addChild(_homebtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_next")); //下一關鈕
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_nextlevelbtn_lose = C3SButton::create();
	_nextlevelbtn_lose->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_3.png", loc);
	_nextlevelbtn_lose->setScale(0.4987f);
	if (_level == 3) _nextlevelbtn_lose->setEnable(false); //關卡為最後一關 不可點選
	_nextlevelbtn_lose->setVisible(false);
	this->addChild(_nextlevelbtn_lose, 3);

	// ------------------------------------------------------------------------------------------------- 

	//ranking Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_okay")); //排名 OK鈕
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_okaybtn = C3SButton::create();
	_okaybtn->setButtonInfo(NORMAL_BTN, "btn_okay_0.png", "btn_okay_2.png", "btn_okay_1.png", loc);
	_okaybtn->setScale(0.61f);
	_okaybtn->setVisible(false);
	this->addChild(_okaybtn, 3);

	// ------------------------------------------------------------------------------------------------- 

	//排名視窗
	_scoring = new CScoring("ranking.csb", *this);


	//教學視窗
	_black_bg->setVisible(true);
	if(_level == 2) _tutorial_lv2->setVisible(true);
	else if (_level == 3) _tutorial_lv3->setVisible(true);
	else {
		_tutorial_lv1_1->setVisible(true);
		_nextbtn->setVisible(true);
	}

	//暫停視窗
	_pause_window = dynamic_cast<Sprite*>(rootNode->getChildByName("pause_window")); this->addChild(_pause_window, 2);

	// ------------------------------------------------------------------------------------------------- 

// 讀入 plist 檔
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rungame.plist");
// ------------------------------------------------------------------------------------------------- 
//  plist animation
	auto animCache = AnimationCache::getInstance();
	animCache->addAnimationsWithFile("runneranim.plist"); //抓取plist檔

	auto mouse_ani = animCache->getAnimation("mouseanim"); //滑鼠動畫
	auto _mouseAnimation = RepeatForever::create(Animate::create(mouse_ani)); //建立無限循環動畫
	_mouseAni = Sprite::createWithSpriteFrameName("tutorial_mouse1.png"); //建立主體
	_mouseAni->setPosition(779, 401);
	this->addChild(_mouseAni, 4);
	_mouseAni->runAction(_mouseAnimation);//主體執行

	if (_level != 2 && _level != 3)_mouseAni->setVisible(true); //第一關時開啟
	else _mouseAni->setVisible(false);
// --------------------------------------------------------------------

	auto wasd_ani = animCache->getAnimation("wasdanim"); //WASD鍵盤動畫
	auto wasdAni = RepeatForever::create(Animate::create(wasd_ani));
	_wasdkey = Sprite::createWithSpriteFrameName("tutorial_wasd1.png");
	_wasdkey->setPosition(814.0f, 270.0f);
	this->addChild(_wasdkey, 4);
	_wasdkey->runAction(wasdAni);

	if (_level == 2)_wasdkey->setVisible(true); //第二關時開啟
	else _wasdkey->setVisible(false);
// --------------------------------------------------------------------

	//auto ud_ani = animCache->getAnimation("udanim"); //上下左右鍵盤動畫
	//auto udAni = RepeatForever::create(Animate::create(ud_ani));
	//_udkey = Sprite::createWithSpriteFrameName("tutorial_updown1.png");
	//_udkey->setPosition(860.31f, 255.56f);
	//this->addChild(_udkey);
	//_udkey->runAction(udAni);

	//if (_level == 2)_udkey->setVisible(true); //第二關時開啟
	//else _udkey->setVisible(false);
// --------------------------------------------------------------------

	auto track_ani = animCache->getAnimation("trackanim"); //跑道動畫
	trackAni = RepeatForever::create(Animate::create(track_ani));
	trackAni->retain();
	_track = dynamic_cast<Sprite*>(rootNode->getChildByName("line"));
	_track->setPosition(640.0f, 301.68f);
	this->addChild(_track);
	_track->setVisible(false);

// ------------------------------------------------------------------------------------------------- 

//RunnerNode
	auto SpritePos = dynamic_cast<Sprite*>(rootNode->getChildByName("circle_pos")); //角色位置
	loc = SpritePos->getPosition();
	SpritePos->setVisible(false);

	_runner = new CRunner("RunnerNode.csb", *this);
	_runner->setjumpTag(101);	// 用於取得該物件
	_runner->setAnimation("runneranim.plist");
	_runner->setScale(0.5f);
	_runner->setPosition(loc);
	_runner->setVisible(false);
	
//-------------------------------------------------------------------------------------------------

//Jump 
	_bJumps = false;
	_myJump = JumpBy::create(0.65f, Point(0, 0), 300, 1); //time, 動作終點, height, 次數
	_myJump->retain();
	_myJump2 = JumpBy::create(0.65f, Point(0, 0), 250, 1);
	_myJump2->retain();
	_myJump3 = JumpBy::create(0.65f, Point(0, 0), 200, 1);
	_myJump3->retain();
	_myJump4 = JumpBy::create(0.65f, Point(0, 0), 150, 1);
	_myJump4->retain();

	_jumpCallback = CallFunc::create(this, callfunc_selector(AnimationAction::JumpActionFinished));
	_jumpCallback->retain();
//-------------------------------------------------------------------------------------------------

//Slide
	_SlideUp = MoveBy::create(0.5f, Point(0, 158.0f));
	_SlideUp->retain();
	_SlideDown = MoveBy::create(0.5f, Point(0, -158.0f));
	_SlideDown->retain();
	_slideCallback = CallFunc::create(this, callfunc_selector(AnimationAction::SlideActionFinished));
	_slideCallback->retain();
//-------------------------------------------------------------------------------------------------
	
	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(AnimationAction::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(AnimationAction::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(AnimationAction::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(AnimationAction::doStep));
    return true;
}

void AnimationAction::JumpActionFinished()
{
	if (_bJumps == true) {
		_runner->setNormalFace();
		_runner->setColor(cocos2d::Color3B::WHITE);
		_bJumps = false;
	}
}

void AnimationAction::SlideActionFinished()
{
	if (_bSlide == true) {
		_bSlide = false;
	}
}

void  AnimationAction::deleteObstacle()
{
	_bCollision = false; //關閉偵測碰撞

	if (_level == 2 || _level == 3) {
		_pDel_upper = _pUp_head;
		while (_pUp_head != nullptr) { //上方跑道
			_pUp_head = _pUp_head->link;
			this->removeChild(_pDel_upper);
			delete _pDel_upper;
			_pDel_upper = _pUp_head;
		}
		_pDel_lower = _pLow_head;
		while (_pLow_head != nullptr) { //下方跑道
			_pLow_head = _pLow_head->link;
			this->removeChild(_pDel_lower);
			delete _pDel_lower;
			_pDel_lower = _pLow_head;
		}
	}
	_pDel_middle = _pMid_head;
	while (_pMid_head != nullptr) { //中間跑道
		_pMid_head = _pMid_head->link;
		this->removeChild(_pDel_middle);
		delete _pDel_middle;
		_pDel_middle = _pMid_head;
	}

	_pDel_bg = _pBG_head;
	while (_pBG_head != nullptr) { //背景
		_pBG_head = _pBG_head->link;
		this->removeChild(_pDel_bg);
		delete _pDel_bg;
		_pDel_bg = _pBG_head;
	}
}

void AnimationAction::GameOver()					//遊戲結束
{
	deleteObstacle();					//delete obstacles
	if (_snowman != nullptr) {			//delete snowman
		this->removeChild(_snowman);
		_snowmanDel = _snowman;
		_snowman = nullptr;
		delete _snowmanDel;
	}
	//---------------------------------------------

	(CScoring::_iplayerNum)++;
	_scoring->updateRanking(_iScore); //排行更新

	_track->stopAllActions(); _track->setVisible(false);
	_runner->stopAllActions(); _runner->setVisible(false);
	_pausebtn->setVisible(false); //暫停鈕
	_black_bg->setVisible(true); //黑屏

	//---------------------------------------------
	if (_bWin) { //成功
		_winScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數
		_window_win->setVisible(true);
		_retrybtn_win->setVisible(true);
		_rankingbtn_win->setVisible(true);
		_homebtn_win->setVisible(true);
		_nextlevelbtn_win->setVisible(true);
	}
	else { //失敗
		_loseScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數
		_window_lose->setVisible(true);
		_retrybtn_lose->setVisible(true);
		_rankingbtn_lose->setVisible(true);
		_homebtn_lose->setVisible(true);
		_nextlevelbtn_lose->setVisible(true);
	}
}

void AnimationAction::doStep(float dt)
{
	auto runnerX = _runner->getPositionX(); //玩家x座標
	auto runnerY = _runner->getPositionY(); //玩家y座標

	//--------------------------------------------------------------------------------------------
	if (_bStartGame) {									//Start鈕按下
		_fcount += dt;
		if (_fcount >= 1.0f) {
			_fcount -= 1.0f;
			_iCountdowntime--;
		}
		if (_iCountdowntime == 0) _countdownText->setString("GO");
		else  if (_iCountdowntime < 0) {							//遊戲開始
			_fcount = 0; //計時歸零
			_bStartGame = false;
			_bGameing = true; //遊戲進行中
			_track->runAction(trackAni);//重複播放軌道
			_runner->runAction(); //跑者動畫
			_bCollision = true; //碰撞偵測開啟
			
			auto signAni = MoveBy::create(4.0f, Point(1500.0f, 0)); //倒數牌動畫
			_sign->runAction(signAni);
			auto signAni2 = MoveBy::create(4.0f, Point(1500.0f, 0));
			_countdownText->runAction(signAni2);

			//產生每行第一個node
			if (_level == 2 || _level == 3) { // Level 2 && 3
				auto rand = (bool)cocos2d::RandomHelper::random_int(0, 1); //上方跑道
				_pUp_head = new CObstacle(UPPER_TRACK);
				this->addChild(_pUp_head, 1);
				_pUp_head->link = nullptr;
				_pUp_tail = _pUp_head;
				_pUp_head->setVisible(rand); //隨機出現
				_col_up = _pUp_head; //角色前方障礙物

				auto rand3 = (bool)cocos2d::RandomHelper::random_int(0, 1); //下方跑道
				_pLow_head = new CObstacle(LOWER_TRACK);
				this->addChild(_pLow_head, 1);
				_pLow_head->link = nullptr;
				_pLow_tail = _pLow_head;
				_pLow_head->setVisible(rand3); //隨機出現
				_col_low = _pLow_head; //角色前方障礙物
			}
			auto rand2 = (bool)cocos2d::RandomHelper::random_int(0, 1); //中間跑道
			_pMid_head = new CObstacle(MIDDLE_TRACK);
			this->addChild(_pMid_head, 1);
			_pMid_head->link = nullptr;
			_pMid_tail = _pMid_head;
			_pMid_head->setVisible(rand2); //隨機出現
			_col_mid = _pMid_head; //角色前方障礙物

			//-----------------------------------------------------------------
			auto rand4 = (bool)cocos2d::RandomHelper::random_int(0, 1); //背景
			_pBG_head = new CObstacle(BG_TRACK);
			this->addChild(_pBG_head, 0);
			_pBG_head->link = nullptr;
			_pBG_tail = _pBG_head;
			_pBG_head->setVisible(rand4); //隨機出現
		}
		else _countdownText->setString(StringUtils::format("%d", _iCountdowntime)); //倒數
	}
	//--------------------------------------------------------------------------------------------
	if (_bGameing && _iCountdowntime < 0) {
		if (_fpercent <= 0) {								//遊戲結束
			_bGameing = false;
			_bWin = true; //勝利
			GameOver();
		}
		else {												//遊戲進行中
			//進度條
			_fpercent -= dt * 3;
			_loadingbar->setPercent(_fpercent);

			//產生移動背景
			_fcount_bgTree += dt; //計時
			_fRandTime_bgTree = cocos2d::RandomHelper::random_real(0.5f, 2.0f);	//隨機時間
			if (_fcount_bgTree >= _fRandTime_bgTree) {
				_fcount_bgTree -= _fRandTime_bgTree;
				_pGet_bg = new CObstacle(BG_TRACK);
				this->addChild(_pGet_bg, 0);
				_pGet_bg->link = nullptr;
				_pBG_tail->link = _pGet_bg;
				_pBG_tail = _pGet_bg;
			}

			//產生障礙物
			_fcount += dt; //計時
			_fRandTime = cocos2d::RandomHelper::random_real(1.5f, 3.0f);	//隨機時間
			_bUp = cocos2d::RandomHelper::random_int(0, 1);  //上方跑道		//隨機障礙物
			_bMid = cocos2d::RandomHelper::random_int(0, 1); //中間跑道
			_bLow = cocos2d::RandomHelper::random_int(0, 1); //下方跑道
			if (_fcount >= _fRandTime) {
				_fcount -= _fRandTime;
				if(_level == 2 || _level == 3) {					// Level 2 && 3
					if (_bUp) { //上方跑道
						_pGet_upper = new CObstacle(UPPER_TRACK);
						this->addChild(_pGet_upper, 1);
						_pGet_upper->link = nullptr;
						_pUp_tail->link = _pGet_upper;
						_pUp_tail = _pGet_upper;
					}
					if (_bMid) { //中間跑道
						_pGet_middle = new CObstacle(MIDDLE_TRACK);
						this->addChild(_pGet_middle, 1);
						_pGet_middle->link = nullptr;
						_pMid_tail->link = _pGet_middle;
						_pMid_tail = _pGet_middle;
					}
					if (_bLow) { //下方跑道
						_pGet_lower = new CObstacle(LOWER_TRACK);
						this->addChild(_pGet_lower, 1);
						_pGet_lower->link = nullptr;
						_pLow_tail->link = _pGet_lower;
						_pLow_tail = _pGet_lower;
					}
				}
				else {												// Level 1
					_pGet_middle = new CObstacle(MIDDLE_TRACK);
					this->addChild(_pGet_middle, 1);
					_pGet_middle->link = nullptr;
					_pMid_tail->link = _pGet_middle;
					_pMid_tail = _pGet_middle;
				}
			}

			//障礙物動畫結束 自行刪除
			if (_level == 2 || _level == 3) {						// Level 2 && 3
				if (_pUp_head->link != nullptr) {
					if (_pUp_head->_bMoveDone) {		//UP

						if (_pUp_head->getType() == SMALL_ROCK) _iScore = _scoring->addSmallRockScore(_iScore);
						if (_pUp_head->getType() == BIG_ROCK) _iScore = _scoring->addBigRockScore(_iScore);
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數

						_pDel_upper = _pUp_head;
						_pUp_head = _pUp_head->link;
						this->removeChild(_pDel_upper);
						delete _pDel_upper;
					}
				}
				if (_pLow_head->link != nullptr) {
					if (_pLow_head->_bMoveDone) {		//LOW

						if (_pLow_head->getType() == SMALL_ROCK) _iScore = _scoring->addSmallRockScore(_iScore);
						if (_pLow_head->getType() == BIG_ROCK) _iScore = _scoring->addBigRockScore(_iScore);
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數

						_pDel_lower = _pLow_head;
						_pLow_head = _pLow_head->link;
						this->removeChild(_pDel_lower);
						delete _pDel_lower;
					}
				}
			}
			if (_pMid_head->link != nullptr) {							// Level 1
				if (_pMid_head->_bMoveDone) {			//MID

					if (_pMid_head->getType() == SMALL_ROCK) _iScore = _scoring->addSmallRockScore(_iScore);
					if (_pMid_head->getType() == BIG_ROCK) _iScore = _scoring->addBigRockScore(_iScore);
					_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數

					_pDel_middle = _pMid_head;
					_pMid_head = _pMid_head->link;
					this->removeChild(_pDel_middle);
					delete _pDel_middle;
				}
			}
			if (_pBG_head->link != nullptr) {							// 背景
				if (_pBG_head->_bMoveDone) {			//MID
					_pDel_bg = _pBG_head;
					_pBG_head = _pBG_head->link;
					this->removeChild(_pDel_bg);
					delete _pDel_bg;
				}
			}

			//產生雪人
			if (_level == 3) {
				_fcount_snowman += dt; //計時
				_fRandTime_snowman = cocos2d::RandomHelper::random_real(2.0f, 4.0f);	//雪人隨機時間
				if (_fcount_snowman >= _fRandTime_snowman && !_bSnowman_show) {
					if (_snowman == nullptr) {
						_snowman = new CEnemy(_iRunPosition); //create Snowman
						this->addChild(_snowman, 2);
					}
					_fcount_duration = 0; //重設顯示計時
					_bSnowman_show = true;
					//_fcount_snowman = 0; //重設計時
					_fcount_snowball = 0;
				}

				_fRandTime_snowball = cocos2d::RandomHelper::random_real(2.0f, 3.0f);	//雪球隨機時間
				if (_bSnowman_show) {
					_fcount_duration += dt; //計時
					if (_fcount_duration >= 7.0f) { //持續出現7秒
						_snowman->snowmanFadeOut(); //雪人&雪球淡出
					}

					_fcount_snowball += dt;
					if (_fcount_snowball >= _fRandTime_snowball) { //雪人出現
						_fcount_snowball -= _fRandTime_snowball;
						_snowman->throwSnowball(); //丟出雪球
					}

					if (_snowman->_bFadeOutDone) { //加分
						_iScore = _scoring->addSnowBallScore(_iScore);
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //顯示分數
					}

					//雪球碰撞玩家
					if (_bCollision) {
						auto snowball1x = _snowman->getSnowballX(1); //第一顆雪球X座標
						auto snowball2x = _snowman->getSnowballX(2); //第二顆雪球X座標
						auto snowball3x = _snowman->getSnowballX(3); //第三顆雪球X座標
						//auto snowbally = _snowman->getSnowballY(); //雪球Y座標

						if (snowball1x > (runnerX - 55.f) && snowball1x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(1, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //扣血
						}
						else if (snowball2x > (runnerX - 55.f) && snowball2x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(2, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //扣血
						}
						else if (snowball3x >(runnerX - 55.f) && snowball3x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(3, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //扣血
						}
						else {
							if (!_bJumps) {
								_runner->setNormalFace();
								_runner->setColor(cocos2d::Color3B::WHITE);
							}
						}
					}	
				}
				if (_snowman != nullptr) {
					//重啟雪人
					if (_snowman->_bFadeOutDone) {
						_bSnowman_show = false; //關閉雪人
						_fcount_snowman = 0; //重設時間

						this->removeChild(_snowman); //刪除
						_snowmanDel = _snowman;
						_snowman = nullptr;
						delete _snowmanDel;
					}
				}
			} //level 3 end

		} //else(gaming) end
	}
	

	//--------------------------------------------------------------------------------------------
	if (_bMoving && _level > 1) {						//滑動螢幕
		if (!_bSlide && !_bJumps && _bGameing && _iCountdowntime < 0 && !_pauseBtnPressed) {
			if (_fBeganY < _fEndY && (_fEndY - _fBeganY) > 3.0f && _iRunPosition < UPPER_TRACK) { //往上滑
				_iRunPosition++; //往上一個跑道
				_bSlide = true;
				auto actionBody = this->getChildByName("runnerRoot");
				actionBody->runAction(Sequence::create(_SlideUp, _slideCallback, NULL));
				if (_snowman != nullptr) _snowman->slideUp();

				auto SlideUp = MoveBy::create(0.5f, Point(0, 158.0f));
			}
			else if (_fBeganY > _fEndY && (_fBeganY - _fEndY) > 3.0f && _iRunPosition > LOWER_TRACK) { //往下滑
				_iRunPosition--; //往下一個跑道
				_bSlide = true;
				auto actionBody = this->getChildByName("runnerRoot");
				actionBody->runAction(Sequence::create(_SlideDown, _slideCallback, NULL));
				if (_snowman != nullptr) _snowman->slideDown();

				auto SlideDown = MoveBy::create(0.5f, Point(0, -158.0f));
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	//Runner Scale (根據血量)
	if (_fHP >= 75.0f) _runner->setScale(0.5f);
	else if (_fHP < 75.0f && _fHP >= 50.0f) _runner->setScale(0.55f);
	else if (_fHP < 50.0f && _fHP >= 25.0f) _runner->setScale(0.6f);
	else _runner->setScale(0.65f);
	//--------------------------------------------------------------------------------------------
	//碰撞偵測
	if (_iRunPosition == UPPER_TRACK && _bCollision) {			//UP
		auto rockPos_Upx = _pUp_head->getPositionX();
		if (rockPos_Upx >(runnerX - 55.f) && rockPos_Upx < (runnerX + 55.f) && _pUp_head->getVisible()) {
			if ((_pUp_head->getType() == BIG_ROCK && _fHP < 50.0f) || !_bJumps) {
				_runner->setDepressedFace();
				_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
				_fHP -= dt * 30.f;
				_HPbar->setPercent(_fHP); //扣血
			}
		}
		else {
			if (!_bJumps) {
				_runner->setNormalFace();
				_runner->setColor(cocos2d::Color3B::WHITE);
			}
		}
	}
	if (_iRunPosition == MIDDLE_TRACK && _bCollision) {			//MID
		auto rockPos_Midx = _pMid_head->getPositionX();
		if (rockPos_Midx > (runnerX - 55.f) && rockPos_Midx < (runnerX + 55.f) && _pMid_head->getVisible()) {
			if ((_pMid_head->getType() == BIG_ROCK && _fHP < 50.0f) || !_bJumps) {
				_runner->setDepressedFace();
				_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
				_fHP -= dt * 30.f;
				_HPbar->setPercent(_fHP); //扣血
			}
		}
		else {
			if (!_bJumps) {
				_runner->setNormalFace();
				_runner->setColor(cocos2d::Color3B::WHITE);
			}
		}
	}
	if (_iRunPosition == LOWER_TRACK && _bCollision) {			//LOW
		auto rockPos_Lowx = _pLow_head->getPositionX();
		if (rockPos_Lowx > (runnerX - 55.f) && rockPos_Lowx < (runnerX + 55.f) && _pLow_head->getVisible()) {
			if ((_pLow_head->getType() == BIG_ROCK && _fHP < 50.0f) || !_bJumps) {
				_runner->setDepressedFace();
				_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
				_fHP -= dt * 30.f;
				_HPbar->setPercent(_fHP); //扣血
			}
		}
		else {
			if (!_bJumps) {
				_runner->setNormalFace();
				_runner->setColor(cocos2d::Color3B::WHITE);
			}
		}
	}
	
	//--------------------------------------------------------------------------------------------
	if (_fHP <= 0 && _bGameing) {								//遊戲結束
		_bGameing = false;
		_bWin = false; //失敗
		GameOver();
	}
}

bool AnimationAction::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();

	//slide
	_fBeganY = touchLoc.y;

	//按鈕狀態
	_startbtn->onTouchBegan(touchLoc);
	_nextbtn->onTouchBegan(touchLoc);
	_prebtn->onTouchBegan(touchLoc);
	if (_pausebtn->onTouchBegan(touchLoc)) _pauseBtnPressed = _pausebtn->getPressed(); //按鈕點擊時不跳躍
	_resumebtn->onTouchBegan(touchLoc);
	_retrybtn->onTouchBegan(touchLoc);
	_homebtn->onTouchBegan(touchLoc);
	_okaybtn->onTouchBegan(touchLoc);

	_retrybtn_win->onTouchBegan(touchLoc); //win
	_rankingbtn_win->onTouchBegan(touchLoc);
	_homebtn_win->onTouchBegan(touchLoc);
	_nextlevelbtn_win->onTouchBegan(touchLoc);
	_retrybtn_lose->onTouchBegan(touchLoc); //lose
	_rankingbtn_lose->onTouchBegan(touchLoc);
	_homebtn_lose->onTouchBegan(touchLoc);
	_nextlevelbtn_lose->onTouchBegan(touchLoc);

	return true;
}

void  AnimationAction::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();

	//slide
	_fEndY = touchLoc.y;
	_bMoving = true;

	//按鈕狀態
	_startbtn->onTouchMoved(touchLoc);
	_nextbtn->onTouchMoved(touchLoc);
	_prebtn->onTouchMoved(touchLoc);
	if(_pausebtn->onTouchMoved(touchLoc)) _pauseBtnPressed = _pausebtn->getPressed();
	_resumebtn->onTouchMoved(touchLoc);
	_retrybtn->onTouchMoved(touchLoc);
	_homebtn->onTouchMoved(touchLoc);
	_okaybtn->onTouchMoved(touchLoc);

	_retrybtn_win->onTouchMoved(touchLoc); //win
	_rankingbtn_win->onTouchMoved(touchLoc);
	_homebtn_win->onTouchMoved(touchLoc);
	_nextlevelbtn_win->onTouchMoved(touchLoc);
	_retrybtn_lose->onTouchMoved(touchLoc); //lose
	_rankingbtn_lose->onTouchMoved(touchLoc);
	_homebtn_lose->onTouchMoved(touchLoc);
	_nextlevelbtn_lose->onTouchMoved(touchLoc);
}

void  AnimationAction::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();

	//Jump
	auto actionBody = this->getChildByName("runnerRoot")->getChildByTag(101);
	if (!_bJumps && _bGameing && _iCountdowntime < 0 && !_pauseBtnPressed && !_bMoving) {  // 跳躍

		//高度根據血量條整
		if (_fHP >= 75.0f) actionBody->runAction(Sequence::create(_myJump, _jumpCallback, NULL));
		else if (_fHP < 75.0f && _fHP >= 50.0f) actionBody->runAction(Sequence::create(_myJump2, _jumpCallback, NULL));
		else if (_fHP < 50.0f && _fHP >= 25.0f) actionBody->runAction(Sequence::create(_myJump3, _jumpCallback, NULL));
		else actionBody->runAction(Sequence::create(_myJump4, _jumpCallback, NULL));

		_runner->setHappyFace();
		_runner->setColor(cocos2d::Color3B(211, 230, 255));
		unsigned int eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", false);  // 播放音效檔
		_bJumps = true;
	}

	//slide
	_fEndY = touchLoc.y;
	_bMoving = false; //判斷完是否可跳躍才關掉

	//按鈕狀態
	if (_startbtn->onTouchEnded(touchLoc)) {						//開始鈕
		_black_bg->setVisible(false);
		_startbtn->setVisible(false);
		_nextbtn->setVisible(false);
		_prebtn->setVisible(false);
		_tutorial_lv1_1->setVisible(false);
		_tutorial_lv1_2->setVisible(false);
		_tutorial_lv2->setVisible(false);
		_tutorial_lv3->setVisible(false);
		_mouseAni->setVisible(false); //滑鼠動畫關閉
		_wasdkey->setVisible(false); //WASD鍵盤動畫
		//_udkey->setVisible(false); //鍵盤動畫
		_track->setVisible(true); //軌道顯示
		//runner->setVisible(true); //角色顯示
		_runner->setVisible(true); //角色顯示
		_pausebtn->setVisible(true); //暫停鈕顯示
		_bStartGame = true; //遊戲開始
	}
	if (_nextbtn->onTouchEnded(touchLoc)) {							//下一頁鈕
		_tutorial_lv1_1->setVisible(false);
		_tutorial_lv1_2->setVisible(true);
		_nextbtn->setVisible(false);
		_prebtn->setVisible(true);
		_mouseAni->setVisible(false); //滑鼠動畫關閉
	}
	if (_prebtn->onTouchEnded(touchLoc)) {							//上一頁鈕
		_tutorial_lv1_1->setVisible(true);
		_tutorial_lv1_2->setVisible(false);
		_nextbtn->setVisible(true);
		_prebtn->setVisible(false);
		_mouseAni->setVisible(true); //滑鼠動畫開啟
	}
	if (_pausebtn->onTouchEnded(touchLoc)) {						//暫停鈕
		_pause_window->setVisible(true);
		_black_bg->setVisible(true);
		_track->setVisible(false);
		_runner->setVisible(false); //角色隱藏
		_pausebtn->setVisible(false); //隱藏暫停鈕
		_resumebtn->setVisible(true);
		_retrybtn->setVisible(true);
		_homebtn->setVisible(true);
		_bGameing = false; //暫停進度條
		if (_iCountdowntime >= 0) _bStartGame = false; //暫停倒數
		_pauseBtnPressed = _pausebtn->getPressed();

		//BG stop and hide
		_pTrackObj_bg = _pBG_head;
		while (_pTrackObj_bg != nullptr) { //mid
			_pTrackObj_bg->pause();
			_pTrackObj_bg->setVisible(false);
			_pTrackObj_bg = _pTrackObj_bg->link;
		}

		//obstacle stop and hide
		_pTrackObj_middle = _pMid_head;
		while (_pTrackObj_middle != nullptr) { //mid
			_pTrackObj_middle->pause();
			_pTrackObj_middle->setVisible(false);
			_pTrackObj_middle = _pTrackObj_middle->link;
		}
		if (_level == 2 || _level == 3) {
			_pTrackObj_upper = _pUp_head;
			while (_pTrackObj_upper != nullptr) { //up
				_pTrackObj_upper->pause();
				_pTrackObj_upper->setVisible(false);
				_pTrackObj_upper = _pTrackObj_upper->link;
			}
			_pTrackObj_lower = _pLow_head;
			while (_pTrackObj_lower != nullptr) { //low
				_pTrackObj_lower->pause();
				_pTrackObj_lower->setVisible(false);
				_pTrackObj_lower = _pTrackObj_lower->link;
			}
		}

		//Snowman
		if (_snowman != nullptr) {
			_snowman->pause();
			_snowman->setVisible(false);
		}
	}
	if (_resumebtn->onTouchEnded(touchLoc)) {						//繼續遊戲鈕
		_pause_window->setVisible(false);
		_black_bg->setVisible(false);
		_track->setVisible(true);
		_runner->setVisible(true); //角色顯示
		_pausebtn->setVisible(true); //暫停鈕顯示
		_resumebtn->setVisible(false);
		_retrybtn->setVisible(false);
		_homebtn->setVisible(false);
		_bGameing = true; //繼續進度條
		if(_iCountdowntime >= 0) _bStartGame = true; //繼續倒數

		//BG runAni and show
		_pTrackObj_bg = _pBG_head;
		while (_pTrackObj_bg != nullptr) { //mid
			_pTrackObj_bg->resume();
			_pTrackObj_bg->setVisible(true);
			_pTrackObj_bg = _pTrackObj_bg->link;
		}

		//obstacle runAni and show
		_pTrackObj_middle = _pMid_head;
		while (_pTrackObj_middle != nullptr) { //mid
			_pTrackObj_middle->resume();
			_pTrackObj_middle->setVisible(true);
			_pTrackObj_middle = _pTrackObj_middle->link;
		}
		if (_level == 2 || _level == 3) {
			_pTrackObj_upper = _pUp_head;
			while (_pTrackObj_upper != nullptr) { //up
				_pTrackObj_upper->resume();
				_pTrackObj_upper->setVisible(true);
				_pTrackObj_upper = _pTrackObj_upper->link;
			}
			_pTrackObj_lower = _pLow_head;
			while (_pTrackObj_lower != nullptr) { //low
				_pTrackObj_lower->resume();
				_pTrackObj_lower->setVisible(true);
				_pTrackObj_lower = _pTrackObj_lower->link;
			}
		}

		//Snowman
		if (_snowman != nullptr) {
			_snowman->resume();
			_snowman->setVisible(true);
		}
	}
	if (_retrybtn->onTouchEnded(touchLoc) || _retrybtn_win->onTouchEnded(touchLoc) || _retrybtn_lose->onTouchEnded(touchLoc)) {		//重新遊戲鈕
		deleteObstacle(); //delete obstacles
		if (_snowman != nullptr) {
			this->removeChild(_snowman); //delete snowman
			_snowmanDel = _snowman;
			_snowman = nullptr;
			delete _snowmanDel;
		}
		auto Scene = AnimationAction::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, Scene, Color3B(255, 255, 255)));
	}
	if (_homebtn->onTouchEnded(touchLoc) || _homebtn_win->onTouchEnded(touchLoc) || _homebtn_lose->onTouchEnded(touchLoc)) {		//回首頁鈕
		deleteObstacle();//delete obstacles
		if (_snowman != nullptr) {
			this->removeChild(_snowman); //delete snowman
			_snowmanDel = _snowman;
			_snowman = nullptr;
			delete _snowmanDel;
		}
		auto mainScene = MainMenu::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, mainScene, Color3B(255, 255, 255)));
	}
	if (_rankingbtn_win->onTouchEnded(touchLoc) || _rankingbtn_lose->onTouchEnded(touchLoc)) {	//查看排行鈕
		_window_win->setVisible(false);			//關閉遊戲結束視窗
		_window_lose->setVisible(false);
		_retrybtn_win->setVisible(false);
		_rankingbtn_win->setVisible(false);
		_homebtn_win->setVisible(false);
		_nextlevelbtn_win->setVisible(false);
		_retrybtn_lose->setVisible(false);
		_rankingbtn_lose->setVisible(false);
		_homebtn_lose->setVisible(false);
		_nextlevelbtn_lose->setVisible(false);
		//開啟排行榜-------------------------------------
		_okaybtn->setVisible(true);
		_scoring->setVisible(true);
	}
	if (_okaybtn->onTouchEnded(touchLoc)) {														//關閉排行鈕
		if (_bWin) {							//開啟遊戲結束視窗
			_window_win->setVisible(true);
			_retrybtn_win->setVisible(true);
			_rankingbtn_win->setVisible(true);
			_homebtn_win->setVisible(true);
			_nextlevelbtn_win->setVisible(true);
		}
		else {
			_window_lose->setVisible(true);
			_retrybtn_lose->setVisible(true);
			_rankingbtn_lose->setVisible(true);
			_homebtn_lose->setVisible(true);
			_nextlevelbtn_lose->setVisible(true);
		}
		//關閉排行榜-------------------------------------
		_okaybtn->setVisible(false);
		_scoring->setVisible(false);
	}
	if (_nextlevelbtn_win->onTouchEnded(touchLoc) || _nextlevelbtn_lose->onTouchEnded(touchLoc)) {	//下一關鈕
		if (_level < 3) (MainMenu::level)++; //到下一關
		auto Scene = AnimationAction::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, Scene, Color3B(255, 255, 255)));
	}
}