
#include "BgMap.h"
#include <fstream>
#include <math.h>
#include "figure/Player.h"
#include "game_scene/GameScene.h"
#include "game_scene/GameLoading.h"
#include "PortalSprite.h"
#include "SimpleAudioEngine.h"

#include "publicDef/PublicDef.h"

#define RADIUS_PASSAGEWAY 64

#define TYPE_PROTAL 100
#define TYPE_BORN_POINT 101
#define TYPE_NPC 200
#define TYPE_ENEMY 300
#define TYPE_BACKGROUND_MUSIC 400

#pragma -
#pragma JSON

#pragma -
#pragma BgMap

void BgMap::readGirdData()
{
    //CSJson::Reader reader;
    //CSJson::Value json;
	rapidjson::Document json;
    
    CCString* jsonPath = CCString::createWithFormat("map/s%d/data_gird_%d.json", m_mapID, m_mapID);
    std::string absolutePath = FileUtils::getInstance()->fullPathForFilename(jsonPath->getCString());
    CCString* filePath = CCString::createWithContentsOfFile(absolutePath.c_str());
    
    //ifstream filePath(file.c_str(), ios::in);
    
    CCAssert(filePath, "file is open fail!");
	json.Parse<0>(filePath->getCString());
	//校验Json
	if (json.HasParseError()) 
	{
		log("BgMap::readGirdData() GetParseError %s\n",json.GetParseError());
	}

    unsigned int width = json["mapW"].GetUint();
    unsigned int height = json["mapH"].GetUint();
    m_bgSize = Size(width, height);
    
    unsigned int gWidth = json["mapGridW"].GetUint();
    unsigned int gHeight = json["mapGridH"].GetUint();
    m_gridSize = Size(gWidth, gHeight);
    MapPoint::setGridSize(m_gridSize);
    m_gridRow = (unsigned int)ceilf(width/m_gridSize.width);
    m_gridCol = (unsigned int)ceilf(height/m_gridSize.height);
    
    //CSJson::Value arr = json["mapFlagArr"];
	const rapidjson::Value &arr = DICTOOL->getSubDictionary_json(json,"mapFlagArr");
    this->initGridData(arr);
    
    unsigned int iWidth = json["divideBlockW"].GetUint();
    unsigned int iHeight = json["divideBlockH"].GetUint();
    m_imageSize = Size(iWidth, iHeight);
    m_imageRow = (unsigned int)ceilf(width/m_imageSize.width);
    m_imageCol = (unsigned int)ceilf(height/m_imageSize.height);
}

