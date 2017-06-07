// QListViewColorItem.cpp: implementation of the QListViewColorItem class.
//
//////////////////////////////////////////////////////////////////////

#include "list_view_color_item.h"
#include <QHeaderView>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

List_view_color_item::List_view_color_item( Event_cfg *pCfg,  QTreeWidget * parent, QColor &clr )
	:QTreeWidgetItem( parent )
{
	m_color = clr;
	m_cfg = pCfg;

// 	int id = text(0).toInt();
// 	if(id%2==0)
// 		m_backBrush.setColor( Qt::cyan );
// 	else
// 		m_backBrush.setColor( Qt::gray );
// 
// 	for ( int i =1; i< columnCount() ;i++)
// 	{
// 		m_foreBrush.setColor( m_color );
// 		QTreeWidgetItem::setForeground( i, m_foreBrush );
// 		QTreeWidgetItem::setBackground( i, m_backBrush );
// 	}
}

List_view_color_item::~List_view_color_item()
{

}


// void QListViewColorItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align )
void List_view_color_item::paintCell ( int column )
{
// 	QBrush g( brush );

	  if( column > 0 )
// 		g.setColor( QColorGroup::Text, m_color );
		m_fore_brush.setColor( m_color );

// 	int id = text(0).toInt();
// 	if(id%2==0)
// 		m_backBrush.setColor( Qt::cyan );
// 	else
// 		m_backBrush.setColor( Qt::gray );
// 
// 	QTreeWidgetItem::setForeground( column, m_foreBrush );
// 	QTreeWidgetItem::setBackground( column, m_backBrush );


// 	int id = text(0).toInt();
// 	QColor doubleclr(238,242,245,76);
// 	if(id%2==0)
// 		QTreeWidgetItem::setBackgroundColor( column, doubleclr );
// 	else
// 		QTreeWidgetItem::setBackgroundColor( column, Qt::white );

	QTreeWidgetItem::setForeground( column, m_fore_brush );
}

bool List_view_color_item::operator <( const QTreeWidgetItem & other ) const
{
	if ( other.text(0).toInt() - text(0).toInt() > 0 )
	{
		return true ;
	}
	else 
		return false ;
}

int List_view_color_item::compare( QTreeWidgetItem * i, int col, bool ascending ) const
{	
	int ord_cmp = text(0).toInt() - i->text(0).toInt();
// 	int ord_cmp = data(0 ,Qt::DisplayRole ).toInt()  - i->data(0 ,Qt::DisplayRole ).toInt() ;
	
	if(col==0)
		return ord_cmp;
	else 
	{
		QString qsColName = treeWidget()->headerItem()->text(col);
		int id = m_cfg->get_colid_by_colname(qsColName);
		switch(id) 
		{
		case LIST_COL_FLOAT_VAL:
			{
				double rt = text(col).toDouble() - i->text(col).toDouble();
				if(rt == 0)
					return ord_cmp;
				else
					return rt>0?1:-1;
			}
			break;
		case LIST_COL_STATUS_VAL:
		case LIST_COL_SECU_LEV:
			{
				int rt = text(col).toInt() - i->text(col).toInt();
				if(rt==0)
					return ord_cmp;
				else
					return rt>0?1:-1;
			}
			break;
		case LIST_COL_SORT:
// 			{
// 				return data(0 ,Qt::DisplayRole ).toInt()  - i->data(0 ,Qt::DisplayRole ).toInt() ;
// 			}
// 			break;
		case LIST_COL_TYPE:
		case LIST_COL_TIME:
		case LIST_COL_DES:
		case LIST_COL_GROUP_NAME:
		case LIST_COL_MEMBER_NAME0:
		case LIST_COL_MEMBER_NAME1:
		case LIST_COL_MEMBER_NAME2:
		case LIST_COL_MEMBER_NAME3:
		case LIST_COL_GRAPH_NAME:
		case LIST_COL_TONE_INFO:
		default:
			{
// 				int rt = QTreeWidgetItem::compare(i, col, ascending);
				int rt = QString::compare(text(col) , i->text(col));
				if(rt == 0)
					return ord_cmp;
				else
					return rt;
			}
			break;
		}
	}
}