#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "inlife.h"
#include "shop.h"
#include "Game1.h"
#include "Game2.h"

using namespace CocosDenshion;

//<관리 메뉴 변수들>
int sound2; //사운드 on(=0)/off(=1)
int eggtouch = 0;
int touchenable; // 여기서 나갈때는 1, 들어올때는 0   0 일때 터치가능, 1일때 터치 불가능
int touch_make;
int eat_nity; // 먹이줄때 몇번째 니티에게 줄건지 구분해주는 변수
int onfood; //먹이주기 창이 떠잇을때
int onstat; //스텟 창이 떠잇을때
int food_max = 100; //포만도의 최댓값
int rand_sound;
int onstate2=0;
int dirt_op;
int emotion_start = 0;

//<니티들의 최대 경험치>
int exp_max1; //니티의 경험치 최댓값
int exp_max2; //니티의 경험치 최댓값
int exp_max3; //니티의 경험치 최댓값

//<테그 관리 메뉴들>
#define message 1
#define UI 2


//<니티 움직임 관리 메뉴들>
float move_first;
float move_second;
float move_third;
int move_fourth;
int move_fifth;

//<니티 상태이미지 메뉴들>
char* first_img;
char* second_img;
char* third_img;
char* fourth_img;
char* fifth_img;

//<니티 이모션 활성화 상태>
int emo_nity1;
int emo_nity2;
int emo_nity3;


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
	//랜덤함수
	srand(time(NULL));
	//윈도우 사이즈 크기
	winSize = Director::getInstance()->getWinSize();

	touch_make = UserDefault::getInstance()->getIntegerForKey("touch_make", 0);
	//터치의 가능
	touchenable = 0;
	//이벤트 리스너에 터치기능 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(inlife::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(inlife::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	//이벤트 리스너에 키패드 기능 등록
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(inlife::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	int tutor = UserDefault::getInstance()->getIntegerForKey("tutorial", 0);

	if (tutor < 11)
	{
		//배경음악 설정
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/main2.mp3");

		tutorial();
	}
	else
		//게임 화면의 초기화
		initmenu();

	return true;
}


//기본 화면의 초기화
void inlife::initmenu()
{
	//배경음악 설정
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//니티들의 초기화
	init_nity();

	rand_sound = rand() % 5 + 1;

	//배경음악 설정
	if (sound2 == 0)
	{
		if (rand_sound == 1)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife1.mp3");
			this->schedule(schedule_selector(inlife::sound), 150.0);
			this->schedule(schedule_selector(inlife::nity_sound), 20.0);
		}
		else if (rand_sound == 2)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife2.mp3");
			this->schedule(schedule_selector(inlife::sound), 173.0);
			this->schedule(schedule_selector(inlife::nity_sound), 20.0);
		}
		else if (rand_sound == 3)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife3.mp3");
			this->schedule(schedule_selector(inlife::sound), 95.0);
			this->schedule(schedule_selector(inlife::nity_sound), 20.0);
		}
		else if (rand_sound == 4)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife4.mp3");
			this->schedule(schedule_selector(inlife::sound), 29.0);
			this->schedule(schedule_selector(inlife::nity_sound), 20.0);
		}
		else if (rand_sound == 5)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife5.mp3");
			this->schedule(schedule_selector(inlife::sound), 35.0);
			this->schedule(schedule_selector(inlife::nity_sound), 20.0);
		}
	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	up_stat();
	tree_wat(); //시간에따라 나무의 수분이 감소한다.
	tree_tem(); //시간에따라 나무의 온도가 사용자가 설정한 온도에 가까워진다.
	

	//배경화면 설정
	int backgr = UserDefault::getInstance()->getIntegerForKey("backgr");
	auto img1 = Sprite::create("img/tile.png");

	if (backgr==1)
		img1->setTexture("img/backgr1.jpg");  //수풀
	else if (backgr == 2)
		img1->setTexture("img/backgr2.jpg");  //빙판
	else if (backgr == 3)
		img1->setTexture("img/backgr3.jpg");  //구름
	else if (backgr == 4)
		img1->setTexture("img/backgr4.jpg");  //집안 카펫

	img1->setPosition(Point(0, 320));
	img1->setAnchorPoint(Point(0, 1));
	this->addChild(img1);

	//<UI 출력 설정>
	//니트(골드)의 표시
	auto img2 = Sprite::create("img/img_money.png");
	img2->setPosition(Point(475, 317));
	img2->setAnchorPoint(Point(1, 1));
	img2->setScale(1.2);
	this->addChild(img2, 10);

	int money = UserDefault::getInstance()->getIntegerForKey("money", 0);
	auto label1 = Label::createWithSystemFont(StringUtils::format("%d", money), "Arial", 14);
	label1->setAnchorPoint(Point(1, 1));
	label1->setPosition(Point(460, 307));
	this->addChild(label1, 11);

	//남은 먹이량의 표시
	auto img3 = Sprite::create("img/img_food.png");
	img3->setPosition(Point(5, 317));
	img3->setAnchorPoint(Point(0, 1));
	img3->setScale(1.2);
	this->addChild(img3, 10);

	int food = UserDefault::getInstance()->getIntegerForKey("food", 0);
	auto label2 = Label::createWithSystemFont(StringUtils::format("%d", food), "Arial", 14);
	label2->setAnchorPoint(Point(0, 1));
	label2->setPosition(Point(35, 307));
	this->addChild(label2, 11, "food_q2");

	//남은 먹이량을 지속적으로 업데이트해준다.
	this->schedule(schedule_selector(inlife::init_food), 0.2f);

	//포만감을 지속적으로 감소시켜준다.
	this->schedule(schedule_selector(inlife::sh_food), 60.0f);

	//이모션을 지속적으로 업데이트 시켜준다.
	this->schedule(schedule_selector(inlife::emotion), 15.0f);

	//접속시 종료했던 시간만큼 니티들의 포만감을 감소시켜준다.
	time_food();

	//니티의 레벨업 처리를 해준다.
	exp_control();

	//상점 구매 물품들을 초기화 한다.
	init_things();

	//기본 나무의 표시(처음 하나 주는 나무)
	int wat0 = UserDefault::getInstance()->getIntegerForKey("trees_wat0");
	int tem0 = UserDefault::getInstance()->getIntegerForKey("trees_tem0");
	int tree1_x = UserDefault::getInstance()->getIntegerForKey("tree1_x", 0);
	int tree1_y = UserDefault::getInstance()->getIntegerForKey("tree1_y", 0);
	if (tree1_x == 0 && tree1_y == 0)
	{
		tree1_x = (rand() % ((int)winSize.width - 250))+50;
		tree1_y = (rand() % ((int)winSize.height - 70)) + 150;
		UserDefault::getInstance()->setIntegerForKey("tree1_x", tree1_x);
		UserDefault::getInstance()->flush();
		UserDefault::getInstance()->setIntegerForKey("tree1_y", tree1_y);
		UserDefault::getInstance()->flush();

		//기본나무
		UserDefault::getInstance()->setIntegerForKey("trees_wat0", 50);
		UserDefault::getInstance()->flush();
		UserDefault::getInstance()->setIntegerForKey("trees_tem0", 50);
		UserDefault::getInstance()->flush();
		UserDefault::getInstance()->setIntegerForKey("lifetem", 50);
		UserDefault::getInstance()->flush();
		wat0 = 50;
		tem0 = 50;
	}
	auto img4 = Sprite::create("img/tree.png");
	img4->setPosition(Point(tree1_x, tree1_y));
	img4->setAnchorPoint(Point(0, 1));
	img4->setOpacity(128);
	this->addChild(img4, 5);

	//수분과 온도에따른 나무의 형태 변화
	if (wat0 >= 61 && wat0 <= 100 && tem0 >= 61 && tem0 <= 100)  //아열대 나무
		img4->setTexture("img/tree2.png");
	if (wat0 >= 0 && wat0 <= 29 && tem0 >= 61 && tem0 <= 100) //야자나무
		img4->setTexture("img/tree3.png");
	if (tem0 >= 0 && tem0 <= 29) //얼음나무
		img4->setTexture("img/tree4.png");


	//상점 아이콘, 퀘스트 아이콘 추가
	auto item1 = MenuItemImage::create("img/icon_shop.png", "img/icon_shop.png", CC_CALLBACK_1(inlife::shop, this));
	auto item2 = MenuItemImage::create("img/icon_quest.png", "img/icon_quest.png", CC_CALLBACK_1(inlife::quest, this));
	auto item3 = MenuItemImage::create("img/icon_leaf.png", "img/icon_leaf.png", CC_CALLBACK_1(inlife::game, this));
	auto item4 = MenuItemImage::create("img/icon_camera.png", "img/icon_camera.png", CC_CALLBACK_1(inlife::camera, this));
	auto item5 = MenuItemImage::create("img/icon_home.png", "img/icon_home.png", CC_CALLBACK_1(inlife::home, this));
	auto item6 = MenuItemImage::create("img/tree.png", "img/tree.png", CC_CALLBACK_1(inlife::trees, this));
	auto menu = Menu::create(item1,item2, item3, item4,  item5, item6, NULL);
	item1->setPosition(Point(216, 100));
	item2->setPosition(Point(216, 67));
	item3->setPosition(Point(216, 34));
	item4->setPosition(Point(216, 1));
	item5->setPosition(Point(216, -32));
	item6->setScale(0.2);
	item6->setPosition(Point(-216, -130));
	menu->setTag(UI);
	this->addChild(menu, 10);
}


