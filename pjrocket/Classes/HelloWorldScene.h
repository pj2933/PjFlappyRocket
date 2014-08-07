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

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);//�����Ļ�¼�


	virtual void update(float dt);//ÿ֡���������ײ����ĺ���

	//b2World *world;
	
	CCSprite *start;//��ʼ����
	CCLabelAtlas* scoreLable;//��Ϸ�еķ���
	CCSprite *scoreSprite;//��ŷ�����node
	CCSprite *boom;//��ը
	CCSprite *rocket;//���
	CCSize screenSize;//��Ļ�ߴ�
	CCSprite *cloud,*cloud_2;//������
	CCSprite * down_bar;//�·����ϰ���
	CCSprite * up_bar;//�Ϸ����ϰ���
	int flagY,score,best,endFlag;//flagYΪ������µı�ǣ�scoreΪ������bestΪ��߷֣�endFlagΪ�������
	float jumpSpeed,boomX,boomY;//������ٶȣ���ը���x����ը���y����ʵ�ǻ����ײʱ��λ�ã�
	CCSprite *PauseButton ,*EndInterface;//��ͣ��ť����ͣ����
	CCControlButton* controlBtn;//��ʼ��ť
	unsigned char *Rocketdata,*Rockdata_1,*Rockdata_2;//�ֱ��ʾ��Ż���������ϰ���ͼƬ����
private:
	void addPauseButton();//�����ͣ��ť
	void pauseCallBack(CCObject *object, CCControlEvent controlEvent);//��ͣ���Ļص�����
	void playCallBack(CCObject *object, CCControlEvent controlEvent);//�������Ļص�����
	void againCallBack(CCObject *object, CCControlEvent controlEvent);//�������Ļص�����
	void startcallback(CCObject *object, CCControlEvent controlEvent);//��ʼ���Ļص�����
	void initWorld(float dt);//��ʼ������
	void addRocket();//��ӻ��
	void addGround();//��ӵ���
	void addBar(float dt);//����ϰ�
	void addCloud();//����ƶ�
	void addBackground();//��ӱ���
	void callbackN(CCNode *sender);//�˻ص���������һЩ����ִ����һϵ�ж�����remove
	int isCollided(CCSprite *a,char* aPath,CCSprite *b,char * bPath);//�����ײ
	int doPixelCollision(int firstheight,int firstwidth,int secondheight,int secondwidth, float firstImageXOffset, float firstImageYOffset, float secondImageXOffset, float secondImageYOffset,
		char* firstImage, char* secondImage, float intersectWidth, float intersectHeight);//����������ײ���
	void endgame(int direction);//������Ϸ
	void fall(float dt);//�����������
	void addPauseInterface();//�����ͣ����
	void addEndInterface(float dt);//��ӽ�������
	bool isHaveSaveFile();//�ж��Ƿ��һ�α�������ĺ���
	void startInterface();//��ʼ����
	
};


#endif // __HELLOWORLD_SCENE_H__

