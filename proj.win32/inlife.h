#ifndef __inlife_SCENE_H__
#define __inlife_SCENE_H__

#include "cocos2d.h"


USING_NS_CC;  //using namespace cocos2d
using namespace CocosDenshion;

class inlife : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();


	CREATE_FUNC(inlife);
};

#endif


