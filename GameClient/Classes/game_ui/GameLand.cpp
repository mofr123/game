#include "GameLand.h"
#include "publicDef/gameConfig.h"
#include "game_scene/GameScene.h"
#include "RoleCreateLayer.h"

#include "network/SaveDataManager.h"
#include "network/PuzzleRPC.h"

Scene* GameLand::scene()
{
	auto scene = Scene::create();
	auto layer = GameLand::create();
	scene->addChild(layer);
	return scene;
}

GameLand::GameLand():m_accountInputBox(NULL),m_cipherInputBox(NULL)
{
}

GameLand::~GameLand(){
	TextureCache::getInstance()->removeAllTextures();
}

bool GameLand::init()
{
	if(!Layer::init()){
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	auto layerColor = LayerColor::create(Color4B(200,200,200,200),winSize.width,winSize.height);
	this->addChild(layerColor);

	std::string account = ToUTF8::getInstance()->WStrToUTF8(L"请输入您的账号");
	std::string cipher = ToUTF8::getInstance()->WStrToUTF8(L"请输入您的密码");

	Size inputBoxSize = Size(360,50);
	m_accountInputBox = EditBox::create(inputBoxSize,Scale9Sprite::create());
	m_accountInputBox->setPlaceHolder(account.c_str());
	m_accountInputBox->setPlaceholderFontColor(Color3B(0,0,0));
	m_accountInputBox->setInputMode(EditBox::InputMode::ANY);
	m_accountInputBox->setMaxLength(12);
	m_accountInputBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_accountInputBox->setPosition(ccpAdd(winSize/2,Point(0,100)));
	m_accountInputBox->setDelegate(this);
	this->addChild(m_accountInputBox);

	m_cipherInputBox = EditBox::create(inputBoxSize, Scale9Sprite::create());
	m_cipherInputBox->setPlaceHolder(cipher.c_str());
	m_cipherInputBox->setPlaceholderFontColor(Color3B(0, 0, 0));//ccc3
	m_cipherInputBox->setInputMode(EditBox::InputMode::ANY);
	m_cipherInputBox->setInputFlag(EditBox::InputFlag::PASSWORD);//kEditBoxInputFlagPassword
	m_cipherInputBox->setMaxLength(12);
	m_cipherInputBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_cipherInputBox->setPosition(ccpAdd(winSize/2, Point(0, 30)));
	m_cipherInputBox->setDelegate(this);
	this->addChild(m_cipherInputBox);

	//ccc4
	LayerColor* accountBg = LayerColor::create(Color4B(100, 100, 200, 200), inputBoxSize.width, inputBoxSize.height);
	accountBg->setPosition(ccpSub(m_accountInputBox->getPosition(), m_accountInputBox->getContentSize()/2));
	this->addChild(accountBg, -1);

	LayerColor* cipherBg = LayerColor::create(Color4B(100, 100, 200, 200), inputBoxSize.width, inputBoxSize.height);
	cipherBg->setPosition(ccpSub(m_cipherInputBox->getPosition(), m_cipherInputBox->getContentSize()/2));
	this->addChild(cipherBg, -1);

	std::string login = ToUTF8::getInstance()->WStrToUTF8(L"登陆");

	ControlButton* btnLand = ControlButton::create(login, MAIN_FONT_NAME, 40);
	btnLand->setPosition(ccpSub(winSize/2, Point(80, 60)));
	this->addChild(btnLand);
	btnLand->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLand::sendLand), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside

	std::string regi = ToUTF8::getInstance()->WStrToUTF8(L"注册");
	ControlButton* btnReg = ControlButton::create(regi, MAIN_FONT_NAME, 40);
	btnReg->setPosition(ccpSub(winSize/2, Point(-80, 60)));
	this->addChild(btnReg);
	btnReg->addTargetWithActionForControlEvents(this, cccontrol_selector(GameLand::registration), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside


	return true;
}

void GameLand::sendLand(Ref* sender,Control::EventType controlEvent)
{
	string account = m_accountInputBox->getText();
	string pwd = m_cipherInputBox->getText();
	if (strlen(account.c_str()) < 6)
	{
	     MessageBox("请输入不小于6位的账号", "提示");
	}
	else if (strlen(pwd.c_str()) < 6)
	{
	     MessageBox("请输入不小于6位的密码", "提示");
	}
	else
	{
		//UserDefault::getInstance()->setStringForKey("key1", "MMOO1");
		string str = UserDefault::getInstance()->getStringForKey("key");
		log("str====%s" , str.c_str());

		SaveDataManager::getInstance()->initBasicData();
		tagSaveUserInfo userInfo;
		SaveDataManager::getInstance()->getSaveUserInfo(userInfo);
		if (account!=userInfo.strUserName||pwd!=userInfo.strPassWord)
		{
			MessageBox("账号与密码不匹配", "提示");
			return ;
		}
		PuzzleRPC::getInstance()->login("type",account,pwd);
		//joinGame();
	}
}


void GameLand::joinGame()
{
	RoleCreateLayer* layer = RoleCreateLayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer);
	this->removeFromParent();
}

void GameLand::registration(Ref* sender,Control::EventType controlEvent)
{
	for (int i = 0; i < 8; i++)
	{
		float angle = i*45 * M_PI / 180;
		log("xxxxx:%d,%f,%f",i,cosf(angle),sinf(angle));
	}
}

void GameLand::editBoxReturn(EditBox* editBox)
{
	if(editBox){
		log("xx:%s",editBox->getText());
	}
	log("GameLand::editBoxReturn(EditBox* editBox)");
}

