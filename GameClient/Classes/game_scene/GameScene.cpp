#include "GameScene.h"
#include "figure/Enemy.h"
#include "FlutteringFairy.h"
#include "game_ui/LMenu.h"
#include "map/PathAStar.h"


static GameScene* _gameScene = NULL;


Scene* GameScene::scene()
{
    Scene *scene = Scene::create();
    
    GameScene *layer = GameScene::shareGameScene();
    
    scene->addChild(layer);
    
    return scene;
}

GameScene* GameScene::shareGameScene()
{
    return _gameScene;
}

GameScene* GameScene::create()
{
    _gameScene = new GameScene();
    if (_gameScene && _gameScene->init())
    {
        _gameScene->autorelease();
        return _gameScene;
    }
    CC_SAFE_RELEASE_NULL(_gameScene);
    return NULL;
}

GameScene::GameScene()
:m_enemyDictionary(NULL)
,m_otherDictionary(NULL)
,m_selected(NULL)
,m_bgMap(NULL)
,m_touchMouse(NULL)
,m_touchProtected(false)
,m_gameInfoUIController(NULL)
,m_playerDirection(Point::ZERO)
,m_isPlayerMoveActions(false)
{
    m_enemyDictionary = new CCDictionary();
    m_otherDictionary = new CCDictionary();
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE_NULL(m_enemyDictionary);
    CC_SAFE_RELEASE_NULL(m_otherDictionary);
}

void GameScene::registerTouch()
{
	//CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	//touchDispatcher->addTargetedDelegate(this, 100, false);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this );
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this );
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this );
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this );
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void GameScene::onEnter()
{
	Layer::onEnter();
	this->registerTouch();
}
void GameScene::onExit()
{
	Layer::onExit();
}

/*
void GameScene::unRegisterWithTouchDispatcher()
{
	CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	touchDispatcher->removeDelegate(this);
}
*/

void GameScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    this->schedule(schedule_selector(GameScene::playerMovement), 0.5f);
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	this->setKeypadEnabled(true);
#endif
    AttackSkillSystem::shareAttackSkillSystem();
    NpcInfoSystem::shareNpcInfoSystem();
    PropSystem::sharePropSystem();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture_set/blood_return.plist");

    m_selected = Sprite::create("ui/tray_self.png");
    m_selected->retain();
    
    m_touchMouse = CCTouchMouse::create();
    m_touchMouse->retain();
    
    m_gameInfoUIController = GameInfoUIController::create();
    this->addChild(m_gameInfoUIController);
    
    this->setTouchEnabled(true);

    return true;
}

void GameScene::showSelected(Monomer* var)
{
    if (m_selected->getParent())
    {
        m_selected->removeFromParent();
    }
    
    if (var)
    {
        float x = var->getAnchorPoint().x * var->getContentSize().width;
        float y = var->getAnchorPoint().y * var->getContentSize().height;
        m_selected->setPosition(Point(x, y));
        var->addChild(m_selected, -1);
        m_gameInfoUIController->getAttackEnemyInfo()->showAttackInfo(var);
    }
}

Monomer* GameScene::getSelected()
{
    Monomer* var = ((Monomer*)m_selected->getParent());
    
    if (var == NULL)
    {
        m_gameInfoUIController->getAttackEnemyInfo()->hide();
    }
    return var;
}

void GameScene::replaceBgMap(int mapID, const int born)
{
    m_gMapPoint.clear();
    
    if (m_bgMap)
    {
        m_gMapPoint.clear();
        m_bgMap->setDelegate(NULL);
        m_bgMap->removeFromParent();
        TextureCache::getInstance()->removeUnusedTextures();
    }
    
    Player* player = Player::sharePlayer();
    player->stopAllActions();
    player->stand();
    player->clearData();
    
    m_enemyDictionary->removeAllObjects();
    m_otherDictionary->removeAllObjects();
    
    BgMap* bgMap = BgMap::createWithPath(mapID);
    bgMap->setDelegate(this);
    this->addChild(bgMap, -1);
    bgMap->addChildPlayerLead(player, born);
    m_touchMouse->setPosition(Point(-100, -100));
    bgMap->addChild(m_touchMouse, BgMap::getZOrderZero(bgMap));
    m_waitReincarnationEnemy.insert(bgMap->getEnemeyMap().begin(), bgMap->getEnemeyMap().end());
    this->setCurrBgMap(bgMap);
    
    for (int i = 0; i < bgMap->getEnemeyMap().size(); i++)
    {
        this->addEnemy();
    }
}

