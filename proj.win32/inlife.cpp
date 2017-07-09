#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"

using namespace CocosDenshion;



Scene* inlife::createScene()
{
	auto scene = Scene::create();

	auto layer = inlife::create();

	scene->addChild(layer);

	return scene;
}

bool inlife::init()
{

	if (!Layer::init())
	{
		return false;
	}

	return true;
}
