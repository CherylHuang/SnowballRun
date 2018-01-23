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
	SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // ���񭵮���

	//free link list
	deleteObstacle();

	AnimationCache::destroyInstance();  // ���� AnimationCache ���o���귽
	//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("rungame.plist");
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool AnimationAction::init()
{
	_myAction = nullptr;
	_myJump = nullptr;
	_jumpCallback = nullptr;
	trackAni = nullptr; //�y�D�ʵe
	_SlideUp = nullptr;
	_SlideDown = nullptr;
	_slideCallback = nullptr;

	_level = MainMenu::level; //����� �w�]��Lv1 
	//_level = 3;////////////////////////////////////////////////////
	_bStartGame = false;
	_bGameing = false;
	_bWin = false;
	_fcount = 0;
	_iCountdowntime = 3;
	_fpercent = 100;
	_fHP = 100;
	_pauseBtnPressed = false; //�����s�ɰ�����D

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
	_bSnowballCol = false; //���y�I��

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

	//Ū������
	_black_bg = dynamic_cast<Sprite*>(rootNode->getChildByName("black_bg")); this->addChild(_black_bg, 2);
	_tutorial_lv1_1 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv1_1")); this->addChild(_tutorial_lv1_1, 2); //tutorial
	_tutorial_lv1_2 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv1_2")); this->addChild(_tutorial_lv1_2, 2);
	_tutorial_lv2 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv2")); this->addChild(_tutorial_lv2, 2);
	_tutorial_lv3 = dynamic_cast<Sprite*>(rootNode->getChildByName("tutorial_lv3")); this->addChild(_tutorial_lv3, 2);
	_loadingbar = dynamic_cast<Slider*>(rootNode->getChildByName("slider")); //�i�ױ�
	_window_win = dynamic_cast<Sprite*>(rootNode->getChildByName("window_win")); this->addChild(_window_win, 2); //game over window
	_window_lose = dynamic_cast<Sprite*>(rootNode->getChildByName("window_lose")); this->addChild(_window_lose, 2);
	_HPbar = dynamic_cast<LoadingBar*>(rootNode->getChildByName("hp_bar_bk")->getChildByName("hp_bar")); //��q��
	_sign = dynamic_cast<Sprite*>(rootNode->getChildByName("sign")); //�˼ƵP
	_countdownText = dynamic_cast<Text*>(rootNode->getChildByName("count_down"));

	//���Ϲϼh�]�w
	_HPbar_bg = dynamic_cast<ImageView*>(rootNode->getChildByName("hp_bar_bk"));
	_score_window = dynamic_cast<ImageView*>(rootNode->getChildByName("score_window"));
	this->addChild(_HPbar_bg, 1);
	this->addChild(_score_window, 1);

	//����
	_gamingScoreText = dynamic_cast<Text*>(rootNode->getChildByName("score")); this->addChild(_gamingScoreText, 1);
	_winScoreText = dynamic_cast<Text*>(rootNode->getChildByName("window_win")->getChildByName("window_score"));
	_loseScoreText = dynamic_cast<Text*>(rootNode->getChildByName("window_lose")->getChildByName("window_score"));

	//�i�ױ�
	_loadingbar->setMaxPercent(100);
	_loadingbar->setPercent(100);

	//��q��
	_HPbar->setPercent(100);

	// Music -------------------------------------------------------------------------------------
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");	// �w�����J������

	// Button -------------------------------------------------------------------------------------
	auto BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_start")); //�}�l�s
	Point loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_startbtn = C3SButton::create();
	_startbtn->setButtonInfo(NORMAL_BTN, "btn_start_0.png", "btn_start_2.png", "btn_start_1.png", loc);
	_startbtn->setScale(0.58f);
	_startbtn->setVisible(true);
	this->addChild(_startbtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_next")); //�U�@���s (Lv1)
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_nextbtn = C3SButton::create();
	_nextbtn->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_1.png", loc);
	_nextbtn->setScale(0.447f);
	_nextbtn->setVisible(false); //���P�w���d������
	this->addChild(_nextbtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_pre")); //�W�@���s (Lv1)
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_prebtn = C3SButton::create();
	_prebtn->setButtonInfo(NORMAL_BTN, "btn_ previous_0.png", "btn_ previous_2.png", "btn_ previous_1.png", loc);
	_prebtn->setScale(0.447f);
	_prebtn->setVisible(false);
	this->addChild(_prebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_pause")); //�Ȱ��s
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_pausebtn = C3SButton::create();
	_pausebtn->setButtonInfo(NORMAL_BTN, "btn_pause_0.png", "btn_pause_2.png", "btn_pause_1.png", loc);
	_pausebtn->setScale(0.41f);
	_pausebtn->setVisible(false);
	this->addChild(_pausebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_resume")); //�~��C���s
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_resumebtn = C3SButton::create();
	_resumebtn->setButtonInfo(NORMAL_BTN, "btn_play_0.png", "btn_play_2.png", "btn_play_1.png", loc);
	_resumebtn->setScale(0.5362f);
	_resumebtn->setVisible(false);
	this->addChild(_resumebtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_retry")); //���s�C���s
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn = C3SButton::create();
	_retrybtn->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn->setScale(0.5362f);
	_retrybtn->setVisible(false);
	this->addChild(_retrybtn, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("pause_window")->getChildByName("btn_home")); //�^�����s
	loc = rootNode->getChildByName("pause_window")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn = C3SButton::create();
	_homebtn->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn->setScale(0.5362f);
	_homebtn->setVisible(false);
	this->addChild(_homebtn, 3);
	// ------------------------------------------------------------------------------------------------- 

	//win window Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_retry")); //���s�C���s
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn_win = C3SButton::create();
	_retrybtn_win->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn_win->setScale(0.4987f);
	_retrybtn_win->setVisible(false);
	this->addChild(_retrybtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_option")); //�Ʀ�s
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_rankingbtn_win = C3SButton::create();
	_rankingbtn_win->setButtonInfo(NORMAL_BTN, "btn_option_0.png", "btn_option_2.png", "btn_option_1.png", loc);
	_rankingbtn_win->setScale(0.4987f);
	_rankingbtn_win->setVisible(false);
	this->addChild(_rankingbtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_home")); //�^�����s
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn_win = C3SButton::create();
	_homebtn_win->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn_win->setScale(0.4987f);
	_homebtn_win->setVisible(false);
	this->addChild(_homebtn_win, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_win")->getChildByName("btn_next")); //�U�@���s
	loc = rootNode->getChildByName("window_win")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_nextlevelbtn_win = C3SButton::create();
	_nextlevelbtn_win->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_3.png", loc);
	_nextlevelbtn_win->setScale(0.4987f);
	if (_level == 3) _nextlevelbtn_win->setEnable(false); //���d���̫�@�� ���i�I��
	_nextlevelbtn_win->setVisible(false);
	this->addChild(_nextlevelbtn_win, 3);
	// -------------------------------------------------------------------------------------------------

	//lose window Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_retry")); //���s�C���s
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_retrybtn_lose = C3SButton::create();
	_retrybtn_lose->setButtonInfo(NORMAL_BTN, "btn_retry_0.png", "btn_retry_2.png", "btn_retry_1.png", loc);
	_retrybtn_lose->setScale(0.4987f);
	_retrybtn_lose->setVisible(false);
	this->addChild(_retrybtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_option")); //�Ʀ�s
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_rankingbtn_lose = C3SButton::create();
	_rankingbtn_lose->setButtonInfo(NORMAL_BTN, "btn_option_0.png", "btn_option_2.png", "btn_option_1.png", loc);
	_rankingbtn_lose->setScale(0.4987f);
	_rankingbtn_lose->setVisible(false);
	this->addChild(_rankingbtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_home")); //�^�����s
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_homebtn_lose = C3SButton::create();
	_homebtn_lose->setButtonInfo(NORMAL_BTN, "btn_home_0.png", "btn_home_2.png", "btn_home_1.png", loc);
	_homebtn_lose->setScale(0.4987f);
	_homebtn_lose->setVisible(false);
	this->addChild(_homebtn_lose, 3);

	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("window_lose")->getChildByName("btn_next")); //�U�@���s
	loc = rootNode->getChildByName("window_lose")->convertToWorldSpace(BtnPos->getPosition());
	BtnPos->setVisible(false);
	_nextlevelbtn_lose = C3SButton::create();
	_nextlevelbtn_lose->setButtonInfo(NORMAL_BTN, "btn_next_0.png", "btn_next_2.png", "btn_next_3.png", loc);
	_nextlevelbtn_lose->setScale(0.4987f);
	if (_level == 3) _nextlevelbtn_lose->setEnable(false); //���d���̫�@�� ���i�I��
	_nextlevelbtn_lose->setVisible(false);
	this->addChild(_nextlevelbtn_lose, 3);

	// ------------------------------------------------------------------------------------------------- 

	//ranking Button
	BtnPos = dynamic_cast<Button*>(rootNode->getChildByName("btn_okay")); //�ƦW OK�s
	loc = BtnPos->getPosition();
	BtnPos->setVisible(false);
	_okaybtn = C3SButton::create();
	_okaybtn->setButtonInfo(NORMAL_BTN, "btn_okay_0.png", "btn_okay_2.png", "btn_okay_1.png", loc);
	_okaybtn->setScale(0.61f);
	_okaybtn->setVisible(false);
	this->addChild(_okaybtn, 3);

	// ------------------------------------------------------------------------------------------------- 

	//�ƦW����
	_scoring = new CScoring("ranking.csb", *this);


	//�оǵ���
	_black_bg->setVisible(true);
	if(_level == 2) _tutorial_lv2->setVisible(true);
	else if (_level == 3) _tutorial_lv3->setVisible(true);
	else {
		_tutorial_lv1_1->setVisible(true);
		_nextbtn->setVisible(true);
	}

	//�Ȱ�����
	_pause_window = dynamic_cast<Sprite*>(rootNode->getChildByName("pause_window")); this->addChild(_pause_window, 2);

	// ------------------------------------------------------------------------------------------------- 

// Ū�J plist ��
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rungame.plist");
// ------------------------------------------------------------------------------------------------- 
//  plist animation
	auto animCache = AnimationCache::getInstance();
	animCache->addAnimationsWithFile("runneranim.plist"); //���plist��

	auto mouse_ani = animCache->getAnimation("mouseanim"); //�ƹ��ʵe
	auto _mouseAnimation = RepeatForever::create(Animate::create(mouse_ani)); //�إߵL���`���ʵe
	_mouseAni = Sprite::createWithSpriteFrameName("tutorial_mouse1.png"); //�إߥD��
	_mouseAni->setPosition(779, 401);
	this->addChild(_mouseAni, 4);
	_mouseAni->runAction(_mouseAnimation);//�D�����

	if (_level != 2 && _level != 3)_mouseAni->setVisible(true); //�Ĥ@���ɶ}��
	else _mouseAni->setVisible(false);
// --------------------------------------------------------------------

	auto wasd_ani = animCache->getAnimation("wasdanim"); //WASD��L�ʵe
	auto wasdAni = RepeatForever::create(Animate::create(wasd_ani));
	_wasdkey = Sprite::createWithSpriteFrameName("tutorial_wasd1.png");
	_wasdkey->setPosition(814.0f, 270.0f);
	this->addChild(_wasdkey, 4);
	_wasdkey->runAction(wasdAni);

	if (_level == 2)_wasdkey->setVisible(true); //�ĤG���ɶ}��
	else _wasdkey->setVisible(false);
// --------------------------------------------------------------------

	//auto ud_ani = animCache->getAnimation("udanim"); //�W�U���k��L�ʵe
	//auto udAni = RepeatForever::create(Animate::create(ud_ani));
	//_udkey = Sprite::createWithSpriteFrameName("tutorial_updown1.png");
	//_udkey->setPosition(860.31f, 255.56f);
	//this->addChild(_udkey);
	//_udkey->runAction(udAni);

	//if (_level == 2)_udkey->setVisible(true); //�ĤG���ɶ}��
	//else _udkey->setVisible(false);
// --------------------------------------------------------------------

	auto track_ani = animCache->getAnimation("trackanim"); //�]�D�ʵe
	trackAni = RepeatForever::create(Animate::create(track_ani));
	trackAni->retain();
	_track = dynamic_cast<Sprite*>(rootNode->getChildByName("line"));
	_track->setPosition(640.0f, 301.68f);
	this->addChild(_track);
	_track->setVisible(false);

// ------------------------------------------------------------------------------------------------- 

//RunnerNode
	auto SpritePos = dynamic_cast<Sprite*>(rootNode->getChildByName("circle_pos")); //�����m
	loc = SpritePos->getPosition();
	SpritePos->setVisible(false);

	_runner = new CRunner("RunnerNode.csb", *this);
	_runner->setjumpTag(101);	// �Ω���o�Ӫ���
	_runner->setAnimation("runneranim.plist");
	_runner->setScale(0.5f);
	_runner->setPosition(loc);
	_runner->setVisible(false);
	
//-------------------------------------------------------------------------------------------------

//Jump 
	_bJumps = false;
	_myJump = JumpBy::create(0.65f, Point(0, 0), 300, 1); //time, �ʧ@���I, height, ����
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
	
	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(AnimationAction::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(AnimationAction::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(AnimationAction::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
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
	_bCollision = false; //���������I��

	if (_level == 2 || _level == 3) {
		_pDel_upper = _pUp_head;
		while (_pUp_head != nullptr) { //�W��]�D
			_pUp_head = _pUp_head->link;
			this->removeChild(_pDel_upper);
			delete _pDel_upper;
			_pDel_upper = _pUp_head;
		}
		_pDel_lower = _pLow_head;
		while (_pLow_head != nullptr) { //�U��]�D
			_pLow_head = _pLow_head->link;
			this->removeChild(_pDel_lower);
			delete _pDel_lower;
			_pDel_lower = _pLow_head;
		}
	}
	_pDel_middle = _pMid_head;
	while (_pMid_head != nullptr) { //�����]�D
		_pMid_head = _pMid_head->link;
		this->removeChild(_pDel_middle);
		delete _pDel_middle;
		_pDel_middle = _pMid_head;
	}

	_pDel_bg = _pBG_head;
	while (_pBG_head != nullptr) { //�I��
		_pBG_head = _pBG_head->link;
		this->removeChild(_pDel_bg);
		delete _pDel_bg;
		_pDel_bg = _pBG_head;
	}
}

void AnimationAction::GameOver()					//�C������
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
	_scoring->updateRanking(_iScore); //�Ʀ��s

	_track->stopAllActions(); _track->setVisible(false);
	_runner->stopAllActions(); _runner->setVisible(false);
	_pausebtn->setVisible(false); //�Ȱ��s
	_black_bg->setVisible(true); //�«�

	//---------------------------------------------
	if (_bWin) { //���\
		_winScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���
		_window_win->setVisible(true);
		_retrybtn_win->setVisible(true);
		_rankingbtn_win->setVisible(true);
		_homebtn_win->setVisible(true);
		_nextlevelbtn_win->setVisible(true);
	}
	else { //����
		_loseScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���
		_window_lose->setVisible(true);
		_retrybtn_lose->setVisible(true);
		_rankingbtn_lose->setVisible(true);
		_homebtn_lose->setVisible(true);
		_nextlevelbtn_lose->setVisible(true);
	}
}

void AnimationAction::doStep(float dt)
{
	auto runnerX = _runner->getPositionX(); //���ax�y��
	auto runnerY = _runner->getPositionY(); //���ay�y��

	//--------------------------------------------------------------------------------------------
	if (_bStartGame) {									//Start�s���U
		_fcount += dt;
		if (_fcount >= 1.0f) {
			_fcount -= 1.0f;
			_iCountdowntime--;
		}
		if (_iCountdowntime == 0) _countdownText->setString("GO");
		else  if (_iCountdowntime < 0) {							//�C���}�l
			_fcount = 0; //�p���k�s
			_bStartGame = false;
			_bGameing = true; //�C���i�椤
			_track->runAction(trackAni);//���Ƽ���y�D
			_runner->runAction(); //�]�̰ʵe
			_bCollision = true; //�I�������}��
			
			auto signAni = MoveBy::create(4.0f, Point(1500.0f, 0)); //�˼ƵP�ʵe
			_sign->runAction(signAni);
			auto signAni2 = MoveBy::create(4.0f, Point(1500.0f, 0));
			_countdownText->runAction(signAni2);

			//���ͨC��Ĥ@��node
			if (_level == 2 || _level == 3) { // Level 2 && 3
				auto rand = (bool)cocos2d::RandomHelper::random_int(0, 1); //�W��]�D
				_pUp_head = new CObstacle(UPPER_TRACK);
				this->addChild(_pUp_head, 1);
				_pUp_head->link = nullptr;
				_pUp_tail = _pUp_head;
				_pUp_head->setVisible(rand); //�H���X�{
				_col_up = _pUp_head; //����e���ê��

				auto rand3 = (bool)cocos2d::RandomHelper::random_int(0, 1); //�U��]�D
				_pLow_head = new CObstacle(LOWER_TRACK);
				this->addChild(_pLow_head, 1);
				_pLow_head->link = nullptr;
				_pLow_tail = _pLow_head;
				_pLow_head->setVisible(rand3); //�H���X�{
				_col_low = _pLow_head; //����e���ê��
			}
			auto rand2 = (bool)cocos2d::RandomHelper::random_int(0, 1); //�����]�D
			_pMid_head = new CObstacle(MIDDLE_TRACK);
			this->addChild(_pMid_head, 1);
			_pMid_head->link = nullptr;
			_pMid_tail = _pMid_head;
			_pMid_head->setVisible(rand2); //�H���X�{
			_col_mid = _pMid_head; //����e���ê��

			//-----------------------------------------------------------------
			auto rand4 = (bool)cocos2d::RandomHelper::random_int(0, 1); //�I��
			_pBG_head = new CObstacle(BG_TRACK);
			this->addChild(_pBG_head, 0);
			_pBG_head->link = nullptr;
			_pBG_tail = _pBG_head;
			_pBG_head->setVisible(rand4); //�H���X�{
		}
		else _countdownText->setString(StringUtils::format("%d", _iCountdowntime)); //�˼�
	}
	//--------------------------------------------------------------------------------------------
	if (_bGameing && _iCountdowntime < 0) {
		if (_fpercent <= 0) {								//�C������
			_bGameing = false;
			_bWin = true; //�ӧQ
			GameOver();
		}
		else {												//�C���i�椤
			//�i�ױ�
			_fpercent -= dt * 3;
			_loadingbar->setPercent(_fpercent);

			//���Ͳ��ʭI��
			_fcount_bgTree += dt; //�p��
			_fRandTime_bgTree = cocos2d::RandomHelper::random_real(0.5f, 2.0f);	//�H���ɶ�
			if (_fcount_bgTree >= _fRandTime_bgTree) {
				_fcount_bgTree -= _fRandTime_bgTree;
				_pGet_bg = new CObstacle(BG_TRACK);
				this->addChild(_pGet_bg, 0);
				_pGet_bg->link = nullptr;
				_pBG_tail->link = _pGet_bg;
				_pBG_tail = _pGet_bg;
			}

			//���ͻ�ê��
			_fcount += dt; //�p��
			_fRandTime = cocos2d::RandomHelper::random_real(1.5f, 3.0f);	//�H���ɶ�
			_bUp = cocos2d::RandomHelper::random_int(0, 1);  //�W��]�D		//�H����ê��
			_bMid = cocos2d::RandomHelper::random_int(0, 1); //�����]�D
			_bLow = cocos2d::RandomHelper::random_int(0, 1); //�U��]�D
			if (_fcount >= _fRandTime) {
				_fcount -= _fRandTime;
				if(_level == 2 || _level == 3) {					// Level 2 && 3
					if (_bUp) { //�W��]�D
						_pGet_upper = new CObstacle(UPPER_TRACK);
						this->addChild(_pGet_upper, 1);
						_pGet_upper->link = nullptr;
						_pUp_tail->link = _pGet_upper;
						_pUp_tail = _pGet_upper;
					}
					if (_bMid) { //�����]�D
						_pGet_middle = new CObstacle(MIDDLE_TRACK);
						this->addChild(_pGet_middle, 1);
						_pGet_middle->link = nullptr;
						_pMid_tail->link = _pGet_middle;
						_pMid_tail = _pGet_middle;
					}
					if (_bLow) { //�U��]�D
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

			//��ê���ʵe���� �ۦ�R��
			if (_level == 2 || _level == 3) {						// Level 2 && 3
				if (_pUp_head->link != nullptr) {
					if (_pUp_head->_bMoveDone) {		//UP

						if (_pUp_head->getType() == SMALL_ROCK) _iScore = _scoring->addSmallRockScore(_iScore);
						if (_pUp_head->getType() == BIG_ROCK) _iScore = _scoring->addBigRockScore(_iScore);
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���

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
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���

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
					_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���

					_pDel_middle = _pMid_head;
					_pMid_head = _pMid_head->link;
					this->removeChild(_pDel_middle);
					delete _pDel_middle;
				}
			}
			if (_pBG_head->link != nullptr) {							// �I��
				if (_pBG_head->_bMoveDone) {			//MID
					_pDel_bg = _pBG_head;
					_pBG_head = _pBG_head->link;
					this->removeChild(_pDel_bg);
					delete _pDel_bg;
				}
			}

			//���ͳ��H
			if (_level == 3) {
				_fcount_snowman += dt; //�p��
				_fRandTime_snowman = cocos2d::RandomHelper::random_real(2.0f, 4.0f);	//���H�H���ɶ�
				if (_fcount_snowman >= _fRandTime_snowman && !_bSnowman_show) {
					if (_snowman == nullptr) {
						_snowman = new CEnemy(_iRunPosition); //create Snowman
						this->addChild(_snowman, 2);
					}
					_fcount_duration = 0; //���]��ܭp��
					_bSnowman_show = true;
					//_fcount_snowman = 0; //���]�p��
					_fcount_snowball = 0;
				}

				_fRandTime_snowball = cocos2d::RandomHelper::random_real(2.0f, 3.0f);	//���y�H���ɶ�
				if (_bSnowman_show) {
					_fcount_duration += dt; //�p��
					if (_fcount_duration >= 7.0f) { //����X�{7��
						_snowman->snowmanFadeOut(); //���H&���y�H�X
					}

					_fcount_snowball += dt;
					if (_fcount_snowball >= _fRandTime_snowball) { //���H�X�{
						_fcount_snowball -= _fRandTime_snowball;
						_snowman->throwSnowball(); //��X���y
					}

					if (_snowman->_bFadeOutDone) { //�[��
						_iScore = _scoring->addSnowBallScore(_iScore);
						_gamingScoreText->setString(StringUtils::format("%d", _iScore)); //��ܤ���
					}

					//���y�I�����a
					if (_bCollision) {
						auto snowball1x = _snowman->getSnowballX(1); //�Ĥ@�����yX�y��
						auto snowball2x = _snowman->getSnowballX(2); //�ĤG�����yX�y��
						auto snowball3x = _snowman->getSnowballX(3); //�ĤT�����yX�y��
						//auto snowbally = _snowman->getSnowballY(); //���yY�y��

						if (snowball1x > (runnerX - 55.f) && snowball1x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(1, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //����
						}
						else if (snowball2x > (runnerX - 55.f) && snowball2x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(2, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //����
						}
						else if (snowball3x >(runnerX - 55.f) && snowball3x < (runnerX + 55.f) && !_bJumps) {
							_snowman->setSnowballVisible(3, false);
							_runner->setDepressedFace();
							_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
							_fHP -= dt * 30.f;
							_HPbar->setPercent(_fHP); //����
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
					//���ҳ��H
					if (_snowman->_bFadeOutDone) {
						_bSnowman_show = false; //�������H
						_fcount_snowman = 0; //���]�ɶ�

						this->removeChild(_snowman); //�R��
						_snowmanDel = _snowman;
						_snowman = nullptr;
						delete _snowmanDel;
					}
				}
			} //level 3 end

		} //else(gaming) end
	}
	

	//--------------------------------------------------------------------------------------------
	if (_bMoving && _level > 1) {						//�ưʿù�
		if (!_bSlide && !_bJumps && _bGameing && _iCountdowntime < 0 && !_pauseBtnPressed) {
			if (_fBeganY < _fEndY && (_fEndY - _fBeganY) > 3.0f && _iRunPosition < UPPER_TRACK) { //���W��
				_iRunPosition++; //���W�@�Ӷ]�D
				_bSlide = true;
				auto actionBody = this->getChildByName("runnerRoot");
				actionBody->runAction(Sequence::create(_SlideUp, _slideCallback, NULL));
				if (_snowman != nullptr) _snowman->slideUp();

				auto SlideUp = MoveBy::create(0.5f, Point(0, 158.0f));
			}
			else if (_fBeganY > _fEndY && (_fBeganY - _fEndY) > 3.0f && _iRunPosition > LOWER_TRACK) { //���U��
				_iRunPosition--; //���U�@�Ӷ]�D
				_bSlide = true;
				auto actionBody = this->getChildByName("runnerRoot");
				actionBody->runAction(Sequence::create(_SlideDown, _slideCallback, NULL));
				if (_snowman != nullptr) _snowman->slideDown();

				auto SlideDown = MoveBy::create(0.5f, Point(0, -158.0f));
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	//Runner Scale (�ھڦ�q)
	if (_fHP >= 75.0f) _runner->setScale(0.5f);
	else if (_fHP < 75.0f && _fHP >= 50.0f) _runner->setScale(0.55f);
	else if (_fHP < 50.0f && _fHP >= 25.0f) _runner->setScale(0.6f);
	else _runner->setScale(0.65f);
	//--------------------------------------------------------------------------------------------
	//�I������
	if (_iRunPosition == UPPER_TRACK && _bCollision) {			//UP
		auto rockPos_Upx = _pUp_head->getPositionX();
		if (rockPos_Upx >(runnerX - 55.f) && rockPos_Upx < (runnerX + 55.f) && _pUp_head->getVisible()) {
			if ((_pUp_head->getType() == BIG_ROCK && _fHP < 50.0f) || !_bJumps) {
				_runner->setDepressedFace();
				_runner->setColor(cocos2d::Color3B(255, 114, 114)); //red
				_fHP -= dt * 30.f;
				_HPbar->setPercent(_fHP); //����
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
				_HPbar->setPercent(_fHP); //����
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
				_HPbar->setPercent(_fHP); //����
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
	if (_fHP <= 0 && _bGameing) {								//�C������
		_bGameing = false;
		_bWin = false; //����
		GameOver();
	}
}

bool AnimationAction::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();

	//slide
	_fBeganY = touchLoc.y;

	//���s���A
	_startbtn->onTouchBegan(touchLoc);
	_nextbtn->onTouchBegan(touchLoc);
	_prebtn->onTouchBegan(touchLoc);
	if (_pausebtn->onTouchBegan(touchLoc)) _pauseBtnPressed = _pausebtn->getPressed(); //���s�I���ɤ����D
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

void  AnimationAction::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();

	//slide
	_fEndY = touchLoc.y;
	_bMoving = true;

	//���s���A
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

void  AnimationAction::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();

	//Jump
	auto actionBody = this->getChildByName("runnerRoot")->getChildByTag(101);
	if (!_bJumps && _bGameing && _iCountdowntime < 0 && !_pauseBtnPressed && !_bMoving) {  // ���D

		//���׮ھڦ�q����
		if (_fHP >= 75.0f) actionBody->runAction(Sequence::create(_myJump, _jumpCallback, NULL));
		else if (_fHP < 75.0f && _fHP >= 50.0f) actionBody->runAction(Sequence::create(_myJump2, _jumpCallback, NULL));
		else if (_fHP < 50.0f && _fHP >= 25.0f) actionBody->runAction(Sequence::create(_myJump3, _jumpCallback, NULL));
		else actionBody->runAction(Sequence::create(_myJump4, _jumpCallback, NULL));

		_runner->setHappyFace();
		_runner->setColor(cocos2d::Color3B(211, 230, 255));
		unsigned int eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", false);  // ���񭵮���
		_bJumps = true;
	}

	//slide
	_fEndY = touchLoc.y;
	_bMoving = false; //�P�_���O�_�i���D�~����

	//���s���A
	if (_startbtn->onTouchEnded(touchLoc)) {						//�}�l�s
		_black_bg->setVisible(false);
		_startbtn->setVisible(false);
		_nextbtn->setVisible(false);
		_prebtn->setVisible(false);
		_tutorial_lv1_1->setVisible(false);
		_tutorial_lv1_2->setVisible(false);
		_tutorial_lv2->setVisible(false);
		_tutorial_lv3->setVisible(false);
		_mouseAni->setVisible(false); //�ƹ��ʵe����
		_wasdkey->setVisible(false); //WASD��L�ʵe
		//_udkey->setVisible(false); //��L�ʵe
		_track->setVisible(true); //�y�D���
		//runner->setVisible(true); //�������
		_runner->setVisible(true); //�������
		_pausebtn->setVisible(true); //�Ȱ��s���
		_bStartGame = true; //�C���}�l
	}
	if (_nextbtn->onTouchEnded(touchLoc)) {							//�U�@���s
		_tutorial_lv1_1->setVisible(false);
		_tutorial_lv1_2->setVisible(true);
		_nextbtn->setVisible(false);
		_prebtn->setVisible(true);
		_mouseAni->setVisible(false); //�ƹ��ʵe����
	}
	if (_prebtn->onTouchEnded(touchLoc)) {							//�W�@���s
		_tutorial_lv1_1->setVisible(true);
		_tutorial_lv1_2->setVisible(false);
		_nextbtn->setVisible(true);
		_prebtn->setVisible(false);
		_mouseAni->setVisible(true); //�ƹ��ʵe�}��
	}
	if (_pausebtn->onTouchEnded(touchLoc)) {						//�Ȱ��s
		_pause_window->setVisible(true);
		_black_bg->setVisible(true);
		_track->setVisible(false);
		_runner->setVisible(false); //��������
		_pausebtn->setVisible(false); //���üȰ��s
		_resumebtn->setVisible(true);
		_retrybtn->setVisible(true);
		_homebtn->setVisible(true);
		_bGameing = false; //�Ȱ��i�ױ�
		if (_iCountdowntime >= 0) _bStartGame = false; //�Ȱ��˼�
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
	if (_resumebtn->onTouchEnded(touchLoc)) {						//�~��C���s
		_pause_window->setVisible(false);
		_black_bg->setVisible(false);
		_track->setVisible(true);
		_runner->setVisible(true); //�������
		_pausebtn->setVisible(true); //�Ȱ��s���
		_resumebtn->setVisible(false);
		_retrybtn->setVisible(false);
		_homebtn->setVisible(false);
		_bGameing = true; //�~��i�ױ�
		if(_iCountdowntime >= 0) _bStartGame = true; //�~��˼�

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
	if (_retrybtn->onTouchEnded(touchLoc) || _retrybtn_win->onTouchEnded(touchLoc) || _retrybtn_lose->onTouchEnded(touchLoc)) {		//���s�C���s
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
	if (_homebtn->onTouchEnded(touchLoc) || _homebtn_win->onTouchEnded(touchLoc) || _homebtn_lose->onTouchEnded(touchLoc)) {		//�^�����s
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
	if (_rankingbtn_win->onTouchEnded(touchLoc) || _rankingbtn_lose->onTouchEnded(touchLoc)) {	//�d�ݱƦ�s
		_window_win->setVisible(false);			//�����C����������
		_window_lose->setVisible(false);
		_retrybtn_win->setVisible(false);
		_rankingbtn_win->setVisible(false);
		_homebtn_win->setVisible(false);
		_nextlevelbtn_win->setVisible(false);
		_retrybtn_lose->setVisible(false);
		_rankingbtn_lose->setVisible(false);
		_homebtn_lose->setVisible(false);
		_nextlevelbtn_lose->setVisible(false);
		//�}�ұƦ�]-------------------------------------
		_okaybtn->setVisible(true);
		_scoring->setVisible(true);
	}
	if (_okaybtn->onTouchEnded(touchLoc)) {														//�����Ʀ�s
		if (_bWin) {							//�}�ҹC����������
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
		//�����Ʀ�]-------------------------------------
		_okaybtn->setVisible(false);
		_scoring->setVisible(false);
	}
	if (_nextlevelbtn_win->onTouchEnded(touchLoc) || _nextlevelbtn_lose->onTouchEnded(touchLoc)) {	//�U�@���s
		if (_level < 3) (MainMenu::level)++; //��U�@��
		auto Scene = AnimationAction::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, Scene, Color3B(255, 255, 255)));
	}
}