void GameScene::OutOfCombat()
{
    this->showSelected(NULL);
    m_gameInfoUIController->getOperationMenu()->cancelHangUP();
}

bool GameScene::isPointValid(const MapPoint& point)
{
    if (m_bgMap->getCurrentGridValue(point) == 1)
        return false;
    
    if (this->getMapPoint(point))
        return false;
    
    return true;
}

void GameScene::touchProtected()
{
    do
    {
        CC_BREAK_IF(m_touchProtected == false);
        
        this->stopActionByTag(0xff99);
        m_touchProtected = true;
        CallFunc* callFunc = CallFunc::create(this, callfunc_selector(GameScene::untouchProtected));
        DelayTime* delay = DelayTime::create(0.5f);
        Sequence* actions = Sequence::create(delay, callFunc, NULL);
        actions->setTag(0xff99);
        this->runAction(actions);
    }
    while (0);
}

void GameScene::untouchProtected()
{
    m_touchProtected = false;
}

void GameScene::updateImageDisplay(Rect &showRect, Rect &hideRect)
{
    CCArray* enemyKeys = m_enemyDictionary->allKeys();
    if (enemyKeys)
    {
        for (int i=0; i<enemyKeys->count(); i++)
        {
            int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
            Enemy* enemy = (Enemy*)m_enemyDictionary->objectForKey(key);
            if (showRect.containsPoint(enemy->getPosition()))
            {
                enemy->showFigure();
            }
            if (!hideRect.containsPoint(enemy->getPosition()))
            {
                enemy->hideFigure();
            }
        }
    }
    
    CCArray* otherPlayerKeys = m_otherDictionary->allKeys();
    if (otherPlayerKeys)
    {
        for (unsigned int i = 0 ; i < otherPlayerKeys->count(); i++)
        {
            int key = ((CCInteger*)otherPlayerKeys->objectAtIndex(i))->getValue();
            OtherPlayer* otherPlayer = (OtherPlayer*)m_otherDictionary->objectForKey(key);
            if (showRect.containsPoint(otherPlayer->getPosition()))
            {
                otherPlayer->showFigure();
            }
            if (!hideRect.containsPoint(otherPlayer->getPosition()))
            {
                otherPlayer->hideFigure();
            }
        }
    }
}

void GameScene::insterMapPoint(Monomer* var, const MapPoint& point)
{
    m_gMapPoint[var] = point.getValue();
}

void GameScene::eraseMapPoint(Monomer* var)
{
    m_gMapPoint.erase(var);
}

bool GameScene::getMapPoint(const MapPoint& point)
{
    bool relust = false;
    
    std::map<Monomer*, int>::iterator itr;
    for (itr=m_gMapPoint.begin(); itr!=m_gMapPoint.end(); itr++)
    {
        if (itr->second == point.getValue())
        {
            relust = true;
            break;
        }
    }
    return relust;
}

bool GameScene::insterMapPointForProp(PropIconShow* var, const MapPoint& point)
{
    do
    {
        CC_BREAK_IF(m_gPropMap.find(point.getValue()) != m_gPropMap.end());
        
        m_gPropMap[point.getValue()] = var;
        
        return true;
    }
    while (0);
    
    return false;
}

void GameScene::eraseMapPointForProp(const MapPoint& point)
{
    m_gPropMap.erase(point.getValue());
}

PropIconShow* GameScene::getMapPointForProp(const MapPoint& point)
{
    PropIconShow* relust = NULL;
    
    if (m_gPropMap.find(point.getValue()) != m_gPropMap.end())
    {
        relust = m_gPropMap.at(point.getValue());
    }
    return relust;
}

