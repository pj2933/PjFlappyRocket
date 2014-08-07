#include "HelloWorldScene.h"
#include "cocos-ext.h"
#include <iostream>

#include <fstream>

#include <string>



using namespace std;



USING_NS_CC;
USING_NS_CC_EXT;

CCScene* HelloWorld::scene()
{
	//startFlag=startnum;
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    if ( !CCLayer::init() )
    {
        return false;
    }

	
	screenSize=CCDirector::sharedDirector()->getVisibleSize();
	
	//Ԥ�ȶ�ȡͼƬ����
	CCImage *img1=new CCImage();
	
	img1->initWithImageFileThreadSafe("rocket.png");
	
	Rocketdata=img1->getData();
	
	CCImage *img2=new CCImage();
	
	img2->initWithImageFileThreadSafe("rock_1.png");
	
	Rockdata_1=img2->getData();

	CCImage *img3=new CCImage();
	 
	img3->initWithImageFileThreadSafe("rock_2.png");
	
	Rockdata_2=img3->getData();
	
	 
	startInterface();
    return true;
}

//��ʼ����
void HelloWorld::startInterface(){

	endFlag=0;//���ý������Ϊ0
	
	start=CCSprite::create("start.jpg");

	start->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	
	addChild(start,100);
	
	CCScale9Sprite* btnNormal = CCScale9Sprite::create("plane.png");  
  
    /* ���״̬�µİ�ťͼƬ */  
    CCScale9Sprite* btnDown = CCScale9Sprite::create("plane.png");  
  
    /* ��ť���� */  
    CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
  
    /* ��ť�Ĵ�С����ݱ����Զ����� */  
	controlBtn = CCControlButton::create(title, btnNormal);
  
    /* ���ð�ť����ʱ��ͼƬ */  
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);  

    controlBtn->setPosition(ccp(screenSize.width/2,screenSize.height/2+50));

	//���ð�ť�Ļص�����
	controlBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::startcallback),CCControlEventTouchUpInside);

	CCActionInterval * scale1 = CCScaleTo::create(0.5,1.1);
	
	CCActionInterval *scale2=CCScaleTo::create(0.1,1);
    
	//�Ŵ���С�˶��ŵ��˶�������
	CCFiniteTimeAction*seq =CCSequence::create(scale1,scale2,NULL);
	
	CCFiniteTimeAction *repeat = CCRepeatForever::create((CCActionInterval* )seq);
	
	//�ظ��Ŵ���С�˶�
	controlBtn->runAction(repeat);

    start->addChild(controlBtn);  
	
	controlBtn->setPreferredSize(CCSize(179, 179));
	//initWorld();
}

//��ʼ��ť�Ļص����������п�ʼ�������Լ��رտ�ʼ���棬������Ϸ����
void HelloWorld::startcallback(CCObject *object, CCControlEvent controlEvent){

	controlBtn->removeFromParent();
	
	CCSprite *startplane=CCSprite::create("plane.png");
	
	startplane->setPosition(ccp(screenSize.width/2,screenSize.height/2+50));
	
	start->addChild(startplane,2);
	
	//��ת����
	CCAction *rotate=CCRotateTo::create(0.8f,900);
	
	startplane->runAction(rotate);
	
	//��ʱ0.8sֻ����һ��,��ʼ������
	this->scheduleOnce(schedule_selector(HelloWorld::initWorld),0.8f);
}

//��ʼ����Ϸ��ʱ��
void HelloWorld::initWorld(float dt){
	start->removeFromParent();//��ʼ����remove��
	
	//һϵ��ֵ��ʼ��
	best=0;
	
	score=0;
	
	flagY=-1;
	
	jumpSpeed=0;
	
	scoreSprite=CCSprite::create();
	
	scoreSprite->setPosition(ccp(screenSize.width/2-25,screenSize.height-100));
	
	addChild(scoreSprite,5);
	
	//��ʾ����Ϊ0
	scoreLable = CCLabelAtlas::create("0", "shuzi2.png", 48, 46, '0');
	
	scoreLable->setPosition(ccp(0,0));
	
	scoreSprite->addChild(scoreLable);
	
	scoreLable->setScale(1.0f); 
	
	//��ʱ��Ϊ�������
	srand(time(NULL));

	//�����ͣ��ť
	addPauseButton();
    
	//��ӻ��
	addRocket();
	
	//��ӵײ�
	addGround();
	
	//����ƶ�
	addCloud();
	
	//��ӱ���
	addBackground();
    
	//���update���ڼ����ײ
	scheduleUpdate();
	
	//rocketģ����������
	schedule(schedule_selector(HelloWorld::fall),1/60);
	
	//��ʱ����ϰ�
	this->schedule(schedule_selector(HelloWorld::addBar),1.7);
	
	//������Ļ���Ե��
	setTouchEnabled(true);//��Ļ���Ե��

}

