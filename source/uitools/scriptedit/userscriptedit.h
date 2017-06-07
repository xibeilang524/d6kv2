#ifndef USERSCRIPTEDIT_H
#define USERSCRIPTEDIT_H

#include <QPlainTextEdit>
#include <QSet>

class ScriptEditExtraArea;

class UserScriptEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    UserScriptEdit(QWidget *parent = 0);
    ~UserScriptEdit();
	
    int baseLineNumber() const;
    void setBaseLineNumber(int base);
	
    int executionLineNumber() const;
    void setExecutionLineNumber(int lineNumber, bool error);
    void setExecutableLineNumbers(const QSet<int> &lineNumbers);
	
    int currentLineNumber() const;
    void gotoLine(int lineNumber);
	
    int extraAreaWidth() const;
	
signals:
	
protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
	
    void extraAreaPaintEvent(QPaintEvent *e);
    void extraAreaMouseEvent(QMouseEvent *e);
    bool extraAreaEvent(QEvent *e);
	
private slots:
    void updateExtraAreaWidth();
    void updateExtraArea(const QRect& rect, int dy);
    void highlightCurrentLine();
	
private:
    QTextEdit::ExtraSelection currentLineSelection() const;
    QTextEdit::ExtraSelection currentExecutionLineSelection() const;
    void updateExtraSelections();
	
private:
    ScriptEditExtraArea *m_extraArea;
    int m_baseLineNumber;
    int m_executionLineNumber;
    QSet<int> m_executableLineNumbers;
    bool m_executionLineNumberHasError;
    int m_extraAreaToggleBlockNumber;


	friend class ScriptEditExtraArea;
};

#endif