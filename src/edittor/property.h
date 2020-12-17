#ifndef PROPERTY_H
#define PROPERTY_H

#include <QDialog>

namespace Ui {
class Property;
}

class Property : public QDialog
{
    Q_OBJECT

public:
    explicit Property(QWidget *parent, 
        std::string& strPropertyName, 
        std::string& strPropertyValue );
    ~Property();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Property *ui;
    std::string& m_strPropertyName;
    std::string& m_strPropertyValue;
};

#endif // PROPERTY_H
