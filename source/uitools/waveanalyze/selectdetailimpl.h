#ifndef SELECTDETAILIMPL_H
#define SELECTDETAILIMPL_H

#include <QList> 
#include <QListWidgetItem>

#include "ui_selectdetail.h"


class QDetail
{
public:
	QDetail()
	{
		m_strCaption = "";
		m_bCheck = false;
		m_nData = 0;
		m_pDataPointer = NULL;
	}
	QDetail(const QDetail& src)
	{
		m_strCaption = src.m_strCaption;
		m_bCheck = src.m_bCheck;
		m_nData = src.m_nData;
	}

public:
	QString Caption()					{ return m_strCaption; }
	void SetCaption(const QString& str) { m_strCaption = str; }
	int Data()							{ return m_nData; }
	void SetData(int data)				{ m_nData = data; }
	bool Check()						{ return m_bCheck; }
	void SetCheck(bool b)				{ m_bCheck = b; }
	void* DataPointer()					{ return m_pDataPointer; }
	void SetDataPointer(void* p)		{ m_pDataPointer = p; }

private:
	QString m_strCaption;		// 名称
	int		m_nData;			// 关联数据
	bool	m_bCheck;			// 显示
	void*	m_pDataPointer;		// 关联指针

};

class QDetails
{
public:
	QDetails();
	~QDetails();

	int Count();
	int CheckCount();
	void Clear();
	void MoveUp(int index);
	void MoveDown(int index);
	QDetail* Add();
	void Delete(int idx);
	void CheckAll(bool b = true);
	QDetail* Item(int index);

private:
	 QList<QDetail*> m_lstDetail;

};

class QDlgDetailsImpl : public QDialog,public Ui::QDlgDetails
{
    Q_OBJECT

public:
    QDlgDetailsImpl(QDetails* dets, QWidget* parent = 0, bool check = false);
    ~QDlgDetailsImpl();

	void UnEnableMoveButton();
	int ItemValue();
	void SetItemValue(int val);
	void SetValueCaption(const QString& str);

private:
	QDetails* m_pDetais;
	bool	m_bUpMove;
	bool	m_bEditCheck;

protected:
	void UpdateListItem(int index);
	void UpdateView();
	void UpdateButtons();
	int ListItemIndex();
	
	bool eventFilter(QObject* watched, QEvent* e);
	void accept();

protected slots:
	void UpItem();
	void DownItem();
	void ShowItem();
	void HideItem();
// 	void OnItem(QListViewItem* item);
	void OnItem(QListWidgetItem* item);

};

#endif