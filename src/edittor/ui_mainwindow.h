/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
    QAction *actionErasor;
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(812, 724);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QStringLiteral("actionSelect"));
        actionPen = new QAction(MainWindow);
        actionPen->setObjectName(QStringLiteral("actionPen"));
        actionErasor = new QAction(MainWindow);
        actionErasor->setObjectName(QStringLiteral("actionErasor"));
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        actionToolbox = new QAction(MainWindow);
        actionToolbox->setObjectName(QStringLiteral("actionToolbox"));
        actionProperties = new QAction(MainWindow);
        actionProperties->setObjectName(QStringLiteral("actionProperties"));
        actionShow_All = new QAction(MainWindow);
        actionShow_All->setObjectName(QStringLiteral("actionShow_All"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        actionTab_Out = new QAction(MainWindow);
        actionTab_Out->setObjectName(QStringLiteral("actionTab_Out"));
        actionPreview = new QAction(MainWindow);
        actionPreview->setObjectName(QStringLiteral("actionPreview"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        graphicsView = new BlueprintView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
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
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 812, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockToolbox = new QDockWidget(MainWindow);
        dockToolbox->setObjectName(QStringLiteral("dockToolbox"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        toolBox = new BlueprintToolbox(dockWidgetContents);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 256, 182));
        toolBox->addItem(page, QStringLiteral("Default"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 98, 28));
        toolBox->addItem(page_2, QStringLiteral("Page 2"));

        verticalLayout->addWidget(toolBox);

        dockToolbox->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockToolbox);
        dockProperties = new QDockWidget(MainWindow);
        dockProperties->setObjectName(QStringLiteral("dockProperties"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        treeView = new QTreeView(dockWidgetContents_2);
        treeView->setObjectName(QStringLiteral("treeView"));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionSelect->setText(QApplication::translate("MainWindow", "Select", 0));
        actionPen->setText(QApplication::translate("MainWindow", "Pen", 0));
        actionErasor->setText(QApplication::translate("MainWindow", "Erasor", 0));
        action->setText(QApplication::translate("MainWindow", "-", 0));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", 0));
        actionToolbox->setText(QApplication::translate("MainWindow", "Toolbox", 0));
        actionProperties->setText(QApplication::translate("MainWindow", "Properties", 0));
        actionShow_All->setText(QApplication::translate("MainWindow", "Show All", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0));
        action_2->setText(QApplication::translate("MainWindow", "-", 0));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0));
        actionTab_Out->setText(QApplication::translate("MainWindow", "Tab Out", 0));
        actionPreview->setText(QApplication::translate("MainWindow", "Preview", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        dockToolbox->setWindowTitle(QApplication::translate("MainWindow", "Toolbox", 0));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Default", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", 0));
        dockProperties->setWindowTitle(QApplication::translate("MainWindow", "Properties", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
