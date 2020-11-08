#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileSystemModel>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QProcessEnvironment>

#include "blueprintitemmodel.h"
#include "blueprintselectionmodel.h"

#ifndef Q_MOC_RUN
#include <functional>
#include "blueprint/site.h"
#include "blueprint/edit.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pToolActionGroup = new QActionGroup( this );
    m_pModeActionGroup = new QActionGroup( this );

    ui->actionNew->setIcon( QIcon( ":/art/file.png" ) );
    ui->actionLoad->setIcon( QIcon( ":/art/folderOpen.png" ) );
    ui->actionSave->setIcon( QIcon( ":/art/disk.png" ) );
    ui->actionSave_As->setIcon( QIcon( ":/art/disk_saveas.png" ) );

    ui->actionCut->setIcon( QIcon( ":/art/cut.png" ) );
    ui->actionCopy->setIcon( QIcon( ":/art/clipboard_copy.png" ) );
    ui->actionPaste->setIcon( QIcon( ":/art/clipboard_paste.png" ) );

    ui->dockProperties->toggleViewAction()->setIcon(    QIcon( ":/art/dialog.png" ) );
    ui->dockToolbox->toggleViewAction()->setIcon(       QIcon( ":/art/folder.png" ) );

    ui->actionMode_Bitmap->setIcon(         QIcon( ":/art/joystick.png" ) );
    ui->actionMode_CellComplex->setIcon(    QIcon( ":/art/scan.png" ) );
    ui->actionMode_Clearance->setIcon(      QIcon( ":/art/rook.png" ) );
    
    ui->actionRotate_Left->setIcon( QIcon( ":/art/arrow_cycle.png" ) );
    ui->actionRotate_Right->setIcon( QIcon( ":/art/arrow_cycle.png" ) );
    ui->actionFlip_Horizontal->setIcon( QIcon( ":/art/arrow_right.png" ) );
    ui->actionFlip_Vertical->setIcon( QIcon( ":/art/arrow_up.png" ) );
    
    //ui->actionSelect->setIcon( QIcon( ":/art/disk_saveas.png" ) );
    ui->actionSelect->setCheckable( true );
    ui->actionPen->setIcon( QIcon( ":/art/pen_blue.png" ) );
    ui->actionPen->setCheckable( true );
    
    ui->actionArea->setCheckable( true );
    ui->actionContour->setCheckable( true );
    ui->actionConnection->setCheckable( true );
    
    m_pModeActionGroup->addAction( ui->actionArea );
    m_pModeActionGroup->addAction( ui->actionContour );
    m_pModeActionGroup->addAction( ui->actionConnection );
    m_pModeActionGroup->setExclusive( true );
    
    ui->actionMode_Bitmap->setCheckable( true );
    ui->actionMode_CellComplex->setCheckable( true );
    ui->actionMode_Clearance->setCheckable( true );
    
    ui->actionSelect->setData( -2 );
    ui->actionPen->setData( -1 );
    m_pToolActionGroup->addAction( ui->actionSelect );
    m_pToolActionGroup->addAction( ui->actionPen );
    m_pToolActionGroup->setExclusive( true );

    ui->mainToolBar->addAction( ui->actionNew );
    ui->mainToolBar->addAction( ui->actionLoad );
    ui->mainToolBar->addAction( ui->actionSave );
    ui->mainToolBar->addAction( ui->actionSave_As );

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction( ui->dockProperties->toggleViewAction() );
    ui->mainToolBar->addAction( ui->dockToolbox->toggleViewAction() );

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction( ui->actionCut );
    ui->mainToolBar->addAction( ui->actionCopy );
    ui->mainToolBar->addAction( ui->actionPaste );

    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction( ui->actionRotate_Left );
    ui->mainToolBar->addAction( ui->actionRotate_Right );
    ui->mainToolBar->addAction( ui->actionFlip_Horizontal );
    ui->mainToolBar->addAction( ui->actionFlip_Vertical );
    
    ui->mainToolBar->addSeparator();
    
    ui->mainToolBar->addWidget( new QLabel( "Snap", this ) );

    QComboBox* pQuantiseComboBox = new QComboBox( this );
    pQuantiseComboBox->addItem( "Off", 1 );
    pQuantiseComboBox->addItem( "2", 2 );
    pQuantiseComboBox->addItem( "4", 4 );
    pQuantiseComboBox->addItem( "8", 8 );
    pQuantiseComboBox->addItem( "16", 16 );
    pQuantiseComboBox->addItem( "32", 32 );
    pQuantiseComboBox->addItem( "64", 64 );
    pQuantiseComboBox->addItem( "128", 128 );
    pQuantiseComboBox->addItem( "256", 256 );
    pQuantiseComboBox->setCurrentIndex( 4 );
    QObject::connect( pQuantiseComboBox, SIGNAL( currentIndexChanged(int)),ui->graphicsView, SLOT( OnSetQuantise(int) ) );

    ui->mainToolBar->addWidget( pQuantiseComboBox );

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionSelect );
    ui->mainToolBar->addAction( ui->actionPen );

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionArea );
    ui->mainToolBar->addAction( ui->actionContour );
    ui->mainToolBar->addAction( ui->actionConnection );
    
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionMode_Bitmap );
    ui->mainToolBar->addAction( ui->actionMode_CellComplex );
    ui->mainToolBar->addAction( ui->actionMode_Clearance );
    
    ui->menuView->addAction( ui->dockProperties->toggleViewAction() );
    ui->dockProperties->toggleViewAction()->setShortcut( Qt::Key_F1 );
    ui->menuView->addAction( ui->dockToolbox->toggleViewAction() );
    ui->dockToolbox->toggleViewAction()->setShortcut( Qt::Key_F2 );
    
    QObject::connect( ui->actionNew,SIGNAL(triggered()),ui->graphicsView,SLOT(OnNewBlueprint() ));
    ui->actionNew->setShortcut( QKeySequence( "Ctrl+n" ) );
    QObject::connect( ui->actionLoad,SIGNAL(triggered()),ui->graphicsView,SLOT(OnLoadBlueprint() ));
    ui->actionLoad->setShortcut( QKeySequence( "Ctrl+o" ) );
    QObject::connect( ui->actionSave,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSaveBlueprint() ));
    ui->actionSave->setShortcut( QKeySequence( "Ctrl+s" ) );
    QObject::connect( ui->actionSave_As,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSaveAsBlueprint() ));
    ui->actionSave_As->setShortcut( QKeySequence( "Ctrl+Shift+s" ) );
    QObject::connect( ui->actionExit,SIGNAL(triggered()),this, SLOT(close()) );
    ui->actionExit->setShortcut( QKeySequence( "Ctrl+q" ) );

    QObject::connect( ui->actionSelect,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSelectTool_Selector() ));
    ui->actionSelect->setShortcut( Qt::Key_1 );
    QObject::connect( ui->actionPen,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSelectTool_Pen() ));
    ui->actionPen->setShortcut( Qt::Key_2 );
    
    QObject::connect( ui->actionArea,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSelectMode_Area() ));
    ui->actionArea->setShortcut( Qt::Key_3 );
    QObject::connect( ui->actionContour,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSelectMode_Contour() ));
    ui->actionContour->setShortcut( Qt::Key_4 );
    QObject::connect( ui->actionConnection,SIGNAL(triggered()),ui->graphicsView,SLOT(OnSelectMode_Connection() ));
    ui->actionConnection->setShortcut( Qt::Key_5 );
    
    QObject::connect( ui->actionCut,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Cut() ));
    ui->actionCut->setShortcut( QKeySequence( "Ctrl+x" ) );
    QObject::connect( ui->actionCopy,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Copy() ));
    ui->actionCopy->setShortcut( QKeySequence( "Ctrl+c" ) );
    QObject::connect( ui->actionPaste,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Paste() ));
    ui->actionPaste->setShortcut( QKeySequence( "Ctrl+v" ) );

    QObject::connect( ui->actionDelete,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Delete() ));
    ui->actionDelete->setShortcut( Qt::Key_Delete );

    QObject::connect( ui->actionZoom_Out,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_ZoomToAll() ));
    ui->actionZoom_Out->setShortcut( QKeySequence( "Shift+z" ) );
    QObject::connect( ui->actionTab_Out,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_TabOut() ));
    ui->actionTab_Out->setShortcut( Qt::Key_Tab );
    QObject::connect( ui->actionSelect_All,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_SelectAll() ));
    ui->actionSelect_All->setShortcut( QKeySequence( "Ctrl+a" ) );
    
    QObject::connect( ui->actionMode_Bitmap,SIGNAL(triggered()),this,SLOT(OnModeChanged() ));
    QObject::connect( ui->actionMode_CellComplex,SIGNAL(triggered()),this,SLOT(OnModeChanged() ));
    QObject::connect( ui->actionMode_Clearance,SIGNAL(triggered()),this,SLOT(OnModeChanged() ));
    
    QObject::connect( ui->actionRotate_Left,        SIGNAL(triggered()),    ui->graphicsView,SLOT( OnRotateLeft() ));
    QObject::connect( ui->actionRotate_Right,       SIGNAL(triggered()),    ui->graphicsView,SLOT( OnRotateRight() ));
    QObject::connect( ui->actionFlip_Horizontal,    SIGNAL(triggered()),    ui->graphicsView,SLOT( OnFlipHorizontally() ));
    QObject::connect( ui->actionFlip_Vertical,      SIGNAL(triggered()),    ui->graphicsView,SLOT( OnFlipVeritcally() ));
    
    QObject::connect( ui->actionExtrude,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Extrude() ));
    
    //QObject::connect( ui->actionUndo,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Undo() ));
    //ui->actionUndo->setShortcut( QKeySequence( "Ctrl+z" ) );
    //QObject::connect( ui->actionRedo,SIGNAL(triggered()),ui->graphicsView,SLOT(OnCmd_Redo() ));
    //ui->actionRedo->setShortcut( QKeySequence( "Ctrl+r" ) );

    m_pBlueprintItemModel = new BlueprintItemModel;
    m_pBlueprintSelectionModel = new BlueprintSelectionModel( m_pBlueprintItemModel );
    ui->treeView->setModel( m_pBlueprintItemModel );
    ui->treeView->setSelectionModel( m_pBlueprintSelectionModel );

    QObject::connect( ui->graphicsView,SIGNAL(OnBlueprintModified()),this,SLOT(OnBlueprintUpdate() ));
    QObject::connect( ui->graphicsView,SIGNAL(OnBlueprintModified()),m_pBlueprintItemModel,SLOT(OnBlueprintUpdate() ));

    QObject::connect( ui->graphicsView,&BlueprintView::OnBlueprintSelected,
                      this, &MainWindow::OnBlueprintSelected );
    QObject::connect( ui->graphicsView,&BlueprintView::OnBlueprintSelected,
                      m_pBlueprintItemModel, &BlueprintItemModel::OnBlueprintSelected );

    QObject::connect( m_pBlueprintSelectionModel, &BlueprintSelectionModel::currentChanged,
                      ui->graphicsView, &BlueprintView::OnCurrentSelectionItemChanged );
    QObject::connect( m_pBlueprintSelectionModel, &BlueprintSelectionModel::selectionChanged,
                      ui->graphicsView, &BlueprintView::OnSelectionChanged );

    QObject::connect( ui->graphicsView,&BlueprintView::OnEditContextChanged, this, &MainWindow::OnEditContext );
    
    QObject::connect( ui->graphicsView,&BlueprintView::OnWindowTitleModified,
                      this, &MainWindow::OnWindowTitleModified );

    ui->graphicsView->setModel( m_pBlueprintSelectionModel, m_pBlueprintItemModel );
    ui->graphicsView->OnNewBlueprint();

    ui->treeView->setAllColumnsShowFocus( true );
    ui->treeView->setAutoScroll( true );

    //load the toolbox
    {
        QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();

        //std::ostringstream osMsg;
        //for( const QString& str : environment.toStringList() )
        //{
        //    osMsg << str.toLocal8Bit().constData() << "\n";
        //}

        const QString strToolBoxPath = environment.value( "BLUEPRINT_TOOLBOX_PATH" );
        if( strToolBoxPath.isEmpty() )
        {
            THROW_RTE( "Missing BLUEPRINT_TOOLBOX_PATH environment variable" );//: " << osMsg.str() );
        }

        m_pToolbox.reset( new Blueprint::Toolbox( strToolBoxPath.toLocal8Bit().constData() ) );
        ui->graphicsView->setToolbox( m_pToolbox );
    }
    
    ui->actionSelect->trigger();
    ui->actionArea->trigger();

    ui->toolBox->setToolbox( m_pToolbox );

    QObject::connect( ui->graphicsView,&BlueprintView::OnClipboardAction, ui->toolBox, &BlueprintToolbox::updateToolbox );
    
    QObject::connect( 
        ui->toolBox, SIGNAL( currentChanged(int) ), 
        ui->toolBox, SLOT( onCurrentPaletteChanged(int) ) );

    qDebug() << "MainWindow::ctor complete";
}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_pBlueprintItemModel;
}

