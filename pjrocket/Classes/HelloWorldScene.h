#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"B2Sprite.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;
#define RATIO 80.0f

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    //static int startFlag;
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);//点击屏幕事件


	virtual void update(float dt);//每帧用来检测碰撞神马的函数

	//b2World *world;
	
	CCSprite *start;//开始界面
	CCLabelAtlas* scoreLable;//游戏中的分数
	CCSprite *scoreSprite;//存放分数的node
	CCSprite *boom;//爆炸
	CCSprite *rocket;//火箭
	CCSize screenSize;//屏幕尺寸
	CCSprite *cloud,*cloud_2;//两朵云
	CCSprite * down_bar;//下方的障碍物
	CCSprite * up_bar;//上方的障碍物
	int flagY,score,best,endFlag;//flagY为火箭上下的标记；score为分数；best为最高分；endFlag为结束标记
	float jumpSpeed,boomX,boomY;//火箭的速度；爆炸点的x；爆炸点的y（其实是火箭碰撞时的位置）
	CCSprite *PauseButton ,*EndInterface;//暂停按钮，暂停界面
	CCControlButton* controlBtn;//开始按钮
	unsigned char *Rocketdata,*Rockdata_1,*Rockdata_2;//分别表示存放火箭，两种障碍的图片数据
private:
	void addPauseButton();//添加暂停按钮
	void pauseCallBack(CCObject *object, CCControlEvent controlEvent);//暂停键的回调函数
	void playCallBack(CCObject *object, CCControlEvent controlEvent);//继续键的回调函数
	void againCallBack(CCObject *object, CCControlEvent controlEvent);//重来键的回调函数
	void startcallback(CCObject *object, CCControlEvent controlEvent);//开始键的回调函数
	void initWorld(float dt);//初始化世界
	void addRocket();//添加火箭
	void addGround();//添加地面
	void addBar(float dt);//添加障碍
	void addCloud();//添加云朵
	void addBackground();//添加背景
	void callbackN(CCNode *sender);//此回调函数用于一些精灵执行完一系列动作后remove
	int isCollided(CCSprite *a,char* aPath,CCSprite *b,char * bPath);//检测碰撞
	int doPixelCollision(int firstheight,int firstwidth,int secondheight,int secondwidth, float firstImageXOffset, float firstImageYOffset, float secondImageXOffset, float secondImageYOffset,
		char* firstImage, char* secondImage, float intersectWidth, float intersectHeight);//进行像素碰撞检测
	void endgame(int direction);//结束游戏
	void fall(float dt);//火箭自由落体
	void addPauseInterface();//添加暂停界面
	void addEndInterface(float dt);//添加结束界面
	bool isHaveSaveFile();//判断是否第一次保存分数的函数
	void startInterface();//开始界面
	
};


#endif // __HELLOWORLD_SCENE_H__

