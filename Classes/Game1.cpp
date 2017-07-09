
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "shop.h"
#include "Game1.h"
#include "Game2.h"

using namespace CocosDenshion;


int sound4;
int img;
int level;
float jump;
int exps;
float gagebar=0;
float gage_max = 100;
int onstart=0;
int onjump = 0;
int life;
int mob_co=1;
int game_start=0;
int life_end=0;
int game_over = 0;
int money=0;
int die = 0;
int game_win = 0;
int ran_money = 0;
int ran_exp = 0;
int get;
int full_po;
float gage_up;
int double_jump = 0;
int game1_love;
 
Scene* Game1::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Game1::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



bool Game1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//�����͵��� �ʱ�ȭ�Ѵ�
	mobs.clear();
	init_Game();

	//�����Լ�
	srand(time(NULL));

	//���������� �������� ������Ʈ
	this->schedule(schedule_selector(Game1::update_gage), 0.01f);

	//��ǥ���� �¾Ҵ����� �����Ѵ�
	this->schedule(schedule_selector(Game1::hit), 0.01f);

	//������ ��ġ �ʱ�ȭ
	this->schedule(schedule_selector(Game1::make1), 0.01f);

	//�������� ������Ʈ
	this->schedule(schedule_selector(Game1::gage_sh), 0.01f);

	//������� ����
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	if (sound4 == 0)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/ongame.mp3");
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->schedule(schedule_selector(Game1::sound), 35.0);

	//�̺�Ʈ �����ʿ� ��ġ��� ���
	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = CC_CALLBACK_2(Game1::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener2, 1);

	//�⺻ ���� ����
	auto spr1 = Sprite::create("img/game1_br.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//���� Ÿ��Ʋ ����
	auto spr5 = Sprite::create("img/game1_title.png");
	spr5->setAnchorPoint(Point(0, 1));
	spr5->setPosition(0, 320);
	this->addChild(spr5, 10,"title");

	//�������� ��
	auto spr2 = Sprite::create("img/gage_tool.png");
	spr2->setAnchorPoint(Point(0, 1));
	spr2->setPosition(10, 322);
	this->addChild(spr2);

	//���� �������� ��
	auto spr3 = Sprite::create("img/icon_life.png");
	spr3->setAnchorPoint(Point(0, 1));
	spr3->setPosition(200, 315);
	spr3->setScale(1.2);
	this->addChild(spr3);

	//�������� ��ư
	auto item1 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(Game1::home, this));
	item1->setAnchorPoint(Point(0, 1));
	item1->setPosition(210, 153);
	item1->setScale(2.0);
	auto menu1 = Menu::create(item1, NULL);
	this->addChild(menu1, 2);

	return true;
}


//��ǥ���� �¾Ҵ����� ������
void Game1::hit(float delta)
{

	auto removed = Sprite::create("img/blank.png");
	auto sla = (Sprite*)this->getChildByName("sla");
	auto sla_po = sla->getPosition();

		for (Sprite* spr : mobs) // ��������Ʈ ������ : �迭 ó�� smiles���� ���� spr�� �޷�� �� �����Ѵ�.(smiles�� ������ ��ŭ)
		{
			Rect rect1 = spr->getBoundingBox(); // rect��� �Լ��� �簢�� ������ ��������Ʈ spr�� ũ�⿡ ���� ������ �����Ѵ�.}
			Rect rect_sla = sla->getBoundingBox();

			if (rect_sla.intersectsRect(rect1) && die==0)
			{
				//�̹����� ������, ������ ���ҿ��� �����Ѵ�.
				die = 1;

				removed = spr;
				spr->setPosition(0, 0);
				this -> removeChild(spr);


				auto action1 = DelayTime::create(0.5);
				auto action2 = CallFunc::create(CC_CALLBACK_0(Game1::hit_mob, this));
				auto action3 = Sequence::create(action1, action2, NULL);

				this->runAction(action3);

				//����
				sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
				if (sound4 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/attack.ogg");

				get = (rand() % 100) + 1;
				int get2 = (rand() % (level*2+5)) + 1;

				if (get <= 35)
				{
					sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
					if (sound4 == 0)
						float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

					money += get2;
					anno_slow("img/ann_getnit.png");

					//Ư�� ȿ��
					auto apr1 = ParticleSystemQuad::create("par/item_power.plist");
					apr1->setOpacity(140);
					apr1->setScale(0.6);
					apr1->setPosition(sla_po);
					this->addChild(apr1, 50);
				}
			}
		}

}

//������ �� ��� ó��
void Game1::hit_mob()
{
	mob_co--;
	die = 0;
}

//��ǥ������ ��ġ�� �׷��Ȱ� ������ ������ �ʱ�ȭ
void Game1::make1(float delta)
{

	if (mobs.size() >= mob_co || game_start == 0)
		return;

	float x = 320 + rand() % 150;
	float y = 30 + rand() % 260;

	auto spr = Sprite::create("img/bird1.png");
	spr->setPosition(Point(x, y));
	this->addChild(spr);

	float y2 = 30 + rand() % 260;
	auto actio0 = MoveTo::create(4.0 - level*0.1, Point(x, y2));
	auto actio1 = MoveTo::create(4.0 - level*0.1, Point(x, y));

	auto actio2 = Sequence::create(actio0, actio1, NULL);
	auto actio3 = RepeatForever::create(actio2);
	spr->runAction(actio3);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.6);
	animation->addSpriteFrameWithFileName(String::create("img/bird1.png")->getCString());
	animation->addSpriteFrameWithFileName(String::create("img/bird2.png")->getCString());

	auto action_0 = RepeatForever::create(Animate::create(animation));
	spr->runAction(action_0);

	mobs.pushBack(spr); // ���Ϳ� spr ���Ҹ� �߰��Ѵ�
}

