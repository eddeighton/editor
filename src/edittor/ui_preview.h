/********************************************************************************
** Form generated from reading UI file 'preview.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEW_H
#define UI_PREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Preview
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *text_inner;
    QPushButton *btnInner;
    QLabel *label_2;
    QLineEdit *text_outer;
    QPushButton *btnOuter;
    QLabel *label_3;
    QSlider *slider_extrusion;
    QLineEdit *text_extrusion;
    QLabel *label_31;
    QCheckBox *convex;
    QLabel *label_32;
    QLineEdit *text_output;
    QPushButton *btnOutput;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Preview)
    {
        if (Preview->objectName().isEmpty())
            Preview->setObjectName(QString::fromUtf8("Preview"));
        Preview->resize(460, 100);
        Preview->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(Preview);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Preview);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        text_inner = new QLineEdit(Preview);
        text_inner->setObjectName(QString::fromUtf8("text_inner"));
        text_inner->setEnabled(false);

        gridLayout->addWidget(text_inner, 0, 1, 1, 1);

        btnInner = new QPushButton(Preview);
        btnInner->setObjectName(QString::fromUtf8("btnInner"));

        gridLayout->addWidget(btnInner, 0, 2, 1, 1);

        label_2 = new QLabel(Preview);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        text_outer = new QLineEdit(Preview);
        text_outer->setObjectName(QString::fromUtf8("text_outer"));
        text_outer->setEnabled(false);

        gridLayout->addWidget(text_outer, 1, 1, 1, 1);

        btnOuter = new QPushButton(Preview);
        btnOuter->setObjectName(QString::fromUtf8("btnOuter"));

        gridLayout->addWidget(btnOuter, 1, 2, 1, 1);

        label_3 = new QLabel(Preview);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        slider_extrusion = new QSlider(Preview);
        slider_extrusion->setObjectName(QString::fromUtf8("slider_extrusion"));
        slider_extrusion->setMinimum(0);
        slider_extrusion->setMaximum(8);
        slider_extrusion->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_extrusion, 2, 1, 1, 1);

        text_extrusion = new QLineEdit(Preview);
        text_extrusion->setObjectName(QString::fromUtf8("text_extrusion"));

        gridLayout->addWidget(text_extrusion, 2, 2, 1, 1);

        label_31 = new QLabel(Preview);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout->addWidget(label_31, 3, 0, 1, 1);

        convex = new QCheckBox(Preview);
        convex->setObjectName(QString::fromUtf8("convex"));

        gridLayout->addWidget(convex, 3, 1, 1, 1);

        label_32 = new QLabel(Preview);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout->addWidget(label_32, 4, 0, 1, 1);

        text_output = new QLineEdit(Preview);
        text_output->setObjectName(QString::fromUtf8("text_output"));
        text_output->setEnabled(false);

        gridLayout->addWidget(text_output, 4, 1, 1, 1);

        btnOutput = new QPushButton(Preview);
        btnOutput->setObjectName(QString::fromUtf8("btnOutput"));

        gridLayout->addWidget(btnOutput, 4, 2, 1, 1);

        gridLayout->setRowStretch(1, 1);
        gridLayout->setColumnMinimumWidth(1, 400);

        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(Preview);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Preview);
        QObject::connect(buttonBox, SIGNAL(accepted()), Preview, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Preview, SLOT(reject()));

        QMetaObject::connectSlotsByName(Preview);
    } // setupUi

    void retranslateUi(QDialog *Preview)
    {
        Preview->setWindowTitle(QApplication::translate("Preview", "Dialog", nullptr));
        label->setText(QApplication::translate("Preview", "Inner:", nullptr));
        btnInner->setText(QApplication::translate("Preview", "Select", nullptr));
        label_2->setText(QApplication::translate("Preview", "Outer:", nullptr));
        btnOuter->setText(QApplication::translate("Preview", "Select", nullptr));
        label_3->setText(QApplication::translate("Preview", "Output:", nullptr));
        text_extrusion->setText(QString());
        text_extrusion->setPlaceholderText(QApplication::translate("Preview", "1", nullptr));
        label_31->setText(QApplication::translate("Preview", "Convex:", nullptr));
        convex->setText(QApplication::translate("Preview", "Convex Hull:", nullptr));
        label_32->setText(QApplication::translate("Preview", "Output:", nullptr));
        btnOutput->setText(QApplication::translate("Preview", "Choose...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Preview: public Ui_Preview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEW_H
