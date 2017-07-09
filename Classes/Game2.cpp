
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "Game2.h"

using namespace CocosDenshion;

int sound5; //���� on(=0)/off(=1)
int gamestart = 0;
int game2_img;
int game2_level;
int game2_fly;
int game2_money;
float game2_gagebar = 0;
float game2_gage_max = 100;
float time2 = 0.0f;
int stage;
int game_win2 = 0;
int ran_money2;
int ran_exp2;
float game2_times;
int powerful;
int score;
int magnet;
int game2_love;

Scene* Game2::createScene()
{
	auto scene = Scene::create();
	auto layer = Game2::create();
	scene->addChild(layer);
	return scene;
}

bool Game2::init()
{
	if (!Layer::init())
	{
		return false;
	}


	//������� ����
	sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	if (sound5 == 0)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game2.mp3");
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	this->schedule(schedule_selector(Game2::sound), 29.0);

	//���ȭ�� ����
	auto backgr = Sprite::create("img/img_br1.png");
	backgr->setAnchorPoint(Point(0, 1));
	backgr->setPosition(0, 320);
	this->addChild(backgr, 0, "backgr2");

	//���� Ÿ��Ʋ ����
	auto backgr_title = Sprite::create("img/game2_title.png");
	backgr_title->setAnchorPoint(Point(0, 1));
	backgr_title->setPosition(0, 320);
	this->addChild(backgr_title, 0, "backgr_title");

	//ó�� �����°�
	auto backgr3 = Sprite::create("img/game2_firstbg.png");
	backgr3->setAnchorPoint(Point(0, 1));
	backgr3->setPosition(70, 230);
	this->addChild(backgr3, 0, "firstbg1");

	//�ִ� �ű�� �����ֱ�
	int game2_score = UserDefault::getInstance()->getIntegerForKey("game2_score");
	float game2_t = UserDefault::getInstance()->getFloatForKey("game2_time");

	//���ھ�
	auto label_score = Label::createWithSystemFont(StringUtils::format("%d", game2_score), "Arial", 15);
	label_score->setAnchorPoint(Point(0, 1));
	label_score->setPosition(Point(205, 183));
	label_score->setColor(Color3B(-255, -255, -255));
	this->addChild(label_score, 0, "firstbg3");

	//����
	auto label1 = Label::createWithSystemFont(StringUtils::format("%.1f", game2_t), "Arial", 15);
	label1->setAnchorPoint(Point(0, 1));
	label1->setPosition(Point(205, 153));
	label1->setColor(Color3B(-255, -255, -255));
	this->addChild(label1, 0, "firstbg2");

	//�޴� ��ư�� �����Ѵ�.
	auto item2 = MenuItemImage::create("img/img_start.png", "img/img_start_se.png", CC_CALLBACK_1(Game2::onstart, this));
	auto menu = Menu::create(item2, NULL);
	item2->setPosition(0, -50);
	item2->setScale(0.8);
	this->addChild(menu, 0, "menu");

	auto item3 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(Game2::home, this));
	auto menu2 = Menu::create(item3, NULL);
	item3->setScale(2.0);
	item3->setPosition(220, 140);
	this->addChild(menu2);


	return true;
}

//���� ���� ������ �޴����� �ʱ�ȭ
void Game2::onstart(Ref* pSender)
{
	gamestart = 1;
	init_menu();

	//���� ������ �ʱ�ȭ
	time2 = 0.0f;
	stage = 0;
	game_win2 = 0;
	game2_money = 0;
	mobs2.clear();
	item_heart.clear();
	money_vec.clear();
	item_dove.clear();
	magnet_vec.clear();
	mob3.clear();
	power_vec.clear();
	powerful = 0;
	coin_vec.clear();
	coin2_vec.clear();
	coin3_vec.clear();

	ran_money2 = 0;
	ran_exp2 = 0;
	game2_times = 0;
	score = 0;
	magnet = 0;
	game2_love;
}