//상점 구매 조형물들 초기화
void inlife::init_things()
{
	int trees = UserDefault::getInstance()->getIntegerForKey("trees", 0);
	int flower = UserDefault::getInstance()->getIntegerForKey("flower", 0);
	int cloud = UserDefault::getInstance()->getIntegerForKey("cloud", 0);
	int snowman = UserDefault::getInstance()->getIntegerForKey("snowman", 0);
	int effe = UserDefault::getInstance()->getIntegerForKey("effect", 0);

	//사과가 기본 나무에 맺히게 하기
	int fast_time = time(NULL);
	int apple_time = UserDefault::getInstance()->getIntegerForKey("apple_time");
	int apple_time2 = UserDefault::getInstance()->getIntegerForKey("apple_time2");
	int apple_time3 = UserDefault::getInstance()->getIntegerForKey("apple_time3");
	int apple = UserDefault::getInstance()->getIntegerForKey("apple");
	int apple2 = UserDefault::getInstance()->getIntegerForKey("apple2");
	int apple3 = UserDefault::getInstance()->getIntegerForKey("apple3");
	int tree0_tem = UserDefault::getInstance()->getIntegerForKey("trees_tem0");
	int tree1_tem = UserDefault::getInstance()->getIntegerForKey("trees_tem1");
	int tree2_tem = UserDefault::getInstance()->getIntegerForKey("trees_tem2");

	if ((fast_time - apple_time) >= 3600 && tree0_tem>=29)
	{
		UserDefault::getInstance()->setIntegerForKey("apple", 1);
		UserDefault::getInstance()->flush();

		//기본 나무의 표시(처음 하나 주는 나무)
		int tree1_x = UserDefault::getInstance()->getIntegerForKey("tree1_x", 0);
		int tree1_y = UserDefault::getInstance()->getIntegerForKey("tree1_y", 0);

		auto img4 = Sprite::create("img/icon_food.png");
		img4->setPosition(Point(tree1_x + 30, tree1_y - 55));
		img4->setAnchorPoint(Point(0, 1));
		this->addChild(img4, 6, "apples");
	}

	//두번째 사과
	if ((fast_time - apple_time2) >= 3600 && tree1_tem >= 29)
	{
		UserDefault::getInstance()->setIntegerForKey("apple2", 1);
		UserDefault::getInstance()->flush();

		//기본 나무의 표시(처음 하나 주는 나무)
		int tree1_x = UserDefault::getInstance()->getIntegerForKey("trees1_x", 0);
		int tree1_y = UserDefault::getInstance()->getIntegerForKey("trees1_y", 0);

		auto img4 = Sprite::create("img/icon_food.png");
		img4->setPosition(Point(tree1_x + 30, tree1_y - 55));
		img4->setAnchorPoint(Point(0, 1));
		this->addChild(img4, 6, "apples2");
	}


    //세번째 사과
	if ((fast_time - apple_time3) >= 3600 && tree2_tem >= 29)
	{
		UserDefault::getInstance()->setIntegerForKey("apple3", 1);
		UserDefault::getInstance()->flush();

		//기본 나무의 표시(처음 하나 주는 나무)
		int tree1_x = UserDefault::getInstance()->getIntegerForKey("trees2_x", 0);
		int tree1_y = UserDefault::getInstance()->getIntegerForKey("trees2_y", 0);

		auto img4 = Sprite::create("img/icon_food.png");
		img4->setPosition(Point(tree1_x + 30, tree1_y - 55));
		img4->setAnchorPoint(Point(0, 1));
		this->addChild(img4, 6, "apples3");
	}

	//특수효과
	if (effe == 1)
	{
		auto apr1 = ParticleSystemQuad::create("par/snow.plist");
		apr1->setOpacity(60);
		apr1->setScale(0.5);
		apr1->setPosition(240, 160);
		this->addChild(apr1, 9);
	}
	else if (effe == 2)
	{
		auto apr1 = ParticleSystemQuad::create("par/flies.plist");
		apr1->setOpacity(60);
		apr1->setScale(0.5);
		apr1->setPosition(240, 160);
		this->addChild(apr1, 9);
	}

	//나무
	if (trees >= 1)
	{
		int trees1_x = UserDefault::getInstance()->getIntegerForKey("trees1_x", 0);
		int trees1_y = UserDefault::getInstance()->getIntegerForKey("trees1_y", 0);
		if (trees1_x == 0 && trees1_y == 0)
		{
			trees1_x = (rand() % ((int)winSize.width - 250)) + 50;
			trees1_y = (rand() % ((int)winSize.height - 70)) + 150;
			UserDefault::getInstance()->setIntegerForKey("trees1_x", trees1_x);
			UserDefault::getInstance()->flush();
			UserDefault::getInstance()->setIntegerForKey("trees1_y", trees1_y);
			UserDefault::getInstance()->flush();
		}
	}
		if (trees >= 2)
		{
			int trees2_x = UserDefault::getInstance()->getIntegerForKey("trees2_x", 0);
			int trees2_y = UserDefault::getInstance()->getIntegerForKey("trees2_y", 0);
			if (trees2_x == 0 && trees2_y == 0)
			{
				trees2_x = (rand() % ((int)winSize.width - 250)) + 50;
				trees2_y = (rand() % ((int)winSize.height - 70)) + 150;
				UserDefault::getInstance()->setIntegerForKey("trees2_x", trees2_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("trees2_y", trees2_y);
				UserDefault::getInstance()->flush();
			}
		}

		//유채화
		if (flower >= 1)
		{
			int flower1_x = UserDefault::getInstance()->getIntegerForKey("flower1_x", 0);
			int flower1_y = UserDefault::getInstance()->getIntegerForKey("flower1_y", 0);
			if (flower1_x == 0 && flower1_y == 0)
			{
				flower1_x = (rand() % ((int)winSize.width - 100)) + 50;
				flower1_y = (rand() % ((int)winSize.height - 100)) + 50;
				UserDefault::getInstance()->setIntegerForKey("flower1_x", flower1_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower1_y", flower1_y);
				UserDefault::getInstance()->flush();
			}
		}
		if (flower >= 2)
		{
			int flower2_x = UserDefault::getInstance()->getIntegerForKey("flower2_x", 0);
			int flower2_y = UserDefault::getInstance()->getIntegerForKey("flower2_y", 0);
			if (flower2_x == 0 && flower2_y == 0)
			{
				flower2_x = (rand() % ((int)winSize.width - 100)) + 50;
				flower2_y = (rand() % ((int)winSize.height - 100)) + 50;
				UserDefault::getInstance()->setIntegerForKey("flower2_x", flower2_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower2_y", flower2_y);
				UserDefault::getInstance()->flush();
			}
		}
		if (flower >= 3)
		{
			int flower3_x = UserDefault::getInstance()->getIntegerForKey("flower3_x", 0);
			int flower3_y = UserDefault::getInstance()->getIntegerForKey("flower3_y", 0);
			if (flower3_x == 0 && flower3_y == 0)
			{
				flower3_x = (rand() % ((int)winSize.width - 100)) + 50;
				flower3_y = (rand() % ((int)winSize.height - 100)) + 50;
				UserDefault::getInstance()->setIntegerForKey("flower3_x", flower3_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("flower3_y", flower3_y);
				UserDefault::getInstance()->flush();
			}
		}

		//구름
		if (cloud >= 1)
		{
			int cloud1_x = UserDefault::getInstance()->getIntegerForKey("cloud1_x", 0);
			int cloud1_y = UserDefault::getInstance()->getIntegerForKey("cloud1_y", 0);
			if (cloud1_x == 0 && cloud1_y == 0)
			{
				cloud1_x = (rand() % ((int)winSize.width - 150)) + 100;
				cloud1_y = (rand() % ((int)winSize.height - 150)) + 100;
				UserDefault::getInstance()->setIntegerForKey("cloud1_x", cloud1_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("cloud1_y", cloud1_y);
				UserDefault::getInstance()->flush();
			}
		}
		if (cloud >= 2)
		{
			int cloud2_x = UserDefault::getInstance()->getIntegerForKey("cloud2_x", 0);
			int cloud2_y = UserDefault::getInstance()->getIntegerForKey("cloud2_y", 0);
			if (cloud2_x == 0 && cloud2_y == 0)
			{
				cloud2_x = (rand() % ((int)winSize.width - 150)) + 100;
				cloud2_y = (rand() % ((int)winSize.height - 150)) + 100;
				UserDefault::getInstance()->setIntegerForKey("cloud2_x", cloud2_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("cloud2_y", cloud2_y);
				UserDefault::getInstance()->flush();
			}
		}
		if (cloud >= 3)
		{
			int cloud3_x = UserDefault::getInstance()->getIntegerForKey("cloud3_x", 0);
			int cloud3_y = UserDefault::getInstance()->getIntegerForKey("cloud3_y", 0);
			if (cloud3_x == 0 && cloud3_y == 0)
			{
				cloud3_x = (rand() % ((int)winSize.width - 150)) + 100;
				cloud3_y = (rand() % ((int)winSize.height - 150)) + 100;
				UserDefault::getInstance()->setIntegerForKey("cloud3_x", cloud3_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("cloud3_y", cloud3_y);
				UserDefault::getInstance()->flush();
			}
		}

		//구름
		if (snowman >= 1)
		{
			int snowman1_x = UserDefault::getInstance()->getIntegerForKey("snowman1_x", 0);
			int snowman1_y = UserDefault::getInstance()->getIntegerForKey("snowman1_y", 0);
			if (snowman1_x == 0 && snowman1_y == 0)
			{
				snowman1_x = (rand() % ((int)winSize.width - 150)) + 100;
				snowman1_y = (rand() % ((int)winSize.height - 150)) + 100;
				UserDefault::getInstance()->setIntegerForKey("snowman1_x", snowman1_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("snowman1_y", snowman1_y);
				UserDefault::getInstance()->flush();
			}
		}
		if (snowman >= 2)
		{
			int snowman2_x = UserDefault::getInstance()->getIntegerForKey("snowman2_x", 0);
			int snowman2_y = UserDefault::getInstance()->getIntegerForKey("snowman2_y", 0);
			if (snowman2_x == 0 && snowman2_y == 0)
			{
				snowman2_x = (rand() % ((int)winSize.width - 150)) + 100;
				snowman2_y = (rand() % ((int)winSize.height - 150)) + 100;
				UserDefault::getInstance()->setIntegerForKey("snowman2_x", snowman2_x);
				UserDefault::getInstance()->flush();
				UserDefault::getInstance()->setIntegerForKey("snowman2_y", snowman2_y);
				UserDefault::getInstance()->flush();
			}
		}
		//<상점 구매 물품 이미지 초기화>
		//1번 나무
		if (trees >= 1)
		{
			int wat1 = UserDefault::getInstance()->getIntegerForKey("trees_wat1");
			int tem1 = UserDefault::getInstance()->getIntegerForKey("trees_tem1");
			int trees1_x = UserDefault::getInstance()->getIntegerForKey("trees1_x", 0);
			int trees1_y = UserDefault::getInstance()->getIntegerForKey("trees1_y", 0);
			auto img5 = Sprite::create("img/tree.png");
			img5->setPosition(Point(trees1_x, trees1_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(128);
			this->addChild(img5, 5);

			//수분과 온도에따른 나무의 형태 변화
			if (wat1 >= 61 && wat1 <= 100 && tem1 >= 61 && tem1 <= 100)  //아열대 나무
				img5->setTexture("img/tree2.png");
			if (wat1 >= 0 && wat1 <= 29 && tem1 >= 61 && tem1 <= 100) //야자나무
				img5->setTexture("img/tree3.png");
			if (tem1 >= 0 && tem1 <= 29) //얼음나무
				img5->setTexture("img/tree4.png");
		}
		//2번 나무
		if (trees >= 2)
		{
			int wat2 = UserDefault::getInstance()->getIntegerForKey("trees_wat2");
			int tem2 = UserDefault::getInstance()->getIntegerForKey("trees_tem2");
			int trees2_x = UserDefault::getInstance()->getIntegerForKey("trees2_x", 0);
			int trees2_y = UserDefault::getInstance()->getIntegerForKey("trees2_y", 0);
			auto img5 = Sprite::create("img/tree.png");
			img5->setPosition(Point(trees2_x, trees2_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(128);
			this->addChild(img5, 5);

			//수분과 온도에따른 나무의 형태 변화
			if (wat2 >= 61 && wat2 <= 100 && tem2 >= 61 && tem2 <= 100)  //아열대 나무
				img5->setTexture("img/tree2.png");
			if (wat2 >= 0 && wat2 <= 29 && tem2 >= 61 && tem2 <= 100) //야자나무
				img5->setTexture("img/tree3.png");
			if (tem2 >= 0 && tem2 <= 29) //얼음나무
				img5->setTexture("img/tree4.png");
		}

		//1번 유채화
		if (flower >= 1)
		{
			int flower1_x = UserDefault::getInstance()->getIntegerForKey("flower1_x", 0);
			int flower1_y = UserDefault::getInstance()->getIntegerForKey("flower1_y", 0);
			auto img5 = Sprite::create("img/flower.png");
			img5->setPosition(Point(flower1_x, flower1_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 4);
		}
		//2번 유채화
		if (flower >= 2)
		{
			int flower2_x = UserDefault::getInstance()->getIntegerForKey("flower2_x", 0);
			int flower2_y = UserDefault::getInstance()->getIntegerForKey("flower2_y", 0);
			auto img5 = Sprite::create("img/flower.png");
			img5->setPosition(Point(flower2_x, flower2_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 4);
		}
		//3번 유채화
		if (flower >= 3)
		{
			int flower3_x = UserDefault::getInstance()->getIntegerForKey("flower3_x", 0);
			int flower3_y = UserDefault::getInstance()->getIntegerForKey("flower3_y", 0);
			auto img5 = Sprite::create("img/flower.png");
			img5->setPosition(Point(flower3_x, flower3_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 4);
		}
		//1번 구름
		if (cloud >= 1)
		{
			int cloud1_x = UserDefault::getInstance()->getIntegerForKey("cloud1_x", 0);
			int cloud1_y = UserDefault::getInstance()->getIntegerForKey("cloud1_y", 0);
			auto img5 = Sprite::create("img/cloud.png");
			img5->setPosition(Point(cloud1_x, cloud1_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 7);
		}
		//2번 구름
		if (cloud >= 2)
		{
			int cloud2_x = UserDefault::getInstance()->getIntegerForKey("cloud2_x", 0);
			int cloud2_y = UserDefault::getInstance()->getIntegerForKey("cloud2_y", 0);
			auto img5 = Sprite::create("img/cloud.png");
			img5->setPosition(Point(cloud2_x, cloud2_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 7);
		}
		//3번 구름
		if (cloud >= 3)
		{
			int cloud3_x = UserDefault::getInstance()->getIntegerForKey("cloud3_x", 0);
			int cloud3_y = UserDefault::getInstance()->getIntegerForKey("cloud3_y", 0);
			auto img5 = Sprite::create("img/cloud.png");
			img5->setPosition(Point(cloud3_x, cloud3_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 7);
		}
		//1번 눈사람
		if (snowman >= 1)
		{
			int snowman1_x = UserDefault::getInstance()->getIntegerForKey("snowman1_x", 0);
			int snowman1_y = UserDefault::getInstance()->getIntegerForKey("snowman1_y", 0);
			auto img5 = Sprite::create("img/snowman.png");
			img5->setPosition(Point(snowman1_x, snowman1_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 4);
		}
		//2번 눈사람
		if (snowman >= 2)
		{
			int snowman2_x = UserDefault::getInstance()->getIntegerForKey("snowman2_x", 0);
			int snowman2_y = UserDefault::getInstance()->getIntegerForKey("snowman2_y", 0);
			auto img5 = Sprite::create("img/snowman.png");
			img5->setPosition(Point(snowman2_x, snowman2_y));
			img5->setAnchorPoint(Point(0, 1));
			img5->setOpacity(160);
			this->addChild(img5, 4);
		}
}

//니티들의 모든것 초기화의 시점(&&*)  $$*
void inlife::init_nity()
{
	//니티들의 이미지 초기화
	int i;
	float x1, y1, x2, y2, x3, y3;
	int first = UserDefault::getInstance()->getIntegerForKey("first", 0);
	int second = UserDefault::getInstance()->getIntegerForKey("second", 0);
	int third = UserDefault::getInstance()->getIntegerForKey("third", 0);
	int fourth = UserDefault::getInstance()->getIntegerForKey("fourth", 0);
	int fifth = UserDefault::getInstance()->getIntegerForKey("fifth", 0);
	int first_lv = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int third_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");

	auto spr1 = Sprite::create("img/blank.png");
	auto spr2 = Sprite::create("img/blank.png");
	auto spr3 = Sprite::create("img/blank.png");

	//첫번째 니티의  이미지 처리 (&&*)
	x1 = (rand() % ((int)winSize.width-60) + 20);
	y1 = (rand() % ((int)winSize.height-60) +20);

			if (first == 1)
				spr1->setTexture("img/sla_blue_d1.png");
			else if (first == 2)
				spr1->setTexture("img/sla_pink_d1.png");
			else if (first == 3)
				spr1->setTexture("img/sla_green_d1.png");
			else if (first == 4 && first_lv <7)
				spr1->setTexture("img/lity_d1.png");
			else if (first == 4)
				spr1->setTexture("img/lityw1_d1.png");

				spr1->setPosition(Point(x1, y1));
				this->addChild(spr1, 1, "nity1");


	  //두번째 니티의  이미지 처리
				if (second != 0)
				{
					x2 = (rand() % ((int)winSize.width - 60) + 20);
					y2 = (rand() % ((int)winSize.height - 60) + 20);

					if (second == 1)
						spr2->setTexture("img/sla_blue_d1.png");
					else if (second == 2)
						spr2->setTexture("img/sla_pink_d1.png");
					else if (second == 3)
						spr2->setTexture("img/sla_green_d1.png");
					else if (second == 4 && second_lv < 7)
						spr2->setTexture("img/lity_d1.png");
					else if (second == 4)
						spr2->setTexture("img/lityw1_d1.png");


					spr2->setPosition(Point(x2, y2));
					this->addChild(spr2, 1, "nity2");
				}

	  //세번째 니티의  이미지 처리
				if (third != 0 )
				{
					x3 = (rand() % ((int)winSize.width - 60) + 20);
					y3 = (rand() % ((int)winSize.height - 60) + 20);

					if (third == 1)
						spr3->setTexture("img/sla_blue_d1.png");
					else if (third == 2)
						spr3->setTexture("img/sla_pink_d1.png");
					else if (third == 3)
						spr3->setTexture("img/sla_green_d1.png");
					else if (third == 4 && third_lv < 7)
						spr3->setTexture("img/lity_d1.png");
					else if (third == 4)
						spr3->setTexture("img/lityw1_d1.png");

					spr3->setPosition(Point(x3, y3));
					this->addChild(spr3, 1, "nity3");
				}

			//니티들의 모션 초기화
		   this->schedule(schedule_selector(inlife::move), 1.2f); // 스케쥴 메소드는 몇초 간격으로 함수를 실행시킬수 있다.
}

//터치이벤트의 처리 (&&*)  $$*
bool inlife::onTouchBegan(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation(); // 터치했을 경우 그 좌표를 location이라는 변수에 저장한다.

	//터치 할 경우 이펙트효과
	auto apr1 = ParticleSystemQuad::create("par/touch.plist");
	apr1->setOpacity(60);
	apr1->setScale(0.5);
	apr1->setPosition(location);
	this->addChild(apr1, 250);

	int food = UserDefault::getInstance()->getIntegerForKey("food");
	int first = UserDefault::getInstance()->getIntegerForKey("first", 0);
	int second = UserDefault::getInstance()->getIntegerForKey("second", 0);
	int third = UserDefault::getInstance()->getIntegerForKey("third", 0);
	int first_lv = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int third_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");

	if (first == 1)
		first_img = "img/sla_blue_d1.png";
	else if (first == 2)
		first_img = "img/sla_pink_d1.png";
	else if (first == 3)
		first_img = "img/sla_green_d1.png";
	else if (first == 4 && first_lv <7)
		first_img = "img/lity_d1.png";
	else if (first == 4)
		first_img = "img/lityw1_d1.png";

	if (second == 1)
		second_img = "img/sla_blue_d1.png";
	else if (second == 2)
		second_img = "img/sla_pink_d1.png";
	else if (second == 3)
		second_img = "img/sla_green_d1.png";
	else if (second == 4 && second_lv <7)
		second_img = "img/lity_d1.png";
	else if (second == 4)
		second_img = "img/lityw1_d1.png";

	if (third == 1)
		third_img = "img/sla_blue_d1.png";
	else if (third == 2)
		third_img = "img/sla_pink_d1.png";
	else if (third == 3)
		third_img = "img/sla_green_d1.png";
	else if (third == 4 && third_lv <7)
		third_img = "img/lity_d1.png";
	else if (third == 4)
		third_img = "img/lityw1_d1.png";

	touch_make = UserDefault::getInstance()->getIntegerForKey("touch_make", 0);


	//니티의 확장 상태창에있는 이미지를 터치하는경우
	if (onstate2 != 0)
	{
		auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
		Rect rect_nity_pic = nity_pic->getBoundingBox();

		if (rect_nity_pic.containsPoint(location))
		{

			int happy_to;
			happy_to = rand() % 100 + 1;

			if (happy_to <= 5)
			{

				sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sla.ogg");

				anno_slow2("img/ann_happy.png");

				if (eat_nity == 1)
				{
					int love = UserDefault::getInstance()->getIntegerForKey("first_love");
					if (love + 2 <= 100)
						love += 2;
					else
						love = 100;

					UserDefault::getInstance()->setIntegerForKey("first_love", love);
					UserDefault::getInstance()->flush();
				}
				else if (eat_nity == 2)
				{
					int love = UserDefault::getInstance()->getIntegerForKey("second_love");
					if (love + 2 <= 100)
						love += 2;
					else
						love = 100;

					UserDefault::getInstance()->setIntegerForKey("second_love", love);
					UserDefault::getInstance()->flush();
				}
				else if (eat_nity == 3)
				{
					int love = UserDefault::getInstance()->getIntegerForKey("third_love");
					if (love + 2 <= 100)
						love += 2;
					else
						love = 100;

					UserDefault::getInstance()->setIntegerForKey("third_love", love);
					UserDefault::getInstance()->flush();
				}
			}

		}

	}

	auto spr1 = (Sprite*)this->getChildByName("nity1");
	Rect rect_nity1 = spr1->getBoundingBox();

	int apple = UserDefault::getInstance()->getIntegerForKey("apple");
	int apple2 = UserDefault::getInstance()->getIntegerForKey("apple2");
	int apple3 = UserDefault::getInstance()->getIntegerForKey("apple3");

	if (touchenable == 0)
	{
		//<사과를 터치할경우>
		//사과 첫번째를를 터치할경우
		if (apple == 1)
		{
			auto apple_sp = (Sprite*)this->getChildByName("apples");
			Rect rect_apple1 = apple_sp->getBoundingBox();

			if (rect_apple1.containsPoint(location))
			{
				//사운드
				sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/save.ogg");

				auto app1 = FadeTo::create(2.5f, 0);
				auto app2 = MoveBy::create(2.5f, Point(0, 35));
				apple_sp->runAction(app1);
				apple_sp->runAction(app2);

				UserDefault::getInstance()->setIntegerForKey("food", food += 3);
				UserDefault::getInstance()->flush();

				UserDefault::getInstance()->setIntegerForKey("apple_time", time(NULL));
				UserDefault::getInstance()->flush();

				UserDefault::getInstance()->setIntegerForKey("apple", 0);
				UserDefault::getInstance()->flush();
			}
		}

			if (apple2 == 1)
			{
				auto apple_sp = (Sprite*)this->getChildByName("apples2");
				Rect rect_apple1 = apple_sp->getBoundingBox();

				if (rect_apple1.containsPoint(location))
				{
					//사운드
					sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
					if (sound2 == 0)
						float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/save.ogg");

					auto app1 = FadeTo::create(2.5f, 0);
					auto app2 = MoveBy::create(2.5f, Point(0, 35));
					apple_sp->runAction(app1);
					apple_sp->runAction(app2);

					UserDefault::getInstance()->setIntegerForKey("food", food += 3);
					UserDefault::getInstance()->flush();

					UserDefault::getInstance()->setIntegerForKey("apple_time2", time(NULL));
					UserDefault::getInstance()->flush();

					UserDefault::getInstance()->setIntegerForKey("apple2", 0);
					UserDefault::getInstance()->flush();
				}
			}
				if (apple3 == 1)
				{
					auto apple_sp = (Sprite*)this->getChildByName("apples3");
					Rect rect_apple1 = apple_sp->getBoundingBox();

					if (rect_apple1.containsPoint(location))
					{
						//사운드
						sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
						if (sound2 == 0)
							float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/save.ogg");

						auto app1 = FadeTo::create(2.5f, 0);
						auto app2 = MoveBy::create(2.5f, Point(0, 35));
						apple_sp->runAction(app1);
						apple_sp->runAction(app2);

						UserDefault::getInstance()->setIntegerForKey("food", food += 3);
						UserDefault::getInstance()->flush();

						UserDefault::getInstance()->setIntegerForKey("apple_time3", time(NULL));
						UserDefault::getInstance()->flush();

						UserDefault::getInstance()->setIntegerForKey("apple3", 0);
						UserDefault::getInstance()->flush();
					}
				}

		//첫번째 니티의 터치
		if (rect_nity1.containsPoint(location))  // 만약 터치된 좌표가 rect변수의 포인트 안에 포함되면, 실행한다.
		{
			if (sound2 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

			touchenable = 1;
			eat_nity = 1;
			auto item1 = MenuItemImage::create("img/img_state.png", "img/img_state.png", CC_CALLBACK_1(inlife::nothing, this));
			auto item2 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::close_state, this));
			auto item3 = MenuItemImage::create(first_img, first_img, CC_CALLBACK_1(inlife::select_food, this));
			auto item4 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_jump, this));
			auto item5 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_fly, this));
			auto item6 = MenuItemImage::create("img/icon_main.png", "img/icon_main_se.png", CC_CALLBACK_1(inlife::select_main, this));
			auto item7 = MenuItemImage::create("img/icon_status.png", "img/icon_status.png", CC_CALLBACK_1(inlife::nity_state2, this));

			auto menu = Menu::create(item1, item2, item3, item4, item5, item6, item7, NULL);
			item1->setPosition(0, 0);
			item2->setPosition(86, 87);
			item2->setScale(1.6);
			item3->setPosition(-68, 25);
			item3->setScale(1.2);
			item4->setPosition(65, 28);
			item5->setPosition(65, 5);
			item6->setPosition(-68, -14);
			item7->setPosition(110, 50);
			this->addChild(menu, 11, "state1");

			int level = UserDefault::getInstance()->getIntegerForKey("first_lv");
			float jump = UserDefault::getInstance()->getFloatForKey("first_jump");
			float fly = UserDefault::getInstance()->getFloatForKey("first_fly");
			int stat = UserDefault::getInstance()->getIntegerForKey("first_stat");
			int food = UserDefault::getInstance()->getIntegerForKey("first_food");
			int exp = UserDefault::getInstance()->getIntegerForKey("first_exp");
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");

			auto label_lv = Label::createWithSystemFont(StringUtils::format("%d", level), "Arial", 13);
			label_lv->setAnchorPoint(Point(0, 1));
			label_lv->setPosition(Point(260, 217));
			this->addChild(label_lv, 12, "state2");

			auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
			label_ju->setAnchorPoint(Point(0, 1));
			label_ju->setPosition(Point(260, 195));
			this->addChild(label_ju, 12, "state3");

			auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
			label_fly->setAnchorPoint(Point(0, 1));
			label_fly->setPosition(Point(260, 173));
			this->addChild(label_fly, 12, "state4");


			auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
			label_st->setAnchorPoint(Point(0, 1));
			label_st->setPosition(Point(260, 151));
			this->addChild(label_st, 12, "state5");

			auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
			food_bar->setPosition(193, 124);
			food_bar->setAnchorPoint(Point(0, 1));
			this->addChild(food_bar, 12, "state6");

			auto exp_bar = CCSprite::create("img/exp_bar.png", CCRectMake(0, 0, (133 * exp) / exp_max1, 13));
			exp_bar->setPosition(193, 88);
			exp_bar->setAnchorPoint(Point(0, 1));
			this->addChild(exp_bar, 12, "state7");

			auto love_bar = CCSprite::create("img/love_bar.png", CCRectMake(0, 0, (133 * love) / food_max, 13));
			love_bar->setPosition(193, 54);
			love_bar->setAnchorPoint(Point(0, 1));
			this->addChild(love_bar, 12, "state_love");

			auto label_no = Label::createWithSystemFont(StringUtils::format("no.1"), "Arial", 15);
			label_no->setAnchorPoint(Point(0, 1));
			label_no->setPosition(Point(140, 255));
			this->addChild(label_no, 12, "state8");

			int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
			if (main_nity == eat_nity)
			{
				auto label_main = Sprite::create("img/img_main.png");
				label_main->setAnchorPoint(Point(0, 1));
				label_main->setPosition(Point(155, 230));
				this->addChild(label_main, 12, "state9");
			}
		}

		//두번째 니티의 터치
		if (second != 0)
		{
			auto spr2 = (Sprite*)this->getChildByName("nity2");
			Rect rect_nity2 = spr2->getBoundingBox();

			if (rect_nity2.containsPoint(location))  // 만약 터치된 좌표가 rect변수의 포인트 안에 포함되면, 실행한다.
			{
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

				touchenable = 1;
				eat_nity = 2;
				auto item1 = MenuItemImage::create("img/img_state.png", "img/img_state.png", CC_CALLBACK_1(inlife::nothing, this));
				auto item2 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit.png", CC_CALLBACK_1(inlife::close_state, this));
				auto item3 = MenuItemImage::create(second_img, second_img, CC_CALLBACK_1(inlife::select_food, this));
				auto item4 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_jump, this));
				auto item5 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_fly, this));
				auto item6 = MenuItemImage::create("img/icon_main.png", "img/icon_main_se.png", CC_CALLBACK_1(inlife::select_main, this));
				auto item7 = MenuItemImage::create("img/icon_status.png", "img/icon_status_se.png", CC_CALLBACK_1(inlife::nity_state2, this));

				auto menu = Menu::create(item1, item2, item3, item4, item5, item6, item7, NULL);
				item1->setPosition(0, 0);
				item2->setPosition(86, 87);
				item2->setScale(1.6);
				item3->setPosition(-68, 25);
				item3->setScale(1.2);
				item4->setPosition(65, 28);
				item5->setPosition(65, 5);
				item6->setPosition(-68, -14);
				item7->setPosition(110, 50);
				this->addChild(menu, 11, "state1");

				int level = UserDefault::getInstance()->getIntegerForKey("second_lv");
				float jump = UserDefault::getInstance()->getFloatForKey("second_jump");
				float fly = UserDefault::getInstance()->getFloatForKey("second_fly");
				int stat = UserDefault::getInstance()->getIntegerForKey("second_stat");
				int food = UserDefault::getInstance()->getIntegerForKey("second_food");
				int exp = UserDefault::getInstance()->getIntegerForKey("second_exp");
				int love = UserDefault::getInstance()->getIntegerForKey("second_love");

				auto label_lv = Label::createWithSystemFont(StringUtils::format("%d", level), "Arial", 13);
				label_lv->setAnchorPoint(Point(0, 1));
				label_lv->setPosition(Point(260, 217));
				this->addChild(label_lv, 12, "state2");

				auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
				label_ju->setAnchorPoint(Point(0, 1));
				label_ju->setPosition(Point(260, 195));
				this->addChild(label_ju, 12, "state3");

				auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
				label_fly->setAnchorPoint(Point(0, 1));
				label_fly->setPosition(Point(260, 173));
				this->addChild(label_fly, 12, "state4");


				auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
				label_st->setAnchorPoint(Point(0, 1));
				label_st->setPosition(Point(260, 151));
				this->addChild(label_st, 12, "state5");

				auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
				food_bar->setPosition(193, 124);
				food_bar->setAnchorPoint(Point(0, 1));
				this->addChild(food_bar, 12, "state6");

				auto exp_bar = CCSprite::create("img/exp_bar.png", CCRectMake(0, 0, (133 * exp) / exp_max2, 13));
				exp_bar->setPosition(193, 88);
				exp_bar->setAnchorPoint(Point(0, 1));
				this->addChild(exp_bar, 12, "state7");

				auto love_bar = CCSprite::create("img/love_bar.png", CCRectMake(0, 0, (133 * love) / food_max, 13));
				love_bar->setPosition(193, 54);
				love_bar->setAnchorPoint(Point(0, 1));
				this->addChild(love_bar, 12, "state_love");

				auto label_no = Label::createWithSystemFont(StringUtils::format("no.2"), "Arial", 15);
				label_no->setAnchorPoint(Point(0, 1));
				label_no->setPosition(Point(140, 255));
				this->addChild(label_no, 12, "state8");

				int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
				if (main_nity == eat_nity)
				{
					int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
					if (main_nity == eat_nity)
					{
						auto label_main = Sprite::create("img/img_main.png");
						label_main->setAnchorPoint(Point(0, 1));
						label_main->setPosition(Point(155, 230));
						this->addChild(label_main, 12, "state9");
					}
				}
			}
		}


		//세번째 니티의 터치
		if (third != 0)
		{
			auto spr3 = (Sprite*)this->getChildByName("nity3");
			Rect rect_nity3 = spr3->getBoundingBox();

			if (rect_nity3.containsPoint(location))  // 만약 터치된 좌표가 rect변수의 포인트 안에 포함되면, 실행한다.
			{
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

				touchenable = 1;
				eat_nity = 3;
				auto item1 = MenuItemImage::create("img/img_state.png", "img/img_state.png", CC_CALLBACK_1(inlife::nothing, this));
				auto item2 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit.png", CC_CALLBACK_1(inlife::close_state, this));
				auto item3 = MenuItemImage::create(third_img, third_img, CC_CALLBACK_1(inlife::select_food, this));
				auto item4 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_jump, this));
				auto item5 = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::select_fly, this));
				auto item6 = MenuItemImage::create("img/icon_main.png", "img/icon_main_se.png", CC_CALLBACK_1(inlife::select_main, this));
				auto item7 = MenuItemImage::create("img/icon_status.png", "img/icon_status_se.png", CC_CALLBACK_1(inlife::nity_state2, this));

				auto menu = Menu::create(item1, item2, item3, item4, item5, item6, item7, NULL);
				item1->setPosition(0, 0);
				item2->setPosition(86, 87);
				item2->setScale(1.6);
				item3->setPosition(-68, 25);
				item3->setScale(1.2);
				item4->setPosition(65, 28);
				item5->setPosition(65, 5);
				item6->setPosition(-68, -14);
				item7->setPosition(110, 50);
				this->addChild(menu, 11, "state1");

				int level = UserDefault::getInstance()->getIntegerForKey("third_lv");
				float jump = UserDefault::getInstance()->getFloatForKey("third_jump");
				float fly = UserDefault::getInstance()->getFloatForKey("third_fly");
				int stat = UserDefault::getInstance()->getIntegerForKey("third_stat");
				int food = UserDefault::getInstance()->getIntegerForKey("third_food");
				int exp = UserDefault::getInstance()->getIntegerForKey("third_exp");
				int love = UserDefault::getInstance()->getIntegerForKey("third_love");

				auto label_lv = Label::createWithSystemFont(StringUtils::format("%d", level), "Arial", 13);
				label_lv->setAnchorPoint(Point(0, 1));
				label_lv->setPosition(Point(260, 217));
				this->addChild(label_lv, 12, "state2");

				auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
				label_ju->setAnchorPoint(Point(0, 1));
				label_ju->setPosition(Point(260, 195));
				this->addChild(label_ju, 12, "state3");

				auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
				label_fly->setAnchorPoint(Point(0, 1));
				label_fly->setPosition(Point(260, 173));
				this->addChild(label_fly, 12, "state4");


				auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
				label_st->setAnchorPoint(Point(0, 1));
				label_st->setPosition(Point(260, 151));
				this->addChild(label_st, 12, "state5");

				auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
				food_bar->setPosition(193, 124);
				food_bar->setAnchorPoint(Point(0, 1));
				this->addChild(food_bar, 12, "state6");

				auto exp_bar = CCSprite::create("img/exp_bar.png", CCRectMake(0, 0, (133 * exp) / exp_max3, 13));
				exp_bar->setPosition(193, 88);
				exp_bar->setAnchorPoint(Point(0, 1));
				this->addChild(exp_bar, 12, "state7");

				auto love_bar = CCSprite::create("img/love_bar.png", CCRectMake(0, 0, (133 * love) / food_max, 13));
				love_bar->setPosition(193, 54);
				love_bar->setAnchorPoint(Point(0, 1));
				this->addChild(love_bar, 12, "state_love");

				auto label_no = Label::createWithSystemFont(StringUtils::format("no.3"), "Arial", 15);
				label_no->setAnchorPoint(Point(0, 1));
				label_no->setPosition(Point(140, 255));
				this->addChild(label_no, 12, "state8");

				int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
				if (main_nity == eat_nity)
				{
					int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
					if (main_nity == eat_nity)
					{
						auto label_main = Sprite::create("img/img_main.png");
						label_main->setAnchorPoint(Point(0, 1));
						label_main->setPosition(Point(155, 230));
						this->addChild(label_main, 12, "state9");
					}
				}
			}
		}

	}

	return true;
}

//메인 니티를 선정하는 경우
void inlife::select_main(Ref* pSender)
{
	//사운드처리
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/save.ogg");

	if (eat_nity == 1)
	{
		UserDefault::getInstance()->setIntegerForKey("main_nity", 1);
		UserDefault::getInstance()->flush();
	}
	else if (eat_nity == 2)
	{
		UserDefault::getInstance()->setIntegerForKey("main_nity", 2);
		UserDefault::getInstance()->flush();
	}
	else if (eat_nity == 3)
	{
		UserDefault::getInstance()->setIntegerForKey("main_nity", 3);
		UserDefault::getInstance()->flush();
	}

	anno_slow2("img/ann_mainch.png");
	init_foodgage();
}


//<스텟 증가 시스템>
//상태창에서 니티의 점프스텟 증가를 누른경우
void inlife::select_jump(Ref* pSender)
{
	//먹이주기 창이 떠잇지 않은경우
	if (onstat == 0)
	{
		//먹이주기 창이 중복으로 뜨는것을 방지하기위한 변수
		onstat = 1;
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

		//알림 메세지 표시
		anno_slow("img/ann_stat1.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(inlife::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(inlife::stat_jump, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(inlife::exit_jump, this));

		auto menu = Menu::create(item1, item2, item3, NULL);
		item1->setPosition(0, 0);
		item2->setPosition(-30, 5);
		item3->setPosition(25, 5);
		this->addChild(menu, 15, "sel_food");
	}
}

//상태창에서 니티의 비행스텟 증가를 누른경우
void inlife::select_fly(Ref* pSender)
{
	//먹이주기 창이 떠잇지 않은경우
	if (onstat == 0)
	{
		//먹이주기 창이 중복으로 뜨는것을 방지하기위한 변수
		onstat = 1;
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

		//알림 메세지 표시
		anno_slow("img/ann_stat1.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(inlife::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(inlife::stat_fly, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(inlife::exit_fly, this));

		auto menu = Menu::create(item1, item2, item3, NULL);
		item1->setPosition(0, 0);
		item2->setPosition(-30, 5);
		item3->setPosition(25, 5);
		this->addChild(menu, 15, "sel_food");
	}
}

//점프스텟 증가 "예"   (&&*)
void inlife::stat_jump(Ref* pSender)
{
	onstat = 0;

	//사운드 효과
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int stat1 = UserDefault::getInstance()->getIntegerForKey("first_stat");
	int stat2 = UserDefault::getInstance()->getIntegerForKey("second_stat");
	int stat3 = UserDefault::getInstance()->getIntegerForKey("third_stat");

	float jump1 = UserDefault::getInstance()->getFloatForKey("first_jump");
	float jump2 = UserDefault::getInstance()->getFloatForKey("second_jump");
	float jump3 = UserDefault::getInstance()->getFloatForKey("third_jump");

		//첫번째 니티의 점프 스텟을 증가시키는 경우
		if (eat_nity == 1)
		{
			if (stat1 >= 1)
			{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 점프력 증가
				float y = (rand() % 100 + 1);
				y /= 100;

				UserDefault::getInstance()->setFloatForKey("first_jump", jump1+y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat1;
				UserDefault::getInstance()->setIntegerForKey("first_stat", stat1);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
		}
			// 스텟이 부족한경우
			else
			{
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
				anno_slow("img/ann_stat3.png");

			}
	}


		//두번째 니티의 점프 스텟을 증가시키는 경우
		else if (eat_nity == 2)
		{
			if (stat2 >= 1)
			{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 점프력 증가
				float y = (rand() % 100 + 1);
				y /= 100;

				UserDefault::getInstance()->setFloatForKey("second_jump", jump2 + y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat2;
				UserDefault::getInstance()->setIntegerForKey("second_stat", stat2);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
		}
			// 스텟이 부족한경우
			else
			{
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
				anno_slow("img/ann_stat3.png");

			}
		}

			//세번째 니티의 점프 스텟을 증가시키는 경우
			else if (eat_nity == 3)
			{
				if (stat3 >= 1)
				{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 점프력 증가
				float y = (rand() % 100 + 1);
				y /= 100;

				UserDefault::getInstance()->setFloatForKey("third_jump", jump3 + y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat3;
				UserDefault::getInstance()->setIntegerForKey("third_stat", stat3);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
			}
				// 스텟이 부족한경우
				else
				{
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);
					anno_slow("img/ann_stat3.png");

				}
		}

}

//비행스텟 증가 "예"   (&&*)
void inlife::stat_fly(Ref* pSender)
{
	onstat = 0;

	//사운드 효과
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	int stat1 = UserDefault::getInstance()->getIntegerForKey("first_stat");
	int stat2 = UserDefault::getInstance()->getIntegerForKey("second_stat");
	int stat3 = UserDefault::getInstance()->getIntegerForKey("third_stat");

	float fly1 = UserDefault::getInstance()->getFloatForKey("first_fly");
	float fly2 = UserDefault::getInstance()->getFloatForKey("second_fly");
	float fly3 = UserDefault::getInstance()->getFloatForKey("third_fly");

		//첫번째 니티의 비행 스텟을 증가시키는 경우
		if (eat_nity == 1)
		{
			if (stat1 >= 1)
			{
			if (fly1 != 0)
			{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 비행력 증가
				float y = (rand() % 100 + 1);
				y /= 100;
				y += 3;

				UserDefault::getInstance()->setFloatForKey("first_fly", fly1 + y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat1;
				UserDefault::getInstance()->setIntegerForKey("first_stat", stat1);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
			}
			else
			{
				anno_slow("img/ann_stat4.png");
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
			}
		}
			// 스텟이 부족한경우
			else
			{
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
				anno_slow("img/ann_stat3.png");

			}
	}

		//두번째 니티의 비행 스텟을 증가시키는 경우
		else if (eat_nity == 2)
		{
			if (stat2 >= 1)
			{
			if (fly2 != 0)
			{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 비행력 증가
				float y = (rand() % 100 + 1);
				y /= 100;
				y += 3;

				UserDefault::getInstance()->setFloatForKey("second_fly", fly2 + y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat2;
				UserDefault::getInstance()->setIntegerForKey("second_stat", stat2);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
			}
			else
			{
				anno_slow("img/ann_stat4.png");
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
			}
		}
			// 스텟이 부족한경우
			else
			{
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
				anno_slow("img/ann_stat3.png");
			}
	}

		//세번째 니티의 비행 스텟을 증가시키는 경우
		else if (eat_nity == 3)
		{
			if (stat3 >= 1)
			{
			if (fly3 != 0)
			{
				//선택지 삭제
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);

				//니티의 비행력 증가
				float y = (rand() % 100 + 1);
				y /= 100;
				y += 3;

				UserDefault::getInstance()->setFloatForKey("third_fly", fly3 + y);
				UserDefault::getInstance()->flush();

				//알림메세지 표시
				anno_slow2("img/ann_stat2.png");

				//사운드 효과
				if (sound2 == 0)
					float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.ogg");

				//스텟의 감소
				--stat3;
				UserDefault::getInstance()->setIntegerForKey("third_stat", stat3);
				UserDefault::getInstance()->flush();

				//스텟창의 닫기
				init_foodgage();
			}
			else
			{
				anno_slow("img/ann_stat4.png");
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
			}
		}
			// 스텟이 부족한경우
			else
			{
				auto state1 = (Sprite*)this->getChildByName("sel_food");
				this->removeChild(state1);
				anno_slow("img/ann_stat3.png");

			}
	}

}

//점프 증가 선택지에서 "아니요"
void inlife::exit_jump(Ref* pSender)
{
	onstat = 0;
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state1 = (Sprite*)this->getChildByName("sel_food");
	this->removeChild(state1);
}

//비행 증가 선택지에서 "아니요"
void inlife::exit_fly(Ref* pSender)
{
	onstat = 0;
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state1 = (Sprite*)this->getChildByName("sel_food");
	this->removeChild(state1);
}




//상태창에서 니티의 이미지를 누른경우
void inlife::select_food(Ref* pSender)
{
	//먹이주기 창이 떠잇지 않은경우
	if (onfood == 0)
	{
		//먹이주기 창이 중복으로 뜨는것을 방지하기위한 변수
		onfood = 1;
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

		//알림 메세지 표시
		anno_slow("img/ann_food1.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(inlife::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(inlife::give_food, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(inlife::exit_food, this));

		auto menu = Menu::create(item1, item2, item3, NULL);
		item1->setPosition(0, 0);
		item2->setPosition(-30, 5);
		item3->setPosition(25, 5);
		this->addChild(menu, 15, "sel_food");
	}
}

//먹이 먹이기 선택지에서 "예"  (&&*)
void inlife::give_food(Ref* pSender)
{
	onfood = 0;

		int food_q = (rand() % 10) + 11; // 증가하는 랜덤 포만감
		int foods = UserDefault::getInstance()->getIntegerForKey("food"); //소유하고 있는 먹이량

		if (foods >= 1)
		{
			//첫번째 니티의 먹이창에서 먹이를 주는경우
			if (eat_nity == 1)
			{
				int fir_food = UserDefault::getInstance()->getIntegerForKey("first_food");

				//먹이의 최댓값 100을 넘지 않게 설정한다.
				if (fir_food + food_q <= 100)
				{
					//먹이를 줄때 사과, 메세지 , 이모션이 나타난다.
					apple_eat();

					//선택지 삭제
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);

					//니티의 포만감 증가
					UserDefault::getInstance()->setIntegerForKey("first_food", fir_food + food_q);
					UserDefault::getInstance()->flush();

					//게이지바의 재스프라이트
					init_foodgage();

					//사운드 효과
					if (sound2 == 0)
						float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/eat.ogg");

					--foods;
					UserDefault::getInstance()->setIntegerForKey("food", foods);
					UserDefault::getInstance()->flush();

					//먹이 먹은 타임을 저장
					UserDefault::getInstance()->setIntegerForKey("game_close_time", time(NULL));
					UserDefault::getInstance()->flush();
				}
				//먹이를 먹는다면 니티의 포만도가 100을 증가하게되는경우
				else
				{
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);
					anno_slow("img/ann_food4.png");
				}
			}

			//두번째 니티의 먹이창에서 먹이를 주는경우
			else if (eat_nity == 2)
			{
				int fir_food = UserDefault::getInstance()->getIntegerForKey("second_food");

				//먹이의 최댓값 100을 넘지 않게 설정한다.
				if (fir_food + food_q <= 100) //이거 바꾸지 않아도 됨
				{
					apple_eat();

					//선택지 삭제
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);

					//니티의 포만감 증가
					UserDefault::getInstance()->setIntegerForKey("second_food", fir_food + food_q);
					UserDefault::getInstance()->flush();

					//게이지바의 재스프라이트
					init_foodgage();

					//사운드 효과
					if (sound2 == 0)
						float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/eat.ogg");

					--foods;
					UserDefault::getInstance()->setIntegerForKey("food", foods);
					UserDefault::getInstance()->flush();

					UserDefault::getInstance()->setIntegerForKey("game_close_time", time(NULL));
					UserDefault::getInstance()->flush();
				}
				//먹이를 먹는다면 니티의 포만도가 100을 증가하게되는경우
				else
				{
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);
					anno_slow("img/ann_food4.png");
				}
			}

			//세번째 니티의 먹이창에서 먹이를 주는경우
			else if (eat_nity == 3)
			{
				int fir_food = UserDefault::getInstance()->getIntegerForKey("third_food");

				//먹이의 최댓값 100을 넘지 않게 설정한다.
				if (fir_food + food_q <= 100) //이거 바꾸지 않아도 됨
				{
					apple_eat();

					//선택지 삭제
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);

					//니티의 포만감 증가
					UserDefault::getInstance()->setIntegerForKey("third_food", fir_food + food_q);
					UserDefault::getInstance()->flush();

					//게이지바의 재스프라이트
					init_foodgage();

					//사운드 효과
					if (sound2 == 0)
						float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/eat.ogg");

					--foods;
					UserDefault::getInstance()->setIntegerForKey("food", foods);
					UserDefault::getInstance()->flush();

					UserDefault::getInstance()->setIntegerForKey("game_close_time", time(NULL));
					UserDefault::getInstance()->flush();
				}
				//먹이를 먹는다면 니티의 포만도가 100을 증가하게되는경우
				else
				{
					auto state1 = (Sprite*)this->getChildByName("sel_food");
					this->removeChild(state1);
					anno_slow("img/ann_food4.png");
				}
			}


		}
		//먹이가 부족한경우
		else
		{
			auto state1 = (Sprite*)this->getChildByName("sel_food");
			this->removeChild(state1);
			anno_slow("img/ann_food3.png");
		}
}

//먹이먹이기 선택지에서 "아니요"
void inlife::exit_food(Ref* pSender)
{
	onfood = 0;
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto state1 = (Sprite*)this->getChildByName("sel_food");
	auto state2 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);
	this->removeChild(state2);
}

//포만도게이지와 스텟 스프라이트의 지속적인 업데이트 (&&*)
void inlife::init_foodgage()
{
	auto state3 = (Sprite*)this->getChildByName("state3");
	this->removeChild(state3);
	auto state4 = (Sprite*)this->getChildByName("state4");
	this->removeChild(state4);
	auto state5 = (Sprite*)this->getChildByName("state5");
	this->removeChild(state5);
	auto state6 = (Sprite*)this->getChildByName("state6");
	this->removeChild(state6);
	auto state9 = (Sprite*)this->getChildByName("state9");
	this->removeChild(state9);

	//첫번째 니티의 경우
	if (eat_nity == 1)
	{
		int food = UserDefault::getInstance()->getIntegerForKey("first_food");
		float jump = UserDefault::getInstance()->getFloatForKey("first_jump");
		float fly = UserDefault::getInstance()->getFloatForKey("first_fly");
		int stat = UserDefault::getInstance()->getIntegerForKey("first_stat");

		auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
		food_bar->setPosition(193, 124);
		food_bar->setAnchorPoint(Point(0, 1));
		this->addChild(food_bar, 12, "state6");

		auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
		label_ju->setAnchorPoint(Point(0, 1));
		label_ju->setPosition(Point(260, 195));
		this->addChild(label_ju, 12, "state3");

		auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
		label_fly->setAnchorPoint(Point(0, 1));
		label_fly->setPosition(Point(260, 173));
		this->addChild(label_fly, 12, "state4");

		auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
		label_st->setAnchorPoint(Point(0, 1));
		label_st->setPosition(Point(260, 151));
		this->addChild(label_st, 12, "state5");
	}

	//두번째 니티의 경우
	else if (eat_nity == 2)
	{
		int food = UserDefault::getInstance()->getIntegerForKey("second_food");
		float jump = UserDefault::getInstance()->getFloatForKey("second_jump");
		float fly = UserDefault::getInstance()->getFloatForKey("second_fly");
		int stat = UserDefault::getInstance()->getIntegerForKey("second_stat");

		auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
		food_bar->setPosition(193, 124);
		food_bar->setAnchorPoint(Point(0, 1));
		this->addChild(food_bar, 12, "state6");

		auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
		label_ju->setAnchorPoint(Point(0, 1));
		label_ju->setPosition(Point(260, 195));
		this->addChild(label_ju, 12, "state3");

		auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
		label_fly->setAnchorPoint(Point(0, 1));
		label_fly->setPosition(Point(260, 173));
		this->addChild(label_fly, 12, "state4");

		auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
		label_st->setAnchorPoint(Point(0, 1));
		label_st->setPosition(Point(260, 151));
		this->addChild(label_st, 12, "state5");
	}

	//세번째 니티의 경우
	else if (eat_nity == 3)
	{
		int food = UserDefault::getInstance()->getIntegerForKey("third_food");
		float jump = UserDefault::getInstance()->getFloatForKey("third_jump");
		float fly = UserDefault::getInstance()->getFloatForKey("third_fly");
		int stat = UserDefault::getInstance()->getIntegerForKey("third_stat");

		auto food_bar = CCSprite::create("img/food_bar.png", CCRectMake(0, 0, (133 * food) / food_max, 13));
		food_bar->setPosition(193, 124);
		food_bar->setAnchorPoint(Point(0, 1));
		this->addChild(food_bar, 12, "state6");

		auto label_ju = Label::createWithSystemFont(StringUtils::format("%.2f", jump), "Arial", 13);
		label_ju->setAnchorPoint(Point(0, 1));
		label_ju->setPosition(Point(260, 195));
		this->addChild(label_ju, 12, "state3");

		auto label_fly = Label::createWithSystemFont(StringUtils::format("%.2f", fly), "Arial", 13);
		label_fly->setAnchorPoint(Point(0, 1));
		label_fly->setPosition(Point(260, 173));
		this->addChild(label_fly, 12, "state4");

		auto label_st = Label::createWithSystemFont(StringUtils::format("%d", stat), "Arial", 13);
		label_st->setAnchorPoint(Point(0, 1));
		label_st->setPosition(Point(260, 151));
		this->addChild(label_st, 12, "state5");

	}

	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
	if (main_nity == eat_nity)
	{
		int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
		if (main_nity == eat_nity)
		{
			auto label_main = Sprite::create("img/img_main.png");
			label_main->setAnchorPoint(Point(0, 1));
			label_main->setPosition(Point(155, 230));
			this->addChild(label_main, 12, "state9");
		}
	}
}

//먹이 줄때 사과 이미지가 니티에게 나타나며, 메세지와 이모션이 생성된다.
void inlife::apple_eat()
{
	int rand_emo;

	if (eat_nity == 1)
	{
		auto nity1 = (Sprite*)this->getChildByName("nity1");
		auto nity_po = nity1->getPosition();

		auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
		runAction(action2);

		auto apple = Sprite::create("img/icon_food.png");
		apple->setPosition(nity_po);
		this->addChild(apple, 3);

		auto action1 = FadeOut::create(2.0);
		apple->runAction(action1);

		//먹이를 줄경우 랜덤으로 메세지와 이모션을 나타낸다.
		rand_emo = rand() % 100+1;

		if (rand_emo <= 30)
			anno_slow2("img/ann_deli.png");
		else if (rand_emo <= 50)
		{
			anno_slow2("img/ann_happy.png");
			emotion_nity(1, 1);
		}

	}

	if (eat_nity == 2)
	{
		auto nity1 = (Sprite*)this->getChildByName("nity2");
		auto nity_po = nity1->getPosition();

		auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
		runAction(action2);

		auto apple = Sprite::create("img/icon_food.png");
		apple->setPosition(nity_po);
		this->addChild(apple, 3);

		auto action1 = FadeOut::create(2.0);
		apple->runAction(action1);

		//먹이를 줄경우 랜덤으로 메세지와 이모션을 나타낸다.
		rand_emo = rand() % 100 + 1;

		if (rand_emo <= 30)
			anno_slow2("img/ann_deli.png");
		else if (rand_emo <= 50)
		{
			anno_slow2("img/ann_happy.png");
			emotion_nity(2, 1);
		}
	}

	if (eat_nity == 3)
	{
		auto nity1 = (Sprite*)this->getChildByName("nity3");
		auto nity_po = nity1->getPosition();

		auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
		runAction(action2);

		auto apple = Sprite::create("img/icon_food.png");
		apple->setPosition(nity_po);
		this->addChild(apple, 3);

		auto action1 = FadeOut::create(2.0);
		apple->runAction(action1);

		//먹이를 줄경우 랜덤으로 메세지와 이모션을 나타낸다.
		rand_emo = rand() % 100 + 1;

		if (rand_emo <= 30)
			anno_slow2("img/ann_deli.png");
		else if (rand_emo <= 50)
		{
			anno_slow2("img/ann_happy.png");
			emotion_nity(3, 1);
		}
	}

}


//니티 그래픽의 움직임 (&&*)
void inlife::move(float delta)
{
	int max = UserDefault::getInstance()->getIntegerForKey("nities", 0);
	int first = UserDefault::getInstance()->getIntegerForKey("first", 0);
	int second = UserDefault::getInstance()->getIntegerForKey("second", 0);
	int third = UserDefault::getInstance()->getIntegerForKey("third", 0);
	float mo1 = (rand() % 4 + 1); // 첫번째 니티의 움직임 랜덤변수
	float mo2 = (rand() % 4 + 1);
	float mo3 = (rand() % 4 + 1);
	int x1, y1, x2, y2, x3, y3;
	//첫번째 니티의 액션
	auto actio1 = MoveBy::create(1.2, Point(26, 0)); //오른쪽으로 움직임
	auto actio2 = MoveBy::create(1.2, Point(-26, 0)); //왼쪽으로 움직임
	auto actio3 = MoveBy::create(1.2, Point(0, -21)); //위쪽으로 움직임
	auto actio4 = MoveBy::create(1.2, Point(0, 21)); //아래쪽으로 움직임

	//두번째 니티의 액션
	auto actio21 = MoveBy::create(1.2, Point(26, 0)); //오른쪽으로 움직임
	auto actio22 = MoveBy::create(1.2, Point(-26, 0)); //왼쪽으로 움직임
	auto actio23 = MoveBy::create(1.2, Point(0, -21)); //위쪽으로 움직임
	auto actio24 = MoveBy::create(1.2, Point(0, 21)); //아래쪽으로 움직임

	//세번째 니티의 액션
	auto actio31 = MoveBy::create(1.2, Point(26, 0)); //오른쪽으로 움직임
	auto actio32 = MoveBy::create(1.2, Point(-26, 0)); //왼쪽으로 움직임
	auto actio33 = MoveBy::create(1.2, Point(0, -21)); //위쪽으로 움직임
	auto actio34 = MoveBy::create(1.2, Point(0, 21)); //아래쪽으로 움직임

	//첫번째 니티의 움직임
	auto spr1 = (Sprite*)this->getChildByName("nity1");
	x1 = spr1->getPositionX();
	y1 = spr1->getPositionY();
	move_first = mo1;

	//니티가 돌아다니는 화면 제한크기 설정
	if (x1 <= 60){
		spr1->runAction(actio1);
		move_first = 1;}
	else if (x1 >= ((int)winSize.width - 60) + 20){
		spr1->runAction(actio2);
		move_first = 2;
	}
	else if (y1 <= 60)
	{
		spr1->runAction(actio4);
		move_first = 4;
	}
	else if (y1 >= ((int)winSize.height - 60) + 20)
	{
		spr1->runAction(actio3);
		move_first = 3;
	}
	else {
		if (move_first == 1)
			spr1->runAction(actio1);
		else if (move_first == 2)
			spr1->runAction(actio2);
		else if (move_first == 3)
			spr1->runAction(actio3);
		else if (move_first == 4)
			spr1->runAction(actio4);
	}


	//두번째 니티의 움직임
	if (second != 0 )
	{
		auto spr2 = (Sprite*)this->getChildByName("nity2");
		x2 = spr2->getPositionX();
		y2 = spr2->getPositionY();
		move_second = mo2;

		//니티가 돌아다니는 화면 제한크기 설정
		if (x2 <= 60){
			spr2->runAction(actio21);
			move_second = 1;
		}
		else if (x2 >= ((int)winSize.width - 60) + 20){
			spr2->runAction(actio22);
			move_second = 2;
		}
		else if (y2 <= 60)
		{
			spr2->runAction(actio24);
			move_second = 4;
		}
		else if (y2 >= ((int)winSize.height - 60) + 20)
		{
			spr2->runAction(actio23);
			move_second = 3;
		}
		else {
			if (move_second == 1)
				spr2->runAction(actio21);
			else if (move_second == 2)
				spr2->runAction(actio22);
			else if (move_second == 3)
				spr2->runAction(actio23);
			else if (move_second == 4)
				spr2->runAction(actio24);
		}
	}

	//세번째 니티의 움직임
	if (third != 0)
	{
		auto spr3 = (Sprite*)this->getChildByName("nity3");
		x3 = spr3->getPositionX();
		y3 = spr3->getPositionY();
		move_third = mo3;

		//니티가 돌아다니는 화면 제한크기 설정
		if (x3 <= 60){
			spr3->runAction(actio31);
			move_third = 1;
		}
		else if (x3 >= ((int)winSize.width - 60) + 20){
			spr3->runAction(actio32);
			move_third = 2;
		}
		else if (y3 <= 60)
		{
			spr3->runAction(actio34);
			move_third = 4;
		}
		else if (y3 >= ((int)winSize.height - 60) + 20)
		{
			spr3->runAction(actio33);
			move_third = 3;
		}
		else {
			if (move_third == 1)
				spr3->runAction(actio31);
			else if (move_third == 2)
				spr3->runAction(actio32);
			else if (move_third == 3)
				spr3->runAction(actio33);
			else if (move_third == 4)
				spr3->runAction(actio34);
		}
	}

	//니티가 움직거리는 상태
	staying();
}


//니티가 행동하는 애니매이션 (&&*)  $$*
void inlife::staying()
{
	auto spr1 = (Sprite*)this->getChildByName("nity1");
	auto spr2 = (Sprite*)this->getChildByName("nity2");
	auto spr3 = (Sprite*)this->getChildByName("nity3");
	int first = UserDefault::getInstance()->getIntegerForKey("first", 0);
	int second = UserDefault::getInstance()->getIntegerForKey("second", 0);
	int third = UserDefault::getInstance()->getIntegerForKey("third", 0);
	int max = UserDefault::getInstance()->getIntegerForKey("nities", 0);
	int first_lv = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int third_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.6);

	auto animation2 = Animation::create();
	animation2->setDelayPerUnit(0.6);

	auto animation3 = Animation::create();
	animation3->setDelayPerUnit(0.6);

	for (int i = 1; i<4; i++) {
		//<첫번째 니티>
        //니티가 오른쪽 방향을 보고 있는경우
		if (move_first == 1)
		{
			if (first == 1)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_r%d.png", i)->getCString());
			else if (first == 2)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_r%d.png", i)->getCString());
			else if (first == 3)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_r%d.png", i)->getCString());
			else if (first == 4 && first_lv <7)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (first == 4)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_r%d.png", i)->getCString());
		}
		//니티가 왼쪽 방향을 보고 있는경우
		else if (move_first == 2)
		{
			if (first == 1)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_l%d.png", i)->getCString());
			else if (first == 2)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_l%d.png", i)->getCString());
			else if (first == 3)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_l%d.png", i)->getCString());
			else if (first == 4 && first_lv <7)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (first == 4)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_l%d.png", i)->getCString());
		}
		//니티가 아래쪽 방향을 보고 있는경우
		else if (move_first == 3)
		{
			if (first == 1)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_d%d.png", i)->getCString());
			else if (first == 2)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_d%d.png", i)->getCString());
			else if (first == 3)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_d%d.png", i)->getCString());
			else if (first == 4 && first_lv <7)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (first == 4)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_d%d.png", i)->getCString());
		}
		//니티가 위쪽 방향을 보고 있는경우
		else if (move_first == 4)
		{
			if (first == 1)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_u%d.png", i)->getCString());
			else if (first == 2)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_u%d.png", i)->getCString());
			else if (first == 3)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_u%d.png", i)->getCString());
			else if (first == 4 && first_lv <7)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (first == 4)
				animation->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_u%d.png", i)->getCString());
		}


		//<두번째 니티>
		//니티가 오른쪽 방향을 보고 있는경우
		if (move_second == 1)
		{
			if (second == 1)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_r%d.png", i)->getCString());
			else if (second == 2)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_r%d.png", i)->getCString());
			else if (second == 3)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_r%d.png", i)->getCString());
			else if (second == 4 && second_lv <7)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (second == 4)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_r%d.png", i)->getCString());
		}
		//니티가 왼쪽 방향을 보고 있는경우
		else if (move_second == 2)
		{
			if (second == 1)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_l%d.png", i)->getCString());
			else if (second == 2)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_l%d.png", i)->getCString());
			else if (second == 3)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_l%d.png", i)->getCString());
			else if (second == 4 && second_lv <7)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (second == 4)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_l%d.png", i)->getCString());
		}
		//니티가 아래쪽 방향을 보고 있는경우
		else if (move_second == 3)
		{
			if (second == 1)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_d%d.png", i)->getCString());
			else if (second == 2)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_d%d.png", i)->getCString());
			else if (second == 3)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_d%d.png", i)->getCString());
			else if (second == 4 && second_lv <7)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (second == 4)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_d%d.png", i)->getCString());
		}
		//니티가 위쪽 방향을 보고 있는경우
		else if (move_second == 4)
		{
			if (second == 1)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_u%d.png", i)->getCString());
			else if (second == 2)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_u%d.png", i)->getCString());
			else if (second == 3)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_u%d.png", i)->getCString());
			else if (second == 4 && second_lv <7)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (second == 4)
				animation2->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_u%d.png", i)->getCString());
		}


		//<세번째 니티>
		//니티가 오른쪽 방향을 보고 있는경우
		if (move_third == 1)
		{
			if (third == 1)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_r%d.png", i)->getCString());
			else if (third == 2)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_r%d.png", i)->getCString());
			else if (third == 3)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_r%d.png", i)->getCString());
			else if (third == 4 && third_lv <7)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (third == 4)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_r%d.png", i)->getCString());
		}
		//니티가 왼쪽 방향을 보고 있는경우
		else if (move_third == 2)
		{
			if (third == 1)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_l%d.png", i)->getCString());
			else if (third == 2)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_l%d.png", i)->getCString());
			else if (third == 3)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_l%d.png", i)->getCString());
			else if (third == 4 && third_lv <7)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (third == 4)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_l%d.png", i)->getCString());
		}
		//니티가 아래쪽 방향을 보고 있는경우
		else if (move_third == 3)
		{
			if (third == 1)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_d%d.png", i)->getCString());
			else if (third == 2)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_d%d.png", i)->getCString());
			else if (third == 3)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_d%d.png", i)->getCString());
			else if (third == 4 && third_lv <7)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (third == 4)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_d%d.png", i)->getCString());
		}
		//니티가 위쪽 방향을 보고 있는경우
		else if (move_third == 4)
		{
			if (third == 1)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_u%d.png", i)->getCString());
			else if (third == 2)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_u%d.png", i)->getCString());
			else if (third == 3)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_u%d.png", i)->getCString());
			else if (third == 4 && third_lv <7)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
			else if (third == 4)
				animation3->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_u%d.png", i)->getCString());
		}
	}




	auto action_0 = Repeat::create(Animate::create(animation), 1);
	spr1->runAction(action_0);

	if (second != 0)
	{
		auto action_2 = Repeat::create(Animate::create(animation2), 1);
		spr2->runAction(action_2);
	}

	if (third != 0)
	{
		auto action_3 = Repeat::create(Animate::create(animation3), 1);
		spr3->runAction(action_3);
	}
}



//<이모션 기능>
//이모션 이미지 랜덤으로 정하기
void inlife::emotion(float delta)
{
	int first = UserDefault::getInstance()->getIntegerForKey("first");
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");

	int emo1 = (rand() % 100);
	int emo2 = (rand() % 100);
	int emo3 = (rand() % 100);

	//첫번째 니티에 대한 이모션
	if (first != 0 && emo_nity1 == 0)
	{
		if (emo1 <= 10)
		{
			auto spr1 = Sprite::create("img/emo_1.png");
			spr1->setPosition(-100, -100);
			this->addChild(spr1, 5, "emo_1");

			emo1 = 50;
			emo_nity1 = 1;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr1->runAction(action3);

			if (emotion_start==0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo1 <= 20 && emo1 > 10)
		{
			auto spr1 = Sprite::create("img/emo_2.png");
			spr1->setPosition(-100, -100);
			this->addChild(spr1, 5, "emo_1");

			emo1 = 50;
			emo_nity1 = 2;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr1->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo1 <= 30 && emo1 > 20)
		{
			auto spr1 = Sprite::create("img/emo_3.png");
			spr1->setPosition(-100, -100);
			this->addChild(spr1, 5, "emo_1");

			emo1 = 50;
			emo_nity1 = 3;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr1->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
	}

	//두번째 니티에 대한 이모션
	if (second != 0 && emo_nity2 == 0)
	{
		if (emo2 <= 10)
		{
			auto spr2 = Sprite::create("img/emo_1.png");
			spr2->setPosition(-100, -100);
			this->addChild(spr2, 5, "emo_2");

			emo2 = 50;
			emo_nity2 = 1;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr2->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo2 <= 20 && emo2 > 10)
		{
			auto spr2 = Sprite::create("img/emo_2.png");
			spr2->setPosition(-100, -100);
			this->addChild(spr2, 5, "emo_2");

			emo2 = 50;
			emo_nity2 = 2;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr2->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo2 <= 30 && emo2 > 20)
		{
			auto spr2 = Sprite::create("img/emo_3.png");
			spr2->setPosition(-100, -100);
			this->addChild(spr2, 5, "emo_2");

			emo2 = 50;
			emo_nity2 = 3;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr2->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
	}


	//세번째 니티에 대한 이모션
	if (third != 0 && emo_nity3 == 0)
	{
		if (emo3 <= 10)
		{
			auto spr3 = Sprite::create("img/emo_1.png");
			spr3->setPosition(-100, -100);
			this->addChild(spr3, 5, "emo_3");

			emo3 = 50;
			emo_nity3 = 1;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr3->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo3 <= 20 && emo3 > 10)
		{
			auto spr3 = Sprite::create("img/emo_2.png");
			spr3->setPosition(-100, -100);
			this->addChild(spr3, 5, "emo_3");

			emo3 = 50;
			emo_nity3 = 2;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr3->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
		else if (emo3 <= 30 && emo3 > 20)
		{
			auto spr3 = Sprite::create("img/emo_3.png");
			spr3->setPosition(-100, -100);
			this->addChild(spr3, 5, "emo_3");

			emo3 = 50;
			emo_nity3 = 3;

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr3->runAction(action3);

			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);
		}
	}
}

//이모션의 활성화
void inlife::emotion_active(float delta)
{
	emotion_start = 1;
	auto emo_1 = (Sprite*)this->getChildByName("emo_1");
	auto emo_2 = (Sprite*)this->getChildByName("emo_2");
	auto emo_3 = (Sprite*)this->getChildByName("emo_3");

	int first = UserDefault::getInstance()->getIntegerForKey("first");
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");

	if (emo_nity1 != 0)
	{
		auto nity1 = (Sprite*)this->getChildByName("nity1");
		float location_x1 = nity1->getPositionX();
		float location_y1 = nity1->getPositionY();
		emo_1->setPosition(location_x1 + 3, location_y1 + 17);
	}
	if (emo_nity2 != 0 && second != 0)
	{
		auto nity2 = (Sprite*)this->getChildByName("nity2");
		float location_x1 = nity2->getPositionX();
		float location_y1 = nity2->getPositionY();
		emo_2->setPosition(location_x1 + 3, location_y1 + 17);
	}
	if (emo_nity3 != 0 && third != 0)
	{
		auto nity3 = (Sprite*)this->getChildByName("nity3");
		float location_x1 = nity3->getPositionX();
		float location_y1 = nity3->getPositionY();
		emo_3->setPosition(location_x1 + 3, location_y1 + 17);
	}
}

//이모션의 삭제
void inlife::emotion_del()
{

	if (emo_nity1 != 0)
	{
		this->removeChildByName("emo_1"); //첫번째 니티 이모션 삭제
		emo_nity1 = 0;
	}
	else if (emo_nity2 != 0)
	{
		this->removeChildByName("emo_2"); //두번째 니티 이모션 삭제
		emo_nity2 = 0;
	}
	else if (emo_nity3 != 0)
	{
		this->removeChildByName("emo_3"); //세번째 니티 이모션 삭제
		emo_nity3 = 0;
	}

}

//이모션이 직접적으로 나타나게 한다.
void inlife::emotion_nity(int nity, int emo)
{
	//첫번째 니티에 대한 이모션
	if (emo_nity1 == 0 && nity == 1)
	{
			auto spr1 = Sprite::create("img/emo_3.png");
			spr1->setPosition(-100, -100);
			this->addChild(spr1, 5, "emo_1");

			//니티가 행복해하는 경우
			if (emo == 1)
			{
				int love = UserDefault::getInstance()->getIntegerForKey("first_love", 0);
				if (love + 5 <= 100)
				{
					love += 5;
					UserDefault::getInstance()->setIntegerForKey("first_love", love);
					UserDefault::getInstance()->flush();
				}
				else
				{
					UserDefault::getInstance()->setIntegerForKey("first_love", 100);
					UserDefault::getInstance()->flush();
				}
			}
			else if (emo == 2)
			{
				spr1->setTexture("img/emo_4.png");
			}

			emo_nity1 = 1;
			if (emotion_start == 0)
			this->schedule(schedule_selector(inlife::emotion_active), 0.01f);

			auto action1 = DelayTime::create(5.0);
			auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
			auto action3 = Sequence::create(action1, action2, NULL);
			spr1->runAction(action3);
	}

	//두번째 니티에 대한 이모션
	if (emo_nity2 == 0 && nity == 2)
	{
		auto spr1 = Sprite::create("img/emo_3.png");
		spr1->setPosition(-100, -100);
		this->addChild(spr1, 5, "emo_2");

		if (emo == 1) // 행복해 하는경우의 이모션
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love", 0);
			if (love + 5 <= 100)
			{
				love += 5;
				UserDefault::getInstance()->setIntegerForKey("second_love", love);
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("second_love", 100);
				UserDefault::getInstance()->flush();
			}
		}
       else if (emo == 2)  // 배고파 하는경우 이모션
		{
			spr1->setTexture("img/emo_4.png");
		}

		emo_nity2 = 1;
		if (emotion_start == 0)
		this->schedule(schedule_selector(inlife::emotion_active), 0.01f);

		auto action1 = DelayTime::create(5.0);
		auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
		auto action3 = Sequence::create(action1, action2, NULL);
		spr1->runAction(action3);
	}


	//세번째 니티에 대한 이모션
	if (emo_nity3 == 0 && nity == 3)
	{
		auto spr1 = Sprite::create("img/emo_3.png");
		spr1->setPosition(-100, -100);
		this->addChild(spr1, 5, "emo_3");

		if (emo == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love", 0);

			if (love + 5 <= 100)
			{
				love += 5;
				UserDefault::getInstance()->setIntegerForKey("third_love", love);
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("third_love", 100);
				UserDefault::getInstance()->flush();
			}
		}
	   else if (emo == 2)
	 	{
			spr1->setTexture("img/emo_4.png");
		}

		emo_nity3 = 1;
		if (emotion_start == 0)
		this->schedule(schedule_selector(inlife::emotion_active), 0.01f);

		auto action1 = DelayTime::create(5.0);
		auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::emotion_del, this));
		auto action3 = Sequence::create(action1, action2, NULL);
		spr1->runAction(action3);
	}
}


//<상단 메뉴들>
//상점으로
void inlife::shop(Ref* pSender)
{
	if (touchenable == 0)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

		this->setTouchEnabled(false);
		_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);


		Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
		Director::getInstance()->pushScene(shop::createScene());
	}
}

//퀘스트
void inlife::quest(Ref* pSender)
{
	quest_messer();
}

void inlife::quest_messer()
{
	int quest = UserDefault::getInstance()->getIntegerForKey("quest");

	if (touchenable == 0)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

		if (quest == 0)
		{
			auto item1 = MenuItemImage::create("img/mess_qu1.png", "img/mess_qu1.png", CC_CALLBACK_1(inlife::quest_mess, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			this->addChild(menu, 16, "questm");
		}
		else if (quest == 1)
		{
			auto item1 = MenuItemImage::create("img/mess_qu2.png", "img/mess_qu2.png", CC_CALLBACK_1(inlife::quest_mess, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			this->addChild(menu, 16, "questm");
		}
		else if (quest == 2)
		{
			auto item1 = MenuItemImage::create("img/mess_qu3.png", "img/mess_qu3.png", CC_CALLBACK_1(inlife::quest_mess, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			this->addChild(menu, 16, "questm");
		}
		else if (quest == 3)
		{
			auto item1 = MenuItemImage::create("img/mess_qu4.png", "img/mess_qu4.png", CC_CALLBACK_1(inlife::quest_mess, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			this->addChild(menu, 16, "questm");
		}
	}
}

void inlife::quest_mess(Ref* pSender)
{
	int quest = UserDefault::getInstance()->getIntegerForKey("quest");

	auto state1 = (Sprite*)this->getChildByName("questm");
	this->removeChild(state1);

		if (quest == 1)
		{
			int qu_food = UserDefault::getInstance()->getIntegerForKey("food");
			qu_food += 15;
			UserDefault::getInstance()->setIntegerForKey("food", qu_food);
			UserDefault::getInstance()->flush();
		}

		if (quest < 3)
		{
			++quest;
			UserDefault::getInstance()->setIntegerForKey("quest", quest);
			UserDefault::getInstance()->flush();

			quest_messer();

			if (quest == 3)
			{
				auto state2 = (Sprite*)this->getChildByName("questm");
				this->removeChild(state2);
			}
		}
		else
		{
			auto state2 = (Sprite*)this->getChildByName("questm");
			this->removeChild(state2);
		}

}


//<메뉴_게임 부분>
//게임 화면으로
void inlife::game(Ref* pSender)
{
	if (touchenable == 0)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

		touchenable = 1;

		auto me_main = MenuItemImage::create("img/img_game.png", "img/img_game.png", CC_CALLBACK_1(inlife::nothing, this));
		auto me1 = MenuItemImage::create("img/start.png", "img/start_se.png", CC_CALLBACK_1(inlife::Game1_start, this));
		auto me2 = MenuItemImage::create("img/explain.png", "img/explain_se.png", CC_CALLBACK_1(inlife::Game1_explain, this));
		auto me3 = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::Game_exit, this));
		auto me4 = MenuItemImage::create("img/start.png", "img/start_se.png", CC_CALLBACK_1(inlife::Game2_start, this));
		auto me5 = MenuItemImage::create("img/explain.png", "img/explain_se.png", CC_CALLBACK_1(inlife::Game2_explain, this));
		auto menu = Menu::create(me_main, me1, me2, me3, me4, me5, NULL);
		me1->setPosition(Point(45, 37));
		me2->setPosition(Point(105, 37));
		me3->setPosition(Point(125, 108));
		me3->setScale(1.6);
		me4->setPosition(Point(45, -8));
		me5->setPosition(Point(105, -8));
		this->addChild(menu, 15, "Game_stat");
		menu->setOpacity(230);
	}
}

//게임1 시작 -> 게임화면으로의 전환
void inlife::Game1_start(Ref* pSender)
{
	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
	int food1 = UserDefault::getInstance()->getIntegerForKey("first_food");
	int food2 = UserDefault::getInstance()->getIntegerForKey("second_food");
	int food3 = UserDefault::getInstance()->getIntegerForKey("third_food");

	if (main_nity != 0)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/book.ogg");

		//터치이벤트 리스너 기능을 확실히 제거
		this->setTouchEnabled(false);
		_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
		Director::getInstance()->getEventDispatcher()->removeEventListener(listener);

		if (main_nity == 1)
		{
			food1 -= 2;
			UserDefault::getInstance()->setIntegerForKey("first_food", food1);
			UserDefault::getInstance()->flush();
		}

		if (main_nity == 2)
		{
			food2 -= 2;
			UserDefault::getInstance()->setIntegerForKey("second_food", food2);
			UserDefault::getInstance()->flush();
		}

		if (main_nity == 3)
		{
			food3 -= 2;
			UserDefault::getInstance()->setIntegerForKey("third_food", food3);
			UserDefault::getInstance()->flush();
		}
		Director::getInstance()->pushScene(Game1::createScene());
	}
	else
		anno_slow("img/ann_nomain.png");
}

//게임1에 대한 설명
void inlife::Game1_explain(Ref* pSender)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto me_main = MenuItemImage::create("img/Game1_explain.png", "img/Game1_explain.png", CC_CALLBACK_1(inlife::simple_close, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
	menu->setOpacity(230);
}


//게임2 시작 -> 게임화면으로의 전환
void inlife::Game2_start(Ref* pSender)
{
	int main_nity = UserDefault::getInstance()->getIntegerForKey("main_nity");
	int food1 = UserDefault::getInstance()->getIntegerForKey("first_food");
	int food2 = UserDefault::getInstance()->getIntegerForKey("second_food");
	int food3 = UserDefault::getInstance()->getIntegerForKey("third_food");

	float fly1 = UserDefault::getInstance()->getFloatForKey("first_fly");
	float fly2 = UserDefault::getInstance()->getFloatForKey("second_fly");
	float fly3 = UserDefault::getInstance()->getFloatForKey("third_fly");

	if (main_nity != 0)
	{
		if (main_nity == 1 && fly1 != 0)
		{
			food1 -= 2;
			UserDefault::getInstance()->setIntegerForKey("first_food", food1);
			UserDefault::getInstance()->flush();

			//사운드 처리
			sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound2 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/book.ogg");

			//터치이벤트 리스너 기능을 확실히 제거
			this->setTouchEnabled(false);
			_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
			Director::getInstance()->getEventDispatcher()->removeEventListener(listener);

			Director::getInstance()->pushScene(Game2::createScene());
		}
		else
			anno_slow("img/ann_game.png");

		if (main_nity == 2 && fly2 != 0)
		{
			food2 -= 2;
			UserDefault::getInstance()->setIntegerForKey("second_food", food2);
			UserDefault::getInstance()->flush();

			//사운드 처리
			sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound2 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/book.ogg");

			//터치이벤트 리스너 기능을 확실히 제거
			this->setTouchEnabled(false);
			_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
			Director::getInstance()->getEventDispatcher()->removeEventListener(listener);

			Director::getInstance()->pushScene(Game2::createScene());
		}
		else
			anno_slow("img/ann_game.png");

		if (main_nity == 3 && fly3 != 0)
		{
			food3 -= 2;
			UserDefault::getInstance()->setIntegerForKey("third_food", food3);
			UserDefault::getInstance()->flush();

			//사운드 처리
			sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound2 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/book.ogg");

			//터치이벤트 리스너 기능을 확실히 제거
			this->setTouchEnabled(false);
			_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
			Director::getInstance()->getEventDispatcher()->removeEventListener(listener);

			Director::getInstance()->pushScene(Game2::createScene());
		}
		else
			anno_slow("img/ann_game.png");
	}
	else
		anno_slow("img/ann_nomain.png");
}

//게임2에 대한 설명
void inlife::Game2_explain(Ref* pSender)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	auto me_main = MenuItemImage::create("img/Game2_explain.png", "img/Game2_explain.png", CC_CALLBACK_1(inlife::simple_close, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
	menu->setOpacity(230);
}

//게임 상태창 종료
void inlife::Game_exit(Ref* pSender)
{
	//사운드 처리
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	touchenable = 0;

	auto state1 = (Sprite*)this->getChildByName("Game_stat");
	this->removeChild(state1);
}


//스크린샷 찍기
void inlife::camera(Ref* pSender)
{
	int screenrand = rand() % 999;

	Size s = Director::getInstance()->getWinSize();
	auto renderTexture = RenderTexture::create(s.width, s.height, Texture2D::PixelFormat::RGBA8888);
	renderTexture->begin();
	Director::getInstance()->getRunningScene()->visit();
	renderTexture->end();
	renderTexture->saveToFile(StringUtils::format("nity%d.png", screenrand), Image::Format::PNG);

	//사운드의 처리
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/screenshot.wav");

	//알림의 처리
	anno_slow("img/ann_screenshot.png");
}

//메인화면으로
void inlife::home(Ref* pSender)
{
	if (touchenable == 0)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

		//터치이벤트 리스너 기능을 확실히 제거
		this->setTouchEnabled(false);
		_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
		Director::getInstance()->getEventDispatcher()->removeEventListener(listener);


		auto scene = TransitionFade::create(1.5, HelloWorld::createScene());
		Director::getInstance()->pushScene(scene);
	}
}

//니티가 레벨이 7이 되면 비행능력을 가질 수 있는 니티에게 능력을 준다.
void inlife::up_stat()
{
	int first = UserDefault::getInstance()->getIntegerForKey("first");
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");

	int first_lv = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int third_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");

	float fly1 = UserDefault::getInstance()->getFloatForKey("first_fly");
	float fly2 = UserDefault::getInstance()->getFloatForKey("second_fly");
	float fly3 = UserDefault::getInstance()->getFloatForKey("third_fly");

	if (first == 4 && first_lv == 7 && fly1 == 0)
	{
		anno_slow2("img/ann_ups.png");

		UserDefault::getInstance()->setFloatForKey("first_fly", 15.0f);
		UserDefault::getInstance()->flush();
	}
	if (second == 4 && second_lv == 7 && fly2 == 0)
	{
		anno_slow2("img/ann_ups.png");

		UserDefault::getInstance()->setFloatForKey("second_fly", 15.0f);
		UserDefault::getInstance()->flush();
	}
	if (third == 4 && third_lv == 7 && fly3 == 0)
	{
		anno_slow2("img/ann_ups.png");

		UserDefault::getInstance()->setFloatForKey("third_fly", 15.0f);
		UserDefault::getInstance()->flush();
	}

}


//<나무들을 관리한다>
//나무 아이콘을 고르는 메뉴
void inlife::trees(Ref* pSender)
{
	if (touchenable == 0)
	{
		sound_click();

		//다른메뉴들의 터치를 방지한다.
		touchenable = 1;

		//메뉴 버튼을 생성한다.
		auto bgr = MenuItemImage::create("img/trees_adm.png", "img/trees_adm.png", CC_CALLBACK_1(inlife::nothing, this));
		auto exit = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::close_state, this));
		auto tree = MenuItemImage::create("img/tree.png", "img/tree_se.png", CC_CALLBACK_1(inlife::trees_state, this));
		auto flower = MenuItemImage::create("img/flower.png", "img/flower_se.png", CC_CALLBACK_1(inlife::nothing, this));
		auto f1 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(inlife::trees_explain, this));
		auto menu = Menu::create(bgr, exit, tree, flower, f1, NULL);
		exit->setScale(1.4);
		exit->setPosition(132, 32);
		tree->setScale(0.2);
		tree->setPosition(-102, 13);
		flower->setScale(0.7);
		flower->setPosition(-50, 11);
		f1->setPosition(125, -15);

		menu->setOpacity(220);
		this->addChild(menu, 11, "state1");
	}
}

//라이프 식물들 대한 설명
void inlife::trees_explain(Ref* pSender)
{
	sound_click();

	auto me_main = MenuItemImage::create("img/ex_trees.png", "img/ex_trees.png", CC_CALLBACK_1(inlife::simple_close, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
}

//나무의 상태창이 나온다.
void inlife::trees_state(Ref* pSender)
{
	sound_click();

	int trees_wat[3];  //수분
	int trees_tem[3];  //온도

	trees_wat[0] = UserDefault::getInstance()->getIntegerForKey("trees_wat0", 0);
	trees_tem[0] = UserDefault::getInstance()->getIntegerForKey("trees_tem0", 0);
	trees_wat[1] = UserDefault::getInstance()->getIntegerForKey("trees_wat1", 0);
	trees_tem[1] = UserDefault::getInstance()->getIntegerForKey("trees_tem1", 0);
	trees_wat[2] = UserDefault::getInstance()->getIntegerForKey("trees_wat2", 0);
	trees_tem[2] = UserDefault::getInstance()->getIntegerForKey("trees_tem2", 0);
	int trees1_x = UserDefault::getInstance()->getIntegerForKey("trees1_x", 0);
	int trees1_y = UserDefault::getInstance()->getIntegerForKey("trees1_y", 0);
	int trees2_x = UserDefault::getInstance()->getIntegerForKey("trees2_x", 0);
	int trees2_y = UserDefault::getInstance()->getIntegerForKey("trees2_y", 0);

	//메뉴 버튼을 생성한다.
	auto bgr = MenuItemImage::create("img/trees_state.png", "img/trees_state.png", CC_CALLBACK_1(inlife::nothing, this));
	auto exit = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::exit_trees_state, this));
	auto tree1 = MenuItemImage::create("img/tree.png", "img/tree.png", CC_CALLBACK_1(inlife::tree1, this));
	auto tree2 = MenuItemImage::create("img/tree.png", "img/tree.png", CC_CALLBACK_1(inlife::tree2, this));
	auto tree3 = MenuItemImage::create("img/tree.png", "img/tree.png", CC_CALLBACK_1(inlife::tree3, this));
	auto temp_up = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::temp_up, this));
	auto temp_down = MenuItemImage::create("img/icon_up.png", "img/icon_up_se.png", CC_CALLBACK_1(inlife::temp_down, this));
	auto f1 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(inlife::trees_explain2, this));
	auto menu = Menu::create(bgr, exit, tree1, tree2, tree3, temp_up, temp_down, f1, NULL);
	exit->setScale(1.4);
	exit->setPosition(155, 104);
	tree1->setScale(0.2f);
	tree1->setPosition(-149, 83);
	tree2->setScale(0.2f);
	tree2->setPosition(-149, -1);
	tree3->setScale(0.2f);
	tree3->setPosition(-149, -84);
	temp_up->setPosition(150, -15);
	temp_up->setScale(1.5);
	temp_down->setRotation(180);
	temp_down->setScale(1.5);
	temp_down->setPosition(100, -15);
	f1->setPosition(165, -95);

	//나무를 소유하고 있지 않는경우
	if (trees1_x == 0 && trees1_y == 0)
	{
		tree2->setPosition(-300, -300);
	}
	if (trees2_x == 0 && trees2_y == 0)
	{
		tree3->setPosition(-300, -300);
	}

	menu->setOpacity(230);
	this->addChild(menu, 12, "trees_state");


	//수분 게이지바의 스프라이트
	for (int i = 0; i < 3; i++)
	{
		auto food_bar = CCSprite::create("img/water_bar.png", CCRectMake(0, 0, (133 * trees_wat[i]) / food_max, 13));
		food_bar->setPosition(164, 264 - 85 * i);
		food_bar->setAnchorPoint(Point(0, 1));
		food_bar->setOpacity(200);
		this->addChild(food_bar, 13, StringUtils::format("trees_p%d", i));
	}

	//온도 게이지바의 스프라이트
	for (int k = 0; k < 3; k++)
	{
		auto food_bar2 = CCSprite::create("img/temper_bar.png", CCRectMake(0, 0, (133 * trees_tem[k]) / food_max, 13));
		food_bar2->setPosition(164, 236 - 85 * k);
		food_bar2->setAnchorPoint(Point(0, 1));
		food_bar2->setOpacity(200);
		this->addChild(food_bar2, 13, StringUtils::format("trees_p2%d", k));
	}

	int life_temp = UserDefault::getInstance()->getIntegerForKey("lifetemp", -1);

	if (life_temp == -1)
	{
		life_temp = 50;
		UserDefault::getInstance()->setIntegerForKey("lifetemp", life_temp);
		UserDefault::getInstance()->flush();
	}

	//현재 온도의 스프라이트
	auto label1 = Label::createWithSystemFont(StringUtils::format("%d %%", life_temp), "Arial", 13);
	label1->setAnchorPoint(Point(0, 1));
	label1->setPosition(Point(385, 182));
	label1->setColor(Color3B::BLACK);
	label1->setOpacity(170);
	this->addChild(label1, 13, "temp_lab");
}


//나무들 대한 설명
void inlife::trees_explain2(Ref* pSender)
{
	sound_click();

	auto me_main = MenuItemImage::create("img/ex_trees2.png", "img/ex_trees2.png", CC_CALLBACK_1(inlife::simple_close, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 16, "simplat");
}

//나무1에게 물주기
void inlife::tree1(Ref* pSender)
{
	int tree1_x = UserDefault::getInstance()->getIntegerForKey("tree1_x", 0);
	int tree1_y = UserDefault::getInstance()->getIntegerForKey("tree1_y", 0);

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::exit_trees_state, this, (Ref*)20));
	runAction(action1);
	auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
	runAction(action2);

	//효과음
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/water.ogg");

	auto apr1 = ParticleSystemQuad::create("par/givewater.plist");
	apr1->setOpacity(140);
	apr1->setScale(0.6);
	apr1->setPosition(tree1_x + 63, tree1_y + 10);
	this->addChild(apr1, 50);

	int wat = UserDefault::getInstance()->getIntegerForKey("trees_wat0");

	if (wat + 25 >= 100)
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat0", 100);
		UserDefault::getInstance()->flush();
	}
	else
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat0", wat += 25);
		UserDefault::getInstance()->flush();
	}
}

