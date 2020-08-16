/********************************************************************************
** Form generated from reading UI file 'preview.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEW_H
#define UI_PREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
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
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_6;
    QLineEdit *text_model;
    QPushButton *btnOutput;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_2;
    QLineEdit *text_output;
    QPushButton *btnModel;
    QLabel *label_5;
    QLabel *label;
    QPushButton *btnFile;
    QLineEdit *text_file;
    QLineEdit *text_texture;
    QPushButton *btnTexture;
    QSlider *slider_height;
    QLineEdit *text_height;
    QSlider *slider_border;
    QSlider *slider_scale;
    QCheckBox *smoothed;
    QCheckBox *doSubtract;
    QLineEdit *text_border;
    QLineEdit *text_scale;
    QComboBox *type_combo;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Preview)
    {
        if (Preview->objectName().isEmpty())
            Preview->setObjectName(QStringLiteral("Preview"));
        Preview->resize(460, 398);
        Preview->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(Preview);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_7 = new QLabel(Preview);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        label_3 = new QLabel(Preview);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_6 = new QLabel(Preview);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        text_model = new QLineEdit(Preview);
        text_model->setObjectName(QStringLiteral("text_model"));
        text_model->setEnabled(false);

        gridLayout->addWidget(text_model, 1, 1, 1, 1);

        btnOutput = new QPushButton(Preview);
        btnOutput->setObjectName(QStringLiteral("btnOutput"));

        gridLayout->addWidget(btnOutput, 2, 2, 1, 1);

        label_4 = new QLabel(Preview);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_8 = new QLabel(Preview);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        label_2 = new QLabel(Preview);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        text_output = new QLineEdit(Preview);
        text_output->setObjectName(QStringLiteral("text_output"));
        text_output->setEnabled(false);

        gridLayout->addWidget(text_output, 2, 1, 1, 1);

        btnModel = new QPushButton(Preview);
        btnModel->setObjectName(QStringLiteral("btnModel"));

        gridLayout->addWidget(btnModel, 1, 2, 1, 1);

        label_5 = new QLabel(Preview);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label = new QLabel(Preview);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        btnFile = new QPushButton(Preview);
        btnFile->setObjectName(QStringLiteral("btnFile"));

        gridLayout->addWidget(btnFile, 0, 2, 1, 1);

        text_file = new QLineEdit(Preview);
        text_file->setObjectName(QStringLiteral("text_file"));
        text_file->setEnabled(false);

        gridLayout->addWidget(text_file, 0, 1, 1, 1);

        text_texture = new QLineEdit(Preview);
        text_texture->setObjectName(QStringLiteral("text_texture"));
        text_texture->setEnabled(false);

        gridLayout->addWidget(text_texture, 3, 1, 1, 1);

        btnTexture = new QPushButton(Preview);
        btnTexture->setObjectName(QStringLiteral("btnTexture"));

        gridLayout->addWidget(btnTexture, 3, 2, 1, 1);

        slider_height = new QSlider(Preview);
        slider_height->setObjectName(QStringLiteral("slider_height"));
        slider_height->setMinimum(1);
        slider_height->setMaximum(32);
        slider_height->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_height, 5, 1, 1, 1);

        text_height = new QLineEdit(Preview);
        text_height->setObjectName(QStringLiteral("text_height"));

        gridLayout->addWidget(text_height, 5, 2, 1, 1);

        slider_border = new QSlider(Preview);
        slider_border->setObjectName(QStringLiteral("slider_border"));
        slider_border->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_border, 6, 1, 1, 1);

        slider_scale = new QSlider(Preview);
        slider_scale->setObjectName(QStringLiteral("slider_scale"));
        slider_scale->setMaximum(100);
        slider_scale->setSingleStep(1);
        slider_scale->setValue(0);
        slider_scale->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_scale, 7, 1, 1, 1);

        smoothed = new QCheckBox(Preview);
        smoothed->setObjectName(QStringLiteral("smoothed"));

        gridLayout->addWidget(smoothed, 8, 1, 1, 1);

        doSubtract = new QCheckBox(Preview);
        doSubtract->setObjectName(QStringLiteral("doSubtract"));
        doSubtract->setChecked(true);

        gridLayout->addWidget(doSubtract, 9, 1, 1, 1);

        text_border = new QLineEdit(Preview);
        text_border->setObjectName(QStringLiteral("text_border"));

        gridLayout->addWidget(text_border, 6, 2, 1, 1);

        text_scale = new QLineEdit(Preview);
        text_scale->setObjectName(QStringLiteral("text_scale"));

        gridLayout->addWidget(text_scale, 7, 2, 1, 1);

        type_combo = new QComboBox(Preview);
        type_combo->setObjectName(QStringLiteral("type_combo"));

        gridLayout->addWidget(type_combo, 4, 1, 1, 2);

        gridLayout->setRowStretch(1, 1);
        gridLayout->setColumnMinimumWidth(1, 400);

        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(Preview);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
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
        Preview->setWindowTitle(QApplication::translate("Preview", "Dialog", 0));
        label_7->setText(QApplication::translate("Preview", "Border", 0));
        label_3->setText(QApplication::translate("Preview", "Output:", 0));
        label_6->setText(QApplication::translate("Preview", "Height", 0));
        btnOutput->setText(QApplication::translate("Preview", "Choose...", 0));
        label_4->setText(QApplication::translate("Preview", "Texture:", 0));
        label_8->setText(QApplication::translate("Preview", "Scale", 0));
        label_2->setText(QApplication::translate("Preview", "Test Model:", 0));
        btnModel->setText(QApplication::translate("Preview", "Choose...", 0));
        label_5->setText(QApplication::translate("Preview", "Type", 0));
        label->setText(QApplication::translate("Preview", "Blueprint File:", 0));
        btnFile->setText(QApplication::translate("Preview", "Choose...", 0));
        btnTexture->setText(QApplication::translate("Preview", "Choose...", 0));
        text_height->setText(QString());
        text_height->setPlaceholderText(QApplication::translate("Preview", "1", 0));
        smoothed->setText(QApplication::translate("Preview", "Smooth Polyvox Mesh", 0));
        doSubtract->setText(QApplication::translate("Preview", "Do SGCore Subtractions", 0));
        text_border->setText(QString());
        text_border->setPlaceholderText(QApplication::translate("Preview", "1", 0));
        text_scale->setText(QString());
        text_scale->setPlaceholderText(QApplication::translate("Preview", "1", 0));
    } // retranslateUi

};

namespace Ui {
    class Preview: public Ui_Preview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEW_H