//������ ó�� ������ �ʱ�ȭ $$*
void Game1::init_Game()
{
	//�⺻ ������ �ʱ�ȭ
	gagebar = 0;
	gage_max = 100;
	onstart = 0;
	onjump = 0;
	game_start = 0;
	life_end = 0;
	game_over = 0;
	money = 0;
	die = 0;
	mob_co = 1;
	game_win = 0;
	money = 0;
	ran_exp = 0;
	ran_money = 0;
	full_po = 0;
	gage_up = 0.0f;
	game1_love=0;

	//���δ�Ƽ�� ������ �ҷ��´�
	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");

	winSize2 = Director::getInstance()->getWinSize();

	if (main_nity == 1)
	{
		img = UserDefault::getInstance()->getIntegerForKey("first");
		level = UserDefault::getInstance()->getIntegerForKey("first_lv");
		jump = UserDefault::getInstance()->getFloatForKey("first_jump");
		game1_love = UserDefault::getInstance()->getIntegerForKey("first_love");
	}
	else if (main_nity == 2)
	{
		img = UserDefault::getInstance()->getIntegerForKey("second");
		level = UserDefault::getInstance()->getIntegerForKey("second_lv");
		jump = UserDefault::getInstance()->getFloatForKey("second_jump");
		game1_love = UserDefault::getInstance()->getIntegerForKey("second_love");
	}
	else if (main_nity == 3)
	{
		img = UserDefault::getInstance()->getIntegerForKey("third");
		level = UserDefault::getInstance()->getIntegerForKey("third_lv");
		jump = UserDefault::getInstance()->getFloatForKey("third_jump");
		game1_love = UserDefault::getInstance()->getIntegerForKey("third_love");
	}

	//��ŸƮ ������
	auto spr1 = Sprite::create("img/icon_start.png");
	spr1->setPosition(430, 50);
	spr1->setOpacity(200);
	this->addChild(spr1, 2, "img_start");


	//�������� �����ش�
	life = (level * 3)/2 + 1;
	mob_co = level;

	//�̹����� �����ش�
	if (img == 1)
	{
		auto spr2 = Sprite::create("img/sla_blue_r1.png");
		spr2->setPosition(80, 80);
		this->addChild(spr2, 2, "sla");
	}
	if (img == 2)
	{
		auto spr2 = Sprite::create("img/sla_pink_r1.png");
		spr2->setPosition(80, 80);
		this->addChild(spr2, 2, "sla");
	}
	if (img == 3)
	{
		auto spr2 = Sprite::create("img/sla_green_r1.png");
		spr2->setPosition(80, 80);
		this->addChild(spr2, 2, "sla");
	}
	if (img == 4 && level < 7)
	{
		auto spr2 = Sprite::create("img/lity_d1.png");
		spr2->setPosition(80, 80);
		this->addChild(spr2, 2, "sla");
	}
	if (img == 4 && level >= 7)
	{
		auto spr2 = Sprite::create("img/lityw1_r1.png");
		spr2->setPosition(80, 80);
		this->addChild(spr2, 2, "sla");
	}

}

