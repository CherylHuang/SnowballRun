#include "CRunner.h"

USING_NS_CC;

using namespace CocosDenshion;
//
// �إߨ��⤧�e�A�������T�w�w�gŪ�J�x�s����Ҧ��ʺA�Ϥ��� plist ��
//

CRunner::CRunner(const char *csbname, cocos2d::Layer &parent)
{
	// Ū������
	_runnerRoot = CSLoader::createNode(csbname);
//	_runnerRoot = (cocos2d::Node*)getNode->getChildByName("RunnerRoot");
	_runnerRoot->setPosition(0,0); // �w�]��b (0,0)
	_jumpPart = _runnerRoot->getChildByName("jumping_part");
	_body = (cocos2d::Sprite *)_jumpPart->getChildByName("body_circle01");
	_body->setColor(Color3B(255, 255, 255));
	_normalFace = (cocos2d::Node *)_jumpPart->getChildByName("NormalFace");
	_happyFace = (cocos2d::Node *)_jumpPart->getChildByName("HappyFace");
	_depressedFace = (cocos2d::Node *)_jumpPart->getChildByName("DepressedFace");
	_happyFace->setVisible(false);
	_depressedFace->setVisible(false);

	// �]�w�P�_�ϰ�
	float runnerX = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).x;
	float runnerY = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).y;
	_bodySize = _jumpPart->getContentSize();
	_RunnerRect.size = _bodySize;
	_RunnerRect.origin.x = runnerX - _bodySize.width*0.5f;
	_RunnerRect.origin.y = runnerY - _bodySize.height*0.5f;

	parent.addChild(_runnerRoot, 5);
}


CRunner::~CRunner()
{
	_action->release();
}


void CRunner::setAnimation(const char *plistFile)
{
	// ���B���]�x�s����Ҧ��ʺA�Ϥ��� plist �ɤw�gŪ�J
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile(plistFile);
	auto animation = cache->getAnimation("running");
	_action = RepeatForever::create(Animate::create(animation));
	_action->retain();
}

void CRunner::runAction()
{
	_body->runAction(_action);
}

void CRunner::stopAllActions()
{
	_body->stopAllActions();
}

float CRunner::getPositionX()
{
	float runnerX = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).x;
	return(runnerX);
}

float CRunner::getPositionY()
{
	float runnerY = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).y;
	return(runnerY);
}

void CRunner::setPosition(const Point &pos)
{
	_runnerRoot->setPosition(pos);

	// �]�w�P�_�ϰ�
	float runnerX = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).x;
	float runnerY = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).y;
	_bodySize = _jumpPart->getContentSize();
	_RunnerRect.size = _bodySize;
	_RunnerRect.origin.x = runnerX - _bodySize.width*0.5f;
	_RunnerRect.origin.y = runnerY - _bodySize.height*0.5f;
}

void CRunner::setPosition(const float x, const float y)
{
	_runnerRoot->setPosition(x,y);
}

void CRunner::setColor(const Color3B& color)
{
	_body->setColor(color);
}

void CRunner::setScale(const float scale) 
{
	_runnerRoot->setScale(scale);

	// �]�w�P�_�ϰ�
	float runnerX = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).x;
	float runnerY = _runnerRoot->convertToWorldSpace(_jumpPart->getPosition()).y;
	_bodySize = _jumpPart->getContentSize();
	_RunnerRect.size = _bodySize;
	_RunnerRect.origin.x = runnerX - _bodySize.width*0.5f;
	_RunnerRect.origin.y = runnerY - _bodySize.height*0.5f;
}

void CRunner::setVisible(const bool visible)
{
	_runnerRoot->setVisible(visible);
}

void CRunner::setjumpTag(int tag)
{
	_jumpPart->setTag(tag);
}

void CRunner::setNormalFace()
{
	_normalFace->setVisible(true);
	_happyFace->setVisible(false);
	_depressedFace->setVisible(false);
}

void CRunner::setHappyFace()
{
	_normalFace->setVisible(false);
	_happyFace->setVisible(true);
	_depressedFace->setVisible(false);
}

void CRunner::setDepressedFace()
{
	_normalFace->setVisible(false);
	_happyFace->setVisible(false);
	_depressedFace->setVisible(true);
}

bool CRunner::playerCollision(cocos2d::Point inPos) //�I������
{
	if (_RunnerRect.containsPoint(inPos))
	{
		return(true); // ���b�d��
	}
	return(false);
}

float CRunner::getBodySize()
{
	auto size = _jumpPart->getContentSize().width;
	return(size);
}