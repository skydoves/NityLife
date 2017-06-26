#ifndef __shop_SCENE_H__
#define __shop_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;
USING_NS_CC_EXT;

class shop : public Layer, public EditBoxDelegate
{
public:
	static Scene* createScene();

	virtual bool init();

	void init_menu();

	void sound(float delta);

	void nothing(Ref* pSender);

	//에디트박스
protected:
	virtual void editBoxReturn(EditBox* editbox);

	//타일 구매
	void thing_tile_buy1(Ref* pSender);
	void thing_tile_buy2(Ref* pSender);
	void thing_tile_buy3(Ref* pSender);
	void thing_tile_buy4(Ref* pSender);
	void thing_tile_buy5(Ref* pSender);

	void thing2(Ref* pSender);
	void thing2_tile_buy1(Ref* pSender);
	void thing2_tile_buy2(Ref* pSender);
	void thing2_tile_buy3(Ref* pSender);
	void thing2_tile_buy4(Ref* pSender);
	void thing2_tile_buy5(Ref* pSender);

	void thing3(Ref* pSender);
	void thing3_buy1(Ref* pSender);
	void thing3_buy2(Ref* pSender);
	void thing3_buy3(Ref* pSender);

	void food_buy1(Ref* pSender);
	void food_buy2(Ref* pSender);
	void food_buy3(Ref* pSender);
	void food_buy4(Ref* pSender);
	void food_buy5(Ref* pSender);
	void food_buy6(Ref* pSender);

	void coder(Ref* pSender);

	void effect1(Ref* pSender);
	void effect2(Ref* pSender);


	void egg_buy1(Ref* pSender);
	void egg_buy2(Ref* pSender);
	void nity_sell(Ref* pSender);
	void nity_sell2(Ref* pSender);
	void nity_sell3(Ref* pSender);
	void nity_di1(Ref* pSender);
	void nity_di2(Ref* pSender);

	void init_money(float delta);

	void anno(char *ano);
	void anno_slow(char *ano);
	void anno_del();

	void close(Ref* pSender);
	void shop_close(Ref* pSender);

	void food_shop(Ref* pSender);
	void thing_shop(Ref* pSender);
	void egg_shop(Ref* pSender);

	void close_simplat(Ref* pSender);
	void ex_egg1(Ref* pSender);
	void ex_egg2(Ref* pSender);

	CREATE_FUNC(shop);
};

#endif


