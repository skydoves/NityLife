
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "shop.h"
#include "Game1.h"
#include "Game2.h"


using namespace CocosDenshion;

//<���� �޴� ������>
int sound; //���� on(=0)/off(=1)
int open, at2 = 0; // ���� ����� �� ���� ��µǴ� �̹����� ����

//<�ױ׵��� ����>
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




//�޴��� ��������Ʈ���� �����Ѵ�.
void HelloWorld::initmenu()
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//������� ����
	if (open > 10 && at2 > 5 && sound == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/mainmenu.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	//���ȭ�� ����
	auto img1 = Sprite::create("img/img_bagr.png");
	img1->setPosition(Point(0, 320));
	img1->setAnchorPoint(Point(0, 1));
	this->addChild(img1);

	auto apr1 = ParticleSystemQuad::create("par/flies.plist");
	apr1->setOpacity(60);
	apr1->setScale(0.7);
	apr1->setPosition(240, 160);
	this->addChild(apr1, 9);

	//�޴� ��ư�� �����Ѵ�.
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

	//����Ŀ �޴� ��ư�� �����Ѵ�.
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

	// ���� ����� �� ���� ��µǴ� �̹���
	if (open < 10 && at2<5)
	{
		//���� ȿ���� ó��
		if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/me_login.mid");

		//������(�������� �ʿ��� ������ ���̵��� ȭ�� ó���� ��Ÿ��)
		auto spr1 = Sprite::create("img/black.png");
		spr1->setScale(5.0);
		spr1->setAnchorPoint(Point(0, 1));
		spr1->setPosition(Point(-240, 330));
		this->addChild(spr1);

		//�޴����� �����
		auto sprm = (Sprite*)this->getChildByTag(menutag);
		this->removeChild(sprm);

		//��� ȭ���� ó��
		auto item1 = MenuItemImage::create("img/img_title.png", "img/img_title.png", CC_CALLBACK_1(HelloWorld::state_exit, this));
		auto menu1 = Menu::create(item1, NULL);
		item1->setAnchorPoint(Point(0, 1));
		item1->setPosition(-240, 160);
		menu1->setTag(submenutag);
		//menu1->setOpacity(0);
		//auto action1 = FadeIn::create(1.5);  ���۽� ���̵� �� ó��
		//menu1->runAction(action1);
		this->addChild(menu1);

		auto spr2 = Sprite::create("img/img_titleann.png");
		spr2->setAnchorPoint(Point(0, 1));
		spr2->setPosition(0, 320);
		this->addChild(spr2);

		auto action2 = Blink::create(200, 150);
		spr2->runAction(action2);
		
		//���� �ð��� �����Ѵ�.
		UserDefault::getInstance()->setIntegerForKey("game_start_time", time(NULL));
		UserDefault::getInstance()->flush();

		at2 = 10;
	}
	else
	{
	}


}

//������
void HelloWorld::maker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//������� ����
	if (open > 10 && at2 > 5 && sound == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/main2.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();


	//�޴� ��ư�� �����Ѵ�.
	auto item2 = MenuItemImage::create("img/img_maker.png", "img/img_maker.png", CC_CALLBACK_1(HelloWorld::exit_maker, this));
	auto menu = Menu::create(item2, NULL);
	item2->setPosition(0, 0);
	menu->setTag(menutag);
	this->addChild(menu);
}

//������ ����
void HelloWorld::exit_maker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	removeAllChildren();
	initmenu();
}


//���� ȭ���� ��ȯ
void HelloWorld::changescene_inlife(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound == 0)
	float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

	auto scene = TransitionFade::create(1.5, inlife::createScene());
	Director::getInstance()->pushScene(scene);
}


//����Ŀ on/off ó��
void HelloWorld::speaker(Ref *sender)
{
	sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	if (sound == 0)
	{
		//����Ŀ ������ ����
		sound = 1;
		UserDefault::getInstance()->setIntegerForKey("sound", sound);
		UserDefault::getInstance()->flush();

		this->removeAllChildren();
		initmenu();
	}
	else
	{
		//����Ŀ ������ ����
		sound = 0;
		UserDefault::getInstance()->setIntegerForKey("sound", sound);
		UserDefault::getInstance()->flush();

		this->removeAllChildren();
		initmenu();
	}
}


//������ �޴� ����
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

		// �޴��� ��������Ʈ���� ����� �Ѵ�.

		initmenu();
	}
}

//��������
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