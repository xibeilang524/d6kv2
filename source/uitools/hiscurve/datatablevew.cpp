#include <QPrinter>
#include <QPainter>
#include <QHeaderView>
#include <QTimer>
#include "datacurveapp.h"

#include "datatableview.h"


DataTableView::DataTableView( QWidget * parent /*= 0*/ )
:QTableView(parent),currentSelector(static_cast<DataCurveApp*>(qApp)->selpara)
{
	setShowGrid(true);
}

DataTableView::~DataTableView()
{

}


void DataTableView::print( QPrinter *printer )
{
	if (!printer)
		return;
	
	QPainter p;
	if ( !p.begin(printer))
		return;				// paint on printer
	
	int dpiy = printer->logicalDpiY();
	const int margin = (int) ( (1/2.54)*dpiy ); // 1 cm margins
	
	
	//QHeaderView *vHeader = d_table_view->verticalHeader();
	
// 	int rows = numRows();
// 	int cols = numCols();
// 	int height = margin;
// 	int i, vertHeaderWidth = vHeader->width();
// 	int right = margin + vertHeaderWidth;
	
	// print header
/*
	p.setFont(QFont());
	QString header_label = d_matrix_model->headerData(0, Qt::Horizontal).toString();
	QRect br = p.boundingRect(br, Qt::AlignCenter, header_label);
	p.drawLine(right, height, right, height+br.height());
	QRect tr(br);
	
	for(i=0; i<cols; i++){
		int w = d_table_view->columnWidth(i);
		tr.setTopLeft(QPoint(right,height));
		tr.setWidth(w);
		tr.setHeight(br.height());
		header_label = d_matrix_model->headerData(i, Qt::Horizontal).toString();
		p.drawText(tr, Qt::AlignCenter, header_label,-1);
		right += w;
		p.drawLine(right, height, right, height+tr.height());
		
		if (right >= printer->width()-2*margin )
			break;
	}
	
	p.drawLine(margin + vertHeaderWidth, height, right-1, height);//first horizontal line
	height += tr.height();
	p.drawLine(margin, height, right-1, height);
	
	// print table values
	for(i=0;i<rows;i++){
		right = margin;
		QString cell_text = d_matrix_model->headerData(i, Qt::Horizontal).toString()+"\t";
		tr = p.boundingRect(tr, Qt::AlignCenter, cell_text);
		p.drawLine(right, height, right, height+tr.height());
		
		br.setTopLeft(QPoint(right,height));
		br.setWidth(vertHeaderWidth);
		br.setHeight(tr.height());
		p.drawText(br,Qt::AlignCenter,cell_text,-1);
		right += vertHeaderWidth;
		p.drawLine(right, height, right, height+tr.height());
		
		for(int j=0; j<cols; j++){
			int w = d_table_view->columnWidth (j);
			cell_text = text(i,j)+"\t";
			tr = p.boundingRect(tr,Qt::AlignCenter,cell_text);
			br.setTopLeft(QPoint(right,height));
			br.setWidth(w);
			br.setHeight(tr.height());
			p.drawText(br, Qt::AlignCenter, cell_text, -1);
			right += w;
			p.drawLine(right, height, right, height+tr.height());
			
			if (right >= printer->width()-2*margin )
				break;
		}
		height += br.height();
		p.drawLine(margin, height, right-1, height);
		
		if (height >= printer->height()-margin ){
			printer->newPage();
			height = margin;
			p.drawLine(margin, height, right, height);
		}
	}
*/
	p.end();
}


void DataTableView::reset()
{
	for(int i=2; i<model()->columnCount() ;i+=2)
	{
		setColumnHidden(i,false);
	}

	for(int i=2; i<model()->columnCount() ;i+=2)
	{
		if(!currentSelector.isCompType())
		{
			setColumnHidden(i,true);
		}
	}
	QTableView::reset();
}