//나무2에게 물주기
void inlife::tree2(Ref* pSender)
{
	int tree1_x = UserDefault::getInstance()->getIntegerForKey("trees1_x", 0);
	int tree1_y = UserDefault::getInstance()->getIntegerForKey("trees1_y", 0);

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::exit_trees_state, this, (Ref*)20));
	runAction(action1);
	auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
	runAction(action2);

	//효과음
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/water.ogg");

	auto apr1 = ParticleSystemQuad::create("par/givewater.plist");
	apr1->setOpacity(140);
	apr1->setScale(0.6);
	apr1->setPosition(tree1_x + 63, tree1_y);
	this->addChild(apr1, 50);

	int wat = UserDefault::getInstance()->getIntegerForKey("trees_wat1");
	if (wat + 25 >= 100)
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat1", 100);
		UserDefault::getInstance()->flush();
	}
	else
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat1", wat += 25);
		UserDefault::getInstance()->flush();
	}
}

//나무3에게 물주기
void inlife::tree3(Ref* pSender)
{
	int tree1_x = UserDefault::getInstance()->getIntegerForKey("trees2_x", 0);
	int tree1_y = UserDefault::getInstance()->getIntegerForKey("trees2_y", 0);

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::exit_trees_state, this, (Ref*)20));
	runAction(action1);
	auto action2 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
	runAction(action2);

	//효과음
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/water.ogg");

	auto apr1 = ParticleSystemQuad::create("par/givewater.plist");
	apr1->setOpacity(140);
	apr1->setScale(0.6);
	apr1->setPosition(tree1_x + 63, tree1_y);
	this->addChild(apr1, 50);

	int wat = UserDefault::getInstance()->getIntegerForKey("trees_wat2");
	if (wat + 25 >= 100)
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat2", 100);
		UserDefault::getInstance()->flush();
	}
	else
	{
		UserDefault::getInstance()->setIntegerForKey("trees_wat2", wat += 25);
		UserDefault::getInstance()->flush();
	}
}

