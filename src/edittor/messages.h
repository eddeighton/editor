#ifndef MESSAGES_H
#define MESSAGES_H


#ifndef Q_MOC_RUN
#include "blueprint/node.h"
#include "blueprint/editMain.h"
#include "blueprint/toolbox.h"
#endif

//Q_DECLARE_METATYPE
struct BlueprintMsg
{
    BlueprintMsg( Blueprint::Node::PtrCst _pNode ) : pNode( _pNode ) {}
    Blueprint::Node::PtrCst pNode;
};

struct BlueprintContext
{
    BlueprintContext( Blueprint::IEditContext* _pContext )
        :   pContext( _pContext )
    {}
    Blueprint::IEditContext* pContext;
};

struct ClipboardMsg
{
    ClipboardMsg(){}
    ClipboardMsg( Blueprint::Site::Ptr _pSite, Blueprint::Toolbox::Palette::Ptr _pPalette )
        :   pSite( _pSite ),
            pPalette( _pPalette )
    {}

    Blueprint::Site::Ptr pSite;
    Blueprint::Toolbox::Palette::Ptr pPalette;
};
Q_DECLARE_METATYPE( ClipboardMsg )

#endif // MESSAGES_H
