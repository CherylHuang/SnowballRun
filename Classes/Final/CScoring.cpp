#include "CScoring.h"

USING_NS_CC;

using namespace ui;

int CScoring::_iplayerNum = 0; //靜態變數初始值
int CScoring::_iplayer1Score = 0;
int CScoring::_iplayer2Score = 0;
int CScoring::_iplayer3Score = 0;
int CScoring::_iplayer4Score = 0;

CScoring::CScoring(const char *csbname, cocos2d::Layer &parent)
{
	_root = CSLoader::createNode(csbname);
	_root->setPosition(640, 378);
	_ranking_window = dynamic_cast<Sprite*>(_root->getChildByName("ranking_window"));
	_player2_window = dynamic_cast<Sprite*>(_ranking_window->getChildByName("ranking_player2"));
	_player3_window = dynamic_cast<Sprite*>(_ranking_window->getChildByName("ranking_player3"));
	_player4_window = dynamic_cast<Sprite*>(_ranking_window->getChildByName("ranking_player4"));
	_player1Text = dynamic_cast<Text*>(_ranking_window->getChildByName("player1_score"));
	_playerText = dynamic_cast<Text*>(_ranking_window->getChildByName("player_score"));
	_player2Text = dynamic_cast<Text*>(_player2_window->getChildByName("player2_score"));
	_player3Text = dynamic_cast<Text*>(_player3_window->getChildByName("player3_score"));
	_player4Text = dynamic_cast<Text*>(_player4_window->getChildByName("player4_score"));

	_bVisible = false;
	_ranking_window->setVisible(false);
	_player2_window->setVisible(false);
	_player3_window->setVisible(false);
	_player4_window->setVisible(false);

	parent.addChild(_root, 2);
}

CScoring::~CScoring()
{
	
}

void CScoring::setVisible(bool bVisible)
{
	_bVisible = bVisible;
	if (!_bVisible) { //隱藏
		_ranking_window->setVisible(false);
	}
	else { //顯示
		_ranking_window->setVisible(true);
	}
}

int CScoring::addSmallRockScore(int s) //小石頭
{
	auto score = s + 50;
	return(score);
}

int CScoring::addBigRockScore(int s) //大石頭
{
	auto score = s + 100;
	return(score);
}

int CScoring::addSnowBallScore(int s) //雪球
{
	auto score = s + 150;
	return(score);
}

void CScoring::updateRanking(int score)
{
	switch (_iplayerNum)
	{
	case 1:
		_iplayer1Score = score;
		break;
	case 2:
		if (score > _iplayer1Score) {
			_iplayer2Score = _iplayer1Score;
			_iplayer1Score = score;
		}
		else {
			_iplayer2Score = score;
		}
		_player2_window->setVisible(true); //開啟視窗
		break;
	case 3:
		if (score > _iplayer1Score) {
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = _iplayer1Score;
			_iplayer1Score = score;
		}
		else if (score > _iplayer2Score && score <= _iplayer1Score) {
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = score;
		}
		else {
			_iplayer3Score = score;
		}
		_player2_window->setVisible(true); //開啟視窗
		_player3_window->setVisible(true);
		break;
	case 4:
		if (score > _iplayer1Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = _iplayer1Score;
			_iplayer1Score = score;
		}
		else if (score > _iplayer2Score && score <= _iplayer1Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = score;
		}
		else if (score > _iplayer3Score && score <= _iplayer2Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = score;
		}
		else {
			_iplayer4Score = score;
		}
		_player2_window->setVisible(true); //開啟視窗
		_player3_window->setVisible(true);
		_player4_window->setVisible(true);
		break;
	default:
		if (score > _iplayer1Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = _iplayer1Score;
			_iplayer1Score = score;
		}
		else if (score > _iplayer2Score && score <= _iplayer1Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = _iplayer2Score;
			_iplayer2Score = score;
		}
		else if (score > _iplayer3Score && score <= _iplayer2Score) {
			_iplayer4Score = _iplayer3Score;
			_iplayer3Score = score;
		}
		else if (score > _iplayer4Score && score <= _iplayer3Score) {
			_iplayer4Score = score;
		}
		_player2_window->setVisible(true); //開啟視窗
		_player3_window->setVisible(true);
		_player4_window->setVisible(true);
		break;
	}

	_playerText->setString(StringUtils::format("%d", score)); //顯示分數
	_player1Text->setString(StringUtils::format("%d", _iplayer1Score));
	_player2Text->setString(StringUtils::format("%d", _iplayer2Score));
	_player3Text->setString(StringUtils::format("%d", _iplayer3Score));
	_player4Text->setString(StringUtils::format("%d", _iplayer4Score));
}