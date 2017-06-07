
#include <QtGui>
#include <QtNetwork>

#include "ftpwindow.h"

Ftp_window::Ftp_window(QWidget *parent)
    : QDialog(parent), ftp(0), networkSession(0)
{
    ftpServerLabel = new QLabel(tr("Ftp 服务器:"));
    ftpServerLineEdit = new QLineEdit("172.27.27.229");
    ftpServerLabel->setBuddy(ftpServerLineEdit);

	ftpServerPortLabel = new QLabel(tr("端口:"));
	ftpServerPortLineEdit = new QLineEdit("21");
	ftpServerPortLabel->setBuddy(ftpServerLineEdit);

	userNameLabel = new QLabel(tr("用户名:"));
	ftpUserNameLineEdit = new QLineEdit(this);
	userNameLabel->setBuddy(ftpUserNameLineEdit);

	passwordLabel = new QLabel(tr("密码:"));
	ftpPasswordLineEdit = new QLineEdit(this);
	ftpPasswordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLabel->setBuddy(ftpPasswordLineEdit);

    statusLabel = new QLabel(tr("Please enter the name of an FTP server."));
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
    // Use word wrapping to fit the text on screen
    statusLabel->setWordWrap( true );
#endif

    fileList = new QTreeWidget;
    fileList->setEnabled(false);
	fileList->setSortingEnabled(true);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Time") << tr("Owner") << tr("Group"));
    fileList->header()->setStretchLastSection(false);
	fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	fileList->setColumnWidth(0,200);
	fileList->setColumnWidth(2,150);
	fileList->setSortingEnabled(true);

    connectButton = new QPushButton(tr("连接"));
    connectButton->setDefault(true);

    cdToParentButton = new QPushButton;
    cdToParentButton->setIcon(QPixmap(":/res/images/cdtoparent.png"));
    cdToParentButton->setEnabled(false);

    downloadButton = new QPushButton(tr("下载"));
    downloadButton->setEnabled(false);

    quitButton = new QPushButton(tr("退出"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    progressDialog = new QProgressDialog(this);

    connect(fileList, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(process_item(QTreeWidgetItem*,int)));
    connect(fileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(enable_download_button()));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancel_download()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connect_or_disconnect()));
    connect(cdToParentButton, SIGNAL(clicked()), this, SLOT(cd_to_parent()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(download_file()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	QGridLayout *topLayout = new QGridLayout;
	topLayout->setContentsMargins(0, 0, 0, 0);
	topLayout->addWidget(ftpServerLabel,0,0,1,1);
	topLayout->addWidget(ftpServerLineEdit,0,1,1,1);
	topLayout->addWidget(ftpServerPortLabel,0,2,1,1);
	topLayout->addWidget(ftpServerPortLineEdit,0,3,1,1);
	topLayout->addWidget(connectButton,0,4,1,1);
	topLayout->addWidget(userNameLabel,1,0,1,1);
	topLayout->addWidget(ftpUserNameLineEdit,1,1,1,1);
	topLayout->addWidget(passwordLabel,1,2,1,1);
	topLayout->addWidget(ftpPasswordLineEdit,1,3,1,1);
	topLayout->addWidget(cdToParentButton,1,4,1,1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
#ifdef Q_OS_SYMBIAN
    // Make app better lookin on small screen
    mainLayout->addLayout(topLayout2);
#endif
    mainLayout->addWidget(fileList);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("FTP"));

	this->setStyleSheet(tr("QDialog{background:white;}"	
		"QLabel{font-family: Microsoft YaHei;font-size: 12px;color: #0F1923;}"
		"QLineEdit {font-family:Microsoft YaHei;font-size: 12px;color: #4A4A4A;background: #FFFFFF;\
		border: 1px solid #C6CFD9;border-radius: 2px;min-height:22px;}"		
		"QGroupBox{font-family: Microsoft YaHei;font-size: 13px;color: #0F1923;}"
		"QComboBox {font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;\
		min-height:23px;border: 1px solid #C6CFD9;border-radius: 2px;}"
		"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 28px;\
		border-top-right-radius: 2px;border-bottom-right-radius: 2px;}"
		"QComboBox::down-arrow{image: url(:/res/dropdown.png);}"
		"QPushButton{font-family: Microsoft YaHei;font-size: 12px;color: white;\
		background:#4A90E2;border-radius: 2px;min-width:60px;min-height:23px;}"
		"QPushButton:hover{background:#0C2543;}"
		"QPushButton:disabled{background:#D5D5D5;}"
		"QToolBar{spacing:10px;min-height:35px;background:#FAFAFA;	}"));


	QString style_sheet = tr("QTreeWidget{font-family: Microsoft YaHei;font-size: 12px;color:#4A4A4A;\
					 background: #FAFAFA;border: none;}"
					 "QTreeWidget::branch:closed:has-children{ image: url(:/res/drop_down_nomal.png);}\
					 QTreeWidget::branch:open:has-children{ image: url(:/res/drop_down_click.png); }\
					 QTreeWidget::branch{image:none;}\
					 QTreeWidget::item{height:22px;}\
					 QTreeWidget::item:hover{background-color:#7BBDEF}\
					 QTreeWidget::item:selected{background-color:#4A90E2}"
					 "QScrollBar{background:white;height:8px;}"
					 "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}"
					 "QScrollBar::handle:hover{background:gray;}"
					 "QScrollBar::sub-line{background:transparent;}"
					 "QScrollBar::add-line{background:transparent;}"
					 "QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
					 "QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
					 "QScrollBar::handle:vertical:hover{background:gray;}"
					 "QScrollBar::add-line:vertical{{background:transparent;}"
					 "QScrollBar::sub-line:vertical{background:transparent;}"	);
	fileList->setStyleSheet(style_sheet);
	style_sheet = tr("QPushButton{font-family: Microsoft YaHei;font-size: 13px;color: white;\
							 background:#4A90E2;border-radius: 2px;min-width:75px;min-height:25px;}"
							 "QPushButton:hover{background:#0C2543;}"
							 "QPushButton:disabled{background:#D5D5D5;}");
	downloadButton->setStyleSheet(style_sheet);

	style_sheet = tr("QPushButton{ font-family: Microsoft YaHei;font-size: 13px;color: #4A90E2;background:white;\
					 border-radius: 2px;border: 1px solid #4A90E2;min-width:74px;min-height:24px;}"
					 "QPushButton:hover{background:#0C2543;border: 0px;color:white;}"); 
	quitButton->setStyleSheet(style_sheet);

}

QSize Ftp_window::sizeHint() const
{
    return QSize(500, 300);
}

//![0]
void Ftp_window::connect_or_disconnect()
{

    if (ftp) {
		QFtp::State  state = ftp->state();
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
//![0]
        fileList->setEnabled(false);
        cdToParentButton->setEnabled(false);
        downloadButton->setEnabled(false);
        connectButton->setEnabled(true);
        connectButton->setText(tr("连接"));
#ifndef QT_NO_CURSOR
        setCursor(Qt::ArrowCursor);
#endif
        statusLabel->setText(tr("Please enter the name of an FTP server."));
        return;
    }

#ifndef QT_NO_CURSOR
    setCursor(Qt::WaitCursor);
#endif

    if (!networkSession || !networkSession->isOpen()) {
        if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
            if (!networkSession) {
                // Get saved network configuration
                QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
                settings.beginGroup(QLatin1String("QtNetwork"));
                const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
                settings.endGroup();

                // If the saved network configuration is not currently discovered use the system default
                QNetworkConfiguration config = manager.configurationFromIdentifier(id);
                if ((config.state() & QNetworkConfiguration::Discovered) !=
                    QNetworkConfiguration::Discovered) {
                    config = manager.defaultConfiguration();
                }

                networkSession = new QNetworkSession(config, this);
                connect(networkSession, SIGNAL(opened()), this, SLOT(connect_to_ftp()));
                connect(networkSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(enable_connect_button()));
            }
            connectButton->setEnabled(false);
            statusLabel->setText(tr("Opening network session."));
            networkSession->open();
            return;
        }
    }
    connect_to_ftp();
}

void Ftp_window::connect_to_ftp()
{
//![1]
    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftp_command_finished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
            this, SLOT(add_to_list(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(update_data_transfer_progress(qint64,qint64)));

    fileList->clear();
    currentPath.clear();
    isDirectory.clear();
//![1]

//![2]
	QUrl url(ftpServerLineEdit->text());
	url.setUserName(ftpUserNameLineEdit->text());
	url.setPassword(ftpPasswordLineEdit->text());
	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
		ftp->connectToHost(ftpServerLineEdit->text(), 21);
// 		ftp->login("d6000","d6000");
		ftp->login(url.userName(),url.password());
    } else {
        ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
            ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        else
            ftp->login();
        if (!url.path().isEmpty())
            ftp->cd(url.path());
    }
//![2]

    fileList->setEnabled(true);
    connectButton->setEnabled(false);
    connectButton->setText(tr("断开"));
    statusLabel->setText(tr("Connecting to FTP server %1...")
                         .arg(ftpServerLineEdit->text()));
}

//![3]
void Ftp_window::download_file()
{
	QList<QTreeWidgetItem*> selectList = fileList->selectedItems();
	QTreeWidgetItem* item;
// 	for (int i=0;i<selectList.size();i++)
	while (selectList.size()>0)
	{
		item = selectList.takeAt(0);
		item->setSelected(false);
		QString currentpath = QDir::currentPath();
		QString appPath = QCoreApplication::applicationFilePath() ;
		appPath = QCoreApplication::applicationDirPath() ;
// 		QString fileName = fileList->currentItem()->text(0);		//转换成正常的中文文件名
		QString fileName = appPath+"/wave/"+ item->text(0);		//转换成正常的中文文件名
		QString tmpfileName = _ToSpecialEncoding(item->text(0));	//原始乱码的文件名
		//![3]
		//
		currentpath = QDir::currentPath();
		if (QFile::exists(fileName)) {
			QMessageBox::information(this, tr("FTP"),
				tr("There already exists a file called %1 in "
				"the current directory.")
				.arg(fileName));
			continue;
		}

		//![4]
		file = new QFile(fileName);
		if (!file->open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("FTP"),
				tr("Unable to save the file %1: %2.")
				.arg(fileName).arg(file->errorString()));
			delete file;
			continue;
		}

		ftp->get(tmpfileName, file);
// 		isDownload = true;

		progressDialog->setLabelText(tr("Downloading %1...").arg(fileName));
		downloadButton->setEnabled(false);
		progressDialog->show();
		break;		//可以正常下载就退出
	}

}
//![4]

//![5]
void Ftp_window::cancel_download()
{
    ftp->abort();

    if (file->exists()) {
        file->close();
        file->remove();
    }
    delete file;
}
//![5]

//![6]
void Ftp_window::ftp_command_finished(int, bool error)
{
#ifndef QT_NO_CURSOR
    setCursor(Qt::ArrowCursor);
#endif

    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.")
                                     .arg(ftpServerLineEdit->text()));
            connect_or_disconnect();
            return;
        }
        statusLabel->setText(tr("Logged onto %1.")
                             .arg(ftpServerLineEdit->text()));
        fileList->setFocus();
        downloadButton->setDefault(true);
        connectButton->setEnabled(true);
        return;
    }
