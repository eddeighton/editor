#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QAction>

#include "messages.h"

#ifndef Q_MOC_RUN
//#include "common/log.h"
#include "blueprint/toolbox.h"
#endif

class BlueprintItemModel;
class BlueprintSelectionModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnBlueprintUpdate();
    void OnBlueprintSelected( BlueprintMsg msg );
    void OnEditContext( BlueprintContext msg );

private:
    Ui::MainWindow *ui;
    BlueprintItemModel* m_pBlueprintItemModel;
    BlueprintSelectionModel* m_pBlueprintSelectionModel;

    //std::vector< QAction* > m_contextActions;
    QActionGroup* m_pToolActionGroup;
    QActionGroup* m_pModeActionGroup;

    Blueprint::Toolbox::Ptr m_pToolbox;

};

#endif // MAINWINDOW_H