//������ ������ �ʱ�ȭ �Ѵ�.
void Game2::init_menu()
{	
	//�̺�Ʈ �����ʿ� ��ġ��� ���
	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = CC_CALLBACK_2(Game2::onTouchBegan, this);
	listener2->onTouchEnded = CC_CALLBACK_2(Game2::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener2, 1);

	//���ӽ��� �޴��� ����
	auto state1 = (Sprite*)this->getChildByName("menu");
	auto state2 = (Sprite*)this->getChildByName("firstbg1");
	auto state3 = (Sprite*)this->getChildByName("firstbg2");
	auto state4 = (Sprite*)this->getChildByName("firstbg3");
	auto state5 = (Sprite*)this->getChildByName("backgr_title");
	this->removeChild(state1);
	this->removeChild(state2);
	this->removeChild(state3);
	this->removeChild(state4);
	this->removeChild(state5);

	//���� �̹����� ����
	auto img_score = Sprite::create("img/score.png");
	img_score->setAnchorPoint(Point(0, 1));
	img_score->setPosition(0, 320);
	this->addChild(img_score, 10);

	//�����̹����� ����
	auto backgr2 = Sprite::create("img/img_cloud.png");
	backgr2->setAnchorPoint(Point(0, 1));
	backgr2->setPosition(0, 320);
	backgr2->setOpacity(70);
	this->addChild(backgr2,10, "backgr");

	auto move_br = MoveBy::create(1.0f, Point(-60,0));
	auto move_br2 = RepeatForever::create(move_br);
	backgr2->runAction(move_br2);

	//���δ�Ƽ�� ������ �ҷ��´�
	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");

	if (main_nity == 1)
	{
		game2_img = UserDefault::getInstance()->getIntegerForKey("first");
		game2_level = UserDefault::getInstance()->getIntegerForKey("first_lv");
		game2_fly = UserDefault::getInstance()->getFloatForKey("first_fly");
		game2_love = UserDefault::getInstance()->getIntegerForKey("first_love");
	}
	else if (main_nity == 2)
	{
		game2_img = UserDefault::getInstance()->getIntegerForKey("second");
		game2_level = UserDefault::getInstance()->getIntegerForKey("second_lv");
		game2_fly = UserDefault::getInstance()->getFloatForKey("second_fly");
		game2_love = UserDefault::getInstance()->getIntegerForKey("second_love");
	}
	else if (main_nity == 3)
	{
		game2_img = UserDefault::getInstance()->getIntegerForKey("third");
		game2_level = UserDefault::getInstance()->getIntegerForKey("third_lv");
		game2_fly = UserDefault::getInstance()->getFloatForKey("third_fly");
		game2_love = UserDefault::getInstance()->getIntegerForKey("third_love");
	}

	//ĳ������ �̹����� �����ش�
	if (game2_img == 4 && game2_level >= 7)
	{
		auto spr2 = Sprite::create("img/lityw1_r1.png");
		spr2->setPosition(40, 160);
		this->addChild(spr2, 2, "sla");
	}


	//ȭ��ǥ�� ó��   2: down  3 : up  4: right  5 : left
	auto mo_down = Sprite::create("img/move3.png");
	mo_down->setPosition(Point(60, 15));
	mo_down->setScale(1.5);
	mo_down->setOpacity(200);
	this->addChild(mo_down, 3, "down");

	auto mo_up = Sprite::create("img/move3.png");
	mo_up->setRotation(180);
	mo_up->setPosition(Point(60, 85));
	mo_up->setScale(1.5);
	mo_up->setOpacity(200);
	this->addChild(mo_up, 3, "up");

	auto mo_right = Sprite::create("img/move3.png");
	mo_right->setRotation(-90);
	mo_right->setPosition(Point(95, 52));
	mo_right->setScale(1.5);
	mo_right->setOpacity(200);
	this->addChild(mo_right, 3, "right");

	auto mo_left = Sprite::create("img/move3.png");
	mo_left->setRotation(90);
	mo_left->setPosition(Point(25, 52));
	mo_left->setScale(1.5);
	mo_left->setOpacity(200);
	this->addChild(mo_left, 3, "left");

	//������������ �� ǥ���Ѵ�.
	auto spr3 = Sprite::create("img/game2_gagetool.png");
	spr3->setAnchorPoint(Point(0, 1));
	spr3->setPosition(0, 330);
	spr3->setOpacity(160);
	this->addChild(spr3, 15);

	//�⺻���� ������ �ʱ�ȭ
	game2_gagebar = 100;

	//Ÿ���� ǥ���Ѵ�
	winSize = Director::getInstance()->getWinSize();
	auto label = Label::createWithSystemFont(StringUtils::format("Time : %.1lf", time2), "Arial", 15);
	label->setAnchorPoint(Point(0, 1));
	label->setPosition(Point(300, winSize.height - 20));
	label->enableShadow(Color4B::BLACK, Size(-1, -1));
	this->addChild(label, 40, "timer");

	//������ ǥ���Ѵ�
	auto label_score = Label::createWithSystemFont(StringUtils::format("%d", score), "Arial", 15);
	label_score->setAnchorPoint(Point(0, 1));
	label_score->setPosition(Point(90, 268));
	label_score->enableShadow(Color4B::BLACK, Size(-1, -1));
	this->addChild(label_score, 40, "score");

	//������ ��ġ �ʱ�ȭ
	this->schedule(schedule_selector(Game2::make1), 0.1f);

	//������ ȭ����� ������
	this->schedule(schedule_selector(Game2::make2), 0.1f);

	//�����۵��� ����
	this->schedule(schedule_selector(Game2::heart), 5.0f);
	this->schedule(schedule_selector(Game2::item_money), 3.0f);
	this->schedule(schedule_selector(Game2::dove), 20.0f);
	this->schedule(schedule_selector(Game2::black_dove), 15.0f);
	this->schedule(schedule_selector(Game2::item_power), 18.0f);
	this->schedule(schedule_selector(Game2::item_coin), 0.5f);
	this->schedule(schedule_selector(Game2::item_coin2), 2.5f);
	this->schedule(schedule_selector(Game2::item_coin3), 5.0f);
	this->schedule(schedule_selector(Game2::item_magnet), 20.0f);

	//�������� ������Ʈ
	this->schedule(schedule_selector(Game2::update), 0.01);

	//��Ƽ�� �������� �׼��� (���� �ȶ��ȶ�)
	this->schedule(schedule_selector(Game2::actioning_nity), 0.75);
}


