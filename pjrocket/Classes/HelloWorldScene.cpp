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
	
	//预先读取图片数据
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

//开始界面
void HelloWorld::startInterface(){

	endFlag=0;//设置结束标记为0
	
	start=CCSprite::create("start.jpg");

	start->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	
	addChild(start,100);
	
	CCScale9Sprite* btnNormal = CCScale9Sprite::create("plane.png");  
  
    /* 点击状态下的按钮图片 */  
    CCScale9Sprite* btnDown = CCScale9Sprite::create("plane.png");  
  
    /* 按钮标题 */  
    CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
  
    /* 按钮的大小会根据标题自动调整 */  
	controlBtn = CCControlButton::create(title, btnNormal);
  
    /* 设置按钮按下时的图片 */  
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);  

    controlBtn->setPosition(ccp(screenSize.width/2,screenSize.height/2+50));

	//设置按钮的回调函数
	controlBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::startcallback),CCControlEventTouchUpInside);

	CCActionInterval * scale1 = CCScaleTo::create(0.5,1.1);
	
	CCActionInterval *scale2=CCScaleTo::create(0.1,1);
    
	//放大缩小运动放到运动队列中
	CCFiniteTimeAction*seq =CCSequence::create(scale1,scale2,NULL);
	
	CCFiniteTimeAction *repeat = CCRepeatForever::create((CCActionInterval* )seq);
	
	//重复放大缩小运动
	controlBtn->runAction(repeat);

    start->addChild(controlBtn);  
	
	controlBtn->setPreferredSize(CCSize(179, 179));
	//initWorld();
}

//开始按钮的回调函数，运行开始动画，以及关闭开始界面，进入游戏界面
void HelloWorld::startcallback(CCObject *object, CCControlEvent controlEvent){

	controlBtn->removeFromParent();
	
	CCSprite *startplane=CCSprite::create("plane.png");
	
	startplane->setPosition(ccp(screenSize.width/2,screenSize.height/2+50));
	
	start->addChild(startplane,2);
	
	//旋转动画
	CCAction *rotate=CCRotateTo::create(0.8f,900);
	
	startplane->runAction(rotate);
	
	//定时0.8s只调用一次,初始化世界
	this->scheduleOnce(schedule_selector(HelloWorld::initWorld),0.8f);
}

//初始化游戏的时间
void HelloWorld::initWorld(float dt){
	start->removeFromParent();//开始界面remove掉
	
	//一系列值初始化
	best=0;
	
	score=0;
	
	flagY=-1;
	
	jumpSpeed=0;
	
	scoreSprite=CCSprite::create();
	
	scoreSprite->setPosition(ccp(screenSize.width/2-25,screenSize.height-100));
	
	addChild(scoreSprite,5);
	
	//显示分数为0
	scoreLable = CCLabelAtlas::create("0", "shuzi2.png", 48, 46, '0');
	
	scoreLable->setPosition(ccp(0,0));
	
	scoreSprite->addChild(scoreLable);
	
	scoreLable->setScale(1.0f); 
	
	//以时间为随机种子
	srand(time(NULL));

	//添加暂停按钮
	addPauseButton();
    
	//添加火箭
	addRocket();
	
	//添加底部
	addGround();
	
	//添加云朵
	addCloud();
	
	//添加背景
	addBackground();
    
	//添加update用于检测碰撞
	scheduleUpdate();
	
	//rocket模拟自由落体
	schedule(schedule_selector(HelloWorld::fall),1/60);
	
	//定时添加障碍
	this->schedule(schedule_selector(HelloWorld::addBar),1.7);
	
	//设置屏幕可以点击
	setTouchEnabled(true);//屏幕可以点击

}

//添加暂停按钮
void HelloWorld::addPauseButton(){

	PauseButton=CCSprite::create();
	
	PauseButton->setPosition(ccp(60,screenSize.height-50));
	
	addChild(PauseButton,6);
	
	CCScale9Sprite* btnNormal = CCScale9Sprite::create("pause_off.png");  
  
    /* 点击状态下的按钮图片 */  
    CCScale9Sprite* btnDown = CCScale9Sprite::create("pause_on.png");  
  
    /* 按钮标题 */  
    CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt.ttf", 30);  
  
    /* 按钮的大小会根据标题自动调整 */  
    CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);  
  
    /* 设置按钮按下时的图片 */  
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);  
   
	controlBtn->setPosition(ccp(0, 0));
	
	controlBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::pauseCallBack),CCControlEventTouchUpInside);//回调函数当触摸点按钮弹起时
   
	PauseButton->addChild(controlBtn);  
	
	controlBtn->setPreferredSize(CCSize(117, 93)); 
}

