/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "blueprinttoolbox.h"
#include "blueprintview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSelect;
    QAction *actionPen;
    QAction *actionArea;
    QAction *actionContour;
    QAction *actionConnection;
    QAction *action;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionSelect_All;
    QAction *actionToolbox;
    QAction *actionProperties;
    QAction *actionShow_All;
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionLoad;
    QAction *actionExit;
    QAction *actionDelete;
    QAction *action_2;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionZoom_Out;
    QAction *actionTab_Out;
    QAction *actionPreview;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    BlueprintView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockToolbox;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    BlueprintToolbox *toolBox;
    QWidget *page;
    QWidget *page_2;
    QDockWidget *dockProperties;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    QTreeView *treeView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(812, 724);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionPen = new QAction(MainWindow);
        actionPen->setObjectName(QString::fromUtf8("actionPen"));
        actionArea = new QAction(MainWindow);
        actionArea->setObjectName(QString::fromUtf8("actionArea"));
        actionContour = new QAction(MainWindow);
        actionContour->setObjectName(QString::fromUtf8("actionContour"));
        actionConnection = new QAction(MainWindow);
        actionConnection->setObjectName(QString::fromUtf8("actionConnection"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionToolbox = new QAction(MainWindow);
        actionToolbox->setObjectName(QString::fromUtf8("actionToolbox"));
        actionProperties = new QAction(MainWindow);
        actionProperties->setObjectName(QString::fromUtf8("actionProperties"));
        actionShow_All = new QAction(MainWindow);
        actionShow_All->setObjectName(QString::fromUtf8("actionShow_All"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionTab_Out = new QAction(MainWindow);
        actionTab_Out->setObjectName(QString::fromUtf8("actionTab_Out"));
        actionPreview = new QAction(MainWindow);
        actionPreview->setObjectName(QString::fromUtf8("actionPreview"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new BlueprintView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMouseTracking(true);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::NoBrush);
        graphicsView->setBackgroundBrush(brush);
        graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::TextAntialiasing);
        graphicsView->setDragMode(QGraphicsView::NoDrag);
        graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
        graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);

        horizontalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 812, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockToolbox = new QDockWidget(MainWindow);
        dockToolbox->setObjectName(QString::fromUtf8("dockToolbox"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        toolBox = new BlueprintToolbox(dockWidgetContents);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 256, 182));
        toolBox->addItem(page, QString::fromUtf8("Default"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 98, 28));
        toolBox->addItem(page_2, QString::fromUtf8("Page 2"));

        verticalLayout->addWidget(toolBox);

        dockToolbox->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockToolbox);
        dockProperties = new QDockWidget(MainWindow);
        dockProperties->setObjectName(QString::fromUtf8("dockProperties"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        treeView = new QTreeView(dockWidgetContents_2);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setSelectionMode(QAbstractItemView::MultiSelection);

        verticalLayout_2->addWidget(treeView);

        dockProperties->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockProperties);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionZoom_Out);
        menuView->addAction(actionTab_Out);
        menuView->addSeparator();
        menuView->addAction(actionPreview);
        menuView->addSeparator();
        menuEdit->addAction(actionSelect);
        menuEdit->addAction(actionPen);
        menuEdit->addSeparator();
        menuEdit->addAction(actionArea);
        menuEdit->addAction(actionContour);
        menuEdit->addAction(actionConnection);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSelect->setText(QApplication::translate("MainWindow", "Select", nullptr));
        actionPen->setText(QApplication::translate("MainWindow", "Pen", nullptr));
        actionArea->setText(QApplication::translate("MainWindow", "Area", nullptr));
        actionContour->setText(QApplication::translate("MainWindow", "Contour", nullptr));
        actionConnection->setText(QApplication::translate("MainWindow", "Connection", nullptr));
        action->setText(QApplication::translate("MainWindow", "-", nullptr));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", nullptr));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", nullptr));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", nullptr));
        actionToolbox->setText(QApplication::translate("MainWindow", "Toolbox", nullptr));
        actionProperties->setText(QApplication::translate("MainWindow", "Properties", nullptr));
        actionShow_All->setText(QApplication::translate("MainWindow", "Show All", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "New", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", nullptr));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", nullptr));
        action_2->setText(QApplication::translate("MainWindow", "-", nullptr));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", nullptr));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", nullptr));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", nullptr));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", nullptr));
        actionTab_Out->setText(QApplication::translate("MainWindow", "Tab Out", nullptr));
        actionPreview->setText(QApplication::translate("MainWindow", "Preview", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuView->setTitle(QApplication::translate("MainWindow", "View", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        dockToolbox->setWindowTitle(QApplication::translate("MainWindow", "Toolbox", nullptr));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Default", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", nullptr));
        dockProperties->setWindowTitle(QApplication::translate("MainWindow", "Properties", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
