#ifndef __inlife_SCENE_H__
#define __inlife_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"



USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;

class inlife : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(inlife);

	Size winSize;


	//스크린샷 저장부분
	void captureScreen(const std::function<void(bool, const std::string&)>& afterCaptured, const std::string& filename);

	//터치 이벤트 부분
	EventListenerTouchOneByOne *listener;
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);

	void initmenu();
	void move(float delta);
	void staying();
	void init_food(float delta);
	void sh_food(float delta);
	void time_food();

	void ask_exit();

	void exp_control();

	void nothing(Ref* pSender);
	void exit_life(Ref* pSender);
	void close_keypad(Ref* pSender);
	void close_state(Ref* pSender);
	void close_state2();

	//니티의 정보 상태창 2번째꺼
	void nity_state2(Ref* pSender);
	void close_nity_state2(Ref* pSender);
	void nitytouch_explain(Ref* pSender);

	//니티 목욕시키기
	void sham(Ref* pSender);
	void shower(Ref* pSender);
	void wash_close(Ref* pSender);

	//니티 잠재우기
	void sleep(Ref* pSender);

	//간식 먹이기
	void dessert(Ref* pSender);
	void close_menu_de(Ref* pSender);
	void explain_dess(Ref* pSender);
	void dessert1(Ref* pSender);
	void dessert2(Ref* pSender);
	void dessert3(Ref* pSender);
	void dessert4(Ref* pSender);

	//이모션
	void emotion(float delta);
	void emotion_active(float delta);
	void emotion_del();
	void emotion_nity(int nity, int emo);

	void init_things();

	void anno(char *ano);
	void anno_slow(char *ano);
	void anno_slow2(char *ano);
	void anno_del();

	//먹이를 주는 부분
	void select_food(Ref* pSender);
	void give_food(Ref* pSender);
	void exit_food(Ref* pSender);
	void init_foodgage();

	void select_main(Ref* pSender);

	//스텟을 증가시키는 부분
	void select_jump(Ref* pSender);
	void select_fly(Ref* pSender);
	void stat_jump(Ref* pSender);
	void stat_fly(Ref* pSender);
	void exit_jump(Ref* pSender);
	void exit_fly(Ref* pSender);
	void up_stat(); // 레벨 6인 니티가 7이되면 비행력 증가

	//인게임 UI
	void shop(Ref* pSender);
	void quest(Ref* pSender);
	void quest_messer();
	void quest_mess(Ref* pSender);
	void game(Ref* pSender);
	void home(Ref* pSender);
	void camera(Ref* pSender);
	void trees(Ref* pSender);
	void trees_explain(Ref* pSender);
	void trees_explain2(Ref* pSender);


	void apple_eat();


	//나무 관리하기
	void trees_state(Ref* pSender);
	void exit_trees_state(Ref* pSender);
	void tree1(Ref* pSender);
	void tree2(Ref* pSender);
	void tree3(Ref* pSender);
	void temp_up(Ref* pSender);
	void temp_down(Ref* pSender);
	void tree_wat(); // 시간에 따라 수분이 자동으로 감소
	void tree_tem(); // 시간에 따라 온도 변경

	//클릭하는 사운드
	void sound_click();

	//메뉴_게임 부분
	void Game1_start(Ref* pSender);
	void Game1_explain(Ref* pSender);
	void Game2_start(Ref* pSender);
	void Game2_explain(Ref* pSender);
	void Game_exit(Ref* pSender);

	void simple_close(Ref* pSender);

	void init_nity();
	void nitymove();

	//튜토리얼
	void tutorial();
	void mess_tutor(Ref* pSender);
	void touch_egg(Ref* pSender);

	//음향 부분
	void sound(float delta);
	void nity_sound(float delta);

};

#endif