//�����ͣ��ť
void HelloWorld::addPauseButton(){

	PauseButton=CCSprite::create();
	
	PauseButton->setPosition(ccp(60,screenSize.height-50));
	
	addChild(PauseButton,6);
	
	CCScale9Sprite* btnNormal = CCScale9Sprite::create("pause_off.png");  
  
    /* ���״̬�µİ�ťͼƬ */  
    CCScale9Sprite* btnDown = CCScale9Sprite::create("pause_on.png");  
  
    /* ��ť���� */  
    CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
  
    /* ��ť�Ĵ�С����ݱ����Զ����� */  
    CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);  
  
    /* ���ð�ť����ʱ��ͼƬ */  
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);  
   
	controlBtn->setPosition(ccp(0, 0));
	
	controlBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::pauseCallBack),CCControlEventTouchUpInside);//�ص������������㰴ť����ʱ
   
	PauseButton->addChild(controlBtn);  
	
	controlBtn->setPreferredSize(CCSize(117, 93)); 
}

//��ͣ��ť�Ļص�����
void HelloWorld::pauseCallBack(CCObject *object, CCControlEvent controlEvent) {
	//�����Ϸ��������ͣ������
	if(endFlag==1){
		return;
	}
	
	//��ͣ����������
	PauseButton->setVisible(false);
	
	CCArray *children=CCArray::create();
	
	//��ȡ�ӽڵ�
	children=this->getChildren();
	
	CCObject *temp=NULL;
	
	//�������ӽڵ�ļ�ʱ���Ͷ�����ͣ
	CCARRAY_FOREACH(children,temp )  
	{  
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);  
		sprite->pauseSchedulerAndActions();
	}
	
	this->pauseSchedulerAndActions();
	
	//������Ļ���ɴ���
	this->setTouchEnabled(false);
	
	//�����ͣ����
	addPauseInterface();
}

//�����ͣ����
void HelloWorld::addPauseInterface(){
	
	CCSprite *ButtonSprite=CCSprite::create();
	
	ButtonSprite->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	
	//����tagΪ�˺����ȡ�������
	addChild(ButtonSprite,10,10);
	
	CCScale9Sprite* btnNormal = CCScale9Sprite::create("play_off.png");  
	
	CCScale9Sprite* btnDown = CCScale9Sprite::create("play_on.png");  
    
	CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
    
	CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);  
    
	controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);  
    
	controlBtn->setPosition(ccp(-100, -80));
	
	controlBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::playCallBack),CCControlEventTouchUpInside);
    
	ButtonSprite->addChild(controlBtn);  
	
	controlBtn->setPreferredSize(CCSize(117, 93)); 

	CCScale9Sprite* btnNormal2 = CCScale9Sprite::create("again_off.png");  
	
	CCScale9Sprite* btnDown2 = CCScale9Sprite::create("again_on.png");  
    
	CCLabelTTF *title2 = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
    
	CCControlButton* controlBtn2 = CCControlButton::create(title2, btnNormal2);  
    
	controlBtn2->setBackgroundSpriteForState(btnDown2, CCControlStateSelected);  
    
	controlBtn2->setPosition(ccp(100, -80));
	
	controlBtn2->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::againCallBack),CCControlEventTouchUpInside);
    
	ButtonSprite->addChild(controlBtn2);  
	
	controlBtn2->setPreferredSize(CCSize(117, 93)); 
}

//�������Ļص�����
void HelloWorld::againCallBack(CCObject *object, CCControlEvent controlEvent){
	//���������0
	endFlag=0;
	
	CCScene *mScene = HelloWorld::scene();
	
	CCDirector::sharedDirector()->replaceScene(mScene);
	//init();
}

