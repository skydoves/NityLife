
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "shop.h"

using namespace CocosDenshion;

#define di 120

int sound3; //사운드 on(=0)/off(=1)

Scene* shop::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = shop::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



bool shop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//배경음악 설정
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	if (sound3 == 0)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/shop.mp3");
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->schedule(schedule_selector(shop::sound), 171.0);

	//돈 표시
	this->schedule(schedule_selector(shop::init_money), 0.01f);

	init_menu();

	return true;
}


void shop::init_menu()
{
	this->removeAllChildren();

	//기본 바탕 설정
	auto spr1 = Sprite::create("img/shop_backgr.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	auto me1 = MenuItemImage::create("img/shop_ch1.png", "img/shop_ch1_se.png", CC_CALLBACK_1(shop::food_shop, this));
	auto me2 = MenuItemImage::create("img/shop_ch2.png", "img/shop_ch2_se.png", CC_CALLBACK_1(shop::thing_shop, this));
	auto me3 = MenuItemImage::create("img/shop_ch3.png", "img/shop_ch3_se.png", CC_CALLBACK_1(shop::egg_shop, this));
	auto me4 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(shop::shop_close, this));
	auto me5 = MenuItemImage::create("img/post.png", "img/post.png", CC_CALLBACK_1(shop::coder, this));
	auto menu = Menu::create(me1, me2, me3, me4, me5, NULL);
	me1->setAnchorPoint(Point(0, 1));
	me1->setPosition(Point(-215, 110));
	me2->setAnchorPoint(Point(0, 1));
	me2->setPosition(Point(-65, 110));
	me3->setAnchorPoint(Point(0, 1));
	me3->setPosition(Point(85, 110));
	me4->setAnchorPoint(Point(0, 1));
	me4->setPosition(Point(200, -115));
	me4->setScale(2.0);
	me5->setAnchorPoint(Point(0, 1));
	me5->setPosition(Point(-220, -120));
	this->addChild(menu, 1, "menu");

}