//나무의 상태창 닫기
void inlife::exit_trees_state(Ref* pSender)
{
	sound_click();

	auto state1 = (Sprite*)this->getChildByName("trees_state");
	this->removeChild(state1);

	auto state2 = (Sprite*)this->getChildByName("temp_lab");
	this->removeChild(state2);

	auto state3 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state3);

	//수분 게이지바 삭제
	for (int i = 0; i < 3; i++)
	{
		auto state1 = (Sprite*)this->getChildByName(StringUtils::format("trees_p%d", i));
		this->removeChild(state1);
	}

	//온도 게이지바 삭제
	for (int i = 0; i < 3; i++)
	{
		auto state1 = (Sprite*)this->getChildByName(StringUtils::format("trees_p2%d", i));
		this->removeChild(state1);
	}
}

//라이프 온도를 높힌다.
void inlife::temp_up(Ref* pSender)
{
	sound_click();

	int life_temp = UserDefault::getInstance()->getIntegerForKey("lifetemp", -1);
	if (life_temp <= 95)
	{
		auto state1 = (Sprite*)this->getChildByName("temp_lab");
		this->removeChild(state1);

		UserDefault::getInstance()->setIntegerForKey("lifetemp", life_temp += 5);
		UserDefault::getInstance()->flush();

		//현재 온도의 스프라이트
		auto label1 = Label::createWithSystemFont(StringUtils::format("%d %%", life_temp), "Arial", 13);
		label1->setAnchorPoint(Point(0, 1));
		label1->setPosition(Point(385, 182));
		label1->setColor(Color3B::BLACK);
		label1->setOpacity(170);
		this->addChild(label1, 13, "temp_lab");
	}
}

