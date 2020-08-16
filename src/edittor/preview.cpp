#include "preview.h"
#include "ui_preview.h"

#include <sstream>
#include <string>

#include <QMouseEvent>
#include <QProcess>
#include <QFileDialog>



Preview::Preview(QWidget *parent, Blueprint::Edit::Ptr pEdit) :
    QDialog(parent),
    ui(new Ui::Preview),
    m_pBlueprintEdit( pEdit )
{
    ui->setupUi(this);

    if( m_pBlueprintEdit && !m_pBlueprintEdit->getFilePath().empty() )
        ui->text_file->setText( m_pBlueprintEdit->getFilePath().c_str() );

    ui->type_combo->addItem( "sgcore" );
    ui->type_combo->addItem( "polyvox" );

    ui->text_border->setText( "8" );
    ui->text_height->setText( "4" );
    ui->text_scale->setText( "0.125" );
}

Preview::~Preview()
{
    delete ui;
}


void Preview::on_buttonBox_accepted()
{
    const std::string strBlueprintFile = ui->text_file->text().toStdString();
    if( !strBlueprintFile.empty() )
    {
        std::ostringstream os;
        os << "C:/WORKSPACE/Blueprint/bin/Win32/Release/generator.exe --file " << strBlueprintFile;

        const std::string strOutputFile = ui->text_output->text().toStdString();
        if( !strOutputFile.empty() )
            os << " --output " << strOutputFile;


        const std::string strTextureFile = ui->text_texture->text().toStdString();
        if( !strTextureFile.empty() )
            os << " --tex " << strTextureFile;

        const std::string strHeight = ui->text_height->text().toStdString();
        if( !strHeight.empty() )
        {
            std::istringstream is( strHeight );
            double dHeight = 1.0;
            is >> dHeight;
            os << " --height " << dHeight;
        }

        const std::string strBorder = ui->text_border->text().toStdString();
        if( !strBorder.empty() )
        {
            std::istringstream is( strBorder );
            double dBorder = 1.0;
            is >> dBorder;
            os << " --border " << dBorder;
        }

        const std::string strScale = ui->text_scale->text().toStdString();
        if( !strScale.empty() )
        {
            std::istringstream is( strScale );
            double dScale = 1.0;
            is >> dScale;
            os << " --scale " << dScale;
        }

        if( ui->smoothed->checkState() == Qt::Checked )
            os << " --smooth true";

        if( ui->doSubtract->checkState() == Qt::Checked )
            os << " --subtract true";
        else
            os << " --subtract false";

        if( ui->type_combo->currentIndex() == 0 )
            os << " --type sgcore";
        else
            os << " --type polyvox";

        QProcess* pProcess = new QProcess;
        pProcess->start( os.str().c_str() );
    }
}

void Preview::on_btnFile_clicked()
{
    QString strFilePath =
            QFileDialog::getOpenFileName( this,
                tr( "Open Blueprint" ), "C:/WORKSPACE/Blueprint/data",
                tr( "Blueprint Files (*.blu)" ) );
    ui->text_file->setText( strFilePath );
}

void Preview::on_btnModel_clicked()
{
    QString strFilePath =
            QFileDialog::getOpenFileName( this,
                tr( "Open Model" ), "C:/WORKSPACE/Blueprint/data",
                tr( "Any File (*.*)" ) );
    ui->text_model->setText( strFilePath );
}

void Preview::on_btnOutput_clicked()
{
    QString strFilePath =
            QFileDialog::getSaveFileName( this,
                tr( "Choose model file" ), "C:/WORKSPACE/Blueprint/data",
                 tr( "Any File (*.*)" ) );
    ui->text_output->setText( strFilePath );
}

void Preview::on_btnTexture_clicked()
{
    QString strFilePath =
            QFileDialog::getOpenFileName( this,
                tr( "Open Texture" ), "C:/WORKSPACE/Blueprint/data",
                tr( "Any File (*.*)" ) );
    ui->text_texture->setText( strFilePath );
}

void Preview::on_type_combo_activated(const QString&)
{

}

void Preview::on_slider_height_actionTriggered(int )
{

}

void Preview::on_slider_border_actionTriggered(int )
{

}

void Preview::on_slider_scale_actionTriggered(int )
{

}