//�������м��Ļص�����
void HelloWorld::playCallBack(CCObject *object, CCControlEvent controlEvent){
	//����ͣ����remove
	this->getChildByTag(10)->removeFromParent();
	
	CCArray *children=CCArray::create();
	
	children=this->getChildren();
	
	CCObject *temp=NULL;
	
	CCARRAY_FOREACH(children,temp )  
	{  
	
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);  
		//�ָ����о���Ķ����ͼ�ʱ��
		sprite->resumeSchedulerAndActions();
	
	}
	 
	this->resumeSchedulerAndActions();
	
	this->setTouchEnabled(true);
	
	PauseButton->setVisible(true);
}

//��ӱ���
void HelloWorld::addBackground(){
	CCSprite *background1=CCSprite::create("bg.png");
	CCPoint pt=ccp(screenSize.width/2,screenSize.height/2);
	background1->setPosition(pt);
	addChild(background1,0);
}

//��ӻ��
void HelloWorld::addRocket(){

	rocket = CCSprite::create("rocket.png");
	rocket->setPosition(ccp(screenSize.width/5,screenSize.height*2/3));
	addChild(rocket,2);
}

//����ϰ���
void HelloWorld::addBar(float dt){
	
	//�ϰ�����˶�ʱ����2.9~3.3s֮�����
	double time=(rand()%(330-(290))+(290))/100.0;
	//dy��ʾ������ϰ����y������
	int dy=rand()%(200-(-200))+(-200);
	//uy��ʾ������ϰ����y������
	int uy=dy+rand()%(970-(888))+(888);
	
	//����dy����ż�������ϰ���ͼƬ����
	if(dy%2==1)
	{
		down_bar = CCSprite::create("rock_1.png");
		
		down_bar->setPosition(ccp(screenSize.width*3/2,dy));
		
		down_bar->setTag(1);
		
		CCAction * DBmoveto=CCMoveTo::create(time,ccp(-screenSize.width*3/4,dy));
		
		//�������������remove�Լ��ĺ���
		CCCallFuncN  * funcall= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		
		CCArray * test= CCArray::create();
		
		test->addObject(DBmoveto);
		
		test->addObject(funcall);
		
		CCFiniteTimeAction * seq = CCSequence::create(test);
		
		down_bar->runAction(seq);
		

		up_bar=CCSprite::create("rock_1.png");
		
		up_bar->setPosition(ccp(screenSize.width*3/2+200,uy));
		
		//����tag�������ײʱ��Ҫ�õ�
		up_bar->setTag(1);
		
		CCAction *UBmoveto=CCMoveTo::create(time,ccp(-screenSize.width*3/4+200,uy));
		
		CCCallFuncN  * funcall2= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		
		CCArray * test2= CCArray::create();
		
		test2->addObject(UBmoveto);
		
		test2->addObject(funcall2);
		
		CCFiniteTimeAction * seq2 = CCSequence::create(test2);
		
		up_bar->runAction(seq2);
		
		addChild(down_bar,2);
		
		addChild(up_bar,2);
	}
	else
	{
		down_bar = CCSprite::create("rock_2.png");
		down_bar->setPosition(ccp(screenSize.width*3/2,dy));
		down_bar->setTag(2);
		CCAction * DBmoveto=CCMoveTo::create(time,ccp(-screenSize.width*3/4,dy));
		CCCallFuncN  * funcall= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		CCArray * test= CCArray::create();
		test->addObject(DBmoveto);
		test->addObject(funcall);
		CCFiniteTimeAction * seq = CCSequence::create(test);
		down_bar->runAction(seq);
		
		up_bar=CCSprite::create("rock_2.png");
		up_bar->setPosition(ccp(screenSize.width*3/2+200,uy));
		up_bar->setTag(2);
	
		CCAction *UBmoveto=CCMoveTo::create(time,ccp(-screenSize.width*3/4+200,uy));
		CCCallFuncN  * funcall2= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		CCArray * test2= CCArray::create();
		test2->addObject(UBmoveto);
		test2->addObject(funcall2);
		CCFiniteTimeAction * seq2 = CCSequence::create(test2);
		up_bar->runAction(seq2);

		addChild(down_bar,2);
		addChild(up_bar,2);
	}
	

}

