#include "CObstacle.h"

USING_NS_CC;

CObstacle::CObstacle(const int track)
{
	_rockPic = nullptr;
	_Move = nullptr;
	_moveCallback = nullptr;
	_bMoveDone = false;
	_bVisible = true;

	//石頭種類
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("rungame.plist");
	_iType = cocos2d::RandomHelper::random_int(0, 1);
	switch (_iType) //種類
	{
	case SMALL_ROCK:
		_rockPic = (Sprite *)Sprite::createWithSpriteFrameName("stone_low.png");
		break;
	case BIG_ROCK:
		_rockPic = (Sprite *)Sprite::createWithSpriteFrameName("stone_high.png");
		break;
	default:
		break;
	}
	_rockPic->setScale(0.75f);

	//生成
	auto rootNode = CSLoader::createNode("AnimationAction.csb");
	switch (track) //位置
	{
	case UPPER_TRACK: //上方跑道
		if (_iType == SMALL_ROCK) _objPos = (cocos2d::Sprite *)rootNode->getChildByName("upper_track");
		else _objPos = (cocos2d::Sprite *)rootNode->getChildByName("upper_track_h");
		_rockPic->setPosition(_objPos->getPosition());
		break;
	case MIDDLE_TRACK: //中間跑道
		if (_iType == SMALL_ROCK) _objPos = (cocos2d::Sprite *)rootNode->getChildByName("middle_track");
		else _objPos = (cocos2d::Sprite *)rootNode->getChildByName("middle_track_h");
		_rockPic->setPosition(_objPos->getPosition());
		break;
	case LOWER_TRACK: //下方跑道
		if (_iType == SMALL_ROCK) _objPos = (cocos2d::Sprite *)rootNode->getChildByName("lower_track");
		else _objPos = (cocos2d::Sprite *)rootNode->getChildByName("lower_track_h");
		_rockPic->setPosition(_objPos->getPosition());
		break;
		//---------------------------------------------------------------------------------------------
	case BG_TRACK: //背景
		_iBGType = cocos2d::RandomHelper::random_int(0, 1); //樹 種類
		switch (_iBGType) //種類
		{
		case TREE_ONE:
			_rockPic = (Sprite *)Sprite::createWithSpriteFrameName("tree1.png");
			break;
		case TREE_TWO:
			_rockPic = (Sprite *)Sprite::createWithSpriteFrameName("tree2.png");
			break;
		default:
			break;
		}
		_rockPic->setScale(0.667f);
		if (_iBGType == TREE_ONE) _objPos = (cocos2d::Sprite *)rootNode->getChildByName("tree1");
		else _objPos = (cocos2d::Sprite *)rootNode->getChildByName("tree2");
		_rockPic->setPosition(_objPos->getPosition());
		break;
	default:
		break;
	}
	this->addChild(_rockPic, 0);

	_obstacleSize = _rockPic->getContentSize();

	//animation
	_Move = MoveBy::create(4.0f, Point(1500.0f, 0));
	_moveCallback = CallFunc::create(this, callfunc_selector(CObstacle::moveActionFinished));
	_rockPic->runAction(Sequence::create(_Move, _moveCallback, NULL));
}

CObstacle::~CObstacle()
{
	
}

void CObstacle::moveActionFinished()
{
	_bMoveDone = true; //動作完成
}

void CObstacle::setVisible(bool bVisible)
{
	_bVisible = bVisible;
	if (_bVisible) _rockPic->setVisible(true); //顯示
	else _rockPic->setVisible(false); //隱藏
}

bool CObstacle::getVisible()
{
	return(_bVisible);
}

void CObstacle::setScale(float scale)
{
	_rockPic->setScale(scale);
	_obstacleSize = _rockPic->getContentSize();
}

float CObstacle::getPositionX()
{
	return(_rockPic->getPositionX());
}

float CObstacle::getPositionY()
{
	return(_rockPic->getPositionY());
}

void CObstacle::pause()
{
	_rockPic->pause();
}

void CObstacle::resume()
{
	_rockPic->resume();
}

int CObstacle::getType()
{
	return(_iType);
}

void CObstacle::setColor() //for test
{
	_rockPic->setColor(Color3B(200, 25, 105));
}