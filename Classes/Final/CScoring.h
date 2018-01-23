#pragma once
#ifndef __CSCORING_H__
#define __CSCORING_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;

class CScoring
{
private:

	cocos2d::Node   *_root;

	cocos2d::Sprite *_ranking_window;
	cocos2d::Sprite *_player2_window;
	cocos2d::Sprite *_player3_window;
	cocos2d::Sprite *_player4_window;

	cocos2d::ui::Text *_player1Text;
	cocos2d::ui::Text *_player2Text;
	cocos2d::ui::Text *_player3Text;
	cocos2d::ui::Text *_player4Text;
	cocos2d::ui::Text *_playerText; //�ۤv

	bool _bVisible; // �O�_���

public:
	CScoring(const char *csbname, cocos2d::Layer &parent);
	~CScoring();

	static int _iplayerNum; //���a�H��
	static int _iplayer1Score; //���a1����
	static int _iplayer2Score; //���a2����
	static int _iplayer3Score; //���a3����
	static int _iplayer4Score; //���a4����

	void setVisible(bool bVis);
	int addSmallRockScore(int score); //�p���Y�[��
	int addBigRockScore(int score); //�j���Y�[��
	int addSnowBallScore(int score); //���y�[��
	void updateRanking(int score); //��s�Ʀ�

};

#endif // __CSCORING_H__