std::deque<MapPoint> GameScene::getPath(const MapPoint& begin, const MapPoint& end)
{
    std::deque<MapPoint> array = PathAStar::findPathByAStar(m_bgMap->getMapGrid(),
                                                            m_bgMap->getGridRow(),
                                                            m_bgMap->getGridCol(),
                                                            4096, begin, end);
    

    std::deque<MapPoint> relust;
    
    std::deque<MapPoint>::iterator itr = array.begin();
    
    while (itr != array.end())
    {
        if (itr + 1 == array.end())
        {
            relust.push_back(MapPoint(itr->x, itr->z));
            break;
        }
        
        MapPoint p1 = *itr;
        MapPoint p2 = *(itr+1);
        
        if (relust.size() > 0 && mapSub(p1, relust.back()) * 2 == mapSub(p2, relust.back()))
        {
            relust.push_back(p2);
            itr += 2;
        }
        else
        {
            relust.push_back(p1);
            itr ++;
        }
    }
    
    return relust;
}

std::deque<MapPoint> GameScene::getPathNextRunGrid(const MapPoint &begin, const MapPoint &end)
{
    std::deque<MapPoint> array = PathAStar::findPathByAStar(m_bgMap->getMapGrid(),
                                                            m_bgMap->getGridRow(),
                                                            m_bgMap->getGridCol(),
                                                            64, begin, end);
    
    std::deque<MapPoint> relust;
    relust.push_back(array.front());
    
    while (array.size() > 3)
    {
        array.pop_back();
    }
    
    if (array.size() == 2)
    {
        MapPoint p = array.at(1);
        relust.push_back(p);
    }
    else if (array.size() > 2)
    {
        MapPoint p1 = array.at(1);
        MapPoint p2 = array.at(2);
        
        if (mapSub(p2, array.front()).equals(mapSub(p1, array.front()) * 2))
        {
            relust.push_back(p2);
        }
        else
        {
            relust.push_back(p1);
        }
    }
    
    return relust;
}

std::deque<MapPoint> GameScene::getPathNextWalkGrid(const MapPoint &begin, const MapPoint &end)
{
    std::deque<MapPoint> array = PathAStar::findPathByAStar(m_bgMap->getMapGrid(),
                                                            m_bgMap->getGridRow(),
                                                            m_bgMap->getGridCol(),
                                                            64, begin, end);

    while (array.size() > 2)
    {
        array.pop_back();
    }
    
    return array;
}

bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = m_bgMap->convertToNodeSpace(point);
    
	if (!this->isVisible())
		return false;
    
    if (m_touchProtected)
        return false;

    do
    {
        if (this->getSelected())
        {
            if (this->getSelected()->getRect().containsPoint(point))
            {
                m_gameInfoUIController->getOperationMenu()->commonAttack();
                break;
            }
            else
            {
                this->showSelected(NULL);
            }
            
        }
        
        CCArray* otherKeys = m_otherDictionary->allKeys();
        if (otherKeys)
        {
            for (int i=0; i<otherKeys->count(); i++)
            {
                int key = ((CCInteger*)otherKeys->objectAtIndex(i))->getValue();
                Monomer* mon = (Monomer*)m_otherDictionary->objectForKey(key);
                if (mon->getRect().containsPoint(point))
                {
                    this->showSelected(mon);
                    break;
                }
            }
        }
        
        CCArray* enemyKeys = m_enemyDictionary->allKeys();
        if (enemyKeys)
        {
            for (int i=0; i<enemyKeys->count(); i++)
            {
                int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
                Monomer* mon = (Monomer*)m_enemyDictionary->objectForKey(key);
                if (mon->getRect().containsPoint(point))
                {
                    this->showSelected(mon);
                    Player::sharePlayer()->setAttackMonomerMajor(mon);
                    break;
                }
            }
        }

        if (this->getSelected() == NULL)
        {
            m_playerDirection = m_bgMap->convertToWorldSpace(point);
            this->beginMoveActions();
            this->touchProtected();
            m_gameInfoUIController->getOperationMenu()->cancelHangUP();
        }
    }
    while (0);
    
    this->unschedule(schedule_selector(GameScene::log));
    this->schedule(schedule_selector(GameScene::log));
	return true;
}

void GameScene::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = m_bgMap->convertToNodeSpace(point);
    
    do
    {
        CC_BREAK_IF(!m_isPlayerMoveActions);
        m_playerDirection = m_bgMap->convertToWorldSpace(point);
        CC_BREAK_IF(Player::sharePlayer()->getStateType() != FStateStand);
        this->beginMoveActions();
    }
    while (0);
}