//�������� ������Ʈ
void Game2::update(float delta)
{
	//������ �������� ������
	auto backgr = (Sprite*)this->getChildByName("backgr");
	auto backgr_x = backgr->getPositionX();
	Point action_po;

	if (backgr_x <= -480)
		backgr->setPosition(Point(0, 320));

	//�������� �������� ������Ʈ
	auto state1 = (Sprite*)this->getChildByName("gagebar");
	this->removeChild(state1);

	if (game2_gagebar>=0 && powerful == 0)
	game2_gagebar -= 0.03;

	auto food_bar = CCSprite::create("img/game2_gage.png", CCRectMake(0, 0, (228 * game2_gagebar) / game2_gage_max, 15));
	food_bar->setPosition(39, 301);
	food_bar->setAnchorPoint(Point(0, 1));
	this->addChild(food_bar, 16, "gagebar");

	//���ΰ��� �̵��Ҽ� �ִ� �Ѱ輱
	auto actor = (Sprite*)this->getChildByName("sla");
	auto actor_x = actor->getPositionX();
	auto actor_y = actor->getPositionY();

	if (game2_fly >= 100)
		game2_fly = 100;

	if (actor_x <= 20)
		actor->setPosition(20, actor_y);
	if (actor_x >= 460)
		actor->setPosition(460, actor_y);
	if (actor_y <= 20)
		actor->setPosition(actor_x, 20);
	if (actor_y >= 200+game2_fly)
		actor->setPosition(actor_x, 200 + game2_fly);
	if (actor_x <=20 && actor_y <= 20)   //�𼭸� �κе� ó��������Ѵ�.
		actor->setPosition(20, 20);
	if (actor_x <= 20 && actor_y >= 200 + game2_fly)
		actor->setPosition(20, 200 + game2_fly);
	if (actor_x >= 460 && actor_y <= 20)
		actor->setPosition(460, 20);
	if (actor_x >= 460 && actor_y >= 200 + game2_fly)
		actor->setPosition(460, 200 + game2_fly);

	//Ÿ�̸��� ������Ʈ
	if (time2 >= 0.0 && powerful == 0)
	{
		auto label = (Label*)this->getChildByName("timer");
		label->setString(StringUtils::format("Meter : %.1lf", time2));
		time2 += 0.01;
	}
	else if (time2 >= 0.0 && powerful == 1)
	{
		auto label = (Label*)this->getChildByName("timer");
		label->setString(StringUtils::format("Meter : %.1lf", time2));
		time2 += 0.03;
	}

	//������ ������Ʈ
		auto label_score = (Label*)this->getChildByName("score");
		label_score->setString(StringUtils::format("%d", score));

	//���������� ����
	if (time2 >= 60.0 && time2 <= 60.5)
	{
		for (Sprite* spr : mobs2)
		{
			spr->setVisible(false);
			this->removeChild(spr);
		}

		stage = 1;
		mobs2.clear();
		anno_slow("img/ann_stageup.png");
	}
	else if (time2 >= 120.0 && time2 <= 120.5)
	{
		for (Sprite* spr : mobs2)
		{
			spr->setVisible(false);
			this->removeChild(spr);
		}

		stage = 2;
		mobs2.clear();
		anno_slow("img/ann_stageup.png");
	}
	else if (time2 >= 180.0 && time2 <= 180.5)
	{
		for (Sprite* spr : mobs2)
		{
			spr->setVisible(false);
			this->removeChild(spr);
		}

		stage = 3;
		mobs2.clear();
		anno_slow("img/ann_stageup.png");
	}
	else if (time2 >= 240.0 && time2 <= 240.5)
	{
		for (Sprite* spr : mobs2)
		{
			spr->setVisible(false);
			this->removeChild(spr);
		}

		stage = 4;
		mobs2.clear();
		anno_slow("img/ann_stageup.png");
	}


	//���ű��� �ε�ġ�� ó��
	for (Sprite* spr2 : mobs2) // ��������Ʈ ������ : �迭 ó�� smiles���� ���� spr�� �޷�� �� �����Ѵ�.(smiles�� ������ ��ŭ)
	{
		Rect rect1 = spr2->getBoundingBox(); // rect��� �Լ��� �簢�� ������ ��������Ʈ spr�� ũ�⿡ ���� ������ �����Ѵ�.}
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect1) && powerful == 0)
		{
			game2_gagebar -= 15;

			//����
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/attack.ogg");

			int x = 480 + rand() % 100;
			int y = rand() % 320;

			spr2->setPosition(x, y);
		}
	}

	//���� ������ ó���Ѵ�.
	if (game2_gagebar <= 0)
	{
		game2_win();
		game_win2 = 1;
	}

	//<������ �Դ°Ϳ� ���� ó��>

	//��Ʈ ������
	for (Sprite* spr3 : item_heart)
	{
		Rect rect2 = spr3->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect2))
		{
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_heart.wav");

			spr3->setVisible(false);
			this->removeChild(spr3);
			spr3->setPosition(-500, 0);

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/item_heart.plist");
			apr1->setOpacity(140);
			apr1->setScale(0.6);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 50);

			if (game2_gagebar + 15 < 100)
				game2_gagebar += 15;
			else
				game2_gagebar = 100;
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = spr3->getPositionX();
			int coin_y = spr3->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				spr3->runAction(action1);
		}
	}
	//���� ������
	for (Sprite* coin : coin_vec)
	{
		Rect rect2 = coin->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect2))
		{
			coin->setVisible(false);
			this->removeChild(coin);
			coin->setPosition(-500, 0);

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/item_heart.plist");
			apr1->setOpacity(120);
			apr1->setScale(0.3);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 0);

			if (game2_gagebar + 1 < 100)
				game2_gagebar += 1;
			else
				game2_gagebar = 100;

			score += 30;
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = coin->getPositionX();
			int coin_y = coin->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				coin->runAction(action1);
		}

	}

	//������ ù��° ������
	for (Sprite* coin2 : coin2_vec)
	{
		Rect rect2 = coin2->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect2))
		{
			coin2->setVisible(false);
			this->removeChild(coin2);
			coin2->setPosition(-500, 0);

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/item_heart.plist");
			apr1->setOpacity(120);
			apr1->setScale(0.3);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 0);

						if (game2_gagebar + 1 < 100)
				game2_gagebar += 2;
			else
				game2_gagebar = 100;

			score += 50;
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = coin2->getPositionX();
			int coin_y = coin2->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				coin2->runAction(action1);
		}
	}

	//������ �ι�° ������
	for (Sprite* coin3 : coin3_vec)
	{
		Rect rect2 = coin3->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect2))
		{
			coin3->setVisible(false);
			this->removeChild(coin3);
			coin3->setPosition(-500, 0);

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/item_heart.plist");
			apr1->setOpacity(120);
			apr1->setScale(0.3);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 0);

			if (game2_gagebar + 1 < 100)
				game2_gagebar += 3;
			else
				game2_gagebar = 100;

			score += 80;
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = coin3->getPositionX();
			int coin_y = coin3->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				coin3->runAction(action1);
		}
	}

	//��� ������
	for (Sprite* spr4 : money_vec)
	{
		Rect rect3 = spr4->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect3))
		{
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			spr4->setVisible(false);
			this->removeChild(spr4);
			spr4->setPosition(-500, 0);

			game2_money += game2_level;
			anno_slow("img/ann_getnit.png");
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = spr4->getPositionX();
			int coin_y = spr4->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				spr4->runAction(action1);
		}
	}

	//��ѱ� ������
	for (Sprite* spr5 : item_dove)
	{
		Rect rect3 = spr5->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect3))
		{
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/eat.ogg");

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/item_dove.plist");
			apr1->setOpacity(140);
			apr1->setScale(0.6);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 50);

			spr5->setVisible(false);
			this->removeChild(spr5);
			spr5->setPosition(-500, 0);

			game2_gagebar = 100;
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = spr5->getPositionX();
			int coin_y = spr5->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				spr5->runAction(action1);
		}
	}

	//������ѱ�
	for (Sprite* spr6 : mob3)
	{
		Rect rect4 = spr6->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect4) && powerful == 0)
		{
			game2_gagebar -= 10;

			//����
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/attack.ogg");

			spr6->setVisible(false);
			this->removeChild(spr6);
			spr6->setPosition(-500, 0);
		}
	}

	//�Ŀ� ������
	for (Sprite* spr7 : power_vec)
	{
		Rect rect2 = spr7->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect2))
		{
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_power.wav");

			spr7->setVisible(false);
			this->removeChild(spr7);
			spr7->setPosition(-500, 0);

			powerful = 1;

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/powerful.plist");
			apr1->setOpacity(140);
			apr1->setScale(0.6);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 50, "powerful");

			//������ �� �����������Ѵ�.
			auto backgr = (Sprite*)this->getChildByName("backgr");
			backgr->stopAllActions();

			auto move_br = MoveBy::create(0.2f, Point(-60, 0));
			auto move_br2 = RepeatForever::create(move_br);
			backgr->runAction(move_br2);

			//�Ŀ������� ������̶�� �̹��� ǥ��
			auto spr_po = Sprite::create("img/ann_powerTime.png");
			spr_po->setAnchorPoint(Point(0, 1));
			spr_po->setPosition(0, 300);
			this->addChild(spr_po, 20, "poweritem");

			//���ȭ���� ����
			auto backgr2 = (Sprite*)this->getChildByName("backgr2");
			backgr2->setTexture("img/bgr_supertime.png");


			//5�� �Ŀ� �Ŀ� ������ ȿ���� ���ش�
			auto action1 = DelayTime::create(5.0f);
			auto action2 = CallFunc::create(CC_CALLBACK_0(Game2::power_off, this, (Ref*)10));
			auto action3 = Sequence::create(action1, action2, NULL);
			this->runAction(action3);

			for (Sprite* spr23 : mobs2)
			{
				auto now_y = spr23->getPositionY();

				auto actio0 = MoveTo::create(5.0f, Point(-2000, 0));
				spr23->runAction(actio0);
			}
		}

		//�ڼ� ������ �ߵ���
		if (magnet == 1)
		{
			auto action1 = MoveTo::create(0.5f, Point(actor_x, actor_y));

			int coin_x = spr7->getPositionX();
			int coin_y = spr7->getPositionY();
			if (actor_x + 70 > coin_x && actor_x - 70 < coin_x && actor_y + 70 > coin_y && actor_y - 70 < coin_y)
				spr7->runAction(action1);
		}
	}

	//ȿ���� �������� �̵�
	action_po = actor->getPosition();
	if (powerful == 1)
	{
		auto state1 = (ParticleSystemQuad*)this->getChildByName("powerful");
		state1->setPosition(action_po);
	}
	if (magnet == 1)
	{
		auto state2 = (ParticleSystemQuad*)this->getChildByName("magnet");
		state2->setPosition(action_po);
	}

	//�ڼ� ������
	for (Sprite* spr8 : magnet_vec)
	{
		Rect rect3 = spr8->getBoundingBox();
		Rect rect_sla = actor->getBoundingBox();

		if (rect_sla.intersectsRect(rect3))
		{
			sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound5 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/magnet.ogg");

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/magnet2.plist");
			apr1->setOpacity(30);
			apr1->setScale(0.3);
			apr1->setPosition(actor_x, actor_y);
			this->addChild(apr1, 50, "magnet");

			spr8->setVisible(false);
			this->removeChild(spr8);
			spr8->setPosition(-500, 0);

			magnet = 1;

			//7�� �Ŀ� �Ŀ� �ڼ� ȿ���� ���ش�
			auto action1 = DelayTime::create(7.0f);
			auto action2 = CallFunc::create(CC_CALLBACK_0(Game2::magnet_off, this, (Ref*)10));
			auto action3 = Sequence::create(action1, action2, NULL);
			this->runAction(action3);

		}
	}

}

