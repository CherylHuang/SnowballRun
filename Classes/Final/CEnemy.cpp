#include "CEnemy.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d;

CEnemy::CEnemy(const int trackPos)
{
	_Move = nullptr;
	_bVisible = true;
	_bFadeOutDone = false;
	_iballs = 0;
	_ithrow = 0;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rungame.plist");

	_objRoot = CSLoader::createNode("snowmanNode.csb");
	if(trackPos == 3)_objRoot->setPosition(139.0f, 482.8f); //UPPER_TRACK
	else if(trackPos == 1) _objRoot->setPosition(139.0f, 166.8f); //LOWER_TRACK
	else _objRoot->setPosition(139.0f, 324.8f); //MIDDLE_TRACK trackPos == 2
	_objRoot->setScale(0.72f);
	this->addChild(_objRoot, 2);

	_objAction = (ActionTimeline *)CSLoader::createTimeline("snowmanNode.csb");
	_objAction->setDuration(4.0f);
	_objRoot->runAction(_objAction);
	_objAction->gotoFrameAndPlay(0, 24, false);
	_objAction->retain();

	_SnowMan = (Sprite *)_objRoot->getChildByName("SnowMan");
	_snowballPic1 = (Sprite *)_objRoot->getChildByName("snowball1");
	_snowballPic2 = (Sprite *)_objRoot->getChildByName("snowball2");
	_snowballPic3 = (Sprite *)_objRoot->getChildByName("snowball3");
	_snowballPos = (Node *)_objRoot->getChildByName("snowballPos");

	_Move = MoveBy::create(4.5f, Point(1676.25f, 0));
	_MoveAni = RepeatForever::create(_Move); //建立無限循環動畫
	auto move2 = RepeatForever::create(_Move);
	auto move3 = RepeatForever::create(_Move);
	_snowballPic1->runAction(_MoveAni);
	_snowballPic2->runAction(move2);
	_snowballPic3->runAction(move3);
	_snowballPic1->pause();
	_snowballPic2->pause();
	_snowballPic3->pause();
}

CEnemy::~CEnemy()
{
	if (_objAction != nullptr) _objAction->release();
}

bool CEnemy::throwSnowball()
{
	//animation
	if (_iballs == 0 || (_ithrow == 3 && _iballs < 3)) {
		_snowballPic1->resume();
		_ithrow = 1;
		_iballs++;
		return true;
	}
	else if (_ithrow == 1 && _iballs < 3) {
		_snowballPic2->resume();
		_ithrow = 2;
		_iballs++;
		return true;
	}	
	else if (_ithrow == 2 && _iballs < 3) {
		_snowballPic3->resume();
		_ithrow = 3;
		_iballs++;
		return true;
	}
	else return false;
}

bool CEnemy::resetSnowBall() /////////not used
{
	if (_iballs == 1) {
		if (_ithrow == 1) { _snowballPic1->pause(); _snowballPic1->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 2) { _snowballPic2->pause(); _snowballPic2->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 3) { _snowballPic3->pause(); _snowballPic3->setPosition(_snowballPos->getPosition()); }
		_iballs--;
	}
	else if (_iballs == 2) {
		if (_ithrow == 1) { _snowballPic3->pause(); _snowballPic3->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 2) { _snowballPic1->pause(); _snowballPic1->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 3) { _snowballPic2->pause(); _snowballPic2->setPosition(_snowballPos->getPosition()); }
		_iballs--;
	}
	else if (_iballs == 3) {
		if (_ithrow == 1) { _snowballPic2->pause(); _snowballPic2->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 2) { _snowballPic3->pause(); _snowballPic3->setPosition(_snowballPos->getPosition()); }
		else if (_ithrow == 3) { _snowballPic1->pause(); _snowballPic1->setPosition(_snowballPos->getPosition()); }
		_iballs--;
	}
	else return false;
}

bool CEnemy::balloverwindow() /////////not used
{
	auto ball1pos = _snowballPic1->getPositionX();
	auto ball2pos = _snowballPic2->getPositionX();
	auto ball3pos = _snowballPic3->getPositionX();
	if (ball1pos >= 1345.0f || ball2pos >= 1345.0f || ball3pos >= 1345.0f)
		return true;
}

void CEnemy::snowmanFadeOut()
{
	FadeOut * fadeout = FadeOut::create(1.25f);
	_fadeoutCallback = CallFunc::create(this, callfunc_selector(CEnemy::ActionFinished));
	_objRoot->runAction(Sequence::create(fadeout, _fadeoutCallback, NULL));
}

void CEnemy::ActionFinished()
{
	_bFadeOutDone = true; //動作完成
}

void CEnemy::setVisible(bool bVisible)
{
	_bVisible = bVisible;
	if (_bVisible) _objRoot->setVisible(true); //顯示
	else _objRoot->setVisible(false); //隱藏
}

void CEnemy::setSnowballVisible(int i, bool bVis)
{
	if (i == 1) _snowballPic1->setVisible(bVis);
	if (i == 2) _snowballPic2->setVisible(bVis);
	if (i == 3) _snowballPic3->setVisible(bVis);
}

void CEnemy::pause()
{
	_objRoot->pause();
	_snowballPic1->pause();
	_snowballPic2->pause();
	_snowballPic3->pause();
}

void CEnemy::resume()
{
	_objRoot->resume();
	_snowballPic1->resume();
	_snowballPic2->resume();
	_snowballPic3->resume();
}

void CEnemy::slideUp()
{
	auto SlideUp = MoveBy::create(0.5f, Point(0, 158.0f));
	_objRoot->runAction(SlideUp);
}

void CEnemy::slideDown()
{
	auto SlideDown = MoveBy::create(0.5f, Point(0, -158.0f));
	_objRoot->runAction(SlideDown);
}

float CEnemy::getSnowballX(int i)
{
	auto snowball1X = _objRoot->convertToWorldSpace(_snowballPic1->getPosition()).x;
	auto snowball2X = _objRoot->convertToWorldSpace(_snowballPic2->getPosition()).x;
	auto snowball3X = _objRoot->convertToWorldSpace(_snowballPic3->getPosition()).x;
	if (i == 1) return(snowball1X);
	if (i == 2) return(snowball2X);
	if (i == 3) return(snowball3X);
}

float CEnemy::getSnowballY()
{
	auto snowballY = _objRoot->convertToWorldSpace(_snowballPic1->getPosition()).y;
	return(snowballY);
}

float CEnemy::getSnowballwidth()
{
	auto size = _snowballPic1->getContentSize().width;
	return(size);
}