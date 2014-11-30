#ifndef __mir9__ChatInterfaceCell__
#define __mir9__ChatInterfaceCell__

#include "UiControl/LyfTableViewCell.h"
#include "ChatInterface.h"

#include "publicDef/PublicDef.h"

class ChatInterfaceCell: public LyfTableViewCell
{
    
    CC_SYNTHESIZE(unsigned int, m_lenght, Lenght);
    
    Label* m_chatTypeTTF;
    
    Label* m_speakerTTF;
    
    Label* m_contentTTF;
    
    ChatInfoObject* m_object;

public:
    
    ChatInterfaceCell(unsigned int lenght);
    
    ~ChatInterfaceCell();
    
    static ChatInterfaceCell* create(unsigned int lenght);
    
    void setString(ChatInfoObject& object);
    
};


#endif /* defined(__mir9__ChatInterfaceCell__) */