//��Ʈ�� �⿬
void Game2::heart(float delta)
{
	float x = 480;
	float y = rand() % 320;

	auto spr = Sprite::create("img/icon_heart.png");
	spr->setPosition(Point(x, y));
	spr->setScale(1.5);
	this->addChild(spr);

	auto actio0 = MoveBy::create(7.0, Point(-500, 0));
	auto actio1 = RepeatForever::create(actio0);
	spr->runAction(actio1);

	item_heart.pushBack(spr);
}

//����� �⿬
void Game2::item_money(float delta)
{
	float x = 480;
	float y = rand() % 320;
	int ran_mon;
	ran_mon = 0;

	ran_mon = rand() % 100 + 1;

	if (ran_mon <= 20)
	{
		auto spr = Sprite::create("img/icon_money.png");
		spr->setPosition(Point(x, y));
		spr->setScale(1.5);
		this->addChild(spr);

		auto actio0 = MoveBy::create(7.0, Point(-500, 0));
		auto actio1 = RepeatForever::create(actio0);
		spr->runAction(actio1);

		money_vec.pushBack(spr);
	}
}


//��ѱ��� �⿬
void Game2::dove(float delta)
{
	float x = 480;
	float y = rand() % 320;
	int ran_mon;
	ran_mon = 0;

	ran_mon = rand() % 100 + 1;

	if (ran_mon <= 40)
	{
		auto spr = Sprite::create("img/dove1.png");
		spr->setPosition(Point(x, y));
		this->addChild(spr);

		auto actio0 = MoveBy::create(4.0, Point(-500, 0));
		auto actio1 = RepeatForever::create(actio0);
		spr->runAction(actio1);

		auto animation = Animation::create();
		animation->setDelayPerUnit(0.3);
		animation->addSpriteFrameWithFileName(String::create("img/dove2.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/dove3.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/dove4.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/dove1.png")->getCString());

		auto action_0 = RepeatForever::create(Animate::create(animation));
		spr->runAction(action_0);

		item_dove.pushBack(spr);
	}
}


