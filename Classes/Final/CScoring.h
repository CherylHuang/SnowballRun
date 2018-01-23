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
	cocos2d::ui::Text *_playerText; //

	bool _bVisible; // 琌陪ボ

public:
	CScoring(const char *csbname, cocos2d::Layer &parent);
	~CScoring();

	static int _iplayerNum; //產计
	static int _iplayer1Score; //產1だ计
	static int _iplayer2Score; //產2だ计
	static int _iplayer3Score; //產3だ计
	static int _iplayer4Score; //產4だ计

	void setVisible(bool bVis);
	int addSmallRockScore(int score); //ホ繷だ
	int addBigRockScore(int score); //ホ繷だ
	int addSnowBallScore(int score); //撤瞴だ
	void updateRanking(int score); //穝逼︽

};

#endif // __CSCORING_H__