//remove����
void HelloWorld::callbackN(CCNode *sender){
	sender->removeFromParent();
}

//��ӵײ���������mountain�޷�����
void HelloWorld::addGround(){
	CCSprite *ground1 = CCSprite::create("mountain.png");
	CCPoint pt=ccp(screenSize.width/2,30);
	ground1->setPosition(pt);
	CCActionInterval * move1 = CCMoveTo::create(5, ccp(-screenSize.width/2,pt.y));
	CCActionInstant *place=CCPlace::create(pt);
	CCFiniteTimeAction* seq = CCSequence::create(move1,place,NULL);
	CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
	ground1->runAction(repeatForever);
	addChild(ground1,1);

	CCSprite *ground2 = CCSprite::create("mountain.png");
	CCPoint pt2=ccp(screenSize.width*3/2,30);
	ground2->setPosition(pt2);
	CCActionInterval * move2 = CCMoveTo::create(5, ccp(screenSize.width/2,pt2.y));
	CCActionInstant *place2=CCPlace::create(pt2);
	CCFiniteTimeAction* seq2 = CCSequence::create(move2,place2,NULL);
	CCActionInterval * repeatForever2 =CCRepeatForever::create((CCActionInterval* )seq2);
	ground2->runAction(repeatForever2);
	addChild(ground2,1);
}

//��ÿ֡�ı�λ�ã�ģ����������
void HelloWorld::fall(float dt){
	if(flagY==1&&jumpSpeed<=0){
			flagY=-1;
		}
		jumpSpeed-=flagY*2333*dt;
		if(jumpSpeed>0&&flagY!=0){
			rocket->setPosition(ccp(rocket->getPositionX(),rocket->getPositionY()+flagY*jumpSpeed*dt));
		}
}

//ÿ֡���÷����������ײ
void HelloWorld::update(float dt){
	//endFlag=1;
	char s[25];
	sprintf(s, "%d", score); 
	scoreLable->setString(s);
	if(rocket->getPositionY()<=80){
		endgame(50*72+32);
	}
	CCArray *children=CCArray::create();
	children=this->getChildren();
	CCObject *temp=NULL;
	CCARRAY_FOREACH(children,temp )  
	{  
		
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);

		//��ײ��ⷵ�ص�intֵ�����ڴ����ײ��
		int CollidedResult;

		//tagΪ1��3��ʾ��rock_1.png��3��ʾ���Ѿ�������������ϰ�
		if(sprite->getTag() == 1||sprite->getTag()==3)  
		{  
			//tag=1�����ϲ��ϰ���x����<rocket���Ͱ�tag����Ϊ3�����Ҽӷ�
			if((sprite->getPositionY()>400)&&(sprite->getPositionX()<(screenSize.width/5+60))&&(sprite->getTag() == 1)){
				sprite->setTag(3);
				score++;
			}
			//������ײ���
			CollidedResult=isCollided(rocket,"rocket.png",sprite,"rock_1.png");
			
			//����ֵΪ-1��ʾδ��ײ
			if(CollidedResult!=-1)
			{
				//������-1����ײ��������Ϸ
				endgame(CollidedResult);
			}
			
		} 
		else if(sprite->getTag()==2||sprite->getTag()==4){
			CollidedResult=isCollided(rocket,"rocket.png",sprite,"rock_2.png");
			if((sprite->getPositionY()>400)&&(sprite->getPositionX()<(screenSize.width/5))&&(sprite->getTag() == 2)){
				sprite->setTag(4);
				score++;
			}
			if(CollidedResult!=-1)
			{
				endgame(CollidedResult);
			}
			
		}
	}  
	
}

