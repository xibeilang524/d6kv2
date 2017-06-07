/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Apr 7 09:20:13 2017
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
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
      69,   11,   11,   11, 0x08,
      77,   11,   11,   11, 0x08,
      93,   11,   11,   11, 0x08,
     107,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     121,   11,   11,   11, 0x08,
     137,  129,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,
     162,   11,   11,   11, 0x08,
     169,  129,   11,   11, 0x08,
     185,  129,   11,   11, 0x08,
     200,  129,   11,   11, 0x08,
     216,   11,   11,   11, 0x08,
     229,   11,   11,   11, 0x08,
     240,   11,   11,   11, 0x08,
     254,  248,   11,   11, 0x08,
     270,   11,   11,   11, 0x08,
     296,  288,   11,   11, 0x08,
     333,  288,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0printer\0"
    "setPrintPreviewOptions(QPrinter*)\0"
    "printpreview()\0print()\0slotExportCsv()\0"
    "exportImage()\0exit()\0copy()\0paste()\0"
    "checked\0randomPaste(bool)\0undo()\0"
    "redo()\0curveview(bool)\0dataview(bool)\0"
    "showSDate(bool)\0execstatic()\0updatedb()\0"
    "about()\0index\0switchview(int)\0"
    "curveModelReset()\0selpara\0"
    "selectorWidgetChanged(selectorpara&)\0"
    "selectorChanged(selectorpara&)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->setPrintPreviewOptions((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        case 1: _t->printpreview(); break;
        case 2: _t->print(); break;
        case 3: _t->slotExportCsv(); break;
        case 4: _t->exportImage(); break;
        case 5: _t->exit(); break;
        case 6: _t->copy(); break;
        case 7: _t->paste(); break;
        case 8: _t->randomPaste((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->undo(); break;
        case 10: _t->redo(); break;
        case 11: _t->curveview((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->dataview((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->showSDate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->execstatic(); break;
        case 15: _t->updatedb(); break;
        case 16: _t->about(); break;
        case 17: _t->switchview((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->curveModelReset(); break;
        case 19: _t->selectorWidgetChanged((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 20: _t->selectorChanged((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
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
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
