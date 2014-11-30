
#include "BgMapFloorTile.h"

BgMapFloorTile::BgMapFloorTile()
:m_fileName(NULL)
,m_sprite(NULL)
,m_nIsDisplay(false)
{
    
}

BgMapFloorTile::~BgMapFloorTile()
{
    CC_SAFE_DELETE(m_fileName);
}

void BgMapFloorTile::displayImageView()
{
    if (m_nIsDisplay == false)
    {
        m_nIsDisplay = true;
        TextureCache::sharedTextureCache()->addImageAsync(m_fileName->getCString(),
			CC_CALLBACK_1(BgMapFloorTile::initWithImageView,this));
    }
}

void BgMapFloorTile::hideImageView()
{
    if (m_nIsDisplay)
    {
        m_nIsDisplay = false;
        if (m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite = NULL;
        }
        TextureCache::sharedTextureCache()->removeTextureForKey(m_fileName->getCString());
    }
}

void BgMapFloorTile::initWithImageView(Texture2D* texture)
{
    if (m_sprite == NULL)
    {
        m_sprite = Sprite::createWithTexture(texture);
        m_sprite->setAnchorPoint(CCPointZero);
        this->addChild(m_sprite);
    }
}

void BgMapFloorTile::IntelligentDisplay(Rect& showRect, Rect& hideRect)
{
    Point point = ccpAdd(this->getPosition(), OFF_SIZE);
    
    if (showRect.containsPoint(point))
    {
        this->displayImageView();
    }
    
    if (!hideRect.containsPoint(point))
    {
        this->hideImageView();
    }
    
}