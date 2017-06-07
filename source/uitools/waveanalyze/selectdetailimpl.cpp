#include <QMessageBox>
#include <QKeyEvent>

// #include "constutils.h"
#include "globaldef.h"
#include "selectdetailimpl.h"

/* QDetails */

QDetails::QDetails()
{
// 	m_lstDetail = new  QList<QDetail>;
//	m_lstDetail->setAutoDelete(true);
}

QDetails::~QDetails()
{
	Clear();
// 	delete m_lstDetail;
}

int QDetails::Count()
{
	return m_lstDetail.count();
}

int QDetails::CheckCount()
{
	int sum = 0;
// 	 QListIterator<QDetail> it(*m_lstDetail);
// 	QDetail* pDetail;
// 	while ((pDetail = it.current()) != 0)
// 	{
// 		++it;
// 		if (pDetail->Check())
// 			sum++;
// 	}
	QList<QDetail*>::iterator it;
	for(it=m_lstDetail.begin();it!=m_lstDetail.end();++it)
	{
		if ((*it)->Check())
		{
			sum++;
		}
	}
	return sum;
}

void QDetails::Clear()
{
// 	 QListIterator<QDetail> it(*m_lstDetail);
//     QDetail* det;
//     while ((det = it.current()) != 0) 
// 	{
//         ++it;
// 		delete det;
// 		det = NULL;
//     }
// 	m_lstDetail->clear();

	QList<QDetail*>::iterator it;
	for(it=m_lstDetail.begin();it!=m_lstDetail.end();++it)
	{
		delete *it;
		*it = NULL;
	}
	m_lstDetail.clear();

}

void QDetails::MoveUp(int index)
{
	if ((uint)index >= m_lstDetail.count())
		return;

	QDetail* pDetail = m_lstDetail.takeAt(index);
	m_lstDetail.insert(index - 1, pDetail);
}

void QDetails::MoveDown(int index)
{
	if (index <= 0)
		return;

	QDetail* pDetail = m_lstDetail.takeAt(index);
	m_lstDetail.insert(index + 1, pDetail);
}

QDetail* QDetails::Add()
{
	QDetail* pDetail = new QDetail;
	m_lstDetail.append(pDetail);
	return pDetail;
}

void QDetails::Delete(int idx)
{
	if (idx < 0 || (uint)idx >= m_lstDetail.count())
		return;
	QDetail* det = m_lstDetail.at(idx);
	m_lstDetail.removeAt(idx);
	delete det;
}

void QDetails::CheckAll(bool b)
{
// 	 QListIterator<QDetail> it(*m_lstDetail);
// 	QDetail* pDetail;
// 	while ((pDetail = it.current()) != 0)
// 	{
// 		++it;
// 		pDetail->SetCheck(b);
// 	}

	QList<QDetail*>::iterator it;
	for (it=m_lstDetail.begin();it!=m_lstDetail.end();++it)
	{
		(*it)->SetCheck(b);
	}
}

QDetail* QDetails::Item(int index)
{
	if (index < 0 || (uint)index >= m_lstDetail.count())
		return NULL;
	return m_lstDetail.at(index);
}


/* QDlgDetailsImpl */

QDlgDetailsImpl::QDlgDetailsImpl(QDetails* dets, QWidget* parent, bool check)
	: QDialog(parent)
{
	setupUi(this);

	m_bEditCheck = check;
	m_pDetais = dets;
	m_bUpMove = false;
// 	m_lvDetail->addColumn("", 150);
// 	m_lvDetail->setColumnWidthMode(0, QListView::Maximum);
// 	m_lvDetail->header()->hide();
// 	m_lvDetail->setSortColumn(-1);
	UpdateView();
	m_edtAverage->installEventFilter(this);

	connect(m_btnOk, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_btnUp, SIGNAL(clicked()), SLOT(UpItem()));
	connect(m_btnDown, SIGNAL(clicked()), SLOT(DownItem()));
	connect(m_btnShow, SIGNAL(clicked()), SLOT(ShowItem()));
	connect(m_btnHide, SIGNAL(clicked()), SLOT(HideItem()));
	connect(m_lvDetail, SIGNAL(currentChanged(QListViewItem*)), SLOT(OnItem(QListViewItem*)));
	connect(m_lvDetail, SIGNAL(pressed(QListViewItem*)), SLOT(OnItem(QListViewItem*)));
	connect(m_lvDetail, SIGNAL(spacePressed(QListViewItem*)), SLOT(OnItem(QListViewItem*)));
}
    
QDlgDetailsImpl::~QDlgDetailsImpl()
{
}

