
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "shop.h"
#include "Game1.h"
#include "Game2.h"


using namespace CocosDenshion;

//<관리 메뉴 변수들>
int sound; //사운드 on(=0)/off(=1)
int open, at2 = 0; // 어플 실행시 한 번만 출력되는 이미지의 변수

//<테그들의 정의>
#define menutag 1
#define submenutag 2
#define sptag 3


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	initmenu();
    
    return true;
}




//메뉴의 스프라이트들을 생성한다.
void HelloWorld::initmenu()
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//배경음악 설정
	if (open > 10 && at2 > 5 && sound == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/mainmenu.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	//배경화면 설정
	auto img1 = Sprite::create("img/img_bagr.png");
	img1->setPosition(Point(0, 320));
	img1->setAnchorPoint(Point(0, 1));
	this->addChild(img1);

	auto apr1 = ParticleSystemQuad::create("par/flies.plist");
	apr1->setOpacity(60);
	apr1->setScale(0.7);
	apr1->setPosition(240, 160);
	this->addChild(apr1, 9);

	//메뉴 버튼을 생성한다.
	auto item2 = MenuItemImage::create("img/menu1.png", "img/menu1_se.png", CC_CALLBACK_1(HelloWorld::changescene_inlife, this));
	auto item3 = MenuItemImage::create("img/maker.png", "img/maker_se.png", CC_CALLBACK_1(HelloWorld::maker, this));
	auto item4 = MenuItemImage::create("img/menu2.png", "img/menu2_se.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	auto menu = Menu::create(item2, item3, item4, NULL);
	item2->setPosition(0, 0);
	item3->setScale(0.5, 0.6);
	item3->setPosition(-60, -65);
	item4->setScale(0.5, 0.6);
	item4->setPosition(60, -65);
	menu->setTag(menutag);
	this->addChild(menu);

	//스피커 메뉴 버튼을 생성한다.
	if (sound == 0)
	{
		auto item4 = MenuItemImage::create("img/img_sp.png", "img/img_sp.png", CC_CALLBACK_1(HelloWorld::speaker, this));
		auto menu3 = Menu::create(item4, NULL);
		item4->setPosition(Point(-216, 136));
		item4->setScale(0.6);
		menu3->setTag(sptag);
		this->addChild(menu3);
	}
	else
	{
		auto item4 = MenuItemImage::create("img/img_sp2.png", "img/img_sp22.png", CC_CALLBACK_1(HelloWorld::speaker, this));
		auto menu3 = Menu::create(item4, NULL);
		item4->setPosition(Point(-216, 136));
		item4->setScale(0.6);
		menu3->setTag(sptag);
		this->addChild(menu3);
	}

	// 어플 실행시 한 번만 출력되는 이미지
	if (open < 10 && at2<5)
	{
		//접속 효과음 처리
		if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/me_login.mid");

		//바탕색(바탕색이 필요한 이유는 페이드인 화면 처리로 나타남)
		auto spr1 = Sprite::create("img/black.png");
		spr1->setScale(5.0);
		spr1->setAnchorPoint(Point(0, 1));
		spr1->setPosition(Point(-240, 330));
		this->addChild(spr1);

		//메뉴들을 지운다
		auto sprm = (Sprite*)this->getChildByTag(menutag);
		this->removeChild(sprm);

		//배경 화면의 처리
		auto item1 = MenuItemImage::create("img/img_title.png", "img/img_title.png", CC_CALLBACK_1(HelloWorld::state_exit, this));
		auto menu1 = Menu::create(item1, NULL);
		item1->setAnchorPoint(Point(0, 1));
		item1->setPosition(-240, 160);
		menu1->setTag(submenutag);
		//menu1->setOpacity(0);
		//auto action1 = FadeIn::create(1.5);  시작시 페이드 인 처리
		//menu1->runAction(action1);
		this->addChild(menu1);

		auto spr2 = Sprite::create("img/img_titleann.png");
		spr2->setAnchorPoint(Point(0, 1));
		spr2->setPosition(0, 320);
		this->addChild(spr2);

		auto action2 = Blink::create(200, 150);
		spr2->runAction(action2);
		
		//접속 시간을 측정한다.
		UserDefault::getInstance()->setIntegerForKey("game_start_time", time(NULL));
		UserDefault::getInstance()->flush();

		at2 = 10;
	}
	else
	{
	}


}

//만든이
void HelloWorld::maker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//배경음악 설정
	if (open > 10 && at2 > 5 && sound == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/main2.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();


	//메뉴 버튼을 생성한다.
	auto item2 = MenuItemImage::create("img/img_maker.png", "img/img_maker.png", CC_CALLBACK_1(HelloWorld::exit_maker, this));
	auto menu = Menu::create(item2, NULL);
	item2->setPosition(0, 0);
	menu->setTag(menutag);
	this->addChild(menu);
}

//만든이 종료
void HelloWorld::exit_maker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	removeAllChildren();
	initmenu();
}


//게임 화면의 전환
void HelloWorld::changescene_inlife(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
	float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

	auto scene = TransitionFade::create(1.5, inlife::createScene());
	Director::getInstance()->pushScene(scene);
}


//스피커 on/off 처리
void HelloWorld::speaker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	if (sound == 0)
	{
		//스피커 변수값 변경
		sound = 1;
		UserDefault::getInstance()->setIntegerForKey("sound", sound);
		UserDefault::getInstance()->flush();

		this->removeAllChildren();
		initmenu();
	}
	else
	{
		//스피커 변수값 변경
		sound = 0;
		UserDefault::getInstance()->setIntegerForKey("sound", sound);
		UserDefault::getInstance()->flush();

		this->removeAllChildren();
		initmenu();
	}
}


//진도우 메뉴 종료
void HelloWorld::state_exit(Ref *sender)
{
	if (open<10)
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
		open = 15;
	}
	if (at2 > 5)
	{
		sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");


		this->removeAllChildren();

		// 메뉴의 스프라이트들을 재생성 한다.

		initmenu();
	}
}

//게임종료
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}