//������ �ѹ��� �������
void Game1::init2()
{
	//���� ���� ��ư�� �ʱ�ȭ
	auto spr1 = (Sprite*)this->getChildByName("img_start");
	spr1->setTexture("img/icon_start.png");

	//�������� �̹��� ��ġ �ʱ�ȭ
	auto sla = (Sprite*)this->getChildByName("sla");
	sla->setPosition(80, 80);

	//���ӽ�ŸƮ ��ư, �������� ���� �ʱ�ȭ
	onstart = 0;
	onjump = 0;
	double_jump = 0;
}


bool Game1::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto spr1 = (Sprite*)this->getChildByName("img_start");
	auto sla = (Sprite*)this->getChildByName("sla");
	Rect rect_spr1 = spr1->getBoundingBox();
	Point location = touch->getLocation();
	auto sla_po = sla->getPosition();

	auto spr_title = (Sprite*)this->getChildByName("title");
	this->removeChild(spr_title);

		if (rect_spr1.containsPoint(location))
		{
			if (onstart == 0 && onjump == 0)
			{
			sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound4 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");
 
			game_start = 1;

			spr1->setTexture("img/icon_jump.png");

			onstart = 1;
		   }
			else if (onstart == 1 && onjump == 0)
			{
				spr1->setTexture("img/icon_jump_plus.png");

				sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
				if (sound4 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/jump.ogg");

				//���� �ѹ� ������� ������ ����
				--life;  // ������ �ϳ� ����
				onjump = 1; //������

				//���� �ִ�ġ�� �Ѱ�
				if (jump >= 10)
					jump = 10;

				auto action1 = JumpTo::create(3.0, Point(500, -(2.0*gagebar)+3*jump), 5*jump, 1);
				sla->runAction(action1);

				gagebar = 0; //�������� = 0

			}
			//2�� ����
			else if (onstart == 1 && onjump == 1 && double_jump == 0)
			{
				//Ư�� ȿ��
				auto apr1 = ParticleSystemQuad::create("par/item_heart.plist");
				apr1->setOpacity(140);
				apr1->setScale(0.6);
				apr1->setPosition(sla_po);
				this->addChild(apr1, 50);

				spr1->setTexture("img/icon_jump_se.png");

				sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
				if (sound4 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/jump.ogg");

				double_jump = 1;

				auto action1 = RotateBy::create(0.5f, 360);
				auto action2 = JumpBy::create(2.0, Point(0, 0), 15 * jump, 1);
				sla->runAction(action1);
				sla->runAction(action2);
			}
	}

	return true;
}


//�������� ���������� ������Ʈ�� ĳ���� �̹����� �̵��� ������ ����� ����, ���ӿ��� ó��
void Game1::gage_sh(float delta)
{
	gage_up = 0.25 + level*0.01;

	if (gage_up >= 0.4)
		gage_up = 0.4;

	if (mob_co != 0)
	{
		if (onstart == 1 && onjump == 0 && gagebar >= -100 && full_po == 0)
		{
			gagebar -= gage_up;

			if (gagebar <= -100)
				full_po = 1;
		}
		else if (onstart == 1 && onjump == 0 && gagebar <= 0 && full_po == 1)
		{
			gagebar += gage_up;

			if (gagebar >= 0)
				full_po = 0;
		}

		auto sla = (Sprite*)this->getChildByName("sla");
		int sla_x = sla->getPositionX();

		if (sla_x >= 500 && life != life_end)
		{
			auto stop = (Sprite*)this->getChildByName("sla");
			stop->stopAllActions();
			init2();

		}
		//���� ������ ó��
		else if (sla_x >= 500 && life == life_end && game_over == 0)
		{
			gameover();
			game_over = 1;
		}
	}

	if (mob_co == 0 && game_win == 0)
	{
		gamewin();
		game_win = 1;
	}

}

//���ӿ���
void Game1::gameover()
{
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/gameover.mid");

	if (mob_co != 0)
	{
		auto state1 = (Sprite*)this->getChildByName("img_start");
		this->removeChild(state1);

		//�⺻ ���� ����
		auto item1 = MenuItemImage::create("img/img_gameover2.png", "img/img_gameover2.png", CC_CALLBACK_1(Game1::home, this));
		auto menu1 = Menu::create(item1, NULL);
		item1->setAnchorPoint(Point(0, 1));
		item1->setPosition(-240, 160);
		this->addChild(menu1);

		auto spr1 = Sprite::create("img/img_gameover1.png");
		spr1->setAnchorPoint(Point(0, 1));
		spr1->setPosition(0, 320);
		this->addChild(spr1);

		auto action2 = Blink::create(200, 150);
		spr1->runAction(action2);
	}
}

//�������� ���ư���
void Game1::home(Ref* pSender)
{
	mobs.clear();
	onstart = 0;
	onjump = 0;

	//���� ó��
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//��ġ�̺�Ʈ ������ ����� Ȯ���� ����
	this->setTouchEnabled(false);
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener2);
	Director::getInstance()->pushScene(inlife::createScene());
}

