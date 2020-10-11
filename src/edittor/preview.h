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

    void on_btnInner_clicked();

    void on_btnOuter_clicked();

    void on_btnOutput_clicked();

private:
    Ui::Preview *ui;
    Blueprint::Edit::Ptr m_pBlueprintEdit;
};

#endif // PREVIEW_H