void BgMap::readGoodsData()
{
    //CSJson::Reader reader;
    //CSJson::Value json;
    rapidjson::Document json;

    CCString* jsonPath = CCString::createWithFormat("map/s%d/data_goods_%d.json", m_mapID, m_mapID);
    std::string absolutePath = FileUtils::sharedFileUtils()->fullPathForFilename(jsonPath->getCString());
    CCString* filePath = CCString::createWithContentsOfFile(absolutePath.c_str());
    
    CCAssert(filePath, "file is open fail!");
	json.Parse<0>(filePath->getCString());
	//校验Json
	if (json.HasParseError()) 
	{
		log("BgMap::readGoodsData GetParseError %s\n",json.GetParseError());
	}
    
    unsigned int size = DICTOOL->getArrayCount_json(json,"items");//json["items"].size();
    for (unsigned int i=0; i<size; i++)
    {
        //CSJson::Value goods = json["items"][i];
		const rapidjson::Value &goods = DICTOOL->getSubDictionary_json(json,"items",i);
        
        switch (goods["itemType"].GetInt())
        {
            case TYPE_PROTAL:
            {
                int key = goods["itemSN"].GetInt();
                int mapID = goods["portalToMap"].GetInt();
                int born = goods["portalToPos"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
				//(PortalInformation){key, mapID, born, point}
				PortalInformation port = {key, mapID, born, point};
				m_mapPassageway.push_back(port);
            }
                break;
            case TYPE_BORN_POINT:
            {
                int born = goods["itemSN"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
                m_mapBornPoint[born] = point;
            }
                break;
            case TYPE_NPC:
            {
                int key = goods["itemSN"].GetInt();
                NPCDirectionType direction = (NPCDirectionType)goods["npcDirection"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
				//(NpcInformation){key, direction, point}
				NpcInformation npcInfo = {key, direction, point};
                m_npcInfoVec.push_back(npcInfo);
            }
                break;
            case TYPE_ENEMY:
            {
                unsigned int key = goods["itemSN"].GetUint();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
                m_enemyMap.insert(std::multimap<unsigned int, MapPoint>::value_type(key, MapPoint(point)));
            }
                break;
            case TYPE_BACKGROUND_MUSIC:
            {
                unsigned int key = goods["itemSN"].GetUint();
                char s[20];
                sprintf(s, "music/400/%d.mp3", key);
                m_backGroundMusic = s;
            }
                break;
            default:
                break;
        }
    }
}

BgMap::BgMap(int mapID)
:m_grid(NULL)
,m_gridRow(0)
,m_gridCol(0)
,m_gridSize(Size::ZERO)
,m_imageRow(0)
,m_imageCol(0)
,m_imageSize(Size::ZERO)
,m_bgSize(Size::ZERO)
,m_mapID(mapID)
,m_playerLead(NULL)
,m_delegate(NULL)
{
    this->readGirdData();
    this->readGoodsData();
}

BgMap::~BgMap()
{
    for (unsigned int i=0; i<m_gridCol; i++)
    {
        delete[] m_grid[i];
    }
    delete[] m_grid;
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

BgMap* BgMap::createWithPath(int mapID)
{
    BgMap* map = new BgMap(mapID);
    if (map && map->init())
    {
        map->autorelease();
        return map;
    }
    CC_SAFE_DELETE(map);
    return NULL;
}

bool BgMap::init()
{
    if (!Node::init())
        return false;

    char str[24];
    sprintf(str, "map/s%d/min_s%d.jpg", m_mapID, m_mapID);
    m_sMap = Sprite::create(str);
    m_sMap->setAnchorPoint(Point::ZERO);
    this->addChild(m_sMap, BgMap::getZOrderZero(this));
    m_sMap->setScale(10/3.0f);
    
    this->initBgMapFloorTile();
    
    this->initBgMapPassagewayImage();
    
    this->initNpcFigure();

    this->updateImageDisplay();

    if (m_backGroundMusic.compare("") != 0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(m_backGroundMusic.c_str(), true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
    }
    
    return true;
}

void BgMap::initGridData(const rapidjson::Value &json)
{
    m_grid = new unsigned char*[m_gridCol];
    for (unsigned int i=0; i<m_gridCol; i++)
    {
        m_grid[i] = new unsigned char[m_gridRow];
    }
    
    int size = json.Size();
    for (unsigned int i=0; i<size; i++)
    {
        m_grid[i/m_gridRow][i%m_gridRow] = json[i].GetInt();
    }
}

void BgMap::initBgMapFloorTile()
{
    for (unsigned int i=0; i<m_imageCol; i++)
    {
        for (unsigned int j=0; j<m_imageRow; j++)
        {
            BgMapFloorTile* floorTile = BgMapFloorTile::create();
            floorTile->setFloorTileFileName(CCString::createWithFormat("map/s%d/s%d_%d_%d.jpg", m_mapID, m_mapID, i, j));

            floorTile->setPosition(Point(m_imageSize.width*j, m_imageSize.height*i));
            this->addChild(floorTile, BgMap::getZOrderZero(this));//z 
            m_floorTileVec.push_back(floorTile);
        }
    }
}

void BgMap::initBgMapPassagewayImage()
{
    vector<struct PortalInformation>::iterator itr;
    for (itr=m_mapPassageway.begin(); itr!=m_mapPassageway.end(); itr++)
    {
        Point point = MapPoint((*itr).point).getCCPointValue();
        
        //生成传送门
        float value = BgMap::getZOrder(point);//z 
        
        PortalSprite* sprite = PortalSprite::createWithPortalSprite("trans-");
        sprite->setPosition(point);
        this->addChild(sprite, value);
    }
}

void BgMap::initNpcFigure()
{
    vector<struct NpcInformation>::iterator itr;
    for (itr=m_npcInfoVec.begin(); itr!=m_npcInfoVec.end(); itr++)
    {
        NpcFigure* npc = NpcFigure::create(itr->key, itr->directionType);
        npc->setPosition(itr->point);
        this->addChild(npc, this->getZOrder(npc->getPosition()));
        
        m_npcFigureVec.push_back(npc);
    }
}

void BgMap::updateImageDisplay()
{
    Rect rect = this->getShowRect();

    Rect rect2 = this->getHideRect();
    
    std::vector<BgMapFloorTile*>::iterator itr;
    for (itr=m_floorTileVec.begin(); itr!=m_floorTileVec.end(); itr++)
    {
        (*itr)->IntelligentDisplay(rect, rect2);
    }
    
    std::vector<NpcFigure*>:: iterator itr2;
    for (itr2=m_npcFigureVec.begin(); itr2!=m_npcFigureVec.end(); itr2++)
    {
        (*itr2)->IntelligentDisplay(rect, rect2);
    }
    
    if (m_delegate)
    {
        m_delegate->updateImageDisplay(rect, rect2);
    }
    
    TextureCache::sharedTextureCache()->removeUnusedTextures();
}

void BgMap::updateMapPosition(float delay)
{
    if (m_playerLead == NULL)
        return;

    Size winSize = Director::getInstance()->getWinSize();
    Point pMap = ccpSub(winSize/2, m_playerLead->getPosition());
    
    pMap.x = MAX(pMap.x, WINSIZE.width-this->getBgSize().width);
    pMap.y = MAX(pMap.y, WINSIZE.height-this->getBgSize().height);
    pMap.x = MIN(pMap.x, 0);
    pMap.y = MIN(pMap.y, 0);
    
    this->setPosition(pMap);
    
    this->updateImageDisplay();
}

unsigned char BgMap::getCurrentGridValue(const MapPoint& point)
{
    M_INT row = point.x;
    M_INT col = point.z;
    unsigned char relust = 1;
    
    if (1 < col && col < m_gridCol-1 && 1 < row &&row < m_gridRow-1)
    {
        relust = m_grid[col][row];
    }
    
    return relust;
}

PortalInformation BgMap::isMapPassageway(Node *node)
{
    vector<struct PortalInformation>::iterator itr;
    for (itr=m_mapPassageway.begin(); itr!=m_mapPassageway.end(); itr++)
    {
        struct PortalInformation mapPassageway = *itr;
        Point point = mapPassageway.point;
        if (ccpDistance(point, node->getPosition()) <= RADIUS_PASSAGEWAY)
        {
            return mapPassageway;
        }
    }
    return PortalInformationZero;
}

std::multimap<unsigned int, MapPoint>& BgMap::getEnemeyMap()
{
    return m_enemyMap;
}

void BgMap::addChildPlayerLead(Node* node, int bornPoint)
{
    map<int, Point>::iterator itr = m_mapBornPoint.find(bornPoint);
    Point point = MapPoint((*itr).second).getCCPointValue();
    this->addChildPlayerLead(node, point);
}

void BgMap::addChildPlayerLead(Node* node, Point point)
{
    float value = BgMap::getZOrder(this->getPosition());//z
    m_playerLead = node;
    m_playerLead->setPosition(MapPoint(point).getCCPointValue());
    this->addChild(m_playerLead, value);
    this->updateMapPosition();
}

Rect BgMap::getShowRect()
{
    Rect rect = Rect::ZERO;//CCRectZero;
    rect.origin = ccpSub(Point::ZERO, OFF_SIZE);
    rect.origin = ccpSub(rect.origin, this->getPosition());
    rect.size = ccpAdd(WINSIZE, OFF_SIZE * 2);
    return rect;
}

Rect BgMap::getHideRect()
{
    Rect rect =Rect::ZERO;// CCRectZero;
    rect.origin = ccpSub(Point::ZERO, OFF_SIZE * 2);
    rect.origin = ccpSub(rect.origin, this->getPosition());
    rect.size = ccpAdd(WINSIZE, OFF_SIZE * 4);
    return rect;
}