void MainWindow::OnWindowTitleModified( QString strTitle )
{
    setWindowTitle( strTitle );
}

void MainWindow::OnBlueprintUpdate()
{
}

void MainWindow::OnBlueprintSelected( BlueprintMsg msg )
{
    qDebug() << "MainWindow Recieved new blueprint: " << msg.pNode->getName().c_str();
}

void MainWindow::OnModeChanged()
{
    if( ui->graphicsView && 
        ui->actionMode_Bitmap && 
        ui->actionMode_CellComplex && 
        ui->actionMode_Clearance )
    {
        ui->graphicsView->setViewMode( 
            ui->actionMode_Bitmap->isChecked(),
            ui->actionMode_CellComplex->isChecked(),
            ui->actionMode_Clearance->isChecked() );
    }
}

void MainWindow::OnEditContext( BlueprintContext /*msg*/ )
{
    if( QAction* pCurrentAction = m_pToolActionGroup->checkedAction() )
    {
        pCurrentAction->trigger();
    }
    /*
    int iCurrentTool = -3;

    if( QAction* pCurrentAction = m_pToolActionGroup->checkedAction() )
        iCurrentTool = pCurrentAction->data().toInt();
*/
    /*for( std::vector< QAction* >::const_iterator
         i = m_contextActions.begin(),
         iEnd = m_contextActions.end(); i!=iEnd; ++i )
    {
        QAction* pAction = *i;
        ui->mainToolBar->removeAction( pAction );
        pAction->disconnect();
        m_pToolActionGroup->removeAction( pAction );
        delete pAction;
    }
    m_contextActions.clear();*/

    qDebug() << "MainWindow::OnEditContext: ";

    //setup the context tools?
    /*Blueprint::CmdTarget::CmdInfo::List cmds;
    msg.pContext->getCmds( cmds );

    Blueprint::CmdTarget::ToolInfo::List tools;
    msg.pContext->getTools( tools );
    for( Blueprint::CmdTarget::ToolInfo::List::const_iterator
         i = tools.begin(), iEnd = tools.end(); i!=iEnd; ++i )
    {
        QAction* pAction = new QAction( i->strName.c_str(), this );
        pAction->setData( i->uiToolID );
        pAction->setCheckable( true );
        pAction->setShortcut( Qt::Key_3 + i->uiToolID );
        m_contextActions.push_back( pAction );
        m_pToolActionGroup->addAction( pAction );

        QObject::connect( pAction, &QAction::triggered,
            std::function< void() >( std::bind( &BlueprintView::OnSelectTool_Context, ui->graphicsView, i->uiToolID ) )  );

        ui->mainToolBar->addAction( pAction );
    }*/

    /*if( iCurrentTool >= 0 && iCurrentTool < m_contextActions.size() )
    {
        m_contextActions[ iCurrentTool ]->trigger();
    }
    else
    {
        switch( iCurrentTool )
        {
            default:
            case -2:    ui->actionSelect->trigger();    break;
            case -1:    ui->actionPen->trigger();       break;
        }
    }*/
}


