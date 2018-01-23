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
	cocos2d::Sprite *_black_bg; //黑幕遮屏
	cocos2d::ui::Slider *_loadingbar; //進度條
	cocos2d::ui::LoadingBar *_HPbar; //血量條
	cocos2d::ui::ImageView *_HPbar_bg; //血量底圖
	cocos2d::ui::ImageView *_score_window; //分數底圖
	cocos2d::Sprite *_sign; //倒數牌

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
	bool	_bStartGame; //遊戲是否開始
	bool	_bGameing; //遊戲是否進行中
	float	_fcount; //計時
	float	_fpercent; //進度條
	float	_fHP; //血量條
	int		_iCountdowntime; //倒數秒數
	cocos2d::ui::Text *_countdownText;

	//pause window
	bool _pauseBtnPressed; //暫停鈕是否被按
	cocos2d::Sprite *_pause_window;
	C3SButton *_pausebtn;
	C3SButton *_resumebtn;
	C3SButton *_retrybtn;
	C3SButton *_homebtn;

	//runner
	unsigned int _iRunSound;
	CRunner *_runner; //玩家角色
	cocos2d::Sprite *_runner_pos; //角色位置
	cocos2d::Sprite *_track; //軌道
	cocos2d::RepeatForever *trackAni; //軌道動畫

	cocos2d::Sequence *_mySequence;
	cocos2d::Action   *_myAction;

	//Jump
	bool _bJumps; //跳躍動畫是否進行中
	cocos2d::JumpBy   *_myJump; //最高(original)
	cocos2d::JumpBy   *_myJump2;
	cocos2d::JumpBy   *_myJump3;
	cocos2d::JumpBy   *_myJump4; //最矮
	cocos2d::CallFunc *_jumpCallback;
	cocos2d::Point _runnerPt;

	//slide
	int		_iRunPosition; //玩家跑道位置
	bool	_bMoving; //手指在螢幕上滑動
	float	_fBeganY; //滑動起點Y座標
	float	_fEndY; //滑動終點Y座標
	bool	_bSlide; //角色移動動畫是否進行中
	cocos2d::MoveBy   *_SlideUp;
	cocos2d::MoveBy   *_SlideDown;
	cocos2d::CallFunc *_slideCallback;

	//game over
	bool _bWin; //到達終點
	void GameOver();
	
	//win window
	cocos2d::Sprite *_window_win;
	C3SButton *_retrybtn_win, *_rankingbtn_win, *_homebtn_win, *_nextlevelbtn_win;
	//lose window
	cocos2d::Sprite *_window_lose;
	C3SButton *_retrybtn_lose, *_rankingbtn_lose, *_homebtn_lose, *_nextlevelbtn_lose;

	//obstacle generate
	float _fcount_bgTree; //背景用
	float _fRandTime_bgTree; //背景用 隨機時間
	float _fRandTime; //隨機時間
	int	_bUp, _bMid, _bLow; //每行障礙物出現狀態
	//link list
	CObstacle *_pUp_head, *_pMid_head, *_pLow_head, *_pBG_head; //紀錄串列前端
	CObstacle *_pUp_tail, *_pMid_tail, *_pLow_tail, *_pBG_tail; //記錄串列結尾
	CObstacle *_pDel_upper, *_pDel_middle, *_pDel_lower, *_pDel_bg; //刪除node
	CObstacle *_pGet_upper, *_pGet_middle, *_pGet_lower, *_pGet_bg; //生成每行障礙物
	CObstacle *_pTrackObj_upper, *_pTrackObj_middle, *_pTrackObj_lower, *_pTrackObj_bg; //暫停按鈕用
	void deleteObstacle(); //刪除剩餘障礙物

	//SnowMan
	bool	_bSnowman_show; //雪人是否顯示
	float	_fcount_snowman; //計時
	float	_fRandTime_snowman; //隨機時間
	float	_fRandTime_snowball; //隨機時間
	float	_fcount_duration; //計時雪人存在時間
	float	_fcount_snowball; //計時
	CEnemy	*_snowman;
	CEnemy	*_snowmanDel;

	//碰撞偵測
	bool _bCollision;
	bool _bSnowballCol;
	CObstacle *_col_up, *_col_mid, *_col_low; //紀錄各行角色前端障礙物

	//Ranking and score
	int _iScore; //分數紀錄
	cocos2d::ui::Text *_gamingScoreText; //遊戲中更新分數
	cocos2d::ui::Text *_winScoreText; //勝利介面分數
	cocos2d::ui::Text *_loseScoreText; //失敗介面分數
	CScoring	*_scoring; //計分 & 排名
	C3SButton	*_okaybtn; //for ran

public:
	~AnimationAction();
    static cocos2d::Scene* createScene();

	int _level; //紀錄選擇關卡

    virtual bool init();
	void doStep(float dt);

	void JumpActionFinished(); // jump 的 CallBack 函式
	void SlideActionFinished(); // slide 的 CallBack 函式

	//觸碰
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 

    CREATE_FUNC(AnimationAction);
};

#endif // __HELLOWORLD_SCENE_H__
