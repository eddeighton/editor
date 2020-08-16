#ifndef BLUEPRINTSELECTIONMODEL_H
#define BLUEPRINTSELECTIONMODEL_H

#include <QItemSelectionModel>
#include <QAbstractItemModel>

class BlueprintSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    explicit BlueprintSelectionModel(QAbstractItemModel *pModel = 0);

signals:

public slots:

};

#endif // BLUEPRINTSELECTIONMODEL_H