//라이프 온도를 낮춘다.
void inlife::temp_down(Ref* pSender)
{
	sound_click();

	int life_temp = UserDefault::getInstance()->getIntegerForKey("lifetemp", -1);

	if (life_temp >= 5)
	{
		auto state1 = (Sprite*)this->getChildByName("temp_lab");
		this->removeChild(state1);

		UserDefault::getInstance()->setIntegerForKey("lifetemp", life_temp -= 5);
		UserDefault::getInstance()->flush();

		//현재 온도의 스프라이트
		auto label1 = Label::createWithSystemFont(StringUtils::format("%d %%", life_temp), "Arial", 13);
		label1->setAnchorPoint(Point(0, 1));
		label1->setPosition(Point(385, 182));
		label1->setColor(Color3B::BLACK);
		label1->setOpacity(170);
		this->addChild(label1, 13, "temp_lab");
	}
}

//나무의 수분이 시간에 따라 자동적으로 감소한다.
void inlife::tree_wat()
{
	int nowtime = time(NULL);
	int time_count = 0;
	int fallwat = UserDefault::getInstance()->getIntegerForKey("fallwat",0);

	if (fallwat == 0)
	{
		UserDefault::getInstance()->setIntegerForKey("fallwat", time(NULL));
		UserDefault::getInstance()->flush();
	}

	int wat0 = UserDefault::getInstance()->getIntegerForKey("trees_wat0");
	int wat1 = UserDefault::getInstance()->getIntegerForKey("trees_wat1");
	int wat2 = UserDefault::getInstance()->getIntegerForKey("trees_wat2");

	time_count = (nowtime - fallwat) / (4*3600);

	//시간이 지난경우
	if ((nowtime - fallwat) >= 3600 * 4)
	{
		if (wat0 - (10 * time_count) >= 0)
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat0", wat0 - (10 * time_count));
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat0", 0);
			UserDefault::getInstance()->flush();
		}

		if (wat1 - 10 * time_count >= 0)
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat1", wat1 - 10 * time_count);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat1", 0);
			UserDefault::getInstance()->flush();
		}

		if (wat2 - 10 * time_count >= 0)
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat2", wat2 - 10 * time_count);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("trees_wat2", 0);
			UserDefault::getInstance()->flush();
		}

		UserDefault::getInstance()->setIntegerForKey("fallwat", time(NULL));
		UserDefault::getInstance()->flush();
	}
}

