#include "HelloWorldScene.h"

USING_NS_CC;

#define ENEMY_CATEGORY 1
#define BULLET_CATEGORY 1 << 1
#define ENEMY_TAG 100
#define PLAYER_TAG 101

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	//////////////////////////////////create Boundary

	auto boundBox = PhysicsBody::createEdgeBox(Director::getInstance()->getVisibleSize(),PhysicsMaterial(0.0f,1.0f,0.0f),5.0f);
	auto edgeNode = Node::create();

	edgeNode->setPhysicsBody(boundBox);
	edgeNode->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	this->addChild(edgeNode);
	
	////////////////////////////////////////////create Bullet 
	
	bulletSpr = Sprite::create("bullet.png");
	auto bulletPhysics = PhysicsBody::createCircle(16.0f,PhysicsMaterial(0.0f,1.0f,0.0f));

	bulletPhysics->setMass(0);
	bulletPhysics->setCategoryBitmask(BULLET_CATEGORY);

	bulletSpr->setPhysicsBody(bulletPhysics);
	bulletSpr->setPosition(Vec2(visibleSize.width / 2, 40));
	bulletSpr->setVisible(false);

	this->addChild(bulletSpr);

	/////////////////////////////
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
	
	///////////////////////////add TouchListner

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		if (bGameStarted) return true;

		Vec2 touchLoc = touch->getLocation();

		float bulletSpeed = 400.0f;

		Vec2 vVel = touchLoc - bulletSpr->getPosition();
		vVel.normalize();
		vVel *= bulletSpeed;

		bulletSpr->getPhysicsBody()->setVelocity(vVel);
		bulletSpr->setVisible(true);

		scheduleUpdate();

		bGameStarted = true;
		return true;
	};
	touchListener->onTouchMoved = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		if (target->getTag() == PLAYER_TAG)
		{ 
			target->getPhysicsBody()->setVelocity(touch->getDelta());
		}

	};
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//////////////////////////////add PlayerSpr

	playerSpr = Sprite::create("player.png");

	auto playerPhysics = PhysicsBody::createCircle(playerSpr->getContentSize().width / 2,PhysicsMaterial(0.0f,1.0f,0.0f));

	playerPhysics->setDynamic(true);
	playerSpr->setPhysicsBody(playerPhysics);
	
	playerSpr->setPosition(visibleSize.width / 2, 20.0f);
	playerSpr->setTag(PLAYER_TAG);

	addChild(playerSpr);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), playerSpr);

	bGameStarted = false;
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt)
{
	if (times > 3.0f) {
		for (int i = 0; i < 5; i++)
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();
			auto enemy = Enemy::create();
			int ranX = RandomHelper::random_int<int>(0, visibleSize.width);
			enemy->setPosition(ranX, visibleSize.height);

			Vec2 vVel = playerSpr->getPosition() - enemy->getPosition();

			vVel.normalize(); 
			int speed = RandomHelper::random_int<int>(150, 250);
			vVel *= speed;
			
			enemy->getPhysicsBody()->setVelocity(vVel);
			enemy->setTag(ENEMY_TAG);

			addChild(enemy);
		}	
		times = 0.0f;
	}
	else 
		times += dt;

	for (int i = 0; i < _children.size(); i++) {
		auto child = _children.at(i);
		if (child->getTag() == ENEMY_TAG) {
			if (child->getPosition().y < 0)
				this->removeChild(child);
		}
	}
}

bool Enemy::init()
{
	if (!Node::init()) return false;
	
	spr = Sprite::create("enemy.png");
	
	auto enemyPhysics = PhysicsBody::createCircle(spr->getContentSize().width / 2);

	enemyPhysics->setCategoryBitmask(ENEMY_CATEGORY);
	enemyPhysics->setCollisionBitmask(0);
	this->setPhysicsBody(enemyPhysics);

	addChild(spr);

	return true;
}
