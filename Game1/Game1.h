#ifndef __Game1_SCENE_H__
#define __Game1_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;

class Game1 : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	void init_Game();
	void init2();
	void update_gage(float delta);
	void gage_sh(float delta);

	void gameover();
	void gamewin();

	Size winSize2;

	void make1(float delta);
	void make2(float delta);

	void hit(float delta);
	void hit_mob();

	Vector<Sprite*> mobs;

	void sound(float delta);

	void home(Ref* pSender);

	//터치 이벤트 부분
	EventListenerTouchOneByOne *listener2;
	bool onTouchBegan(Touch *touch, Event *unused_event);

	void anno_slow(char *ano);

	CREATE_FUNC(Game1);
};

#endif