//나무의 수분이 시간에 따라 사용자가 설정한 온도로 변경된다.
void inlife::tree_tem()
{
	int nowtime = time(NULL);
	int time_count = 0;
	int valtem = UserDefault::getInstance()->getIntegerForKey("valtem", 0);

	if (valtem == 0)
	{
		UserDefault::getInstance()->setIntegerForKey("valtem", time(NULL));
		UserDefault::getInstance()->flush();
	}

	int tem = UserDefault::getInstance()->getIntegerForKey("lifetemp");
	int tem0 = UserDefault::getInstance()->getIntegerForKey("trees_tem0");
	int tem1 = UserDefault::getInstance()->getIntegerForKey("trees_tem1");
	int tem2 = UserDefault::getInstance()->getIntegerForKey("trees_tem2");

	time_count = (nowtime - valtem) / (4 * 3600);

	//시간이 지난경우
	if ((nowtime - valtem) >= 3600 * 4)
	{
		//첫번째 나무
		//양수인경우
		if (tem - tem0 >= 0)
		{
			if (tem0 + (10 * time_count) <= tem)
			{
				UserDefault::getInstance()->setIntegerForKey("trees_tem0", tem0 + (10 * time_count));
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("trees_tem0", tem);
				UserDefault::getInstance()->flush();
			}
		}
		//음수인경우
		if (tem - tem0 <= 0)
		{
			if (tem0 - (10 * time_count) >= tem)
			{
				UserDefault::getInstance()->setIntegerForKey("trees_tem0", tem0 - (10 * time_count));
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("trees_tem0", tem);
				UserDefault::getInstance()->flush();
			}
		}

		    //두번째 나무
			//양수인경우
			if (tem - tem1 >= 0)
			{
				if (tem1 + (10 * time_count) <= tem)
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem1", tem1 + (10 * time_count));
					UserDefault::getInstance()->flush();
				}
				else
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem1", tem);
					UserDefault::getInstance()->flush();
				}
			}
			//음수인경우
			if (tem - tem1 <= 0)
			{
				if (tem1 - (10 * time_count) >= tem)
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem1", tem1 - (10 * time_count));
					UserDefault::getInstance()->flush();
				}
				else
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem1", tem);
					UserDefault::getInstance()->flush();
				}
			}

			//세번째 나무
			//양수인경우
			if (tem - tem2 >= 0)
			{
				if (tem2 + (10 * time_count) <= tem)
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem2", tem2 + (10 * time_count));
					UserDefault::getInstance()->flush();
				}
				else
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem2", tem);
					UserDefault::getInstance()->flush();
				}
			}
			//음수인경우
			if (tem - tem2 <= 0)
			{
				if (tem2 - (10 * time_count) >= tem)
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem2", tem2 - (10 * time_count));
					UserDefault::getInstance()->flush();
				}
				else
				{
					UserDefault::getInstance()->setIntegerForKey("trees_tem2", tem);
					UserDefault::getInstance()->flush();
				}
			}

		UserDefault::getInstance()->setIntegerForKey("valtem", time(NULL));
		UserDefault::getInstance()->flush();
	}
}


//<기타 부가기능들>

// 지속적인 bgm의 발생
void inlife::sound(float delta)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//배경음악 설정
	if (sound2 == 0)
	{
		if (rand_sound == 1)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife1.mp3");
		else if (rand_sound == 2)
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife2.mp3");
		else if (rand_sound == 3)
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife3.mp3");
		else if (rand_sound == 4)
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife4.mp3");
		else if (rand_sound == 5)
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/inlife5.mp3");


	}
	else
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}

// 지속적인 니티의 소리 발생
void inlife::nity_sound(float delta)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);

	//배경음악 설정
	if (sound2 == 0)
	{
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sla.ogg");
	}

}

//니티의 정보창 닫기1(메뉴의 선택지로)
void inlife::close_state(Ref* pSender)
{
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");


	if (onstate2 == 0)
	touchenable = 0;
	onfood = 0;

	onstat = 0;

	auto state1 = (Sprite*)this->getChildByName("state1");
	auto state2 = (Sprite*)this->getChildByName("state2");
	auto state3 = (Sprite*)this->getChildByName("state3");
	auto state4 = (Sprite*)this->getChildByName("state4");
	auto state5 = (Sprite*)this->getChildByName("state5");
	auto state6 = (Sprite*)this->getChildByName("state6");
	auto state7 = (Sprite*)this->getChildByName("state7");
	auto state8 = (Sprite*)this->getChildByName("sel_food");
	auto state9 = (Sprite*)this->getChildByName("state_love");
	auto state10 = (Sprite*)this->getChildByName("state8");
	auto state11 = (Sprite*)this->getChildByName("state9");
	auto state12 = (Sprite*)this->getChildByName("simplat");

	this->removeChild(state1);
	this->removeChild(state2);
	this->removeChild(state3);
	this->removeChild(state4);
	this->removeChild(state5);
	this->removeChild(state6);
	this->removeChild(state7);
	this->removeChild(state8);
	this->removeChild(state9);
	this->removeChild(state10);
	this->removeChild(state11);
	this->removeChild(state12);
}

//니티의 정보창 닫기2(강제)
void inlife::close_state2()
{
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");

	if (onstate2 == 0)
	touchenable = 0;

	onfood = 0;

	auto state1 = (Sprite*)this->getChildByName("state1");
	auto state2 = (Sprite*)this->getChildByName("state2");
	auto state3 = (Sprite*)this->getChildByName("state3");
	auto state4 = (Sprite*)this->getChildByName("state4");
	auto state5 = (Sprite*)this->getChildByName("state5");
	auto state6 = (Sprite*)this->getChildByName("state6");
	auto state7 = (Sprite*)this->getChildByName("state7");
	auto state8 = (Sprite*)this->getChildByName("sel_food");
	auto state10 = (Sprite*)this->getChildByName("state8");

	this->removeChild(state1);
	this->removeChild(state2);
	this->removeChild(state3);
	this->removeChild(state4);
	this->removeChild(state5);
	this->removeChild(state6);
	this->removeChild(state7);
	this->removeChild(state8);
	this->removeChild(state10);
}

//알림 메세지의 생성
void inlife::anno(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(120, 335));
	this->addChild(img1, 16, "ann");
}

//알림 메세지의 삭제
void inlife::anno_del()
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);
}

//알림 메세지의 느린 생성과 소멸
void inlife::anno_slow(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(120, 335));
	this->addChild(img1, 200, "ann");

	//천천히 이미지를 페이드아웃한다.
	auto action1 = FadeOut::create(5.0);
	img1->runAction(action1);
}

//알림 메세지의 느린 생성과 소멸
void inlife::anno_slow2(char *ano)
{
	auto state1 = (Sprite*)this->getChildByName("ann");
	this->removeChild(state1);

	auto img1 = Sprite::create(ano);
	img1->setAnchorPoint(Point(0, 1));
	img1->setPosition(Point(0, 285));
	img1->setOpacity(190);
	this->addChild(img1, 200, "ann");

	//천천히 이미지를 페이드아웃한다.
	auto action1 = FadeOut::create(5.0);
	img1->runAction(action1);
}

//남은 먹이량의 지속적인 업데이트와 배고파 하는경우 알림(&&*)
void inlife::init_food(float delta)
{
	auto state1 = (Sprite*)this->getChildByName("food_q2");
	this->removeChild(state1);

	int food = UserDefault::getInstance()->getIntegerForKey("food", 0);
	auto label2 = Label::createWithSystemFont(StringUtils::format("%d", food), "Arial", 14);
	label2->setAnchorPoint(Point(0, 1));
	label2->setPosition(Point(35, 307));
	this->addChild(label2, 11, "food_q2");

	int max = UserDefault::getInstance()->getIntegerForKey("nities", 0);
	int food1 = UserDefault::getInstance()->getIntegerForKey("first_food", 0);
	int food2 = UserDefault::getInstance()->getIntegerForKey("second_food", 0);
	int food3 = UserDefault::getInstance()->getIntegerForKey("third_food", 0);

	int first = UserDefault::getInstance()->getIntegerForKey("first");
	int second = UserDefault::getInstance()->getIntegerForKey("second");
	int third = UserDefault::getInstance()->getIntegerForKey("third");

	int exp1 = UserDefault::getInstance()->getIntegerForKey("first_exp", 0);
	int exp2 = UserDefault::getInstance()->getIntegerForKey("second_exp", 0);
	int exp3 = UserDefault::getInstance()->getIntegerForKey("third_exp", 0);

	if (food1 <= 0)
	{
		emotion_nity(1, 2);
		anno_slow("img/ann_hungry.png");
		food1 += 3;
		UserDefault::getInstance()->setIntegerForKey("first_food", food1);
		UserDefault::getInstance()->flush();

		//애정도를 깍는다.
		int love = UserDefault::getInstance()->getIntegerForKey("first_love", 0);

		if (love - 3 >= 0)
		{
			UserDefault::getInstance()->setIntegerForKey("first_love", love-3);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("first_love", 0);
			UserDefault::getInstance()->flush();
		}
	}

	if (second != 0)
	{
		if (food2 <= 0)
		{
			emotion_nity(2, 2);
			anno_slow("img/ann_hungry.png");
			food2 += 3;

			UserDefault::getInstance()->setIntegerForKey("second_food", food2);
			UserDefault::getInstance()->flush();

			//애정도를 깍는다.
			int love = UserDefault::getInstance()->getIntegerForKey("second_love", 0);

			if (love - 3 >= 0)
			{
				UserDefault::getInstance()->setIntegerForKey("second_love", love - 3);
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("second_love", 0);
				UserDefault::getInstance()->flush();
			}
		}
	}

	if (third != 0)
	{
		if (food3 <= 0)
		{
			emotion_nity(3, 2);
			anno_slow("img/ann_hungry.png");
			food3 += 3;

			UserDefault::getInstance()->setIntegerForKey("third_food", food3);
			UserDefault::getInstance()->flush();

			//애정도를 깍는다.
			int love = UserDefault::getInstance()->getIntegerForKey("third_love", 0);

			if (love - 3 >= 0)
			{
				UserDefault::getInstance()->setIntegerForKey("third_love", love - 3);
				UserDefault::getInstance()->flush();
			}
			else
			{
				UserDefault::getInstance()->setIntegerForKey("third_love", 0);
				UserDefault::getInstance()->flush();
			}
		}
	}

}

