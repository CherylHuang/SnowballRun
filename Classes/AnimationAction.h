#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "Final/C3SButton.h"
#include "Final/CRunner.h"
#include "Final/CObstacle.h"
#include "Final/CEnemy.h"
#include "Final/CScoring.h"

#define UPPER_TRACK 3
#define MIDDLE_TRACK 2
#define LOWER_TRACK 1

class AnimationAction : public cocos2d::Layer
{
private:
	cocos2d::Sprite *_black_bg; //�¹��B��
	cocos2d::ui::Slider *_loadingbar; //�i�ױ�
	cocos2d::ui::LoadingBar *_HPbar; //��q��
	cocos2d::ui::ImageView *_HPbar_bg; //��q����
	cocos2d::ui::ImageView *_score_window; //���Ʃ���
	cocos2d::Sprite *_sign; //�˼ƵP

	//tutorial
	cocos2d::Sprite *_tutorial_lv1_1;
	cocos2d::Sprite *_tutorial_lv1_2;
	cocos2d::Sprite *_tutorial_lv2;
	cocos2d::Sprite *_tutorial_lv3;
	cocos2d::Sprite *_mouseAni;
	cocos2d::Sprite *_wasdkey;
	//cocos2d::Sprite *_udkey;
	C3SButton *_startbtn;
	C3SButton *_nextbtn;
	C3SButton *_prebtn;

	//start
	bool	_bStartGame; //�C���O�_�}�l
	bool	_bGameing; //�C���O�_�i�椤
	float	_fcount; //�p��
	float	_fpercent; //�i�ױ�
	float	_fHP; //��q��
	int		_iCountdowntime; //�˼Ƭ��
	cocos2d::ui::Text *_countdownText;

	//pause window
	bool _pauseBtnPressed; //�Ȱ��s�O�_�Q��
	cocos2d::Sprite *_pause_window;
	C3SButton *_pausebtn;
	C3SButton *_resumebtn;
	C3SButton *_retrybtn;
	C3SButton *_homebtn;

	//runner
	unsigned int _iRunSound;
	CRunner *_runner; //���a����
	cocos2d::Sprite *_runner_pos; //�����m
	cocos2d::Sprite *_track; //�y�D
	cocos2d::RepeatForever *trackAni; //�y�D�ʵe

	cocos2d::Sequence *_mySequence;
	cocos2d::Action   *_myAction;

	//Jump
	bool _bJumps; //���D�ʵe�O�_�i�椤
	cocos2d::JumpBy   *_myJump; //�̰�(original)
	cocos2d::JumpBy   *_myJump2;
	cocos2d::JumpBy   *_myJump3;
	cocos2d::JumpBy   *_myJump4; //�̸G
	cocos2d::CallFunc *_jumpCallback;
	cocos2d::Point _runnerPt;

	//slide
	int		_iRunPosition; //���a�]�D��m
	bool	_bMoving; //����b�ù��W�ư�
	float	_fBeganY; //�ưʰ_�IY�y��
	float	_fEndY; //�ưʲ��IY�y��
	bool	_bSlide; //���Ⲿ�ʰʵe�O�_�i�椤
	cocos2d::MoveBy   *_SlideUp;
	cocos2d::MoveBy   *_SlideDown;
	cocos2d::CallFunc *_slideCallback;

	//game over
	bool _bWin; //��F���I
	void GameOver();
	
	//win window
	cocos2d::Sprite *_window_win;
	C3SButton *_retrybtn_win, *_rankingbtn_win, *_homebtn_win, *_nextlevelbtn_win;
	//lose window
	cocos2d::Sprite *_window_lose;
	C3SButton *_retrybtn_lose, *_rankingbtn_lose, *_homebtn_lose, *_nextlevelbtn_lose;

	//obstacle generate
	float _fcount_bgTree; //�I����
	float _fRandTime_bgTree; //�I���� �H���ɶ�
	float _fRandTime; //�H���ɶ�
	int	_bUp, _bMid, _bLow; //�C���ê���X�{���A
	//link list
	CObstacle *_pUp_head, *_pMid_head, *_pLow_head, *_pBG_head; //������C�e��
	CObstacle *_pUp_tail, *_pMid_tail, *_pLow_tail, *_pBG_tail; //�O����C����
	CObstacle *_pDel_upper, *_pDel_middle, *_pDel_lower, *_pDel_bg; //�R��node
	CObstacle *_pGet_upper, *_pGet_middle, *_pGet_lower, *_pGet_bg; //�ͦ��C���ê��
	CObstacle *_pTrackObj_upper, *_pTrackObj_middle, *_pTrackObj_lower, *_pTrackObj_bg; //�Ȱ����s��
	void deleteObstacle(); //�R���Ѿl��ê��

	//SnowMan
	bool	_bSnowman_show; //���H�O�_���
	float	_fcount_snowman; //�p��
	float	_fRandTime_snowman; //�H���ɶ�
	float	_fRandTime_snowball; //�H���ɶ�
	float	_fcount_duration; //�p�ɳ��H�s�b�ɶ�
	float	_fcount_snowball; //�p��
	CEnemy	*_snowman;
	CEnemy	*_snowmanDel;

	//�I������
	bool _bCollision;
	bool _bSnowballCol;
	CObstacle *_col_up, *_col_mid, *_col_low; //�����U�樤��e�ݻ�ê��

	//Ranking and score
	int _iScore; //���Ƭ���
	cocos2d::ui::Text *_gamingScoreText; //�C������s����
	cocos2d::ui::Text *_winScoreText; //�ӧQ��������
	cocos2d::ui::Text *_loseScoreText; //���Ѥ�������
	CScoring	*_scoring; //�p�� & �ƦW
	C3SButton	*_okaybtn; //for ran

public:
	~AnimationAction();
    static cocos2d::Scene* createScene();

	int _level; //����������d

    virtual bool init();
	void doStep(float dt);

	void JumpActionFinished(); // jump �� CallBack �禡
	void SlideActionFinished(); // slide �� CallBack �禡

	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 

    CREATE_FUNC(AnimationAction);
};

#endif // __HELLOWORLD_SCENE_H__