void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = m_bgMap->convertToNodeSpace(point);
    
    this->stopMoveActions();
}

void GameScene::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Point uipoint = pTouch->getLocationInView();
	Point point = Director::getInstance()->convertToGL(uipoint);
	point = m_bgMap->convertToNodeSpace(point);
    
}

void GameScene::removeEnemy(Enemy* enemy)
{
    std::multimap<unsigned int, MapPoint>::value_type type = std::multimap<unsigned int, MapPoint>::value_type(enemy->getRoleNumber(), enemy->getDenPos());
    m_waitReincarnationEnemy.insert(type);
    m_gMapPoint.erase(enemy);
    m_enemyDictionary->removeObjectForKey(enemy->getEnemyID());
    //if (enemy->getName == (this->getSelected()->getName()))// (enemy->isEqual(this->getSelected())
    //{
        //this->showSelected(NULL);
    //}
    float time = CCRANDOM_0_1() * 5 + 3;
    CallFunc* callFunc = CallFunc::create(this, callfunc_selector(GameScene::addEnemy));
    Sequence* actions = Sequence::create(DelayTime::create(time), callFunc, NULL);
    this->runAction(actions);
    
    m_gameInfoUIController->getOperationMenu()->intelligentSearch();
}

void GameScene::addEnemy()
{
    if (m_waitReincarnationEnemy.size() <= 0)
        return;

    static int tag = 1000;
    unsigned int number = m_waitReincarnationEnemy.begin()->first;
    
    Enemy* enemy = Enemy::create(tag, number);
    MapPoint point = m_waitReincarnationEnemy.begin()->second;
    enemy->setPosition(point.getCCPointValue());
    enemy->setDenPos(enemy->getPosition());
    m_bgMap->addChild(enemy);
    enemy->updateVertexZ();
    enemy->setAttackTime(1.0f);
    
    if (number == 11000)
    {
        enemy->setBlood(32000);
        enemy->setBloodCap(32000);
        enemy->setTheAttack(500);
    }
    else if (number == 12000)
    {
        enemy->setBlood(128000);
        enemy->setBloodCap(128000);
        enemy->setTheAttack(1500);
    }
    else if (number == 30000)
    {
        enemy->setBlood(4000);
        enemy->setBloodCap(4000);
        enemy->setTheAttack(50);
        enemy->setActive(false);
    }
    else if (number == 26000)
    {
        enemy->setBlood(16000);
        enemy->setBloodCap(16000);
        enemy->setTheAttack(100);
        enemy->setCanMoved(false);
    }
    else
    {
        enemy->setBlood(8000);
        enemy->setBloodCap(8000);
        enemy->setTheAttack(30);
        enemy->setActive(false);
    }
    
    enemy->patrol();
    
    m_enemyDictionary->setObject(enemy, enemy->getEnemyID());
    enemy->setTag(tag);
    
    tag++;
    
    m_waitReincarnationEnemy.erase(m_waitReincarnationEnemy.begin());
}

vector<Monomer*> GameScene::getMonmerVecIsLenght(Point point, int lenght)
{
    vector<Monomer*> vec;
    CCArray* enemyKeys = m_enemyDictionary->allKeys();
    if (enemyKeys)
    {
        for (int i=0; i<enemyKeys->count(); i++)
        {
            int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
            Enemy* enemy = (Enemy*)m_enemyDictionary->objectForKey(key);
            
            float x = point.x - enemy->getHurtPoint().x;
            float y = point.y - enemy->getHurtPoint().y;
            y /= sqrtf(2);
            if (sqrtf(x * x + y * y) <= lenght)
            {
                vec.push_back(enemy);
            }
        }
    }
    return vec;
}

void GameScene::playerMovement(float dt)
{
    if (!m_bgMap)
        return;
    
    vector<Monomer*> vec = this->getMonmerVecIsLenght(Player::sharePlayer()->getPosition(), MapPoint(6, 1).getCCSizeValue().width);
    for (unsigned int i=0; i<vec.size(); i++)
    {
        Enemy* mon = (Enemy*)vec.at(i);
        if (mon->getAttackMonomerMajor())
            continue;
        
        if (mon->getActive() == false)
            continue;
        
        mon->followAttackAndSetAttackMethods(Player::sharePlayer());
    }
}