//쿠폰 입력
void shop::coder(Ref* pSender)
{
	this->removeAllChildren();

	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int pcode = UserDefault::getInstance()->getIntegerForKey("pcode");

	if (pcode == 0)
	{
		pcode = (rand() % 100000000) + 50;
		UserDefault::getInstance()->setIntegerForKey("pcode", pcode);
		UserDefault::getInstance()->flush();
	}

	//기본배경
	auto spr1 = Sprite::create("img/shop_code.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//쿠폰입력 화면 나가기
	auto me4 = MenuItemImage::create("img/icon_left.png", "img/icon_left_se.png", CC_CALLBACK_1(shop::close, this));
	auto menu = Menu::create(me4, NULL);
	me4->setAnchorPoint(Point(0, 1));
	me4->setPosition(Point(-175, -97));
	me4->setScale(2.0);
	this->addChild(menu, 1, "menu");

	//개인 코드번호의 스프라이트
	auto label1 = Label::createWithSystemFont(StringUtils::format("%d", pcode), "Arial", 15);
	label1->setAnchorPoint(Point(0, 1));
	label1->setPosition(Point(168, 232));
	label1->setColor(Color3B::BLACK);
	this->addChild(label1, 1);

	//에디트박스의 스프라이트
	auto edit = EditBox::create(Size(170, 30), Scale9Sprite::create("img/input.png"));
	edit->setPosition(Point(250, 152));
	edit->setPlaceHolder("          Code    ");
	edit->setMaxLength(10);
	this->addChild(edit);

	edit->setPlaceholderFont("Arial", 15);
	edit->setFont("", 15);
	edit->setFontColor(Color3B::BLACK);
	edit->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	edit->setInputMode(EditBox::InputMode::NUMERIC);

	edit->setDelegate(this);
}

//코드를 입력해서 상품을 얻을수있다.
void shop::editBoxReturn(EditBox* editbox)
{
	int pcode = UserDefault::getInstance()->getIntegerForKey("pcode");
	int cup1 = UserDefault::getInstance()->getIntegerForKey("cup1");
	int money = UserDefault::getInstance()->getIntegerForKey("money");
	char inar[11];
	int inputval=0;
	int size = 11;

	auto input = editbox->getText();
	for (int i = 0; i <= 10; i++)
	inar[i] = *(input+i);

	int i, count2;
	double count = 0;

	for (i = 0; i<size; i++)
	{
		if (inar[i] != '\0')
			count++;
		else
			break;
	}
	count2 = (int)count;
	count--;

	for (i = 0; i<count2; i++)
	{
		if ((int)count != 0){
			inputval += ((int)inar[i] - 48) * (int)pow(10, count);
			count--;
		}
	}
	inputval += ((int)inar[count2 - 1] - 48);

	if (inputval == (pcode-32))
	{
		if (cup1 != 1)
		{
			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

			anno("img/ann_getnit.png");

			money += 10000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("cup1", 1);
			UserDefault::getInstance()->flush();
		}
	}
	else if (inputval == (pcode + 47))
	{
		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

		anno("img/ann_getnit.png");

		money += 100000;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();
	}
}

//먹이 상점
void shop::food_shop(Ref* pSender)
{
	this->removeAllChildren();

	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//기본 배경 설정
	auto spr1 = Sprite::create("img/shop_backgr.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//레이어 생성 , 레이어 크기 설정
	auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	layer->setContentSize(Size(760, 200));

	//shop의 스크롤 배경
	auto br = Sprite::create("img/shopbr_food.png");
	br->setPosition(Point(0, 200));
	br->setAnchorPoint(Point(0, 1));
	layer->addChild(br, 2);

	//스크롤에 레이어 등록
	auto scroll = ScrollView::create(Size(480, 200), layer);
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setBounceable(false);
	scroll->setPosition(0, 60);
	this->addChild(scroll, 2);

	//뒤로 나가기 버튼
	auto exit = MenuItemImage::create("img/icon_left.png", "img/icon_left_se.png", CC_CALLBACK_1(shop::close, this));
	auto exitmenu = Menu::create(exit, NULL);
	exitmenu->setAnchorPoint(Point(0, 1));
	exitmenu->setPosition(0, 0);
	exit->setScale(2.0);
	exit->setPosition(Point(30, 30));
	this->addChild(exitmenu, 4);


	//<아이템들을 관리하는곳>

	//아이템들의 배경
	auto item1 = Sprite::create("img/fo1.png");
	auto item2 = Sprite::create("img/fo2.png");
	auto item3 = Sprite::create("img/fo3.png");
	auto item4 = Sprite::create("img/fo4.png");
	auto item5 = Sprite::create("img/fo5.png");
	auto item6 = Sprite::create("img/fo6.png");

	item1->setPosition(80, 100);
	item2->setPosition(80 + di * 1, 100);
	item3->setPosition(80 + di * 2, 100);
	item4->setPosition(80 + di * 3, 100);
	item5->setPosition(80 + di * 4, 100);
	item6->setPosition(80 + di * 5, 100);

	layer->addChild(item1, 3);
	layer->addChild(item2, 3);
	layer->addChild(item3, 3);
	layer->addChild(item4, 3);
	layer->addChild(item5, 3);
	layer->addChild(item6, 3);

	//아이템들의 가격
	auto label1 = Label::createWithSystemFont(StringUtils::format("200"), "Arial", 13); //먹이 10~15개
	auto label2 = Label::createWithSystemFont(StringUtils::format("450"), "Arial", 13); //먹이 1~40개
	auto label3 = Label::createWithSystemFont(StringUtils::format("110"), "Arial", 13); //딸기
	auto label4 = Label::createWithSystemFont(StringUtils::format("130"), "Arial", 13); //복숭아
	auto label5 = Label::createWithSystemFont(StringUtils::format("150"), "Arial", 13); //바나나
	auto label6 = Label::createWithSystemFont(StringUtils::format("170"), "Arial", 13); //파인애플

	label1->setPosition(Point(95, 72));
	label2->setPosition(Point(95 + di * 1, 72));
	label3->setPosition(Point(95 + di * 2, 72));
	label4->setPosition(Point(95 + di * 3, 72));
	label5->setPosition(Point(95 + di * 4, 72));
	label6->setPosition(Point(95 + di * 5, 72));

	label1->setColor(Color3B(-255, -255, -255));
	label2->setColor(Color3B(-255, -255, -255));
	label3->setColor(Color3B(-255, -255, -255));
	label4->setColor(Color3B(-255, -255, -255));
	label5->setColor(Color3B(-255, -255, -255));
	label6->setColor(Color3B(-255, -255, -255));

	layer->addChild(label1, 4);
	layer->addChild(label2, 4);
	layer->addChild(label3, 4);
	layer->addChild(label4, 4);
	layer->addChild(label5, 4);
	layer->addChild(label6, 4);
	
	//아이템들의 구매버튼
	auto buy1 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy1, this));
	auto buy2 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy2, this));
	auto buy3 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy3, this));
	auto buy4 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy4, this));
	auto buy5 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy5, this));
	auto buy6 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::food_buy6, this));
	auto menu2 = Menu::create(buy1, buy2, buy3, buy4, buy5, buy6, NULL);
	menu2->setAnchorPoint(Point(0, 1));
	menu2->setPosition(0, 0);
	buy1->setPosition(Point(80, 45));
	buy2->setPosition(Point(80 + di * 1, 45));
	buy3->setPosition(Point(80 + di * 2, 45));
	buy4->setPosition(Point(80 + di * 3, 45));
	buy5->setPosition(Point(80 + di * 4, 45));
	buy6->setPosition(Point(80 + di * 5, 45));
	layer->addChild(menu2, 4);

}

void shop::food_buy1(Ref* pSender)
{
	int ran = (rand() % 5 + 10);

	int food = UserDefault::getInstance()->getIntegerForKey("food");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 200)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 200;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		food += ran;
		UserDefault::getInstance()->setIntegerForKey("food", food);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");

}

void shop::food_buy2(Ref* pSender)
{
	int ran = (rand() % 50 + 1);

	int food = UserDefault::getInstance()->getIntegerForKey("food");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 450)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 450;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		food += ran;
		UserDefault::getInstance()->setIntegerForKey("food", food);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
}

void shop::food_buy3(Ref* pSender)
{
	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert1");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 110)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 110;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		dessert += 5;
		UserDefault::getInstance()->setIntegerForKey("dessert1", dessert);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
}

