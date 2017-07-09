#ifndef __Game2_SCENE_H__
#define __Game2_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;

class Game2 : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	void init_menu();

	Size winSize;

	Vector<Sprite*> mobs2;
	Vector<Sprite*> item_heart;
	Vector<Sprite*> money_vec;
	Vector<Sprite*> item_dove;
	Vector<Sprite*> mob3;
	Vector<Sprite*> power_vec;
	Vector<Sprite*> magnet_vec;
	Vector<Sprite*> coin_vec;
	Vector<Sprite*> coin2_vec;
	Vector<Sprite*> coin3_vec;


	void power_off(Ref* pSender);
	void magnet_off(Ref* pSender);
	void game2_win();

	void make1(float delta);
	void make2(float delta);

	void heart(float delta);
	void item_money(float delta);
	void dove(float delta);
	void black_dove(float delta);
	void item_power(float delta);
	void item_coin(float delta);
	void item_coin2(float delta);
	void item_coin3(float delta);
	void item_magnet(float delta);

	void onstart(Ref* pSender);

	void home(Ref* pSender);

	void sound(float delta);

	void actioning_nity(float delta);

	void update(float delta);

	void anno_slow(char *ano);

	Size winSize3;

	//터치 이벤트 부분
	EventListenerTouchOneByOne *listener3;
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	CREATE_FUNC(Game2);
};

#endif