//����Ƿ���ײ
int HelloWorld::isCollided(CCSprite *a,char* aPath,CCSprite *b,char* bPath){
	//�ֱ��ʾABͼ���������ҵ�����
	float ABottom,ATop,ALeft,ARight;
	float BBottom,BTop,BLeft,BRight;

	CCSprite *aa=a;
	CCSprite *bb=b;
	CCSize Asize=aa->getContentSize();
	CCSize Bsize=bb->getContentSize();
	CCPoint Ap=aa->getPosition();
	CCPoint Bp=bb->getPosition();
	//cocos2dx������ϵ�������½�Ϊԭ�㣬��һ�������ϵ�������Ͻ�Ϊԭ��
	ATop=screenSize.height-Ap.y-Asize.height/2;
	ABottom=screenSize.height-Ap.y+Asize.height/2;
	ALeft=Ap.x-Asize.width/2;
	ARight=Ap.x+Asize.width/2;
	BTop=screenSize.height-Bp.y-Bsize.height/2;
	BBottom=screenSize.height-Bp.y+Bsize.height/2;
	BLeft=Bp.x-Bsize.width/2;
	BRight=Bp.x+Bsize.width/2;

	//����ͼƬû�Ӵ������������-1
	if (ALeft > BRight || ATop > BBottom || BLeft > ARight || BTop > ABottom) {
		return -1;
	 }
	
	//�ཻ���ε��������ҵ������Լ�����
	float intersectLeft = ALeft > BLeft ? ALeft : BLeft;
	float intersectTop = ATop >BTop ? ATop : BTop;
	float intersectRight = ARight < BRight ? ARight : BRight;
	float intersectBottom = ABottom < BBottom ? ABottom : BBottom;
	float intersectWidth = intersectRight - intersectLeft;
	float intersectHeight = intersectBottom - intersectTop;

	//�ཻ�ľ��Σ��ڵ�һ�ŵڶ���ͼ��xy��ƫ����
	float thisImageXOffset = intersectLeft - ALeft;
	float thisImageYOffset = intersectTop - ATop;
	float otherImageXOffset = intersectLeft - BLeft;
	float otherImageYOffset = intersectTop - BTop;
	//����������ײ
	int collisiontemp=doPixelCollision(int(Asize.height),(int)Asize.width,(int)Bsize.height,(int)Bsize.width,thisImageXOffset, thisImageYOffset, otherImageXOffset, otherImageYOffset, aPath, bPath, intersectWidth, intersectHeight);
	return collisiontemp;
}

int HelloWorld::doPixelCollision(int firstheight,int firstwidth,int secondheight,int secondwidth,float firstImageXOffset, float firstImageYOffset, float secondImageXOffset, float secondImageYOffset,
   char* Apath, char* Bpath, float intersectWidth, float intersectHeight){
	int Aheight,Awidth,Bheight,Bwidth;
	Aheight=firstheight;
	Awidth=firstwidth;
	Bheight=secondheight;
	Bwidth=secondwidth;
	
	ccColor4B A={0,0,0,0};
	 ccColor4B B={0,0,0,0};
	  
	 //���ȶ�ȡ��ͼƬ������ֵ����Ȼÿ�ζ���̫��
	   unsigned int *Apixel=(unsigned int*)Rocketdata;
	   unsigned int *Bpixel;
	   if(Bpath=="rock_1.png"){
			Bpixel=(unsigned int*)Rockdata_1;
	   }
	   else if(Bpath=="rock_2.png")
	   {
		   Bpixel=(unsigned int*)Rockdata_2;
	   }
	

	int i,j,a,b;
	//ɨ���ཻ�����Ƿ����һ��ABͼ������͸���ĵ㣬�ͱ�ʾ�ཻ
	for(j=firstImageYOffset ,b=secondImageYOffset;j<firstImageYOffset+intersectHeight-1,b<secondImageYOffset+intersectHeight-1;j++,b++)
	for(i=firstImageXOffset ,a=secondImageXOffset;i<firstImageXOffset+intersectWidth-1,a<secondImageXOffset+intersectWidth-1;i++,a++)
	{

		unsigned *Atemp,*Btemp;
		Atemp=Apixel+j*Awidth+i;
		//A.r = *Atemp & 0xff;
		// A.g = (*Atemp >> 8) & 0xff;
       //  A.b = (*Atemp >> 16) & 0xff;
		// A.a=(*Atemp >> 24) ;    
		 
		 Btemp=Bpixel+b*Bwidth+a;
		// B.r = *Btemp & 0xff;
		 //B.g = (*Btemp >> 8) & 0xff;
        // B.b = (*Btemp >> 16) & 0xff;
		 //B.a=(*Btemp >> 24) ;    
		
		 //A��B����͸�����ཻ
		 if(((*Atemp >> 24)& 0xff)!=0&&((*Btemp >> 24)& 0xff)!=0)
		{
			//�����ཻ��
			return j*firstwidth+i;
		}
	}

	   return -1;
}