//�������� �⿬
void Game2::black_dove(float delta)
{
	float x = 480;
	float y = rand() % 320;
	float ran_black;
	ran_black = rand() % 70 + stage*5;
	
	if (ran_black <= 50)
	{
		auto spr = Sprite::create("img/blackbird1.png");
		spr->setPosition(Point(x, y));
		this->addChild(spr);

		auto actio0 = MoveBy::create(2.5, Point(-500, 0));
		auto actio1 = RepeatForever::create(actio0);
		spr->runAction(actio1);

		auto animation = Animation::create();
		animation->setDelayPerUnit(0.3);
		animation->addSpriteFrameWithFileName(String::create("img/blackbird2.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/blackbird1.png")->getCString());
		auto action_0 = RepeatForever::create(Animate::create(animation));
		spr->runAction(action_0);

		mob3.pushBack(spr);
	}
}

//�Ŀ� �������� �⿬
void Game2::item_power(float delta)
{
	float x = 480;
	float y = rand() % 320;
	int ran_mon;
	ran_mon = 0;

	ran_mon = rand() % 100 + 1;

	if (ran_mon <= 65)
	{
		auto spr = Sprite::create("img/icon_po.png");
		spr->setPosition(Point(x, y));
		spr->setScale(1.5);
		this->addChild(spr);

		auto actio0 = MoveBy::create(7.0, Point(-500, 0));
		auto actio1 = RepeatForever::create(actio0);
		spr->runAction(actio1);

		power_vec.pushBack(spr);
	}
}

