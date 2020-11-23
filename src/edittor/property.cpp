#include "property.h"
#include "ui_property.h"

#include <sstream>
#include <string>

#include <QMouseEvent>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcessEnvironment>

Property::Property(QWidget *parent, 
        std::string& strPropertyName, 
        std::string& strPropertyValue )
     :
        QDialog(parent),
        ui(new Ui::Property), 
        m_strPropertyName( strPropertyName ), 
        m_strPropertyValue( strPropertyValue )
{
    ui->setupUi(this);
    
    ui->property_name_value->setText( QString::fromUtf8( strPropertyName.c_str() ) );
    ui->property_value_value->setText( QString::fromUtf8( strPropertyValue.c_str() ) );
}

Property::~Property()
{
    delete ui;
}

void Property::on_buttonBox_accepted()
{
    m_strPropertyName   = ui->property_name_value->text().toStdString();
    m_strPropertyValue  = ui->property_value_value->text().toStdString();
}
