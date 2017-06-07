#ifndef SCRIPTSYNTAXHIGHLIGHTER_H
#define SCRIPTSYNTAXHIGHLIGHTER_H

#include <QtGlobal>
#include <QSyntaxHighlighter>
#include <QTextFormat>

class ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    ScriptSyntaxHighlighter(QTextDocument *document = 0);
    ~ScriptSyntaxHighlighter();
protected:
    void highlightBlock(const QString &text);
private:
    void highlightWord(int currentPos, const QString &buffer);
    enum ScriptFormats {
        ScriptTextFormat, ScriptNumberFormat,
		ScriptStringFormat, ScriptTypeFormat,
		ScriptKeywordFormat, ScriptPreprocessorFormat,
		ScriptLabelFormat, ScriptCommentFormat,
		NumScriptFormats
    };
    QTextCharFormat m_formats[NumScriptFormats];
private:
    Q_DISABLE_COPY(ScriptSyntaxHighlighter)

};

#endif