void shop::food_buy4(Ref* pSender)
{
	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert2");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 130)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 130;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		dessert += 5;
		UserDefault::getInstance()->setIntegerForKey("dessert2", dessert);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
}

void shop::food_buy5(Ref* pSender)
{
	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert3");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 150)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 150;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		dessert += 5;
		UserDefault::getInstance()->setIntegerForKey("dessert3", dessert);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
}

void shop::food_buy6(Ref* pSender)
{
	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert4");
	int money = UserDefault::getInstance()->getIntegerForKey("money");


	if (money >= 170)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 170;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		dessert += 5;
		UserDefault::getInstance()->setIntegerForKey("dessert4", dessert);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
}


//조형물 상점
void shop::thing_shop(Ref* pSender)
{
	this->removeAllChildren();

	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//기본 배경 설정
	auto spr1 = Sprite::create("img/shop_backgr.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//레이어 생성 , 레이어 크기 설정
	auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	layer->setContentSize(Size(1600, 200));

	//shop의 스크롤 배경
	auto br = Sprite::create("img/shopbr_thing.png");
	br->setPosition(Point(0, 200));
	br->setAnchorPoint(Point(0, 1));
	layer->addChild(br, 2);

	//스크롤에 레이어 등록
	auto scroll = ScrollView::create(Size(480, 200), layer);
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setBounceable(false);
	scroll->setPosition(0, 60);
	this->addChild(scroll, 2);

	//뒤로 나가기 버튼
	auto exit = MenuItemImage::create("img/icon_left.png", "img/icon_left_se.png", CC_CALLBACK_1(shop::close, this));
	auto exitmenu = Menu::create(exit, NULL);
	exitmenu->setAnchorPoint(Point(0, 1));
	exitmenu->setPosition(0, 0);
	exit->setScale(2.0);
	exit->setPosition(Point(30, 30));
	this->addChild(exitmenu, 4);


	//<아이템들을 관리하는곳>

	//아이템들의 배경
	auto item1 = Sprite::create("img/th1.png");
	auto item2 = Sprite::create("img/th2.png");
	auto item3 = Sprite::create("img/th3.png");
	auto item4 = Sprite::create("img/th4.png");
	auto item5 = Sprite::create("img/th5.png");
	auto item6 = Sprite::create("img/th6.png");
	auto item7 = Sprite::create("img/th7.png");
	auto item8 = Sprite::create("img/th8.png");
	auto item9 = Sprite::create("img/th9.png");
	auto item10 = Sprite::create("img/th10.png");
	auto item11 = Sprite::create("img/th11.png");
	auto item12 = Sprite::create("img/th12.png");
	auto item13 = Sprite::create("img/th13.png");

	item1->setPosition(80, 100);
	item2->setPosition(80 + di * 1, 100);
	item3->setPosition(80 + di * 2, 100);
	item4->setPosition(80 + di * 3, 100);
	item5->setPosition(80 + di * 4, 100);
	item6->setPosition(80 + di * 5, 100);
	item7->setPosition(80 + di * 6, 100);
	item8->setPosition(80 + di * 7, 100);
	item9->setPosition(80 + di * 8, 100);
	item10->setPosition(80 + di * 9, 100);
	item11->setPosition(80 + di * 10, 100);
	item12->setPosition(80 + di * 11, 100);
	item13->setPosition(80 + di * 12, 100);

	layer->addChild(item1, 3);
	layer->addChild(item2, 3);
	layer->addChild(item3, 3);
	layer->addChild(item4, 3);
	layer->addChild(item5, 3);
	layer->addChild(item6, 3);
	layer->addChild(item7, 3);
	layer->addChild(item8, 3);
	layer->addChild(item9, 3);
	layer->addChild(item10, 3);
	layer->addChild(item11, 3);
	layer->addChild(item12, 3);
	layer->addChild(item13, 3);


	//아이템들의 가격
	auto label1 = Label::createWithSystemFont(StringUtils::format("500"), "Arial", 13); //기본풀밭
	auto label2 = Label::createWithSystemFont(StringUtils::format("750"), "Arial", 13); //거친 풀반
	auto label3 = Label::createWithSystemFont(StringUtils::format("1650"), "Arial", 13); //눈속 얼음
	auto label4 = Label::createWithSystemFont(StringUtils::format("3200"), "Arial", 13); //뭉개구름
	auto label5 = Label::createWithSystemFont(StringUtils::format("3200"), "Arial", 13); //고급 카펫
	auto label6 = Label::createWithSystemFont(StringUtils::format("2000"), "Arial", 13); //나무 한그루
	auto label7 = Label::createWithSystemFont(StringUtils::format("550"), "Arial", 13); //유채화
	auto label8 = Label::createWithSystemFont(StringUtils::format("1650"), "Arial", 13); //구름 하나
	auto label9 = Label::createWithSystemFont(StringUtils::format("1200"), "Arial", 13); //눈사람
	auto label10 = Label::createWithSystemFont(StringUtils::format("500"), "Arial", 13); //조형물 위치 랜덤 변경
	auto label11 = Label::createWithSystemFont(StringUtils::format("free"), "Arial", 13); //효과 없애기
	auto label12 = Label::createWithSystemFont(StringUtils::format("1000"), "Arial", 13); //눈내리기 효과
	auto label13 = Label::createWithSystemFont(StringUtils::format("1000"), "Arial", 13); //반딧불이


	label1->setPosition(Point(95, 72));
	label2->setPosition(Point(95 + di * 1, 72));
	label3->setPosition(Point(95 + di * 2, 72));
	label4->setPosition(Point(95 + di * 3, 72));
	label5->setPosition(Point(95 + di * 4, 72));
	label6->setPosition(Point(95 + di * 5, 72));
	label7->setPosition(Point(95 + di * 6, 72));
	label8->setPosition(Point(95 + di * 7, 74));
	label9->setPosition(Point(95 + di * 8, 74));
	label10->setPosition(Point(95 + di * 9, 74));
	label11->setPosition(Point(95 + di * 10, 74));
	label12->setPosition(Point(95 + di * 11, 74));
	label13->setPosition(Point(95 + di * 12, 74));

	label1->setColor(Color3B(-255, -255, -255));
	label2->setColor(Color3B(-255, -255, -255));
	label3->setColor(Color3B(-255, -255, -255));
	label4->setColor(Color3B(-255, -255, -255));
	label5->setColor(Color3B(-255, -255, -255));
	label6->setColor(Color3B(-255, -255, -255));
	label7->setColor(Color3B(-255, -255, -255));
	label8->setColor(Color3B(-255, -255, -255));
	label9->setColor(Color3B(-255, -255, -255));
	label10->setColor(Color3B(-255, -255, -255));
	label11->setColor(Color3B(-255, -255, -255));
	label12->setColor(Color3B(-255, -255, -255));
	label13->setColor(Color3B(-255, -255, -255));

	layer->addChild(label1, 4);
	layer->addChild(label2, 4);
	layer->addChild(label3, 4);
	layer->addChild(label4, 4);
	layer->addChild(label5, 4);
	layer->addChild(label6, 4);
	layer->addChild(label7, 4);
	layer->addChild(label8, 4);
	layer->addChild(label9, 4);
	layer->addChild(label10, 4);
	layer->addChild(label11, 4);
	layer->addChild(label12, 4);
	layer->addChild(label13, 4);

	//아이템들의 구매버튼
	auto buy1 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing_tile_buy1, this));
	auto buy2 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing_tile_buy2, this));
	auto buy3 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing_tile_buy3, this));
	auto buy4 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing_tile_buy4, this));
	auto buy5 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing_tile_buy5, this));
	auto buy6 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing2_tile_buy1, this));
	auto buy7 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing2_tile_buy2, this));
	auto buy8 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing2_tile_buy3, this));
	auto buy9 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing2_tile_buy4, this));
	auto buy10 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing2_tile_buy5, this));
	auto buy11 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing3_buy1, this));
	auto buy12 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing3_buy2, this));
	auto buy13 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::thing3_buy3, this));
	auto menu2 = Menu::create(buy1, buy2, buy3, buy4, buy5, buy6, buy7, buy8, buy9, buy10, buy11, buy12, buy13, NULL);
	menu2->setAnchorPoint(Point(0, 1));
	menu2->setPosition(0, 0);
	buy1->setPosition(Point(80, 45));
	buy2->setPosition(Point(80 + di * 1, 45));
	buy3->setPosition(Point(80 + di * 2, 45));
	buy4->setPosition(Point(80 + di * 3, 45));
	buy5->setPosition(Point(80 + di * 4, 45));
	buy6->setPosition(Point(80 + di * 5, 45));
	buy7->setPosition(Point(80 + di * 6, 45));
	buy8->setPosition(Point(80 + di * 7, 45));
	buy9->setPosition(Point(80 + di * 8, 45));
	buy10->setPosition(Point(80 + di * 9, 45));
	buy11->setPosition(Point(80 + di * 10, 45));
	buy12->setPosition(Point(80 + di * 11, 45));
	buy13->setPosition(Point(80 + di * 12, 45));
	layer->addChild(menu2, 4);

}