//���� �¸�
void Game1::gamewin()
{

	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/win.mid");

	auto state1 = (Sprite*)this->getChildByName("img_start");
	this->removeChild(state1);

	//�⺻ ���� ����
	auto item1 = MenuItemImage::create("img/img_win2.png", "img/img_win2.png", CC_CALLBACK_1(Game1::home, this));
	auto menu1 = Menu::create(item1, NULL);
	item1->setAnchorPoint(Point(0, 1));
	item1->setPosition(-240, 160);
	this->addChild(menu1, 50);

	//��ũ ����
	auto spr2 = Sprite::create("img/img_win1.png");
	spr2->setAnchorPoint(Point(0, 1));
	spr2->setPosition(0, 320);
	this->addChild(spr2, 50);

	auto action2 = Blink::create(200, 150);
	spr2->runAction(action2);

	ran_money = 55 + (rand() % (25+level)) + level * 3;

	money += ran_money;
	money += game1_love;

	auto label1 = Label::createWithSystemFont(StringUtils::format("%d", money), "Arial", 15);
	label1->setAnchorPoint(Point(0, 1));
	label1->setPosition(Point(165, 165));
	label1->setColor(Color3B(-255, -255, -255));
	this->addChild(label1, 51);

	ran_exp = 20 + (rand() % 10) + level * 3 + game1_love / 10;

	auto label2 = Label::createWithSystemFont(StringUtils::format("%d", ran_exp), "Arial", 15);
	label2->setAnchorPoint(Point(0, 1));
	label2->setPosition(Point(185, 128));
	label2->setColor(Color3B(-255, -255, -255));
	this->addChild(label2, 51);

	//������ ȹ��
	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
	int exp1= UserDefault::getInstance()->getIntegerForKey("first_exp");
	int exp2 = UserDefault::getInstance()->getIntegerForKey("second_exp");
	int exp3 = UserDefault::getInstance()->getIntegerForKey("third_exp");

	if (main_nity == 1)
	{
		money += UserDefault::getInstance()->getIntegerForKey("money");
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		ran_exp += exp1;
		UserDefault::getInstance()->setIntegerForKey("first_exp", ran_exp);
		UserDefault::getInstance()->flush();
	}
	else if (main_nity == 2)
	{
		money += UserDefault::getInstance()->getIntegerForKey("money");
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		ran_exp += exp2;
		UserDefault::getInstance()->setIntegerForKey("second_exp", ran_exp);
		UserDefault::getInstance()->flush();
	}
	else if (main_nity == 3)
	{
		money += UserDefault::getInstance()->getIntegerForKey("money");
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		ran_exp += exp3;
		UserDefault::getInstance()->setIntegerForKey("third_exp", ran_exp);
		UserDefault::getInstance()->flush();
	}
}


//�������� �������� ������Ʈ 
void Game1::update_gage(float delta)
{
	auto state1 = (Sprite*)this->getChildByName("gagebar");
	this->removeChild(state1);

	auto food_bar = CCSprite::create("img/Game1_gage.png", CCRectMake(0, 0, 15, (133 * gagebar) / gage_max));
	food_bar->setPosition(30, 130);
	food_bar->setAnchorPoint(Point(0, 1));
	this->addChild(food_bar, 12, "gagebar");

	auto state2 = (Sprite*)this->getChildByName("life");
	this->removeChild(state2);

	auto label2 = Label::createWithSystemFont(StringUtils::format("%d", life), "Arial", 14);
	label2->setAnchorPoint(Point(0, 1));
	label2->setPosition(Point(240, 303));
	this->addChild(label2, 11, "life");
}


// �������� bgm�� �߻�
void Game1::sound(float delta)
{
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//������� ����
	if (sound4 == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/ongame.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

//�˸� �޼��� ǥ��
void Game1::anno_slow(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(165, 315));
	this->addChild(img1, 16, "ann");

	//õõ�� �̹����� ���̵�ƿ��Ѵ�.
	auto action1 = FadeOut::create(4.0);
	img1->runAction(action1);
}