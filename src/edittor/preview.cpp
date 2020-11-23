#include "preview.h"
#include "ui_preview.h"

#include <sstream>
#include <string>

#include <QMouseEvent>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcessEnvironment>

Preview::Preview(QWidget *parent, Blueprint::Edit::Ptr pEdit) :
    QDialog(parent),
    ui(new Ui::Preview),
    m_pBlueprintEdit( pEdit )
{
    ui->setupUi(this);

    if( m_pBlueprintEdit && !m_pBlueprintEdit->getFilePath().empty() )
    {
        const QString qstrBlueprintFile = QString::fromUtf8( m_pBlueprintEdit->getFilePath().c_str() );
        if( !qstrBlueprintFile.isEmpty() )
        {
            const QString filename = qstrBlueprintFile.split( ".", QString::SkipEmptyParts ).at( 0 );
            
            ui->text_inner->setText( filename + "_inner.blu" );
            ui->text_outer->setText( filename + "_outer.blu" );
            ui->text_output->setText( "" );
        }
    }
}

Preview::~Preview()
{
    delete ui;
}


void Preview::on_buttonBox_accepted()
{
    const std::string strBlueprintFile = ui->text_output->text().toStdString();
    if( m_pBlueprintEdit )
    {
        float dExtrusion = 0.0;
        {
            const std::string strHeight = ui->text_extrusion->text().toStdString();
            if( !strHeight.empty() )
            {
                std::istringstream is( strHeight );
                is >> dExtrusion;
            }
        }
        const bool bConvex = ui->convex->checkState() == Qt::Checked;
        try
        {
            std::set< Blueprint::IGlyph* > selection =
                m_pBlueprintEdit->generateExtrusion( dExtrusion, bConvex );
                
            if( !strBlueprintFile.empty() )
            {
                m_pBlueprintEdit->save( selection, strBlueprintFile );
            }
        }
        catch( std::runtime_error& ex )
        {
            QMessageBox::warning( this,
                                  tr( "Extrusion Failed" ),
                                  QString::fromUtf8( ex.what() ) );
        }
    }
    else
    {
        QMessageBox::warning( this,
                              tr( "Extrusion" ),
                              "Failed to generate extrusion blueprint." );
    }
}

void Preview::on_btnInner_clicked()
{
    ui->text_output->setText( ui->text_inner->text() );
}

void Preview::on_btnOuter_clicked()
{
    ui->text_output->setText( ui->text_outer->text() );
}

void Preview::on_btnOutput_clicked()
{
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString strFilePath =
            QFileDialog::getSaveFileName( this,
                tr( "Choose model file" ), 
                environment.value( "BLUEPRINT_TOOLBOX_PATH" ),
                tr( "Any File (*.*)" ) );
    ui->text_output->setText( strFilePath );
}
