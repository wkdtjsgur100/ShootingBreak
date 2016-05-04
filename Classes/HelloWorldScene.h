#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Enemy : public cocos2d::Node
{
	cocos2d::Sprite* spr;
	float vel;
public:
	float getSpeed() const { return vel; }
	virtual bool init() override;
	CREATE_FUNC(Enemy);
};

class HelloWorld : public cocos2d::Layer
{
	cocos2d::Sprite* bulletSpr;
	float times;
	cocos2d::Sprite* playerSpr;
	bool bGameStarted;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
