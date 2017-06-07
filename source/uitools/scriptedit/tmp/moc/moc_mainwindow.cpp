/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Apr 7 09:20:29 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      22,   11,   11,   11, 0x08,
      29,   11,   11,   11, 0x08,
      51,   11,   46,   11, 0x08,
      58,   11,   46,   11, 0x08,
      67,   11,   46,   11, 0x08,
      85,   77,   11,   11, 0x08,
     119,   77,   11,   11, 0x08,
     136,   11,   11,   11, 0x28,
     144,   11,   11,   11, 0x08,
     159,   11,   11,   11, 0x08,
     166,   11,   11,   11, 0x08,
     173,   11,   11,   11, 0x08,
     180,   11,   11,   11, 0x08,
     186,   11,   11,   11, 0x08,
     193,   11,   11,   11, 0x08,
     201,   11,   11,   11, 0x08,
     214,   11,   11,   11, 0x08,
     230,   11,   11,   11, 0x08,
     249,   11,   11,   11, 0x08,
     268,   11,   11,   11, 0x08,
     293,  285,   11,   11, 0x08,
     311,  285,   11,   11, 0x08,
     331,  285,   11,   11, 0x08,
     348,   11,   11,   11, 0x08,
     358,   11,   11,   11, 0x08,
     369,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newFile()\0open()\0"
    "openRecentFile()\0bool\0save()\0saveAs()\0"
    "saveAll()\0printer\0setPrintPreviewOptions(QPrinter*)\0"
    "print(QPrinter*)\0print()\0printPreview()\0"
    "exit()\0undo()\0redo()\0cut()\0copy()\0"
    "paste()\0insertFunc()\0insertSCDFunc()\0"
    "insertCustomFunc()\0deleteCustomFunc()\0"
    "editCustomFunc()\0checked\0viewToolBar(bool)\0"
    "viewStatusBar(bool)\0viewOutput(bool)\0"
    "compile()\0execProc()\0about()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->open(); break;
        case 2: _t->openRecentFile(); break;
        case 3: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->saveAll();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->setPrintPreviewOptions((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        case 7: _t->print((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        case 8: _t->print(); break;
        case 9: _t->printPreview(); break;
        case 10: _t->exit(); break;
        case 11: _t->undo(); break;
        case 12: _t->redo(); break;
        case 13: _t->cut(); break;
        case 14: _t->copy(); break;
        case 15: _t->paste(); break;
        case 16: _t->insertFunc(); break;
        case 17: _t->insertSCDFunc(); break;
        case 18: _t->insertCustomFunc(); break;
        case 19: _t->deleteCustomFunc(); break;
        case 20: _t->editCustomFunc(); break;
        case 21: _t->viewToolBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->viewStatusBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->viewOutput((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->compile(); break;
        case 25: _t->execProc(); break;
        case 26: _t->about(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
