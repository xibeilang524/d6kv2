#ifndef DATACURVECOMMAND_H
#define DATACURVECOMMAND_H

#include <QUndoCommand>
#include "hisdatamodel.h"

class DataEditCellcommand : public QUndoCommand  
{
public:
	DataEditCellcommand(HisDataModel *model, const QModelIndex & index,mData& befor,mData& val,const QString & text);
	virtual void redo();
    virtual void undo();
private:
	HisDataModel *model;
    QModelIndex  index;
	mData        befor;
	mData        val;
};

#endif
