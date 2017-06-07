/**
@file ftpwindow.h
@brief ftp对话框头文件
可以连接远程ftp，下载装置上comtrade录波文件
@author lujiashun 
@version 1.0.0
@date 2016-11-24
*/
#ifndef FTPWINDOW_H
#define FTPWINDOW_H

#include <QDialog>
#include <QHash>
#include <QNetworkConfigurationManager>

// QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QNetworkSession;
// QT_END_NAMESPACE

class Ftp_window : public QDialog
{
    Q_OBJECT

public:
    Ftp_window(QWidget *parent = 0);
    QSize sizeHint() const;

public:
	void set_server_name(QString servername);
	void set_server_port(QString port);
	void set_user_name(QString user);
	void set_password(QString password);
//![0]
private slots:
    void connect_or_disconnect();
    void download_file();
    void cancel_download();
    void connect_to_ftp();

    void ftp_command_finished(int commandId, bool error);
    void add_to_list(const QUrlInfo &urlInfo);
    void process_item(QTreeWidgetItem *item, int column);
    void cd_to_parent();
    void update_data_transfer_progress(qint64 readBytes,
                                    qint64 totalBytes);
    void enable_download_button();
    void enable_connect_button();
	QString _FromSpecialEncoding(const QString &InputStr);
	QString _ToSpecialEncoding(const QString &InputStr);

//![0]

private:
    QLabel *ftpServerLabel;	
    QLineEdit *ftpServerLineEdit;
	QLabel *ftpServerPortLabel;
	QLineEdit *ftpServerPortLineEdit;
	QLabel *userNameLabel;
	QLineEdit *ftpUserNameLineEdit;
	QLabel *passwordLabel;
	QLineEdit *ftpPasswordLineEdit;

    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *cdToParentButton;
    QPushButton *connectButton;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;

//![1]
    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;

    QNetworkSession *networkSession;
    QNetworkConfigurationManager manager;

	QString m_server_name;
	QString m_server_port;
	QString m_user_name;
	QString m_password;

//![1]
};

#endif