//![6]

//![7]
    if (ftp->currentCommand() == QFtp::Login)
        ftp->list();
//![7]

//![8]
    if (ftp->currentCommand() == QFtp::Get) {
        if (error) {
            statusLabel->setText(tr("Canceled download of %1.")
                                 .arg(file->fileName()));
            file->close();
            file->remove();
        } else {
            statusLabel->setText(tr("Downloaded %1 to current directory.")
                                 .arg(file->fileName()));
            file->close();
        }
        delete file;
// 		isDownload = false;
		progressDialog->hide();
		download_file();
// 		downloadButton->click();
        enable_download_button();
//![8]
//![9]
    } else if (ftp->currentCommand() == QFtp::List) {
        if (isDirectory.isEmpty()) {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
            fileList->setEnabled(false);
        }
    }
//![9]
}

//![10]
void Ftp_window::add_to_list(const QUrlInfo &urlInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
	QString itemName=_FromSpecialEncoding(urlInfo.name());
	item->setText(0, itemName);
    item->setText(1, QString::number(urlInfo.size()));
	item->setText(2, urlInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    item->setText(3, urlInfo.owner());
    item->setText(4, urlInfo.group());

    QPixmap pixmap(urlInfo.isDir() ? ":/res/images/dir.png" : ":/res/images/file.png");
    item->setIcon(0, pixmap);

    isDirectory[urlInfo.name()] = urlInfo.isDir();
// 	isDirectory[itemName] = urlInfo.isDir();
   fileList->addTopLevelItem(item);
    if (!fileList->currentItem()) {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }
}
//![10]

//![11]
void Ftp_window::process_item(QTreeWidgetItem *item, int /*column*/)
{
    QString name = _ToSpecialEncoding(item->text(0));
// 	QString name =item->text(0);
    if (isDirectory.value(name)) {
        fileList->clear();
        isDirectory.clear();
        currentPath += '/';
        currentPath += name;
        ftp->cd(name);
        ftp->list();
        cdToParentButton->setEnabled(true);
#ifndef QT_NO_CURSOR
        setCursor(Qt::WaitCursor);
#endif
        return;
    }
}
//![11]

//![12]
void Ftp_window::cd_to_parent()
{
#ifndef QT_NO_CURSOR
    setCursor(Qt::WaitCursor);
#endif
    fileList->clear();
    isDirectory.clear();
    currentPath = currentPath.left(currentPath.lastIndexOf('/'));
    if (currentPath.isEmpty()) {
        cdToParentButton->setEnabled(false);
        ftp->cd("/");
    } else {
        ftp->cd(currentPath);
    }
    ftp->list();
}
//![12]

//![13]
void Ftp_window::update_data_transfer_progress(qint64 readBytes,
                                           qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
}
//![13]

//![14]
void Ftp_window::enable_download_button()
{
    QTreeWidgetItem *current = fileList->currentItem();
    if (current) {
        QString currentFile = _ToSpecialEncoding(current->text(0));
        downloadButton->setEnabled(!isDirectory.value(currentFile));
    } else {
        downloadButton->setEnabled(false);
    }
}
//![14]

void Ftp_window::enable_connect_button()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    connectButton->setEnabled(true);
    statusLabel->setText(tr("Please enter the name of an FTP server."));
}