MapPoint GameScene::getPlayerDirection()
{
    MapPoint relust;
    
    Point begin = Player::sharePlayer()->getPosition();
    Point end = m_bgMap->convertToNodeSpace(m_playerDirection);
    
    float lenghtX = end.x - begin.x;
    float lenghtY = end.y - begin.y;
    float lenght = ccpDistance(begin, end);
    float angle_X = acosf(lenghtX / lenght) * 180 / M_PI;;
	float angle_Y = acosf(lenghtY / lenght) * 180 / M_PI;
    
    float angle = angle_X;
    if (angle_Y > 90)
    {
        angle = 360 - angle_X;
    }
    angle *= M_PI / 180;
    float x = cosf(angle);
    float y = sinf(angle);
    float tan = fabsf(tanf(angle));
    float tanMin = tanf(22.5f * M_PI / 180);
    float tanMax = tanf(67.5f * M_PI / 180);
    
    if (tanMin <= tan && tan < tanMax)
    {
        relust = MapPoint(x / fabsf(x), y / fabsf(y));
    }
    else if (tan < tanMin)
    {
        relust = MapPoint(x / fabsf(x), 0);
    }
    else
    {
        relust = MapPoint(0, y / fabsf(y));
    }
    
    relust = relust * 2;
    return relust;
}

void GameScene::playerRunning(float delay)
{
    if (m_gameInfoUIController->getOperationMenu()->getMoveMethods() == 0)
    {
        Player::sharePlayer()->runBy(this->getPlayerDirection());
    }
    else if (m_gameInfoUIController->getOperationMenu()->getMoveMethods() == 1)
    {
        Player::sharePlayer()->walkBy(this->getPlayerDirection());
    }
    
}

void GameScene::beginMoveActions()
{
    do
    {
        if (m_gameInfoUIController->getOperationMenu()->getMoveMethods() == 2)
        {
            MapPoint point = m_bgMap->convertToNodeSpace(m_playerDirection);
            Player::sharePlayer()->goTo(point);
            m_touchMouse->playEffect(point.getCCPointValue());
            break;
        }
        
        CC_BREAK_IF(Player::sharePlayer()->isMoveRunning());
        m_isPlayerMoveActions = true;
        Player::sharePlayer()->setAttackMonomerMajor(NULL);
        GAME_SCENE->OutOfCombat();
        this->playerRunning();
    }
    while (0);
}

void GameScene::stopMoveActions()
{
    m_isPlayerMoveActions = false;
    m_playerDirection = Point::ZERO;
}

void GameScene::AccurateMoveActions(const MapPoint& point)
{
    do
    {
        CC_BREAK_IF(!m_isPlayerMoveActions);
        CC_BREAK_IF(Player::sharePlayer()->isMoveRunning());
        m_playerDirection = point.getCCPointValue();
        CC_BREAK_IF(Player::sharePlayer()->getStateType() != FStateStand);
        this->beginMoveActions();
    }
    while (0);
}

//#pragma mark -
//#pragma mark androidKeyMenu

//modify
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)//keyBackClicked()
{
    if (Director::getInstance()->getRunningScene()->getChildByTag(0xffffff))
        return;
    
    LAlertView* alert = LAlertView::create("��ʾ", "�Ƿ��˳���Ϸ?");
    alert->addButtonWithTitle("��");
    alert->addButtonWithTitle("��");
    alert->show(this, Alert_selector(GameScene::alertCallBack));
    alert->setTag(0xffffff);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)//keyMenuClicked
{
    
}

void GameScene::alertCallBack(const int number)
{
    if (number == 0)
    {
        Director::getInstance()->end();
    }
}

void GameScene::log(float dt)
{
    MapPoint p = MapPoint(m_gMapPoint[Player::sharePlayer()]);
    MapPoint q = MapPoint(Player::sharePlayer()->getPosition());
    //CCLog("playerRunning x=%d, z=%d, x=%d, z=%d ", p.x, p.z, q.x ,q.z);
}