//니티 포만도의 지속적인 감소
void inlife::sh_food(float delta)
{
	int food1 = UserDefault::getInstance()->getIntegerForKey("first_food");
	int food2 = UserDefault::getInstance()->getIntegerForKey("second_food");
	int food3 = UserDefault::getInstance()->getIntegerForKey("third_food");

	if (food1 > 0)
	{
		--food1;
		UserDefault::getInstance()->setIntegerForKey("first_food", food1);
		UserDefault::getInstance()->flush();
	}


	if (food2 > 0)
	{
		--food2;
		UserDefault::getInstance()->setIntegerForKey("second_food", food2);
		UserDefault::getInstance()->flush();
	}


	if (food3 > 0)
	{
		--food3;
		UserDefault::getInstance()->setIntegerForKey("third_food", food3);
		UserDefault::getInstance()->flush();
	}
}

//니티 포만도의 시간에 따른 감소(종료시에도 감소)
void inlife::time_food()
{
	//접속한 시간에 따라 포만도의 감소
	int now_time = UserDefault::getInstance()->getIntegerForKey("game_start_time");
	int last_time = UserDefault::getInstance()->getIntegerForKey("game_close_time");
	int time_h = (now_time - last_time) / 3600;
	int love_h = (now_time - last_time) / 7200;

	int food1 = UserDefault::getInstance()->getIntegerForKey("first_food");
	int food2 = UserDefault::getInstance()->getIntegerForKey("second_food");
	int food3 = UserDefault::getInstance()->getIntegerForKey("third_food");

	int love1 = UserDefault::getInstance()->getIntegerForKey("first_love");
	int love2 = UserDefault::getInstance()->getIntegerForKey("second_love");
	int love3 = UserDefault::getInstance()->getIntegerForKey("third_love");

	if (now_time > last_time && last_time != 0)
	{
		//포만도의 감소
		if (food1 - time_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("first_food", food1 - time_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("first_food", 0);
			UserDefault::getInstance()->flush();
		}


		if (food2 - time_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("second_food", food2 - time_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("second_food", 0);
			UserDefault::getInstance()->flush();
		}


		if (food3 - time_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("third_food", food3 - time_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("third_food", 0);
			UserDefault::getInstance()->flush();
		}

		//애정도의 감소
		if (love1 - love_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("first_love", love1 - love_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("first_love", 0);
			UserDefault::getInstance()->flush();
		}

		if (love2 - love_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("second_love", love2 - love_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("second_love", 0);
			UserDefault::getInstance()->flush();
		}


		if (love3 - love_h > 0)
		{
			UserDefault::getInstance()->setIntegerForKey("third_love", love3 - love_h);
			UserDefault::getInstance()->flush();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("third_love", 0);
			UserDefault::getInstance()->flush();
		}

	}
}
 
//니티의 레벨업 처리
void inlife::exp_control()
{
	int level1 = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int level2 = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int level3 = UserDefault::getInstance()->getIntegerForKey("third_lv");

	int exp1 = UserDefault::getInstance()->getIntegerForKey("first_exp");
	int exp2 = UserDefault::getInstance()->getIntegerForKey("second_exp");
	int exp3 = UserDefault::getInstance()->getIntegerForKey("third_exp");

	int stat1 = UserDefault::getInstance()->getIntegerForKey("first_stat");
	int stat2 = UserDefault::getInstance()->getIntegerForKey("second_stat");
	int stat3 = UserDefault::getInstance()->getIntegerForKey("third_stat");

	exp_max1 = 100 + (level1 * 30);
	exp_max2 = 100 + (level2 * 30);
	exp_max3 = 100 + (level3 * 30);

	//첫번째 니티의 레벨업
	if (exp1 >= exp_max1)
	{
		anno_slow2("img/ann_levelup.png");
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

		//레벨의 증가
		++level1;
		UserDefault::getInstance()->setIntegerForKey("first_lv", level1);
		UserDefault::getInstance()->flush();

		//소유 경험치량의 초기화
		UserDefault::getInstance()->setIntegerForKey("first_exp", 0);
		UserDefault::getInstance()->flush();

		//스텟의 1 증가
		stat1++;
		UserDefault::getInstance()->setIntegerForKey("first_stat", stat1);
		UserDefault::getInstance()->flush();
	}

	//두번째 니티의 레벨업
	if (exp2 >= exp_max2)
	{
		anno_slow2("img/ann_levelup.png");
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

		//레벨의 증가
		++level2;
		UserDefault::getInstance()->setIntegerForKey("second_lv", level2);
		UserDefault::getInstance()->flush();

		//소유 경험치량의 초기화
		UserDefault::getInstance()->setIntegerForKey("second_exp", 0);
		UserDefault::getInstance()->flush();

		//스텟의 1 증가
		stat2++;
		UserDefault::getInstance()->setIntegerForKey("second_stat", stat2);
		UserDefault::getInstance()->flush();
	}

	//세번째 니티의 레벨업
	if (exp3 >= exp_max3)
	{
		anno_slow2("img/ann_levelup.png");
		//사운드처리
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

		//레벨의 증가
		++level3;
		UserDefault::getInstance()->setIntegerForKey("third_lv", level3);
		UserDefault::getInstance()->flush();

		//소유 경험치량의 초기화
		UserDefault::getInstance()->setIntegerForKey("third_exp", 0);
		UserDefault::getInstance()->flush();

		//스텟의 1 증가
		stat3++;
		UserDefault::getInstance()->setIntegerForKey("third_stat", stat3);
		UserDefault::getInstance()->flush();
	}
}

//간단한 플랫창 터치로 닫기
void inlife::simple_close(Ref* pSender)
{
	sound_click();

	auto state1 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state1);

}

//<튜토리얼>
void inlife::tutorial()
{

	int tutor = UserDefault::getInstance()->getIntegerForKey("tutorial", 0);

	//배경화면 설정
	if (tutor == 0)
	{
		//메뉴들을 생성한다.
		auto item1 = MenuItemImage::create("img/img_mes1.png", "img/img_mes1.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 1)
	{
		auto item1 = MenuItemImage::create("img/img_mes2.png", "img/img_mes2.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 2)
	{
		auto item1 = MenuItemImage::create("img/img_mes3.png", "img/img_mes3.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 3)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/message.wav");

		auto item1 = MenuItemImage::create("img/img_mes4.png", "img/img_mes4.png", CC_CALLBACK_1(inlife::touch_egg, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);

		auto spr2 = Sprite::create("img/img_mes5.png");
		spr2->setAnchorPoint(Point(0, 1));
		spr2->setPosition(0, 320);
		this->addChild(spr2);

		auto action2 = Blink::create(200, 150);
		spr2->runAction(action2);
	}

	else if (tutor == 4)
	{
		float x = (rand() % 3 + 1);

		if (x == 1) //파란색 슬라임
		{
			//첫번재 니티 이미지코드
			UserDefault::getInstance()->setIntegerForKey("first", 1);
			UserDefault::getInstance()->flush();

			auto item1 = MenuItemImage::create("img/img_mes6.png", "img/img_mes6.png", CC_CALLBACK_1(inlife::mess_tutor, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			menu->setTag(message);
			this->addChild(menu);
		}
		else if (x == 2) //분홍색 슬라임
		{
			//첫번재 니티 이미지코드
			UserDefault::getInstance()->setIntegerForKey("first", 2);
			UserDefault::getInstance()->flush();

			auto item1 = MenuItemImage::create("img/img_mes7.png", "img/img_mes7.png", CC_CALLBACK_1(inlife::mess_tutor, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			menu->setTag(message);
			this->addChild(menu);
		}
		else if (x == 3) //초록색 슬라임
		{
			//첫번재 니티 이미지코드
			UserDefault::getInstance()->setIntegerForKey("first", 3);
			UserDefault::getInstance()->flush();

			auto item1 = MenuItemImage::create("img/img_mes8.png", "img/img_mes8.png", CC_CALLBACK_1(inlife::mess_tutor, this));
			auto menu = Menu::create(item1, NULL);
			menu->alignItemsVertically();
			menu->setTag(message);
			this->addChild(menu);
		}

		//보유중인 니티 마리수 = 1
		UserDefault::getInstance()->setIntegerForKey("nities", 1);
		UserDefault::getInstance()->flush();

		//소유 가능한 최대 니티 마릿수 3
		UserDefault::getInstance()->setIntegerForKey("max_nities", 3);
		UserDefault::getInstance()->flush();

		//첫번재 니티정보 초기화
		UserDefault::getInstance()->setIntegerForKey("first_lv", 1);
		UserDefault::getInstance()->flush();

		float y = (rand() % 100 + 1);
		y /= 100;
		float x1 = (rand() % 2 + 1 + y);
		UserDefault::getInstance()->setFloatForKey("first_jump", x1);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setFloatForKey("first_fly", 0.0f);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("first_stat", 0);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("first_food", 50);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("first_exp", 0);
		UserDefault::getInstance()->flush();

		//초반에 기본자금 지여
		int money = UserDefault::getInstance()->getIntegerForKey("money");
		money += 3000;
		UserDefault::getInstance()->setIntegerForKey("money", money);
		UserDefault::getInstance()->flush();
	}
	else if (tutor == 5)
	{
		auto item1 = MenuItemImage::create("img/img_mes9.png", "img/img_mes9.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 6)
	{
		auto item1 = MenuItemImage::create("img/img_mes10.png", "img/img_mes10.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 7)
	{
		auto item1 = MenuItemImage::create("img/img_mes11.png", "img/img_mes11.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 8)
	{
		auto item1 = MenuItemImage::create("img/img_mes12.png", "img/img_mes12.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 9)
	{
		auto item1 = MenuItemImage::create("img/img_mes13.png", "img/img_mes13.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else if (tutor == 10)
	{
		auto item1 = MenuItemImage::create("img/img_mes14.png", "img/img_mes14.png", CC_CALLBACK_1(inlife::mess_tutor, this));
		auto menu = Menu::create(item1, NULL);
		menu->alignItemsVertically();
		menu->setTag(message);
		this->addChild(menu);
	}
	else
	{
		this->removeAllChildren();

		initmenu();
	}
}

void inlife::mess_tutor(Ref* pSender)
{
	//사운드 처리
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

	auto sprm = (Sprite*)this->getChildByTag(message);
	this->removeChild(sprm);

	int tutor = UserDefault::getInstance()->getIntegerForKey("tutorial", 0);
	tutor++;
	UserDefault::getInstance()->setIntegerForKey("tutorial", tutor);
	UserDefault::getInstance()->flush();


	tutorial();
}

void inlife::touch_egg(Ref* pSender)
{
	if (eggtouch < 5)
	{
		//사운드 처리
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/touch.ogg");

		eggtouch++;

	}
	else
	{   //알을 다 깬경우 튜토리얼의 다음 진행
		int tutor = UserDefault::getInstance()->getIntegerForKey("tutorial", 0);
		tutor++;
		UserDefault::getInstance()->setIntegerForKey("tutorial", tutor);
		UserDefault::getInstance()->flush();

		tutorial();
	}

}

void inlife::nothing(Ref* pSender)
{

}

void inlife::sound_click()
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/click.ogg");
}



//<키패드 백버튼을 눌렀을경우>

//터치 패드를 눌럿을 경우
void inlife::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)  // KEY_ESCAPE는 윈도우에선 ESC키, 스마트폰의 BACK키
	{
		sound_click();
		ask_exit();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_HOME)  // 홈 버튼을 눌럿을 경우
	{
		sound_click();
		ask_exit();
	}
}

//종료할지를 묻는다.
void inlife::ask_exit()
{
	if (touchenable == 0)
	{
		touchenable = 1;
		anno_slow("img/ann_exit.png");

		auto item1 = MenuItemImage::create("img/select.png", "img/select.png", CC_CALLBACK_1(inlife::nothing, this));
		auto item2 = MenuItemImage::create("img/select2.png", "img/select2_se.png", CC_CALLBACK_1(inlife::exit_life, this));
		auto item3 = MenuItemImage::create("img/select3.png", "img/select3_se.png", CC_CALLBACK_1(inlife::close_keypad, this));
		auto menu = Menu::create(item1, item2, item3, NULL);
		item2->setPosition(Point(-25, 5));
		item3->setPosition(Point(25, 5));
		this->addChild(menu, 30, "keypad");
	}
}

//게임을 종료한다.
void inlife::exit_life(Ref* pSender)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/se_sel.ogg");

	Director::getInstance()->end(); // ESC(or BACK키) 를 누르면 종료됨.
}

//키패드 질문 창을 종료한다.
void inlife::close_keypad(Ref* pSender)
{
	sound_click();
	touchenable = 0;
	auto state1 = (Sprite*)this->getChildByName("keypad");
	this->removeChild(state1);
}



//<니티의 2번째 상태창>
//누르고 움직이는경우
void inlife::onTouchMoved(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation(); // 터치했을 경우 그 좌표를 location이라는 변수에 저장한다.


	//목욕시키는 경우
	if (onstate2 == 2)
	{
		auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
		auto nity_po = nity_pic->getPosition();

		Rect rect_nity = nity_pic->getBoundingBox();
		auto washer = (Sprite*)this->getChildByName("washer");
		auto dirt = (Sprite*)this->getChildByName("dirt");
		auto bubble = (Sprite*)this->getChildByName("bubble");
		Rect rect_washer = washer->getBoundingBox();
		washer->setPosition(location);


		//때밀이로 니티를 닦아준다
		if (rect_nity.intersectsRect(rect_washer))  // 만약 터치된 좌표가 rect변수의 포인트 안에 포함되면, 실행한다.
		{
			auto dirt_op2 = dirt->getOpacity();

			if (dirt_op2 >= 1)
			{
				dirt->setOpacity(dirt_op);
				dirt_op -= 1;
			}

			//니티를 깨끗하게 다 닦은경우
			if (dirt_op2 == 0)
			{
				anno_slow2("img/ann_wash2.png");

				bubble->setOpacity(200);
				onstate2 = 1;

				washer->setPosition(-50, -50);
			}
		}

	}
}

//<간식 시스템>
//간식 메뉴
void inlife::dessert(Ref* pSender)
{
	if (onstate2 == 1)
	{
		sound_click();

		//간식창을 한번 더 누를경우 한번 더뜨는거 방지용
		auto state1 = (Sprite*)this->getChildByName("menu_des");
		this->removeChild(state1);
		auto state3 = (Sprite*)this->getChildByName("fr_la1");
		this->removeChild(state3);
		auto state4 = (Sprite*)this->getChildByName("fr_la2");
		this->removeChild(state4);
		auto state5 = (Sprite*)this->getChildByName("fr_la3");
		this->removeChild(state5);
		auto state6 = (Sprite*)this->getChildByName("fr_la4");
		this->removeChild(state6);

		//메뉴 버튼을 생성한다.
		auto bgr = MenuItemImage::create("img/img_mini.png", "img/img_mini.png", CC_CALLBACK_1(inlife::nothing, this));
		auto exit = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::close_menu_de, this));
		auto f1 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(inlife::explain_dess, this));
		auto food1 = MenuItemImage::create("img/icon_fruit1.png", "img/icon_fruit1.png", CC_CALLBACK_1(inlife::dessert1, this));
		auto food2 = MenuItemImage::create("img/icon_fruit2.png", "img/icon_fruit2.png", CC_CALLBACK_1(inlife::dessert2, this));
		auto food3 = MenuItemImage::create("img/icon_fruit3.png", "img/icon_fruit3.png", CC_CALLBACK_1(inlife::dessert3, this));
		auto food4 = MenuItemImage::create("img/icon_fruit4.png", "img/icon_fruit4.png", CC_CALLBACK_1(inlife::dessert4, this));
		auto menu = Menu::create(bgr, exit, f1, food1, food2, food3, food4, NULL);
		exit->setScale(1.4);
		exit->setPosition(132, 32);;
		f1->setPosition(125, -15);
		food1->setPosition(-100, 13);
		food2->setPosition(-50, 13);
		food3->setPosition(0, 13);
		food4->setPosition(50, 13);

		menu->setOpacity(220);
		this->addChild(menu, 106, "menu_des");


		//간식을 각각 몇개씩 가지고 있는지 표시해준다
		int dess1 = UserDefault::getInstance()->getIntegerForKey("dessert1");
		int dess2 = UserDefault::getInstance()->getIntegerForKey("dessert2");
		int dess3 = UserDefault::getInstance()->getIntegerForKey("dessert3");
		int dess4 = UserDefault::getInstance()->getIntegerForKey("dessert4");

		auto label1 = Label::createWithSystemFont(StringUtils::format("%d", dess1), "Arial", 13); //딸기
		auto label2 = Label::createWithSystemFont(StringUtils::format("%d", dess2), "Arial", 13); //복숭아
		auto label3 = Label::createWithSystemFont(StringUtils::format("%d", dess3), "Arial", 13); //바나나
		auto label4 = Label::createWithSystemFont(StringUtils::format("%d", dess4), "Arial", 13); //파인애플

		label1->setPosition(Point(140, 140));
		label2->setPosition(Point(190, 140));
		label3->setPosition(Point(240, 140));
		label4->setPosition(Point(290, 140));


		label1->setColor(Color3B(-255, -255, -255));
		label2->setColor(Color3B(-255, -255, -255));
		label3->setColor(Color3B(-255, -255, -255));
		label4->setColor(Color3B(-255, -255, -255));

		this->addChild(label1, 106, "fr_la1");
		this->addChild(label2, 106, "fr_la2");
		this->addChild(label3, 106, "fr_la3");
		this->addChild(label4, 106, "fr_la4");
	}
}

//딸기
void inlife::dessert1(Ref* pSender)
{
	auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
	auto nity_po = nity_pic->getPosition();

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::close_menu_de, this, (Ref*)20));
	runAction(action1);

	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert1");

	if (dessert > 0)
	{
		//효과음
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_heart.wav");

		anno_slow2("img/ann_happy.png");

		dessert -= 1;
		UserDefault::getInstance()->setIntegerForKey("dessert1", dessert);
		UserDefault::getInstance()->flush();

		auto fruit = Sprite::create("img/icon_fruit1.png");
		fruit->setPosition(nity_po);
		this->addChild(fruit, 105);

		auto app1 = FadeTo::create(2.0f, 0);
		fruit->runAction(app1);

		//애정도의 증가와 포만도의 증가
		if (eat_nity == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");
			int food = UserDefault::getInstance()->getIntegerForKey("first_food");

			if (love + 3 <= 100)
				love += 3;
			else
				love = 100;

			if (food + 3 <= 100)
				food += 3;
			else
				food = 100;


			UserDefault::getInstance()->setIntegerForKey("first_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("first_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 2)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love");
			int food = UserDefault::getInstance()->getIntegerForKey("second_food");

			if (love + 3 <= 100)
				love += 3;
			else
				love = 100;

			if (food + 3 <= 100)
				food += 3;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("second_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 3)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love");
			int food = UserDefault::getInstance()->getIntegerForKey("third_food");

			if (love + 3 <= 100)
				love += 3;
			else
				love = 100;

			if (food + 3 <= 100)
				food += 3;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("third_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", love);
			UserDefault::getInstance()->flush();
		}
	}
	else
		anno_slow2("img/ann_dess1.png");

}

//복숭아
void inlife::dessert2(Ref* pSender)
{
	auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
	auto nity_po = nity_pic->getPosition();

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::close_menu_de, this, (Ref*)20));
	runAction(action1);

	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert2");

	if (dessert > 0)
	{
		//효과음
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_heart.wav");

		anno_slow2("img/ann_happy.png");

		dessert -= 1;
		UserDefault::getInstance()->setIntegerForKey("dessert2", dessert);
		UserDefault::getInstance()->flush();

		auto fruit = Sprite::create("img/icon_fruit2.png");
		fruit->setPosition(nity_po);
		this->addChild(fruit, 105);

		auto app1 = FadeTo::create(2.0f, 0);
		fruit->runAction(app1);

		//애정도의 증가와 포만도의 증가
		if (eat_nity == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");
			int food = UserDefault::getInstance()->getIntegerForKey("first_food");

			if (love + 4 <= 100)
				love += 4;
			else
				love = 100;

			if (food + 4 <= 100)
				food += 4;
			else
				food = 100;


			UserDefault::getInstance()->setIntegerForKey("first_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("first_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 2)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love");
			int food = UserDefault::getInstance()->getIntegerForKey("second_food");

			if (love + 4 <= 100)
				love += 4;
			else
				love = 100;

			if (food + 4 <= 100)
				food += 4;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("second_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 3)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love");
			int food = UserDefault::getInstance()->getIntegerForKey("third_food");

			if (love + 4 <= 100)
				love += 4;
			else
				love = 100;

			if (food + 4 <= 100)
				food += 4;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("third_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", love);
			UserDefault::getInstance()->flush();
		}
	}
	else
		anno_slow2("img/ann_dess1.png");

}

//바나나
void inlife::dessert3(Ref* pSender)
{
	auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
	auto nity_po = nity_pic->getPosition();

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::close_menu_de, this, (Ref*)20));
	runAction(action1);

	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert3");

	if (dessert > 0)
	{
		//효과음
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_heart.wav");

		anno_slow2("img/ann_happy.png");

		dessert -= 1;
		UserDefault::getInstance()->setIntegerForKey("dessert3", dessert);
		UserDefault::getInstance()->flush();

		auto fruit = Sprite::create("img/icon_fruit3.png");
		fruit->setPosition(nity_po);
		this->addChild(fruit, 105);

		auto app1 = FadeTo::create(2.0f, 0);
		fruit->runAction(app1);

		//애정도의 증가와 포만도의 증가
		if (eat_nity == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");
			int food = UserDefault::getInstance()->getIntegerForKey("first_food");

			if (love + 5 <= 100)
				love += 5;
			else
				love = 100;

			if (food + 5 <= 100)
				food += 5;
			else
				food = 100;


			UserDefault::getInstance()->setIntegerForKey("first_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("first_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 2)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love");
			int food = UserDefault::getInstance()->getIntegerForKey("second_food");

			if (love + 5 <= 100)
				love += 5;
			else
				love = 100;

			if (food + 5 <= 100)
				food += 5;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("second_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 3)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love");
			int food = UserDefault::getInstance()->getIntegerForKey("third_food");

			if (love + 5 <= 100)
				love += 5;
			else
				love = 100;

			if (food + 5 <= 100)
				food += 5;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("third_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", love);
			UserDefault::getInstance()->flush();
		}
	}
	else
		anno_slow2("img/ann_dess1.png");

}

//파인애플
void inlife::dessert4(Ref* pSender)
{
	auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
	auto nity_po = nity_pic->getPosition();

	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::close_menu_de, this, (Ref*)20));
	runAction(action1);

	int dessert = UserDefault::getInstance()->getIntegerForKey("dessert4");

	if (dessert > 0)
	{
		//효과음
		sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
		if (sound2 == 0)
			float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/item_heart.wav");

		anno_slow2("img/ann_happy.png");

		dessert -= 1;
		UserDefault::getInstance()->setIntegerForKey("dessert4", dessert);
		UserDefault::getInstance()->flush();

		auto fruit = Sprite::create("img/icon_fruit4.png");
		fruit->setPosition(nity_po);
		this->addChild(fruit, 105);

		auto app1 = FadeTo::create(2.0f, 0);
		fruit->runAction(app1);

		//애정도의 증가와 포만도의 증가
		if (eat_nity == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");
			int food = UserDefault::getInstance()->getIntegerForKey("first_food");

			if (love + 6 <= 100)
				love += 6;
			else
				love = 100;

			if (food + 6 <= 100)
				food += 6;
			else
				food = 100;


			UserDefault::getInstance()->setIntegerForKey("first_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("first_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 2)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love");
			int food = UserDefault::getInstance()->getIntegerForKey("second_food");

			if (love + 6 <= 100)
				love += 6;
			else
				love = 100;

			if (food + 6 <= 100)
				food += 6;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("second_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("second_food", love);
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 3)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love");
			int food = UserDefault::getInstance()->getIntegerForKey("third_food");

			if (love + 6 <= 100)
				love += 6;
			else
				love = 100;

			if (food + 6 <= 100)
				food += 6;
			else
				food = 100;

			UserDefault::getInstance()->setIntegerForKey("third_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("third_food", love);
			UserDefault::getInstance()->flush();
		}
	}
	else
		anno_slow2("img/ann_dess1.png");

}

//간식 메뉴창을 닫는다.
void inlife::close_menu_de(Ref* pSender)
{
	onstate2 = 1;

	sound_click();

	auto state1 = (Sprite*)this->getChildByName("menu_des");
	this->removeChild(state1);
	auto state2 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state2);
    auto state3 = (Sprite*)this->getChildByName("fr_la1");
	this->removeChild(state3);
	auto state4 = (Sprite*)this->getChildByName("fr_la2");
	this->removeChild(state4);
	auto state5 = (Sprite*)this->getChildByName("fr_la3");
	this->removeChild(state5);
	auto state6 = (Sprite*)this->getChildByName("fr_la4");
	this->removeChild(state6);
}

//간식을 주면 어떤가에 대한 설명
void inlife::explain_dess(Ref* pSender)
{
	auto state1 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state1);

		sound_click();

		auto me_main = MenuItemImage::create("img/explain_dessert.png", "img/explain_dessert.png", CC_CALLBACK_1(inlife::simple_close, this));
		auto menu = Menu::create(me_main, NULL);
		this->addChild(menu, 107, "simplat");
}


//니티를 터치하면 가끔씩 행복해 하는거 설명
void inlife::nitytouch_explain(Ref* pSender)
{
	auto state1 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state1);

	sound_click();

	auto me_main = MenuItemImage::create("img/explain_nitytouch.png", "img/explain_nitytouch.png", CC_CALLBACK_1(inlife::simple_close, this));
	auto menu = Menu::create(me_main, NULL);
	this->addChild(menu, 107, "simplat");
}



//니티의 추가적인 상태를 보여준다
void inlife::nity_state2(Ref* pSender)
{
	//다른 메뉴들의 터치 방지
	onstate2 = 1;

	//기본 상태창을 닫아준다.
	auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::close_state, this, (Ref*)20));
	runAction(action1);

	//배경 설정
	auto back = Sprite::create("img/img_state2.png");
	back->setScale(0.9);
	back->setAnchorPoint(Point(0, 1));
	back->setPosition(24, 304);
	back->setOpacity(230);
	this->addChild(back, 100, "nity_state2_back");

	//메뉴 버튼을 생성한다.
	auto close = MenuItemImage::create("img/icon_exit.png", "img/icon_exit_se.png", CC_CALLBACK_1(inlife::close_nity_state2, this));
	auto sham = MenuItemImage::create("img/icon_sham.png", "img/icon_sham.png", CC_CALLBACK_1(inlife::sham, this));
	auto shower = MenuItemImage::create("img/icon_shower.png", "img/icon_shower.png", CC_CALLBACK_1(inlife::shower, this));
	auto dissert = MenuItemImage::create("img/icon_water.png", "img/icon_water.png", CC_CALLBACK_1(inlife::dessert, this));
	auto sleep = MenuItemImage::create("img/icon_sleep.png", "img/icon_sleep.png", CC_CALLBACK_1(inlife::sleep, this));
	auto f1 = MenuItemImage::create("img/icon_f1.png", "img/icon_f1.png", CC_CALLBACK_1(inlife::nitytouch_explain, this));
	auto menu = Menu::create(close, sham, shower, dissert, sleep, f1, NULL);
	close->setPosition(191, 120);
	close->setScale(1.5);
	sham->setPosition(-100, -80);
	sham->setScale(1.4);
	shower->setPosition(-40, -80);
	shower->setScale(1.4);
	f1->setPosition(182, -120);
	dissert->setPosition(20, -80);
	dissert->setScale(1.1);
	sleep->setPosition(80, -80);
	sleep->setScale(1.1);
	this->addChild(menu, 101, "nity_state2");

	//니티 이미지를 가져온다
	int first = UserDefault::getInstance()->getIntegerForKey("first", 0);
	int second = UserDefault::getInstance()->getIntegerForKey("second", 0);
	int third = UserDefault::getInstance()->getIntegerForKey("third", 0);
	int first_lv = UserDefault::getInstance()->getIntegerForKey("first_lv");
	int second_lv = UserDefault::getInstance()->getIntegerForKey("second_lv");
	int third_lv = UserDefault::getInstance()->getIntegerForKey("third_lv");
	char* now_img;
	int nity_code;
	int now_lv;

	if (first == 1)
		first_img = "img/sla_blue_d1.png";
	else if (first == 2)
		first_img = "img/sla_pink_d1.png";
	else if (first == 3)
		first_img = "img/sla_green_d1.png";
	else if (first == 4 && first_lv <7)
		first_img = "img/lity_d1.png";
	else if (first == 4)
		first_img = "img/lityw1_d1.png";

	if (second == 1)
		second_img = "img/sla_blue_d1.png";
	else if (second == 2)
		second_img = "img/sla_pink_d1.png";
	else if (second == 3)
		second_img = "img/sla_green_d1.png";
	else if (second == 4 && second_lv <7)
		second_img = "img/lity_d1.png";
	else if (second == 4)
		second_img = "img/lityw1_d1.png";

	if (third == 1)
		third_img = "img/sla_blue_d1.png";
	else if (third == 2)
		third_img = "img/sla_pink_d1.png";
	else if (third == 3)
		third_img = "img/sla_green_d1.png";
	else if (third == 4 && third_lv <7)
		third_img = "img/lity_d1.png";
	else if (third == 4)
		third_img = "img/lityw1_d1.png";

	if (eat_nity == 1)
	{
		now_img = first_img;
		now_lv = first_lv;
		nity_code = first;
	}
	else if (eat_nity == 2)
	{
		now_img = second_img;
		now_lv = second_lv;
		nity_code = second;
	}
	else if (eat_nity == 3)
	{
		now_img = third_img;
		now_lv = third_lv;
		nity_code = third;
	}

	auto nity = Sprite::create(now_img);
	nity->setPosition(240, 160);
	nity->setScale(1.3);
	this->addChild(nity, 101, "nity_pic");


	//니티의 움직임 부여
	auto animation8 = Animation::create();
	animation8->setDelayPerUnit(0.6);

	for (int i = 1; i < 4; i++)
	{
		if (nity_code == 1)
			animation8->addSpriteFrameWithFileName(String::createWithFormat("img/sla_blue_d%d.png", i)->getCString());
		else if (nity_code == 2)
			animation8->addSpriteFrameWithFileName(String::createWithFormat("img/sla_pink_d%d.png", i)->getCString());
		else if (nity_code == 3)
			animation8->addSpriteFrameWithFileName(String::createWithFormat("img/sla_green_d%d.png", i)->getCString());
		else if (nity_code == 4 && now_lv < 7)
			animation8->addSpriteFrameWithFileName(String::createWithFormat("img/lity_d%d.png", i)->getCString());
		else if (nity_code == 4)
			animation8->addSpriteFrameWithFileName(String::createWithFormat("img/lityw1_d%d.png", i)->getCString());
	}

	auto action_0 = RepeatForever::create(Animate::create(animation8));
	nity->runAction(action_0);

	int dirt_time = 0;

	if (eat_nity==1)
		dirt_time = UserDefault::getInstance()->getIntegerForKey("dirt_time1", 0);
	else if (eat_nity == 2)
		dirt_time = UserDefault::getInstance()->getIntegerForKey("dirt_time2", 0);
	else if (eat_nity == 3)
		dirt_time = UserDefault::getInstance()->getIntegerForKey("dirt_time3", 0);

	//먼지의 추가
	int fast_time = time(NULL);
	auto nity_po = nity->getPosition();

	auto dirt = Sprite::create("img/dirt.png");

	//시간에 따른 먼지의 농도
	if ((fast_time - dirt_time) > 28800)
	{
		dirt->setOpacity(200);
		dirt_op = 190;
		anno_slow2("img/ann_wash6.png");
	}
	else if ((fast_time - dirt_time) > 14400)
	{
		dirt->setOpacity(100);
		dirt_op = 90;
		anno_slow2("img/ann_wash6.png");
	}
	else
		dirt->setOpacity(0);

	dirt->setPosition(nity_po);
	this->addChild(dirt, 102, "dirt");

	//거품 추가
	auto bubble = Sprite::create("img/bubble.png");
	bubble->setOpacity(0);
	bubble->setPosition(nity_po);
	this->addChild(bubble, 103, "bubble");

	//washer 추가
	auto washer = Sprite::create("img/icon_washer.png");
	washer->setPosition(-50, -50);
	this->addChild(washer, 105, "washer");

}


//<잠재우기 시스템>
void inlife::sleep(Ref* pSender)
{
	sound_click();

	int fast_time = time(NULL);
	int sleep_time;

	if (eat_nity == 1)
		sleep_time = UserDefault::getInstance()->getIntegerForKey("sleep_time1", 0);
	else if (eat_nity == 2)
		sleep_time = UserDefault::getInstance()->getIntegerForKey("sleep_time2", 0);
	else if (eat_nity == 3)
		sleep_time = UserDefault::getInstance()->getIntegerForKey("sleep_time3", 0);


	if ((fast_time - sleep_time) >= 7200)
	{

		auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
		auto nity_x = nity_pic->getPositionX();
		auto nity_y = nity_pic->getPositionY();

		auto apr1 = ParticleSystemQuad::create("par/sleep.plist");
		apr1->setOpacity(140);
		apr1->setScale(0.8);
		apr1->setPosition(nity_x, nity_y);
		this->addChild(apr1, 105);

		anno_slow2("img/ann_sleep2.png");

		if (eat_nity == 1)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("first_love");
			if (love + 15 <= 100)
				love += 15;
			else
				love = 100;

			UserDefault::getInstance()->setIntegerForKey("first_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("sleep_time1", time(NULL));
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 2)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("second_love");
			if (love + 15 <= 100)
				love += 15;
			else
				love = 100;

			UserDefault::getInstance()->setIntegerForKey("second_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("sleep_time2", time(NULL));
			UserDefault::getInstance()->flush();
		}
		else if (eat_nity == 3)
		{
			int love = UserDefault::getInstance()->getIntegerForKey("third_love");
			if (love + 15 <= 100)
				love += 15;
			else
				love = 100;

			UserDefault::getInstance()->setIntegerForKey("third_love", love);
			UserDefault::getInstance()->flush();

			UserDefault::getInstance()->setIntegerForKey("sleep_time3", time(NULL));
			UserDefault::getInstance()->flush();
		}
	}
	//잠자기의 시간이 충족되지 않은경우
	else
	{
		anno_slow2("img/ann_sleep1.png");
	}

}





//<목욕 시스템>
//닦아 주기
void inlife::sham(Ref* pSender)
{
	sound_click();

	auto dirt = (Sprite*)this->getChildByName("dirt");
	auto dirt_po2 = dirt->getOpacity();

	if (onstate2 == 1)
	{
		if (dirt_po2 > 0)
		{
			anno_slow2("img/ann_wash1.png");
			onstate2 = 2;
		}
		else
			anno_slow2("img/ann_wash3.png");
	}
}

//샤워
void inlife::shower(Ref* pSender)
{
	sound_click();

	if (onstate2 == 1)
	{
		auto bubble = (Sprite*)this->getChildByName("bubble");
		auto bubble_po = bubble->getOpacity();
		auto dirt = (Sprite*)this->getChildByName("dirt");
		auto dirt_po = dirt->getOpacity();
		auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
		auto nity_x = nity_pic->getPositionX();
		auto nity_y = nity_pic->getPositionY();

		if (bubble_po > 0)
		{
			sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
			if (sound2 == 0)
				float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/water.ogg");

			auto apr1 = ParticleSystemQuad::create("par/givewater.plist");
			apr1->setOpacity(140);
			apr1->setScale(0.6);
			apr1->setPosition(nity_x, nity_y + 60);
			this->addChild(apr1, 105);

			auto del_bubble = FadeOut::create(2.0);
			auto action1 = CallFunc::create(CC_CALLBACK_0(inlife::wash_close, this, (Ref*)20));
			auto action2 = DelayTime::create(1.0);
			auto action3 = Sequence::create(del_bubble, action2, action1, NULL);
			bubble->runAction(action3);
		}
		else if (dirt_po > 0)
			anno_slow2("img/ann_wash5.png");
		else
			anno_slow2("img/ann_wash3.png");
	}
}


//씻겨주기가 모두 끝난경우
void inlife::wash_close(Ref* pSender)
{
	sound2 = UserDefault::getInstance()->getIntegerForKey("sound", 0);
	if (sound2 == 0)
		float sound_0 = SimpleAudioEngine::getInstance()->playEffect("sound/Message.wav");

	auto nity_pic = (Sprite*)this->getChildByName("nity_pic");
	auto nity_x = nity_pic->getPositionX();
	auto nity_y = nity_pic->getPositionY();

	auto apr1 = ParticleSystemQuad::create("par/item_power.plist");
	apr1->setOpacity(140);
	apr1->setScale(0.8);
	apr1->setPosition(nity_x, nity_y);
	this->addChild(apr1, 105);

	anno_slow2("img/ann_wash4.png");

	//애정도의 증가와 씻어진 시간 저장
	if (eat_nity == 1)
	{
		int love = UserDefault::getInstance()->getIntegerForKey("first_love");
		if (love + 20 <= 100)
			love += 20;
		else
			love = 100;

		UserDefault::getInstance()->setIntegerForKey("first_love", love);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("dirt_time1", time(NULL));
		UserDefault::getInstance()->flush();
	}
	else if (eat_nity == 2)
	{
		int love = UserDefault::getInstance()->getIntegerForKey("second_love");
		if (love + 20 <= 100)
			love += 20;
		else
			love = 100;

		UserDefault::getInstance()->setIntegerForKey("second_love", love);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("dirt_time2", time(NULL));
		UserDefault::getInstance()->flush();
	}
	else if (eat_nity == 3)
	{
		int love = UserDefault::getInstance()->getIntegerForKey("third_love");
		if (love + 20 <= 100)
			love += 20;
		else
			love = 100;

		UserDefault::getInstance()->setIntegerForKey("third_love", love);
		UserDefault::getInstance()->flush();

		UserDefault::getInstance()->setIntegerForKey("dirt_time3", time(NULL));
		UserDefault::getInstance()->flush();
	}


}

//니티의 제2 상태창의 제거
void inlife::close_nity_state2(Ref* pSender)
{
	sound_click();
	
	onstate2 = 0;
	touchenable = 0;

	//이미지의 제거
	auto state1 = (Sprite*)this->getChildByName("nity_state2_back");
	this->removeChild(state1);
	auto state2 = (Sprite*)this->getChildByName("nity_state2");
	this->removeChild(state2);
	auto state3 = (Sprite*)this->getChildByName("nity_pic");
	this->removeChild(state3);
	auto state4 = (Sprite*)this->getChildByName("dirt");
	this->removeChild(state4);
	auto state5 = (Sprite*)this->getChildByName("washer");
	this->removeChild(state5);
	auto state6 = (Sprite*)this->getChildByName("bubble");
	this->removeChild(state6);
	auto state7 = (Sprite*)this->getChildByName("menu_des");
	this->removeChild(state7);
	auto state8 = (Sprite*)this->getChildByName("simplat");
	this->removeChild(state8);

	auto state9 = (Sprite*)this->getChildByName("fr_la1");
	this->removeChild(state9);
	auto state10 = (Sprite*)this->getChildByName("fr_la2");
	this->removeChild(state10);
	auto state11 = (Sprite*)this->getChildByName("fr_la3");
	this->removeChild(state11);
	auto state12 = (Sprite*)this->getChildByName("fr_la4");
	this->removeChild(state12);
}