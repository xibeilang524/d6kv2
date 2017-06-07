/****************************************************************************
** Meta object code from reading C++ file 'dqinfo_mainwindow.h'
**
** Created: Fri Apr 7 09:20:32 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dqinfo_mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dqinfo_mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      48,   12,   11,   11, 0x08,
      76,   11,   11,   11, 0x08,
      91,   11,   11,   11, 0x08,
     104,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     130,   11,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,
     165,   11,   11,   11, 0x08,
     180,   11,   11,   11, 0x08,
     192,   11,   11,   11, 0x08,
     204,   11,   11,   11, 0x08,
     214,   11,   11,   11, 0x08,
     231,  225,   11,   11, 0x08,
     255,   11,   11,   11, 0x08,
     265,   11,   11,   11, 0x08,
     298,  281,   11,   11, 0x08,
     341,  331,   11,   11, 0x08,
     372,   12,   11,   11, 0x08,
     399,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mainWindow[] = {
    "mainWindow\0\0index\0selectTaskChange(QModelIndex)\0"
    "taskRightClick(QModelIndex)\0showTaskMenu()\0"
    "logOutTask()\0killTask()\0setWindowTop()\0"
    "setPause()\0setContinue()\0setSave()\0"
    "clearDbgInfo()\0checkTime()\0setFilter()\0"
    "exitApp()\0onRemote()\0state\0"
    "onRemoteModeChange(int)\0onTimer()\0"
    "clearTaskList()\0typeNo,type,desc\0"
    "recvDbgInfo(int,QString,QString)\0"
    "type,desc\0saveDbgToFile(QString,QString)\0"
    "selectDbgMsgs(QModelIndex)\0copyInfo()\0"
};

void mainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        mainWindow *_t = static_cast<mainWindow *>(_o);
        switch (_id) {
        case 0: _t->selectTaskChange((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->taskRightClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->showTaskMenu(); break;
        case 3: _t->logOutTask(); break;
        case 4: _t->killTask(); break;
        case 5: _t->setWindowTop(); break;
        case 6: _t->setPause(); break;
        case 7: _t->setContinue(); break;
        case 8: _t->setSave(); break;
        case 9: _t->clearDbgInfo(); break;
        case 10: _t->checkTime(); break;
        case 11: _t->setFilter(); break;
        case 12: _t->exitApp(); break;
        case 13: _t->onRemote(); break;
        case 14: _t->onRemoteModeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->onTimer(); break;
        case 16: _t->clearTaskList(); break;
        case 17: _t->recvDbgInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 18: _t->saveDbgToFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 19: _t->selectDbgMsgs((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 20: _t->copyInfo(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData mainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_mainWindow,
      qt_meta_data_mainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainWindow))
        return static_cast<void*>(const_cast< mainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int mainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