void shop::thing_tile_buy1(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");
	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");

	if (backgr != 0)
	{
		if (money >= 500)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 500;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("backgr", 0);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}

void shop::thing_tile_buy2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");

	if (backgr != 1)
	{
		if (money >= 750)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 750;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("backgr", 1);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}

void shop::thing_tile_buy3(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");
	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");

	if (backgr != 2)
	{
		if (money >= 1650)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 1650;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("backgr", 2);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}

void shop::thing_tile_buy4(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");

	if (backgr != 3)
	{
		if (money >= 3200)
		{
			anno_slow("img/ann_buy2.png");
			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");


			money -= 3200;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("backgr", 3);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}

void shop::thing_tile_buy5(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");

	if (backgr != 4)
	{
	if (money >= 3200)
	{
		anno_slow("img/ann_buy2.png");

		sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound3 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

		money -= 3200;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("backgr", 4);
		UserDefault::getInstance()->flush();
	}
	else
		anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}


void shop::thing2_tile_buy1(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");
	int trees = UserDefault::getInstance()->getIntegerForKey("trees");

	if (trees < 2)
	{
		if (money >= 2000)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			//나무의 수분과 온도
			if (trees == 0)
			{
				UserDefault::getInstance()->setIntegerForKey("trees_wat1", 50);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("trees_tem1", 50);
				UserDefault::getInstance()->flush();
			}
			else if (trees == 1)
			{
				UserDefault::getInstance()->setIntegerForKey("trees_wat2", 50);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("trees_tem2", 50);
				UserDefault::getInstance()->flush();
			}

			money -= 2000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			trees++;
			UserDefault::getInstance()->setIntegerForKey("trees", trees);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy5.png");
}

void shop::thing2_tile_buy2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int flower = UserDefault::getInstance()->getIntegerForKey("flower");

	if (flower < 3)
	{
		if (money >= 550)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 550;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			flower++;
			UserDefault::getInstance()->setIntegerForKey("flower", flower);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy5.png");
}

void shop::thing2_tile_buy3(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int cloud = UserDefault::getInstance()->getIntegerForKey("cloud");

	if (cloud < 3)
	{
		if (money >= 1650)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 1650;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			cloud++;
			UserDefault::getInstance()->setIntegerForKey("cloud", cloud);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy5.png");
}

void shop::thing2_tile_buy4(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int snowman = UserDefault::getInstance()->getIntegerForKey("snowman");

	if (snowman < 2)
	{
		if (money >= 1200)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 1200;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			snowman++;
			UserDefault::getInstance()->setIntegerForKey("snowman", snowman);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy5.png");
}

void shop::thing2_tile_buy5(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");

	int snowman = UserDefault::getInstance()->getIntegerForKey("snowman");

		if (money >= 500)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");

			money -= 500;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			int trees = UserDefault::getInstance()->getIntegerForKey("trees", 0);
			int flower = UserDefault::getInstance()->getIntegerForKey("flower", 0);
			int cloud = UserDefault::getInstance()->getIntegerForKey("cloud", 0);
			int snowman = UserDefault::getInstance()->getIntegerForKey("snowman", 0);

			UserDefault::getInstance()->setIntegerForKey("tree1_x", 0);
			UserDefault::getInstance()->flush();
			UserDefault::getInstance()->setIntegerForKey("tree1_y", 0);
			UserDefault::getInstance()->flush();

			if (trees >= 1)
			{
				UserDefault::getInstance()->setIntegerForKey("trees1_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("trees1_y", 0);
				UserDefault::getInstance()->flush();
			}

			if (trees >= 2)
			{
				UserDefault::getInstance()->setIntegerForKey("trees2_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("trees2_y", 0);
				UserDefault::getInstance()->flush();
			}

			if (flower >= 1)
			{
				UserDefault::getInstance()->setIntegerForKey("flower1_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower1_y", 0);
				UserDefault::getInstance()->flush();
			}

			if (flower >= 2)
			{
				UserDefault::getInstance()->setIntegerForKey("flower2_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower2_y", 0);
				UserDefault::getInstance()->flush();
			}

			if (flower >= 3)
			{
				UserDefault::getInstance()->setIntegerForKey("flower3_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower3_y", 0);
				UserDefault::getInstance()->flush();
			}
			if (cloud >= 1)
			{
				UserDefault::getInstance()->setIntegerForKey("cloud1_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("cloud1_y", 0);
				UserDefault::getInstance()->flush();
			}
			if (cloud >= 2)
			{
				UserDefault::getInstance()->setIntegerForKey("cloud2_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("cloud2_y", 0);
				UserDefault::getInstance()->flush();
			}
			if (snowman >= 1)
			{
				UserDefault::getInstance()->setIntegerForKey("snowman1_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("snowman1_y", 0);
				UserDefault::getInstance()->flush();
			}
			if (snowman >= 2)
			{
				UserDefault::getInstance()->setIntegerForKey("snowman2_x", 0);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("snowman2_y", 0);
				UserDefault::getInstance()->flush();
			}
		}
		else
			anno_slow("img/ann_buy1.png");
	}


/*effect 
  0:  효과 제거
  1:  눈 내리기
  2:  반딧불이
*/

void shop::thing3_buy1(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");
	int effe = UserDefault::getInstance()->getIntegerForKey("effect");

		if (money >= 1000)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");


			money -= 1000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("effect", 0);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
}

void shop::thing3_buy2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");
	int effe = UserDefault::getInstance()->getIntegerForKey("effect");

	if (effe != 1)
	{
		if (money >= 1000)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");


			money -= 1000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("effect", 1);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}

void shop::thing3_buy3(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int money = UserDefault::getInstance()->getIntegerForKey("money");
	int effe = UserDefault::getInstance()->getIntegerForKey("effect");

	if (effe != 2)
	{
		if (money >= 1000)
		{
			anno_slow("img/ann_buy2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/buy.ogg");


			money -= 1000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("effect", 2);
			UserDefault::getInstance()->flush();
		}
		else
			anno_slow("img/ann_buy1.png");
	}
	else
		anno_slow("img/ann_buy4.png");
}


//니티알 구매
void shop::egg_shop(Ref* pSender)
{
	this->removeAllChildren();

	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//기본 배경 설정
	auto spr1 = Sprite::create("img/shop_backgr.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//레이어 생성 , 레이어 크기 설정
	auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	layer->setContentSize(Size(480, 200));

	//shop의 스크롤 배경
	auto br = Sprite::create("img/shopbr_egg.png");
	br->setPosition(Point(0, 200));
	br->setAnchorPoint(Point(0, 1));
	layer->addChild(br, 2);
	layer->setTag(20);

	//스크롤에 레이어 등록
	auto scroll = ScrollView::create(Size(480, 200), layer);
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setBounceable(false);
	scroll->setPosition(0, 60);
	this->addChild(scroll, 2);
	

	//뒤로 나가기 버튼
	auto exit = MenuItemImage::create("img/icon_left.png", "img/icon_left_se.png", CC_CALLBACK_1(shop::close, this));
	auto exitmenu = Menu::create(exit, NULL);
	exitmenu->setAnchorPoint(Point(0, 1));
	exitmenu->setPosition(0, 0);
	exit->setScale(2.0);
	exit->setPosition(Point(30, 30));
	this->addChild(exitmenu, 4);


	//<아이템들을 관리하는곳>

	//아이템들의 배경
	auto item1 = Sprite::create("img/egg1.png");
	auto item2 = Sprite::create("img/egg2.png");
	auto item3 = Sprite::create("img/egg3.png");

	item1->setPosition(80, 100);
	item2->setPosition(80 + di * 1, 100);
	item3->setPosition(80 + di * 2, 100);

	layer->addChild(item1, 3);
	layer->addChild(item2, 3);
	layer->addChild(item3, 3);

	//아이템들의 가격
	auto label1 = Label::createWithSystemFont(StringUtils::format("2000"), "Arial", 13); //슬라임의알
	auto label2 = Label::createWithSystemFont(StringUtils::format("3500"), "Arial", 13); //리티의 알

	label1->setPosition(Point(95, 72));
	label2->setPosition(Point(95 + di * 1, 72));

	label1->setColor(Color3B(-255, -255, -255));
	label2->setColor(Color3B(-255, -255, -255));

	layer->addChild(label1, 4);
	layer->addChild(label2, 4);

	//아이템들의 구매버튼
	auto buy1 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::egg_buy1, this));
	auto buy2 = MenuItemImage::create("img/buy.png", "img/buy_se.png", CC_CALLBACK_1(shop::egg_buy2, this));
	auto buy3 = MenuItemImage::create("img/ann_nitysell.png", "img/ann_nitysell_se.png", CC_CALLBACK_1(shop::nity_sell, this));
	auto menu2 = Menu::create(buy1, buy2, buy3, NULL);
	menu2->setAnchorPoint(Point(0, 1));
	menu2->setPosition(0, 0);
	buy1->setPosition(Point(80, 45));
	buy2->setPosition(Point(80 + di * 1, 45));
	buy3->setPosition(Point(81 + di * 2, 90));
	layer->addChild(menu2, 4);

	auto ex1 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(shop::ex_egg1, this));
	auto ex2 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(shop::ex_egg2, this));
	auto menu3 = Menu::create(ex1, ex2, NULL);
	menu3->setAnchorPoint(Point(0, 1));
	menu3->setPosition(0, 0);
	ex1->setPosition(Point(110, 130));
	ex2->setPosition(Point(110 + di * 1, 130));
	layer->addChild(menu3, 4);
}

//슬라임의알 설명
void shop::ex_egg1(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto me_main = MenuItemImage::create("img/ex_egg1.png", "img/ex_egg1.png", CC_CALLBACK_1(shop::close_simplat, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
}

//리티알 설명
void shop::ex_egg2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto me_main = MenuItemImage::create("img/ex_egg2.png", "img/ex_egg2.png", CC_CALLBACK_1(shop::close_simplat, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
}

//니티알을 구매한다.
void shop::egg_buy1(Ref* pSender)
{
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");
	int money = UserDefault::getInstance()->getIntegerForKey("money");

	if (money >= 2000)
	{
		if (second == 0)
		{
			anno_slow("img/ann_egg2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/open.mid");

			money -= 2000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			float x = (rand() % 3 + 1);

			if (x == 1) //파란색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("second", 1);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes6.png", "img/img_mes6.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}
			else if (x == 2) //분홍색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("second", 2);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes7.png", "img/img_mes7.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}
			else if (x == 3) //초록색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("second", 3);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes8.png", "img/img_mes8.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}

			//보유중인 니티 마리수 = 1
			UserDefault::getInstance()->setIntegerForKey("nities", 2);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_love", 0);
			UserDefault::getInstance()->flush();

			//첫번재 니티정보 초기화
			UserDefault::getInstance()->setIntegerForKey("second_lv", 1);
			UserDefault::getInstance()->flush();

			float y = (rand() % 100 + 1);
			y /= 100;
			float x1 = (rand() % 2 + 1 + y);
			UserDefault::getInstance()->setFloatForKey("second_jump", x1);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setFloatForKey("second_fly", 0.0f);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_stat", 0);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", 50);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_exp", 0);
			UserDefault::getInstance()->flush();
		}
		else if (third == 0)
		{
			anno_slow("img/ann_egg2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/open.mid");

			money -= 2000;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			float x = (rand() % 3 + 1);

			if (x == 1) //파란색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("third", 1);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes6.png", "img/img_mes6.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}
			else if (x == 2) //분홍색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("third", 2);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes7.png", "img/img_mes7.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}
			else if (x == 3) //초록색 슬라임
			{
				//첫번재 니티 이미지코드
				UserDefault::getInstance()->setIntegerForKey("third", 3);
				UserDefault::getInstance()->flush();

				auto item1 = MenuItemImage::create("img/img_mes8.png", "img/img_mes8.png", CC_CALLBACK_1(shop::close, this));
				auto menu = Menu::create(item1, NULL);
				menu->alignItemsVertically();
				this->addChild(menu, 2);
			}

			//보유중인 니티 마리수 = 1
			UserDefault::getInstance()->setIntegerForKey("nities", 3);
			UserDefault::getInstance()->flush();

			//첫번재 니티정보 초기화
			UserDefault::getInstance()->setIntegerForKey("third_lv", 1);
			UserDefault::getInstance()->flush();

			float y = (rand() % 100 + 1);
			y /= 100;
			float x1 = (rand() % 2 + 1 + y);
			UserDefault::getInstance()->setFloatForKey("third_jump", x1);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_love", 0);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setFloatForKey("third_fly", 0.0f);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_stat", 0);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", 50);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_exp", 0);
			UserDefault::getInstance()->flush();

		}
		else
			anno_slow("img/ann_egg1.png");
	}
	else
		anno_slow("img/ann_buy1.png");
}

void shop::egg_buy2(Ref* pSender)
{
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");
	int money = UserDefault::getInstance()->getIntegerForKey("money");

	if (money >= 3500)
	{
		if (second == 0)
		{
			anno_slow("img/ann_egg2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/open.mid");

			money -= 3500;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();


			UserDefault::getInstance()->setIntegerForKey("second", 4);
			UserDefault::getInstance()->flush();

			//보유중인 니티 마리수 = 1
			UserDefault::getInstance()->setIntegerForKey("nities", 2);
			UserDefault::getInstance()->flush();

			//첫번재 니티정보 초기화
			UserDefault::getInstance()->setIntegerForKey("second_lv", 1);
			UserDefault::getInstance()->flush();

			float y = (rand() % 100 + 1);
			y /= 100;
			float x1 = (rand() % 2 + 1 + y);
			UserDefault::getInstance()->setFloatForKey("second_jump", x1);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setFloatForKey("second_fly", 0.0f);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_stat", 0);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", 50);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_exp", 0);
			UserDefault::getInstance()->flush();
		}
		else if (third == 0)
		{
			anno_slow("img/ann_egg2.png");

			sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound3 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/open.mid");

			money -= 3500;
			UserDefault::getInstance()->setIntegerForKey("money", money);
			UserDefault::getInstance()->flush();

			//첫번재 니티 이미지코드
			UserDefault::getInstance()->setIntegerForKey("third", 4);
			UserDefault::getInstance()->flush();

			//보유중인 니티 마리수 = 1
			UserDefault::getInstance()->setIntegerForKey("nities", 3);
			UserDefault::getInstance()->flush();

			//첫번재 니티정보 초기화
			UserDefault::getInstance()->setIntegerForKey("third_lv", 1);
			UserDefault::getInstance()->flush();

			float y = (rand() % 100 + 1);
			y /= 100;
			float x1 = (rand() % 2 + 1 + y);
			UserDefault::getInstance()->setFloatForKey("third_jump", x1);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setFloatForKey("third_fly", 0.0f);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_stat", 0);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", 50);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_exp", 0);
			UserDefault::getInstance()->flush();

		}
		else
			anno_slow("img/ann_egg1.png");
	}
	else
		anno_slow("img/ann_buy1.png");
}

//니티를 풀어준다.
void shop::nity_sell(Ref* pSender)
{
	this->removeAllChildren();

	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//기본 배경 설정
	auto spr1 = Sprite::create("img/shop_backgr.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	auto state1 = (Sprite*)this->getChildByName("egg_menu");
	this->removeChild(state1);
	int first_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");

	auto sell = Sprite::create("img/shop_eggsel.png");
	sell->setAnchorPoint(Point(0, 1));
	sell->setPosition(0, 320);
	this->addChild(sell);

	//니티의 정보 표시
	auto label1 = Label::createWithSystemFont(StringUtils::format("Lv : %d", first_lv), "Arial", 14);
	label1->setPosition(Point(170, 231));
	label1->setColor(Color3B::BLACK);
	this->addChild(label1);

	auto label2 = Label::createWithSystemFont(StringUtils::format("Lv : %d", second_lv), "Arial", 14);
	label2->setPosition(Point(170, 198));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2);

	auto item1 = MenuItemImage::create("img/sell.png", "img/sell_se.png", CC_CALLBACK_1(shop::nity_sell2, this));
	auto close = MenuItemImage::create("img/icon_left.png", "img/icon_left_se.png", CC_CALLBACK_1(shop::close, this));
	auto item2 = MenuItemImage::create("img/sell.png", "img/sell_se.png", CC_CALLBACK_1(shop::nity_sell3, this));
	auto menu = Menu::create(item1, close, item2, NULL);
	item1->setPosition(Point(120, 69));
	item2->setPosition(Point(120, 37));
	close->setScale(2.0);
	close->setPosition(Point(-170, -102));
	this->addChild(menu, 1, "egg_menu");
}

void shop::nity_sell2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int second = UserDefault::getInstance()->getIntegerForKey("second");

	if (second != 0)
	{
		anno("img/ann_sell1.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(shop::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(shop::nity_di1, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(shop::close, this));
		auto menu = Menu::create(item1, item2, item3, NULL);
		item2->setPosition(Point(-25, 5));
		item3->setPosition(Point(25, 5));
		this->addChild(menu, 1, "egg_menu2");
	}
	else
		anno("img/ann_sell3.png");
}

void shop::nity_sell3(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int third = UserDefault::getInstance()->getIntegerForKey("third");

	if (third != 0)
	{
		anno("img/ann_sell1.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(shop::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(shop::nity_di2, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(shop::close, this));
		auto menu = Menu::create(item1, item2, item3, NULL);
		item2->setPosition(Point(-25, 5));
		item3->setPosition(Point(25, 5));
		this->addChild(menu, 1, "egg_menu2");
	}
	else
		anno("img/ann_sell3.png");
}


//니티를 정말 풀어준다.
void shop::nity_di1(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state1 = (Sprite*)this->getChildByName("egg_menu2");
	this->removeChild(state1);

	anno("img/ann_sell2.png");

	UserDefault::getInstance()->setIntegerForKey("main_nity", 1);
	UserDefault::getInstance()->flush();

	int nities = UserDefault::getInstance()->getIntegerForKey("nities");
	--nities;
	UserDefault::getInstance()->setIntegerForKey("nities", nities);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("second", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("second_lv", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setFloatForKey("second_jump", 0.0f);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setFloatForKey("second_fly", 0.0f);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("second_exp", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("second_food", 0);
	UserDefault::getInstance()->flush();
}

void shop::nity_di2(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state1 = (Sprite*)this->getChildByName("egg_menu2");
	this->removeChild(state1);

	anno("img/ann_sell2.png");

	UserDefault::getInstance()->setIntegerForKey("main_nity", 1);
	UserDefault::getInstance()->flush();

	int nities = UserDefault::getInstance()->getIntegerForKey("nities");
	--nities;
	UserDefault::getInstance()->setIntegerForKey("nities", nities);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("third", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("third_lv", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setFloatForKey("third_jump", 0.0f);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setFloatForKey("third_fly", 0.0f);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("third_exp", 0);
	UserDefault::getInstance()->flush();

	UserDefault::getInstance()->setIntegerForKey("third_food", 0);
	UserDefault::getInstance()->flush();
}


//아무것도 하지않는다.
void shop::nothing(Ref* pSender)
{

}



//상점의 처음 화면으로 돌아간다
void shop::close(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");
	init_menu();
}


//게임 화면의 전환 - 상점을 나간다
void shop::shop_close(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");
	Director::getInstance()->pushScene(inlife::createScene());
}


// 지속적인 bgm의 발생
void shop::sound(float delta)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//배경음악 설정
	if (sound3 == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/shop.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

//알림 메세지의 생성
void shop::anno(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(120, 335));
	this->addChild(img1, 16, "ann");
}

//알림 메세지의 삭제
void shop::anno_del()
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);
}

//알림 메세지의 느린 생성과 소멸
void shop::anno_slow(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(120, 335));
	this->addChild(img1, 16, "ann");

	//천천히 이미지를 페이드아웃한다.
	auto action1 = FadeOut::create(5.0);
	img1->runAction(action1);
}

//먹이와 돈의 상태창을 실시간으로 업데이트 한다.
void shop::init_money(float delta)
{
	auto state1 = (Sprite*)this->getChildByName("spr1");
	this->removeChild(state1);
	auto state2 = (Sprite*)this->getChildByName("spr2");
	this->removeChild(state2);
	auto state3 = (Sprite*)this->getChildByName("spr3");
	this->removeChild(state3);
	auto state4 = (Sprite*)this->getChildByName("spr4");
	this->removeChild(state4);

	//니트(골드)의 표시
	auto img2 = Sprite::create("img/img_money.png");
	img2->setPosition(Point(475, 317));
	img2->setAnchorPoint(Point(1, 1));
	img2->setScale(1.2);
	this->addChild(img2, 10, "spr1");

	int money = UserDefault::getInstance()->getIntegerForKey("money", 0);
	auto label1 = Label::createWithSystemFont(StringUtils::format("%d", money), "Arial", 14);
	label1->setAnchorPoint(Point(1, 1));
	label1->setPosition(Point(460, 307));
	this->addChild(label1, 11, "spr2");

	//남은 먹이량의 표시
	auto img3 = Sprite::create("img/img_food.png");
	img3->setPosition(Point(5, 317));
	img3->setAnchorPoint(Point(0, 1));
	img3->setScale(1.2);
	this->addChild(img3, 10, "spr3");

	int food = UserDefault::getInstance()->getIntegerForKey("food", 0);
	auto label2 = Label::createWithSystemFont(StringUtils::format("%d", food), "Arial", 14);
	label2->setAnchorPoint(Point(0, 1));
	label2->setPosition(Point(35, 307));
	this->addChild(label2, 11, "spr4");
}


void shop::close_simplat(Ref* pSender)
{
	sound3 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound3 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state3 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state3);
}