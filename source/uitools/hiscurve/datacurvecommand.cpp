
#include <QHeaderView>
#include "datacurvecommand.h"
#include "mainwindow.h"
#include "datatableview.h"
#include "datacurveview.h"
#include "hisdatamodel.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataEditCellcommand::DataEditCellcommand( HisDataModel *model, const QModelIndex & index,
										 mData& befor,mData& val,const QString & text ):
QUndoCommand(text),
model(model),
index(index),
befor(befor),
val(val)
{
	setText(model->mainWindow()->objectName() + ": " + text);
}

void DataEditCellcommand::redo()
{
	if (!model)
		return;
	MainWindow *mainWin = model->mainWindow();
	if (mainWin){
		int mCurveColIdx=  index.column()/2;
		int mCurveRowIdx=  index.row()- model->curveBeginRow;
		if(mCurveColIdx >= model->modelCurveData.size())
			return ;
		
		mOneCurveData &mOneCurve = model->modelCurveData[mCurveColIdx];
		if(mCurveRowIdx >= mOneCurve.mDataSeq.size())
		return ;

		mOneCurve.mDataSeq[mCurveRowIdx].val= val.val;
		mOneCurve.mDataSeq[mCurveRowIdx].state =val.state;

		
//		model->reset();
//		mainWin->dataTableView()->verticalHeader()->reset();
//		mainWin->dataTableView()->reset();
//		mainWin->dataCurveView()->reset();
	}
}

void DataEditCellcommand::undo()
{
	if (!model)
		return;
	MainWindow *mainWin = model->mainWindow();
	if (mainWin)
	{
		int mCurveColIdx=  index.column()/2;
		int mCurveRowIdx=  index.row()- model->curveBeginRow;
		if(mCurveColIdx >= model->modelCurveData.size())
			return ;
		
		mOneCurveData &mOneCurve = model->modelCurveData[mCurveColIdx];
		if(mCurveRowIdx >= mOneCurve.mDataSeq.size())
			return ;
		
		mOneCurve.mDataSeq[mCurveRowIdx].val= befor.val;
		mOneCurve.mDataSeq[mCurveRowIdx].state =befor.state;
		
		//model->setData(index,QVariant(val));
		//model->setData(index,)
		//model->reset();
//		mainWin->dataTableView()->verticalHeader()->reset();
//    	mainWin->dataTableView()->reset();
//		mainWin->dataCurveView()->reset();
	}
}