void QDlgDetailsImpl::UpItem()
{
	QListWidgetItem* after = m_lvDetail->currentItem();
	int idx = ListItemIndex();
	if (idx < 2)
		return;

	m_pDetais->MoveUp(idx);
	//是否还需要移动？？？？？？？？？？
// 	QListWidgetItem* item = after->itemAbove();
// 	item->moveItem(after);
	UpdateButtons();
}

void QDlgDetailsImpl::DownItem()
{
	QListWidgetItem* item = m_lvDetail->currentItem();
	int idx = ListItemIndex();
	if (idx < 1 || idx > m_lvDetail->count() - 1)
		return;

	m_pDetais->MoveDown(idx);
	//是否还需要移动？？？？？？？？？？
// 	QListWidgetItem* after = item->itemBelow();
// 	item->moveItem(after);
	UpdateButtons();
}

void QDlgDetailsImpl::ShowItem()
{
	QListWidgetItem* item = (QListWidgetItem*)(m_lvDetail->currentItem());
	if (item == NULL)
		return;
	item->setCheckState(Qt::Checked);
	m_pDetais->Item(ListItemIndex())->SetCheck(true);
	UpdateButtons();
}

void QDlgDetailsImpl::HideItem()
{
	QListWidgetItem* item = (QListWidgetItem*)(m_lvDetail->currentItem());
	if (item == NULL)
		return;
	item->setCheckState(Qt::Checked);
	m_pDetais->Item(ListItemIndex())->SetCheck(false);
	UpdateButtons();
}

void QDlgDetailsImpl::OnItem(QListWidgetItem* item)
{
	if (item == NULL)
		return;
	m_pDetais->Item(ListItemIndex())->SetCheck(((QListWidgetItem*)item)->isSelected());
	UpdateButtons();
}

void QDlgDetailsImpl::UpdateView()
{
	m_lvDetail->clear();
	QDetail* pDetail;
	QListWidgetItem* item = NULL;
	for (int i = 0; i < m_pDetais->Count(); i++)
	{
		pDetail = m_pDetais->Item(i);
		item = new QListWidgetItem(pDetail->Caption(),m_lvDetail);
		if (pDetail->Check())
		{
			item->setCheckState(Qt::Checked);
		}
		else
		{
			item->setCheckState(Qt::Unchecked);
		}
	}
	item = (QListWidgetItem*)(m_lvDetail->item(0));
	item->setFlags(Qt::NoItemFlags);//setEnable(false)???????
}

void QDlgDetailsImpl::UpdateButtons()
{
	int idx = ListItemIndex();
	if (idx < 0)
		return;

	m_btnUp->setEnabled(idx > 0 && (! m_bUpMove));
	m_btnDown->setEnabled((idx < (m_pDetais->Count() - 1)) && 
						  (idx > 0) && 
						  (! m_bUpMove));
	m_btnShow->setEnabled(! m_pDetais->Item(idx)->Check());
	m_btnHide->setEnabled(! m_btnShow->isEnabled());
	QString str = m_lvDetail->currentItem()->text();
	QString p = ANAINFOSTR[3];
	m_edtAverage->setEnabled(str== p);
}

int QDlgDetailsImpl::ItemValue()
{
	return m_edtAverage->text().toInt();
}

void QDlgDetailsImpl::SetItemValue(int val)
{
	m_edtAverage->setText(QString::number(val));
}

void QDlgDetailsImpl::SetValueCaption(const QString& str)
{
	m_lblDetail->setText(str);
}

void QDlgDetailsImpl::UnEnableMoveButton()
{
	m_bUpMove = true;
	m_btnUp->setEnabled(false);
	m_btnDown->setEnabled(false);
}

int QDlgDetailsImpl::ListItemIndex()
{
	QListWidgetItem* item = m_lvDetail->currentItem();
	if (item == NULL)
		return -1;
	
	int index = 0;
	index = m_lvDetail->currentRow();
    return index >= m_lvDetail->count() ? -1 : index;
}

bool QDlgDetailsImpl::eventFilter(QObject* watched, QEvent* e)
{
	if (e->type() == QEvent::KeyPress)
	{
		if (watched == m_edtAverage)
		{
			bool ok(true);
			int key = ((QKeyEvent*)e)->key();
			if (key >= Qt::Key_0 && key <= Qt::Key_9 || key == Qt::Key_Backspace || key == Qt::Key_Delete)
				ok = false;
			return ok;
		}
	}

	return QDialog::eventFilter(watched, e);
}

void QDlgDetailsImpl::accept()
{		
	if (m_bEditCheck)
	{
		int n = m_edtAverage->text().toInt();
		if (n < 12 || n > 96)
		{
			QMessageBox::warning( NULL, QObject::tr("警告"),
				QObject::tr("平均值计算点数应该12~96之间!"), QObject::tr("确定"));
			return;
		}
	}
		
	QDialog::accept();
}
