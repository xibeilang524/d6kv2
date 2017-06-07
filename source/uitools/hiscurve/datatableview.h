#ifndef DATATABLE_VIEW
#define DATATABLE_VIEW

#include <QTableView>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QDockWidget;
class QVBoxLayout;
class QDateEdit;
class QTimeEdit;
class QComboBox;
class QLabel;
class QWidgetAction;
class QPrinter;
QT_END_NAMESPACE
class selectorpara;

class DataTableView : public QTableView  
{
	Q_OBJECT
public slots:
	virtual void reset ();
	
public:
	DataTableView(QWidget * parent = 0);
	~DataTableView();

	void print(QPrinter *printer);
private:
	selectorpara             &currentSelector;
};



#endif 
