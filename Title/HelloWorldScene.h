#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;

class HelloWorld : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);



	// <HelloworldScene.cpp 에서 관리되는 함수들>
	void menuCloseCallback(Ref* pSender);

	void initmenu();
	
	void nothing();

	void changescene_inlife(Ref *sender);

	void state_exit(Ref *sender);

	void speaker(Ref *sender);

	void maker(Ref *sender);

	void exit_maker(Ref *sender);

};

#endif // __HELLOWORLD_SCENE_H__