//������Ϸ
void HelloWorld::endgame(int direction){
	//���������1
	endFlag=1;
	//������ͣ��
	PauseButton->setVisible(false);
	//ֹͣһϵ�ж�ʱ��
	unscheduleUpdate();
	unschedule(schedule_selector(HelloWorld::fall));
	unschedule(schedule_selector(HelloWorld::addBar));

	CCArray *children=CCArray::create();
	children=this->getChildren();
	CCObject *temp=NULL;
	CCARRAY_FOREACH(children,temp )  
	{  
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);
		sprite->stopAllActions();
	}  
	
	//��ȡrocket������
	boomX=rocket->getPositionX();
	boomY=rocket->getPositionY();
	rocket->setVisible(false);
	boom=CCSprite::create();
	
	/*char strL[10];	
	int L=direction;
	sprintf(strL,"%d",L);
	CCMessageBox(strL,"123");
	*/
	//������ײ���ԵĽ�����趨��ը��
	if(direction%72>=65)
		boom->setPosition(ccp(boomX+direction%72-50-50,boomY-(direction/72-30)));
	else if(direction/72>32)
		boom->setPosition(ccp(boomX+direction%72-50,boomY-(direction/72-30)+50));
	else
		boom->setPosition(ccp(boomX+direction%72-50,boomY-(direction/72-30)-50));
	addChild(boom,3);
	CCAnimation *animation = CCAnimation::create();  
//�ӱ����ļ�ϵͳ�м���ͼƬ�ļ���CCSpriteFrame������Ȼ����ӵ�CCAnimation��  
	for (int i = 1; i <=15; i++)  
	{  
		char szImageFileName[128] = {0};
		if(direction%72>=65)
			sprintf(szImageFileName, "fire/fire_%03d.png", i);  
		else if(direction/72>32)
			sprintf(szImageFileName, "fire_down/fire_%03d.png", i);  
		else
			sprintf(szImageFileName, "fire_up/fire_%03d.png", i);  
		animation->addSpriteFrameWithFileName(szImageFileName);
	}  
animation->setDelayPerUnit(0.5f / 15.0f); // �����������15֡���������2��.  
animation->setRestoreOriginalFrame(true); // 14֡������֮�󷵻ص���һ֡  
  
CCAnimate *action = CCAnimate::create(animation);  
boom->runAction(action);  
scoreSprite->setVisible(false);
scheduleOnce(schedule_selector(HelloWorld::addEndInterface),0.5f);

}

//��ӽ�������
void HelloWorld::addEndInterface(float dt){
	EndInterface=CCSprite::create();
	addChild(EndInterface,10);
	
	//��ӼƷְ�
	EndInterface->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	CCSprite* scoreboard=CCSprite::create("scoreBoard.png");
	EndInterface->addChild(scoreboard);
	CCLabelAtlas *endscore=CCLabelAtlas::create("0", "shuzi2.png", 48, 46, '0');
	char s[25];
	sprintf(s, "%d", score); 
	endscore->setString(s);
	scoreboard->addChild(endscore);
	//���ݷ������ȣ����ı������λ��
	if(score<10)
		endscore->setPosition(225,80);
		else if(score>=10&&score<100)
			endscore->setPosition(177,80);
		else
			endscore->setPosition(129,80);
	
		//�ж��Ƿ��ǵ�һ����
	if(isHaveSaveFile()==true)
	{	
		//���ǵ�һ�Σ���xml��ȡ��best ��ֵ
		best=CCUserDefault::sharedUserDefault()->getIntegerForKey("best");
		//�ж�xml��best�͵�ǰscore�Ĵ�С
		if(score>best)
		{
			best=score;
			CCUserDefault::sharedUserDefault()->setIntegerForKey("best",best);
			CCUserDefault::sharedUserDefault()->flush();//д�˶���Ҫ�ύ  
		}
	}
	else
	{
		best=score;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("best",best);
		CCUserDefault::sharedUserDefault()->flush();
	}

	CCLabelAtlas* bestscore=CCLabelAtlas::create("0","shuzi2.png",48,46,'0');
	char ss[25];
	sprintf(ss,"%d",best);
	bestscore->setString(ss);
	scoreboard->addChild(bestscore);
	if(best<10)
	bestscore->setPosition(225,10);
	else if(best>=10&&best<100)
		bestscore->setPosition(177,10);
	else
		bestscore->setPosition(129,10);

	CCSprite*  gameover=CCSprite::create("gameover.png");
	gameover->setPosition(ccp(0,200));
	EndInterface->addChild(gameover);
	//���������
	CCScale9Sprite* btnNormal2 = CCScale9Sprite::create("again_off.png");  
	CCScale9Sprite* btnDown2 = CCScale9Sprite::create("again_on.png");  
    CCLabelTTF *title2 = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
    CCControlButton* controlBtn2 = CCControlButton::create(title2, btnNormal2);  
    controlBtn2->setBackgroundSpriteForState(btnDown2, CCControlStateSelected);  
    controlBtn2->setPosition(ccp(0, -180));
	controlBtn2->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::againCallBack),CCControlEventTouchUpInside);
    EndInterface->addChild(controlBtn2);  
	controlBtn2->setPreferredSize(CCSize(117, 93)); 

}

