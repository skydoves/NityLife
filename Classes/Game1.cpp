
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

	//데이터들을 초기화한다
	mobs.clear();
	init_Game();

	//랜덤함수
	srand(time(NULL));

	//게이지바의 지속적인 업데이트
	this->schedule(schedule_selector(Game1::update_gage), 0.01f);

	//목표물에 맞았는지를 판정한다
	this->schedule(schedule_selector(Game1::hit), 0.01f);

	//몹들의 위치 초기화
	this->schedule(schedule_selector(Game1::make1), 0.01f);

	//게이지바 업데이트
	this->schedule(schedule_selector(Game1::gage_sh), 0.01f);

	//배경음악 설정
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	if (sound4 == 0)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/ongame.mp3");
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->schedule(schedule_selector(Game1::sound), 35.0);

	//이벤트 리스너에 터치기능 등록
	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = CC_CALLBACK_2(Game1::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener2, 1);

	//기본 바탕 설정
	auto spr1 = Sprite::create("img/game1_br.png");
	spr1->setAnchorPoint(Point(0, 1));
	spr1->setPosition(0, 320);
	this->addChild(spr1);

	//게임 타이틀 설정
	auto spr5 = Sprite::create("img/game1_title.png");
	spr5->setAnchorPoint(Point(0, 1));
	spr5->setPosition(0, 320);
	this->addChild(spr5, 10,"title");

	//게이지바 툴
	auto spr2 = Sprite::create("img/gage_tool.png");
	spr2->setAnchorPoint(Point(0, 1));
	spr2->setPosition(10, 322);
	this->addChild(spr2);

	//남은 라이프의 수
	auto spr3 = Sprite::create("img/icon_life.png");
	spr3->setAnchorPoint(Point(0, 1));
	spr3->setPosition(200, 315);
	spr3->setScale(1.2);
	this->addChild(spr3);

	//게임종료 버튼
	auto item1 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(Game1::home, this));
	item1->setAnchorPoint(Point(0, 1));
	item1->setPosition(210, 153);
	item1->setScale(2.0);
	auto menu1 = Menu::create(item1, NULL);
	this->addChild(menu1, 2);

	return true;
}


//목표물에 맞았는지를 판정함
void Game1::hit(float delta)
{

	auto removed = Sprite::create("img/blank.png");
	auto sla = (Sprite*)this->getChildByName("sla");
	auto sla_po = sla->getPosition();

		for (Sprite* spr : mobs) // 스프라이트 포인터 : 배열 처럼 smiles들의 값을 spr에 쭈루룩 다 저장한다.(smiles의 사이즈 만큼)
		{
			Rect rect1 = spr->getBoundingBox(); // rect라는 함수에 사각형 식으로 스프라이트 spr의 크기에 대해 범위를 생성한다.}
			Rect rect_sla = sla->getBoundingBox();

			if (rect_sla.intersectsRect(rect1) && die==0)
			{
				//이미지의 삭제와, 벡터의 원소에서 삭제한다.
				die = 1;

				removed = spr;
				spr->setPosition(0, 0);
				this -> removeChild(spr);


				auto action1 = DelayTime::create(0.5);
				auto action2 = CallFunc::create(CC_CALLBACK_0(Game1::hit_mob, this));
				auto action3 = Sequence::create(action1, action2, NULL);

				this->runAction(action3);

				//사운드
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

					//특수 효과
					auto apr1 = ParticleSystemQuad::create("par/item_power.plist");
					apr1->setOpacity(140);
					apr1->setScale(0.6);
					apr1->setPosition(sla_po);
					this->addChild(apr1, 50);
				}
			}
		}

}

//딜레이 후 계산 처리
void Game1::hit_mob()
{
	mob_co--;
	die = 0;
}

//목표물들의 위치와 그래픽과 움직임 정보를 초기화
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

	mobs.pushBack(spr); // 벡터에 spr 원소를 추가한다
}

//게임의 처음 데이터 초기화 $$*
void Game1::init_Game()
{
	//기본 데이터 초기화
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

	//메인니티의 정보를 불러온다
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

	//스타트 아이콘
	auto spr1 = Sprite::create("img/icon_start.png");
	spr1->setPosition(430, 50);
	spr1->setOpacity(200);
	this->addChild(spr1, 2, "img_start");


	//라이프를 정해준다
	life = (level * 3)/2 + 1;
	mob_co = level;

	//이미지를 정해준다
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

//던지기 한번이 끝난경우
void Game1::init2()
{
	//게임 시작 버튼의 초기화
	auto spr1 = (Sprite*)this->getChildByName("img_start");
	spr1->setTexture("img/icon_start.png");

	//슬라임의 이미지 위치 초기화
	auto sla = (Sprite*)this->getChildByName("sla");
	sla->setPosition(80, 80);

	//게임스타트 버튼, 점프중의 상태 초기화
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

				//점프 한번 날린경우 데이터 관리
				--life;  // 라이프 하나 감소
				onjump = 1; //점프중

				//점프 최대치의 한계
				if (jump >= 10)
					jump = 10;

				auto action1 = JumpTo::create(3.0, Point(500, -(2.0*gagebar)+3*jump), 5*jump, 1);
				sla->runAction(action1);

				gagebar = 0; //게이지바 = 0

			}
			//2중 점프
			else if (onstart == 1 && onjump == 1 && double_jump == 0)
			{
				//특수 효과
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


//지속적인 게이지바의 업데이트와 캐릭터 이미지의 이동시 게임의 재시작 관리, 게임오버 처리
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
		//게임 오버의 처리
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

//게임오버
void Game1::gameover()
{
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/gameover.mid");

	if (mob_co != 0)
	{
		auto state1 = (Sprite*)this->getChildByName("img_start");
		this->removeChild(state1);

		//기본 바탕 설정
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

//라이프로 돌아가기
void Game1::home(Ref* pSender)
{
	mobs.clear();
	onstart = 0;
	onjump = 0;

	//사운드 처리
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	//터치이벤트 리스너 기능을 확실히 제거
	this->setTouchEnabled(false);
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener2);
	Director::getInstance()->pushScene(inlife::createScene());
}

//게임 승리
void Game1::gamewin()
{

	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound4 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/win.mid");

	auto state1 = (Sprite*)this->getChildByName("img_start");
	this->removeChild(state1);

	//기본 바탕 설정
	auto item1 = MenuItemImage::create("img/img_win2.png", "img/img_win2.png", CC_CALLBACK_1(Game1::home, this));
	auto menu1 = Menu::create(item1, NULL);
	item1->setAnchorPoint(Point(0, 1));
	item1->setPosition(-240, 160);
	this->addChild(menu1, 50);

	//블링크 설정
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

	//보상량의 획득
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


//지속적인 게이지바 업데이트 
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


// 지속적인 bgm의 발생
void Game1::sound(float delta)
{
	sound4 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//배경음악 설정
	if (sound4 == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/ongame.mp3");
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

//알림 메세지 표시
void Game1::anno_slow(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(165, 315));
	this->addChild(img1, 16, "ann");

	//천천히 이미지를 페이드아웃한다.
	auto action1 = FadeOut::create(4.0);
	img1->runAction(action1);
}