QString Ftp_window::_FromSpecialEncoding(const QString &InputStr)  
{  
#ifdef Q_OS_WIN  
	return  QString::fromLocal8Bit(InputStr.toLatin1());  
#else  
	QTextCodec *codec = QTextCodec::codecForName("gbk");  
	if (codec)  
	{  
		return codec->toUnicode(InputStr.toLatin1());  
	}  
	else  
	{  
		return QString("");  
	}  
#endif  
} 

QString Ftp_window::_ToSpecialEncoding(const QString &InputStr)  
{  
#ifdef Q_OS_WIN  
	return QString::fromLatin1(InputStr.toLocal8Bit());  
#else  
	QTextCodec *codec= QTextCodec::codecForName("gbk");  
	if (codec)  
	{  
		return QString::fromLatin1(codec->fromUnicode(InputStr));  
	}  
	else  
	{  
		return QString("");  
	}  
#endif  
}  

void Ftp_window::set_server_name( QString servername )
{
	m_server_name = servername;
	ftpServerLineEdit->setText(m_server_name);
}

void Ftp_window::set_server_port( QString port )
{
	m_server_port = port;
	ftpServerPortLineEdit->setText(m_server_port);
}

void Ftp_window::set_user_name( QString user )
{
	m_user_name = user;
	ftpUserNameLineEdit->setText(m_user_name);
}

void Ftp_window::set_password( QString password )
{
	m_password = password;
	ftpPasswordLineEdit->setText(m_password);
}	
