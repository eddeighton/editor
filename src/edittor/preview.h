#ifndef PREVIEW_H
#define PREVIEW_H

#include <QDialog>

#ifndef Q_MOC_RUN
#include "blueprint/edit.h"
#endif

namespace Ui {
class Preview;
}



class Preview : public QDialog
{
    Q_OBJECT

public:
    explicit Preview(QWidget *parent, Blueprint::Edit::Ptr pEdit );
    ~Preview();

private slots:
    void on_buttonBox_accepted();

    void on_btnFile_clicked();

    void on_btnModel_clicked();

    void on_btnOutput_clicked();

    void on_btnTexture_clicked();

    void on_type_combo_activated(const QString &arg1);

    void on_slider_height_actionTriggered(int action);

    void on_slider_border_actionTriggered(int action);

    void on_slider_scale_actionTriggered(int action);

private:
    Ui::Preview *ui;
    Blueprint::Edit::Ptr m_pBlueprintEdit;
};

#endif // PREVIEW_H