//�Ŀ��������� ��
void Game2::power_off(Ref* pSender)
{
	powerful = 0;

	auto state135 = (ParticleSystemQuad*)this->getChildByName("powerful");
	this->removeChild(state135);

	//�Ŀ������� ���� ����
	auto state1 = (Sprite*)this->getChildByName("poweritem");
	this->removeChild(state1);

	//���ȭ�� �������
	if (stage == 0)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr2");
		backgr->setTexture("img/img_br1.png");
	}
	else if (stage == 1)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr2");
		backgr->setTexture("img/img_br2.png");
	}
	else if (stage == 2)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr2");
		backgr->setTexture("img/img_br3.png");
	}
	else if (stage == 3)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr2");
		backgr->setTexture("img/img_br4.png");
	}
	else if (stage == 4)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr2");
		backgr->setTexture("img/img_br5.png");
	}

	//������ �ӵ� �����·�
	auto backgr = (Sprite*)this->getChildByName("backgr");
	backgr->stopAllActions();

	auto move_br = MoveBy::create(1.0f, Point(-60, 0));
	auto move_br2 = RepeatForever::create(move_br);
	backgr->runAction(move_br2);

	//���ű�� �ӵ� �����·�
	for (Sprite* spr : mobs2)
	{
		spr->stopAllActions();

		if (stage == 0)
		{
			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(6.0 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(6.0 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 1)
		{
			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(5.5 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(5.5 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 2)
		{
			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(5.0 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(5.0 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 3)
		{
			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(4.5 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(4.5 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 4)
		{
			float mobmove = rand() % 100;
			int ran_p = rand() % 2 + 1;
			int ran_y = rand() % 80;

			if (ran_p == 1)
				ran_y = -ran_y;

			mobmove *= 0.01;

			auto actio0 = MoveBy::create(4.0 - mobmove, Point(-500, ran_y));
			auto actio1 = MoveBy::create(4.0 - mobmove, Point(-500, ran_y));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}

		//������ �ִϸ��̼� ����
		auto animation = Animation::create();

		if (stage == 0 || stage == 1)
			animation->setDelayPerUnit(0.6);
		else if (stage == 2 || stage == 3)
			animation->setDelayPerUnit(0.5);
		else
			animation->setDelayPerUnit(0.4);

		animation->addSpriteFrameWithFileName(String::create("img/birdg1.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/birdg2.png")->getCString());

		auto action_0 = RepeatForever::create(Animate::create(animation));
		spr->runAction(action_0);
	}

}

//�ڼ� �������� �⿬
void Game2::item_magnet(float delta)
{
	float x = 480;
	float y = rand() % 320;
	int ran_mon;
	ran_mon = 0;

	ran_mon = rand() % 100 + 1;

	if (ran_mon <= 65)
	{
		auto spr = Sprite::create("img/magnet.png");
		spr->setPosition(Point(x, y));
		spr->setScale(1.2);
		this->addChild(spr);

		auto actio0 = MoveBy::create(7.0, Point(-500, 0));
		auto actio1 = RepeatForever::create(actio0);
		spr->runAction(actio1);

		magnet_vec.pushBack(spr);
	}
}

//�ڼ� �������� ��
void Game2::magnet_off(Ref* pSender)
{
	magnet = 0;

	auto state135 = (ParticleSystemQuad*)this->getChildByName("magnet");
	this->removeChild(state135);
}

//������ �⿬
void Game2::item_coin(float delta)
{
	float x = 480;
	float y = rand() % 320;

	auto spr = Sprite::create("img/coin.png");
	spr->setPosition(Point(x, y));
	this->addChild(spr);

	auto action1 = RotateBy::create(2.0f, 0, 360);
	auto action2 = RepeatForever::create(action1);

	auto actio0 = MoveBy::create(7.0, Point(-500, 0));
	auto actio1 = RepeatForever::create(actio0);
	spr->runAction(actio1);
	spr->runAction(action2);

	coin_vec.pushBack(spr);
}

//�������� �⿬
void Game2::item_coin2(float delta)
{
	float x = 480;
	float y = rand() % 320;

	auto spr = Sprite::create("img/coin2.png");
	spr->setPosition(Point(x, y));
	this->addChild(spr);

	auto action1 = RotateBy::create(2.0f, 0, 360);
	auto action2 = RepeatForever::create(action1);

	auto actio0 = MoveBy::create(7.0, Point(-500, 0));
	auto actio1 = RepeatForever::create(actio0);
	spr->runAction(actio1);
	spr->runAction(action2);

	coin2_vec.pushBack(spr);
}

//�������� �ι�°�� �⿬
void Game2::item_coin3(float delta)
{
	float x = 480;
	float y = rand() % 320;

	auto spr = Sprite::create("img/coin3.png");
	spr->setPosition(Point(x, y));
	this->addChild(spr);

	auto action1 = RotateBy::create(2.0f, 0, 360);
	auto action2 = RepeatForever::create(action1);

	auto actio0 = MoveBy::create(7.0, Point(-500, 0));
	auto actio1 = RepeatForever::create(actio0);
	spr->runAction(actio1);
	spr->runAction(action2);

	coin3_vec.pushBack(spr);
}

//���� �¸�
void Game2::game2_win()
{
	if (game_win2 == 0)
	{
		auto actor = (Sprite*)this->getChildByName("sla");
		actor->setPosition(-30, -40);

		sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound5 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/win.mid");

		auto state1 = (Sprite*)this->getChildByName("img_start");
		this->removeChild(state1);

		//�⺻ ���� ����
		auto item1 = MenuItemImage::create("img/game2_win1.png", "img/game2_win1.png", CC_CALLBACK_1(Game2::home, this));
		auto menu1 = Menu::create(item1, NULL);
		item1->setAnchorPoint(Point(0, 1));
		item1->setPosition(-240, 160);
		this->addChild(menu1, 50);

		//��ũ ����
		auto spr2 = Sprite::create("img/game2_win2.png");
		spr2->setAnchorPoint(Point(0, 1));
		spr2->setPosition(0, 320);
		this->addChild(spr2, 50);

		//�ִ� �ű�� ����
		//����
		game2_times = UserDefault::getInstance()->getFloatForKey("game2_time");
		if (game2_times < time2)
		{
			UserDefault::getInstance()->setFloatForKey("game2_time", time2);
			UserDefault::getInstance()->flush();
		}

		// ���ھ�
		int game2_score = UserDefault::getInstance()->getIntegerForKey("game2_score");
		if (game2_score < score)
		{
			//�ִ�ű�� �̹��� ǥ��
			auto spr_highscore = Sprite::create("img/ann_highscore.png");
			spr_highscore->setAnchorPoint(Point(0, 1));
			spr_highscore->setPosition(0, 320);
			spr_highscore->setOpacity(0);
			this->addChild(spr_highscore, 55);

			auto actio1 = FadeIn::create(0.5);
			spr_highscore->runAction(actio1);

			//Ư�� ȿ��
			auto apr1 = ParticleSystemQuad::create("par/boom.plist");
			apr1->setOpacity(120);
			apr1->setScale(0.3);
			apr1->setPosition(240, 220);
			this->addChild(apr1, 55);

			UserDefault::getInstance()->setIntegerForKey("game2_score", score);
			UserDefault::getInstance()->flush();
		}

		auto action2 = Blink::create(200, 150);
		spr2->runAction(action2);


		if (time2>=30)
		ran_money2 = time2*1.5 + game2_level * 2 + score/50;

		if (time2 >= 80)
			ran_money2 += game2_level * 2;

		game2_money += ran_money2;
		game2_money += game2_love;

		//ȹ�� ���� ǥ��
		auto label1 = Label::createWithSystemFont(StringUtils::format("%d", game2_money), "Arial", 15);
		label1->setAnchorPoint(Point(0, 1));
		label1->setPosition(Point(165, 165));
		label1->setColor(Color3B(-255, -255, -255));
		this->addChild(label1, 51);


		if (time2 >= 30)
		ran_exp2 = time2 / 3 + (rand() % 5) + game2_level * 3 + game2_love/10;


		//ȹ�� ����ġ�� ǥ��
		auto label2 = Label::createWithSystemFont(StringUtils::format("%d", ran_exp2), "Arial", 15);
		label2->setAnchorPoint(Point(0, 1));
		label2->setPosition(Point(185, 128));
		label2->setColor(Color3B(-255, -255, -255));
		this->addChild(label2, 51);

		//������ ȹ��
		int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
		int exp1 = UserDefault::getInstance()->getIntegerForKey("first_exp");
		int exp2 = UserDefault::getInstance()->getIntegerForKey("second_exp");
		int exp3 = UserDefault::getInstance()->getIntegerForKey("third_exp");

		//���� ȹ��
		game2_money += UserDefault::getInstance()->getIntegerForKey("money");
		UserDefault::getInstance()->setIntegerForKey("money", game2_money);
		UserDefault::getInstance()->flush();

		//����ġ�� ȹ��
		if (main_nity == 1)
		{
			ran_exp2 += exp1;
			UserDefault::getInstance()->setIntegerForKey("first_exp", ran_exp2);
			UserDefault::getInstance()->flush();
		}
		else if (main_nity == 2)
		{
			ran_exp2 += exp2;
			UserDefault::getInstance()->setIntegerForKey("second_exp", ran_exp2);
			UserDefault::getInstance()->flush();
		}
		else if (main_nity == 3)
		{
			ran_exp2 += exp3;
			UserDefault::getInstance()->setIntegerForKey("third_exp", ran_exp2);
			UserDefault::getInstance()->flush();
		}
	}
}


//��ǥ������ ��ġ�� �׷��Ȱ� ������ ������ �ʱ�ȭ
void Game2::make1(float delta)
{

	if (mobs2.size() >= 10 + stage* 3)
		return;

	auto mobran = rand() % 1000+1;

	if (mobran <= 100)
	{
		float x = 480;
		float y = rand() % 320;

		auto spr = Sprite::create("img/birdg1.png");
		spr->setPosition(Point(x, y));
		this->addChild(spr);

		if (stage == 0)
		{
			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(6.0 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(6.0 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 1)
		{
			//���ȭ���� ����
			if (powerful == 0)
			{
				auto backgr = (Sprite*)this->getChildByName("backgr2");
				backgr->setTexture("img/img_br2.png");
			}

			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(5.5 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(5.5 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 2)
		{
			//���ȭ���� ����
			if (powerful == 0)
			{
				auto backgr = (Sprite*)this->getChildByName("backgr2");
				backgr->setTexture("img/img_br3.png");
			}

			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(5.0 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(5.0 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 3)
		{
			//���ȭ���� ����
			if (powerful == 0)
			{
				auto backgr = (Sprite*)this->getChildByName("backgr2");
				backgr->setTexture("img/img_br4.png");
			}

			float mobmove = rand() % 100;
			mobmove *= 0.01;

			auto actio0 = MoveBy::create(4.5 - mobmove, Point(-500, 0));
			auto actio1 = MoveBy::create(4.5 - mobmove, Point(-500, 0));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}
		else if (stage == 4)
		{
			//���ȭ���� ����
			if (powerful == 0)
			{
				auto backgr = (Sprite*)this->getChildByName("backgr2");
				backgr->setTexture("img/img_br5.png");
			}

			float mobmove = rand() % 100;
			int ran_p = rand() % 2 + 1;
			int ran_y = rand() % 80;

			if (ran_p == 1)
				ran_y = -ran_y;

			mobmove *= 0.01;

			auto actio0 = MoveBy::create(4.0 - mobmove, Point(-500, ran_y));
			auto actio1 = MoveBy::create(4.0 - mobmove, Point(-500, ran_y));

			auto actio2 = Sequence::create(actio0, actio1, NULL);
			auto actio3 = RepeatForever::create(actio2);
			spr->runAction(actio3);
		}

		//������ �ִϸ��̼� ����
			auto animation = Animation::create();

			if (stage == 0 || stage == 1)
			animation->setDelayPerUnit(0.6);
			else if (stage == 2 || stage == 3)
			animation->setDelayPerUnit(0.5);
			else
			animation->setDelayPerUnit(0.4);

			animation->addSpriteFrameWithFileName(String::create("img/birdg1.png")->getCString());
			animation->addSpriteFrameWithFileName(String::create("img/birdg2.png")->getCString());

			auto action_0 = RepeatForever::create(Animate::create(animation));
			spr->runAction(action_0);

		mobs2.pushBack(spr); // ���Ϳ� spr ���Ҹ� �߰��Ѵ�
	}
}

//������ ȭ�� ������ �����°��
void Game2::make2(float delta)
{

	for (Sprite* spr : mobs2)
	{
		auto spr_x = spr->getPositionX();

		if (spr_x < -40)
		{
			auto ran_x = rand() % 100;

			float x = 480 + ran_x;
			float y = rand() % 320;
			spr->setPosition(x, y);
		}

	}

}

//���������� ��Ƽ�� �Ŷ��Ÿ���.
void Game2::actioning_nity(float delta)
{
	auto actor = (Sprite*)this->getChildByName("sla");

	//���ΰ��� �ִϸ��̼� ó��
	if (game2_img == 4 && game2_level >= 7)
	{
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.25);
		animation->addSpriteFrameWithFileName(String::create("img/lityw1_r2.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/lityw1_r3.png")->getCString());
		animation->addSpriteFrameWithFileName(String::create("img/lityw1_r1.png")->getCString());

		auto action_0 = RepeatForever::create(Animate::create(animation));
		actor->runAction(action_0);
	}
}

//��ġ�� �����ϴ°��
bool Game2::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation(); // ��ġ���� ��� �� ��ǥ�� location�̶�� ������ �����Ѵ�.

	auto actor = (Sprite*)this->getChildByName("sla");

		auto down = (Sprite*)this->getChildByName("down");
		auto up = (Sprite*)this->getChildByName("up");
		auto right = (Sprite*)this->getChildByName("right");
		auto left = (Sprite*)this->getChildByName("left");
		Rect rect_down = down->getBoundingBox();
		Rect rect_up = up->getBoundingBox();
		Rect rect_right = right->getBoundingBox();
		Rect rect_left = left->getBoundingBox();

		//���ΰ��� �ִϸ��̼� ó��
		if (game2_img == 4 && game2_level >= 7)
		{
			auto animation = Animation::create();
			animation->setDelayPerUnit(0.25);
			animation->addSpriteFrameWithFileName(String::create("img/lityw1_r2.png")->getCString());
			animation->addSpriteFrameWithFileName(String::create("img/lityw1_r3.png")->getCString());
			animation->addSpriteFrameWithFileName(String::create("img/lityw1_r1.png")->getCString());

			auto action_0 = RepeatForever::create(Animate::create(animation));
			actor->runAction(action_0);
		}

		//�̵��ӵ��� �ִ�ġ
		if (game2_level >= 30)
			game2_level = 30;


			//down
			if (rect_down.containsPoint(location))  // ���� ��ġ�� ��ǥ�� rect������ ����Ʈ �ȿ� ���ԵǸ�, �����Ѵ�.
			{
				auto action1 = MoveBy::create(5.0 - game2_level*0.1, Point(0, -500));
				actor->runAction(action1);
				down->setTexture("img/move3_se.png");
			}

			//up
			if (rect_up.containsPoint(location))  // ���� ��ġ�� ��ǥ�� rect������ ����Ʈ �ȿ� ���ԵǸ�, �����Ѵ�.
			{
				auto action1 = MoveBy::create(5.0 - game2_level*0.1, Point(0, 500));
				actor->runAction(action1);
				up->setTexture("img/move3_se.png");
			}


			//right
			if (rect_right.containsPoint(location))  // ���� ��ġ�� ��ǥ�� rect������ ����Ʈ �ȿ� ���ԵǸ�, �����Ѵ�.
			{
				auto action1 = MoveBy::create(5.0 - game2_level*0.1, Point(500, 0));
				actor->runAction(action1);
				right->setTexture("img/move3_se.png");

				if (powerful == 0)
				{
					//������ �� �����������Ѵ�.
					auto backgr = (Sprite*)this->getChildByName("backgr");
					backgr->stopAllActions();

					auto move_br = MoveBy::create(0.6f, Point(-60, 0));
					auto move_br2 = RepeatForever::create(move_br);
					backgr->runAction(move_br2);
				}
			}

			//left
			if (rect_left.containsPoint(location))  // ���� ��ġ�� ��ǥ�� rect������ ����Ʈ �ȿ� ���ԵǸ�, �����Ѵ�.
			{
				auto action1 = MoveBy::create(5.0 - game2_level*0.1, Point(-500, 0));
				actor->runAction(action1);
				left->setTexture("img/move3_se.png");
			}

	return true;
}


//��ġ�� ���� ���
void Game2::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (gamestart == 1)
	{
		auto up = (Sprite*)this->getChildByName("sla");
		up->stopAllActions();
	}

	auto down = (Sprite*)this->getChildByName("down");
	auto up = (Sprite*)this->getChildByName("up");
	auto right = (Sprite*)this->getChildByName("right");
	auto left = (Sprite*)this->getChildByName("left");
	down->setTexture("img/move3.png");
	up->setTexture("img/move3.png");
	right->setTexture("img/move3.png");
	left->setTexture("img/move3.png");

	//������ �ӵ� �����·�
	if (powerful == 0)
	{
		auto backgr = (Sprite*)this->getChildByName("backgr");
		backgr->stopAllActions();

		auto move_br = MoveBy::create(1.0f, Point(-60, 0));
		auto move_br2 = RepeatForever::create(move_br);
		backgr->runAction(move_br2);
	}

}


// �������� bgm�� �߻�
void Game2::sound(float delta)
{
	sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//������� ����
	if (sound5 == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game2.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

//�˸� �޼��� ǥ��
void Game2::anno_slow(char *ano)
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

//�������� ���ư���
void Game2::home(Ref* pSender)
{
	//��Ÿ ������ ����
	gamestart = 0;

	//���� ������ �ʱ�ȭ
	time2 = 0.0f;
	game2_money = 0;
	mobs2.clear();
	item_heart.clear();
	money_vec.clear();
	item_dove.clear();
	mob3.clear();
	power_vec.clear();
	powerful = 0;
	coin_vec.clear();
	coin2_vec.clear();
	coin3_vec.clear();

	ran_money2 = 0;
	ran_exp2 = 0;
	game2_times = 0;
	score = 0;

	//���� ó��
	sound5 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound5 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//��ġ�̺�Ʈ ������ ����� Ȯ���� ����
	this->setTouchEnabled(false);
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener3);
	Director::getInstance()->pushScene(inlife::createScene());
}