//����ƶ䣬�ƶ����Ļ�ұ����������ƶ�
void HelloWorld::addCloud(){
	cloud=CCSprite::create("cloud_1.png");
	CCPoint pt=ccp(screenSize.width*5/3,screenSize.height*3/4);
	cloud->setPosition(pt);
	CCActionInterval * move1 = CCMoveTo::create(10, ccp(-screenSize.width*2/3,pt.y));
	CCActionInstant *place=CCPlace::create(ccp(pt.x,pt.y+rand()%(100-(-100))+(-100)));
	CCFiniteTimeAction* seq = CCSequence::create(move1,place,NULL);
	CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
	cloud->runAction(repeatForever);
	addChild(cloud,5);
	cloud_2=CCSprite::create("cloud_2.png");
	CCPoint pt2=ccp(screenSize.width*3/2,screenSize.height/3);
	cloud_2->setPosition(pt2);
	CCActionInterval * move2 = CCMoveTo::create(10, ccp(-screenSize.width*1/2,pt2.y));
	CCActionInstant *place2=CCPlace::create(ccp(pt2.x,pt2.y+rand()%(100-(-100))+(-100)));
	CCFiniteTimeAction* seq2 = CCSequence::create(move2,place2,NULL);
	CCActionInterval * repeatForever2 =CCRepeatForever::create((CCActionInterval* )seq2);
	cloud_2->runAction(repeatForever2);
	addChild(cloud_2,5);
	
}

//����¼�
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	{
		//�������λ�ù���,����Ծ
	if(rocket->getPositionY()>=screenSize.height)
		return;
	flagY=1;
	jumpSpeed=642;
	
	//��Ծʱ���������������Ч��
	CCSprite *fire= CCSprite::create();
	//fire->setPosition(ccp(rocket->getPositionX()-50,rocket->getPositionY()));
	rocket->addChild(fire);
	fire->setPosition(ccp(-25,rocket->getContentSize().height/2));
	CCAnimation *animation = CCAnimation::create();  
//�ӱ����ļ�ϵͳ�м���ͼƬ�ļ���CCSpriteFrame������Ȼ����ӵ�CCAnimation��  
	for (int i = 1; i < 5; i++)  
	{  
		char szImageFileName[128] = {0};  
			sprintf(szImageFileName, "f/%03d.png", i);  
		animation->addSpriteFrameWithFileName(szImageFileName);
	}  
	animation->setDelayPerUnit(0.4f / 4.0f); // �����������14֡���������2.8��.  
	animation->setRestoreOriginalFrame(true); // 14֡������֮�󷵻ص���һ֡  
  
	CCAnimate *action = CCAnimate::create(animation);  
	CCCallFuncN  * funcall= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		CCArray * test= CCArray::create();
		test->addObject(action);
		test->addObject(funcall);
		CCFiniteTimeAction * seq = CCSequence::create(test);
	fire->runAction(seq);
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

bool HelloWorld::isHaveSaveFile(){
    if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("best",0);
        CCUserDefault::sharedUserDefault()->flush();//�ύ
        //        CCLog("�洢�ļ�������,ͷ�ο�ʼ������Ϸ");
        return false;
    }else{
        //        CCLog("�洢�ļ��Ѵ���");
        return true;
    }
}

