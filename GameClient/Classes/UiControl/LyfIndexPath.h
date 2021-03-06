#ifndef __DDHero__LyfIndexPath__
#define __DDHero__LyfIndexPath__


#include "publicDef/PublicDef.h"

#define MAXCELLCOUNT 30


class LyfIndexPath: public Ref {
    
public:
    
    int row;
    
    int page;
    
    int maxRowOfPage;
    
public:

    virtual ~LyfIndexPath();
    
    LyfIndexPath(int row, int page, int maxRowOfPage);

    LyfIndexPath(int index, int maxRowOfPage);

    LyfIndexPath(const LyfIndexPath& otherIndexPath);

    LyfIndexPath& operator= (const LyfIndexPath& other);
    
    int getIndex();

    bool isEqual(LyfIndexPath otherPath);

};

const LyfIndexPath LyfIndexPathZero = LyfIndexPath(0, 0, 0);

#endif /* defined(__DDHero__LyfIndexPath__) */