//暂停按钮的回调函数
void HelloWorld::pauseCallBack(CCObject *object, CCControlEvent controlEvent) {
	//如果游戏结束，暂停不可用
	if(endFlag==1){
		return;
	}
	
	//暂停键设置隐藏
	PauseButton->setVisible(false);
	
	CCArray *children=CCArray::create();
	
	//获取子节点
	children=this->getChildren();
	
	CCObject *temp=NULL;
	
	//把所有子节点的计时器和动作暂停
	CCARRAY_FOREACH(children,temp )  
	{  
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);  
		sprite->pauseSchedulerAndActions();
	}
	
	this->pauseSchedulerAndActions();
	
	//设置屏幕不可触摸
	this->setTouchEnabled(false);
	
	//添加暂停界面
	addPauseInterface();
}

//添加暂停界面
void HelloWorld::addPauseInterface(){
	
	CCSprite *ButtonSprite=CCSprite::create();
	
	ButtonSprite->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	
	//设置tag为了后面获取这个精灵
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

//重来键的回调函数
void HelloWorld::againCallBack(CCObject *object, CCControlEvent controlEvent){
	//结束标记置0
	endFlag=0;
	
	CCScene *mScene = HelloWorld::scene();
	
	CCDirector::sharedDirector()->replaceScene(mScene);
	//init();
}

//继续运行键的回调函数
void HelloWorld::playCallBack(CCObject *object, CCControlEvent controlEvent){
	//把暂停界面remove
	this->getChildByTag(10)->removeFromParent();
	
	CCArray *children=CCArray::create();
	
	children=this->getChildren();
	
	CCObject *temp=NULL;
	
	CCARRAY_FOREACH(children,temp )  
	{  
	
		CCSprite *sprite = dynamic_cast<CCSprite*>(temp);  
		//恢复所有精灵的动作和计时器
		sprite->resumeSchedulerAndActions();
	
	}
	 
	this->resumeSchedulerAndActions();
	
	this->setTouchEnabled(true);
	
	PauseButton->setVisible(true);
}

//添加背景
void HelloWorld::addBackground(){
	CCSprite *background1=CCSprite::create("bg.png");
	CCPoint pt=ccp(screenSize.width/2,screenSize.height/2);
	background1->setPosition(pt);
	addChild(background1,0);
}

//添加火箭
void HelloWorld::addRocket(){

	rocket = CCSprite::create("rocket.png");
	rocket->setPosition(ccp(screenSize.width/5,screenSize.height*2/3));
	addChild(rocket,2);
}

//添加障碍物
void HelloWorld::addBar(float dt){
	
	//障碍物的运动时间在2.9~3.3s之间随机
	double time=(rand()%(330-(290))+(290))/100.0;
	//dy表示下面的障碍物的y轴坐标
	int dy=rand()%(200-(-200))+(-200);
	//uy表示上面的障碍物的y轴坐标
	int uy=dy+rand()%(970-(888))+(888);
	
	//根据dy的奇偶，设置障碍的图片类型
	if(dy%2==1)
	{
		down_bar = CCSprite::create("rock_1.png");
		
		down_bar->setPosition(ccp(screenSize.width*3/2,dy));
		
		down_bar->setTag(1);
		
		CCAction * DBmoveto=CCMoveTo::create(time,ccp(-screenSize.width*3/4,dy));
		
		//动作结束后调用remove自己的函数
		CCCallFuncN  * funcall= CCCallFuncN ::create(this, callfuncN_selector(HelloWorld::callbackN));
		
		CCArray * test= CCArray::create();
		
		test->addObject(DBmoveto);
		
		test->addObject(funcall);
		
		CCFiniteTimeAction * seq = CCSequence::create(test);
		
		down_bar->runAction(seq);
		

		up_bar=CCSprite::create("rock_1.png");
		
		up_bar->setPosition(ccp(screenSize.width*3/2+200,uy));
		
		//设置tag，检测碰撞时需要用到
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

//remove自身
void HelloWorld::callbackN(CCNode *sender){
	sender->removeFromParent();
}

//添加底部，把两张mountain无缝连接
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

//，每帧改变位置，模拟自由落体
void HelloWorld::fall(float dt){
	if(flagY==1&&jumpSpeed<=0){
			flagY=-1;
		}
		jumpSpeed-=flagY*2333*dt;
		if(jumpSpeed>0&&flagY!=0){
			rocket->setPosition(ccp(rocket->getPositionX(),rocket->getPositionY()+flagY*jumpSpeed*dt));
		}
}

//每帧设置分数，检测碰撞
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

		//碰撞检测返回的int值，用于存放碰撞点
		int CollidedResult;

		//tag为1或3表示是rock_1.png，3表示是已经被火箭经过的障碍
		if(sprite->getTag() == 1||sprite->getTag()==3)  
		{  
			//tag=1并且上部障碍的x坐标<rocket，就吧tag设置为3，并且加分
			if((sprite->getPositionY()>400)&&(sprite->getPositionX()<(screenSize.width/5+60))&&(sprite->getTag() == 1)){
				sprite->setTag(3);
				score++;
			}
			//进行碰撞检测
			CollidedResult=isCollided(rocket,"rocket.png",sprite,"rock_1.png");
			
			//返回值为-1表示未碰撞
			if(CollidedResult!=-1)
			{
				//不等于-1，碰撞，结束游戏
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

//检测是否碰撞
int HelloWorld::isCollided(CCSprite *a,char* aPath,CCSprite *b,char* bPath){
	//分别表示AB图的上下左右的坐标
	float ABottom,ATop,ALeft,ARight;
	float BBottom,BTop,BLeft,BRight;

	CCSprite *aa=a;
	CCSprite *bb=b;
	CCSize Asize=aa->getContentSize();
	CCSize Bsize=bb->getContentSize();
	CCPoint Ap=aa->getPosition();
	CCPoint Bp=bb->getPosition();
	//cocos2dx的坐标系是以左下角为原点，而一般的坐标系是以左上角为原点
	ATop=screenSize.height-Ap.y-Asize.height/2;
	ABottom=screenSize.height-Ap.y+Asize.height/2;
	ALeft=Ap.x-Asize.width/2;
	ARight=Ap.x+Asize.width/2;
	BTop=screenSize.height-Bp.y-Bsize.height/2;
	BBottom=screenSize.height-Bp.y+Bsize.height/2;
	BLeft=Bp.x-Bsize.width/2;
	BRight=Bp.x+Bsize.width/2;

	//两张图片没接触的情况，返回-1
	if (ALeft > BRight || ATop > BBottom || BLeft > ARight || BTop > ABottom) {
		return -1;
	 }
	
	//相交矩形的上下左右的坐标以及长宽
	float intersectLeft = ALeft > BLeft ? ALeft : BLeft;
	float intersectTop = ATop >BTop ? ATop : BTop;
	float intersectRight = ARight < BRight ? ARight : BRight;
	float intersectBottom = ABottom < BBottom ? ABottom : BBottom;
	float intersectWidth = intersectRight - intersectLeft;
	float intersectHeight = intersectBottom - intersectTop;

	//相交的矩形，在第一张第二张图的xy轴偏移量
	float thisImageXOffset = intersectLeft - ALeft;
	float thisImageYOffset = intersectTop - ATop;
	float otherImageXOffset = intersectLeft - BLeft;
	float otherImageYOffset = intersectTop - BTop;
	//进行像素碰撞
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
	  
	 //事先读取好图片的像素值，不然每次都读太卡
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
	//扫描相交区域是否存在一点AB图都不是透明的点，就表示相交
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
		
		 //A和B都不透明，相交
		 if(((*Atemp >> 24)& 0xff)!=0&&((*Btemp >> 24)& 0xff)!=0)
		{
			//返回相交点
			return j*firstwidth+i;
		}
	}

	   return -1;
}

//结束游戏
void HelloWorld::endgame(int direction){
	//结束标记置1
	endFlag=1;
	//隐藏暂停键
	PauseButton->setVisible(false);
	//停止一系列定时器
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
	
	//获取rocket的坐标
	boomX=rocket->getPositionX();
	boomY=rocket->getPositionY();
	rocket->setVisible(false);
	boom=CCSprite::create();
	
	/*char strL[10];	
	int L=direction;
	sprintf(strL,"%d",L);
	CCMessageBox(strL,"123");
	*/
	//根据碰撞测试的结果，设定爆炸点
	if(direction%72>=65)
		boom->setPosition(ccp(boomX+direction%72-50-50,boomY-(direction/72-30)));
	else if(direction/72>32)
		boom->setPosition(ccp(boomX+direction%72-50,boomY-(direction/72-30)+50));
	else
		boom->setPosition(ccp(boomX+direction%72-50,boomY-(direction/72-30)-50));
	addChild(boom,3);
	CCAnimation *animation = CCAnimation::create();  
//从本地文件系统中加载图片文件到CCSpriteFrame中区，然后添加到CCAnimation中  
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
animation->setDelayPerUnit(0.5f / 15.0f); // 这个动画包含15帧，将会持续2秒.  
animation->setRestoreOriginalFrame(true); // 14帧播放完之后返回到第一帧  
  
CCAnimate *action = CCAnimate::create(animation);  
boom->runAction(action);  
scoreSprite->setVisible(false);
scheduleOnce(schedule_selector(HelloWorld::addEndInterface),0.5f);

}

//添加结束界面
void HelloWorld::addEndInterface(float dt){
	EndInterface=CCSprite::create();
	addChild(EndInterface,10);
	
	//添加计分板
	EndInterface->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	CCSprite* scoreboard=CCSprite::create("scoreBoard.png");
	EndInterface->addChild(scoreboard);
	CCLabelAtlas *endscore=CCLabelAtlas::create("0", "shuzi2.png", 48, 46, '0');
	char s[25];
	sprintf(s, "%d", score); 
	endscore->setString(s);
	scoreboard->addChild(endscore);
	//根据分数长度，来改变分数的位置
	if(score<10)
		endscore->setPosition(225,80);
		else if(score>=10&&score<100)
			endscore->setPosition(177,80);
		else
			endscore->setPosition(129,80);
	
		//判断是否是第一次玩
	if(isHaveSaveFile()==true)
	{	
		//不是第一次，从xml中取出best 的值
		best=CCUserDefault::sharedUserDefault()->getIntegerForKey("best");
		//判断xml中best和当前score的大小
		if(score>best)
		{
			best=score;
			CCUserDefault::sharedUserDefault()->setIntegerForKey("best",best);
			CCUserDefault::sharedUserDefault()->flush();//写了东西要提交  
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
	//添加重来键
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

//添加云朵，云朵从屏幕右边往左慢慢移动
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

//点击事件
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	{
		//若火箭的位置过高,则不跳跃
	if(rocket->getPositionY()>=screenSize.height)
		return;
	flagY=1;
	jumpSpeed=642;
	
	//跳跃时，火箭后面添加喷火效果
	CCSprite *fire= CCSprite::create();
	//fire->setPosition(ccp(rocket->getPositionX()-50,rocket->getPositionY()));
	rocket->addChild(fire);
	fire->setPosition(ccp(-25,rocket->getContentSize().height/2));
	CCAnimation *animation = CCAnimation::create();  
//从本地文件系统中加载图片文件到CCSpriteFrame中区，然后添加到CCAnimation中  
	for (int i = 1; i < 5; i++)  
	{  
		char szImageFileName[128] = {0};  
			sprintf(szImageFileName, "f/%03d.png", i);  
		animation->addSpriteFrameWithFileName(szImageFileName);
	}  
	animation->setDelayPerUnit(0.4f / 4.0f); // 这个动画包含14帧，将会持续2.8秒.  
	animation->setRestoreOriginalFrame(true); // 14帧播放完之后返回到第一帧  
  
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
        CCUserDefault::sharedUserDefault()->flush();//提交
        //        CCLog("存储文件不存在,头次开始加载游戏");
        return false;
    }else{
        //        CCLog("存储文件已存在");
        return true;
    }
}

