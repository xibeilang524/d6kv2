/****************************************************************************
** Meta object code from reading C++ file 'fesoper.h'
**
** Created: Fri Apr 7 09:05:16 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/fesoper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fesoper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QFesOper[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      26,    9,    9,    9, 0x08,
      39,    9,    9,    9, 0x08,
      51,    9,    9,    9, 0x08,
      64,    9,    9,    9, 0x08,
      80,   78,    9,    9, 0x08,
     114,    9,    9,    9, 0x08,
     124,    9,    9,    9, 0x08,
     144,  133,    9,    9, 0x08,
     179,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QFesOper[] = {
    "QFesOper\0\0onCallAllData()\0onSendTime()\0"
    "onCallKwh()\0onResetRtu()\0onResetChan()\0"
    ",\0onRTUSelect(QTreeWidgetItem*,int)\0"
    "onTimer()\0onAuto()\0item,order\0"
    "onChanSelect(QTreeWidgetItem*,int)\0"
    "onMannal_Load_Para()\0"
};

void QFesOper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QFesOper *_t = static_cast<QFesOper *>(_o);
        switch (_id) {
        case 0: _t->onCallAllData(); break;
        case 1: _t->onSendTime(); break;
        case 2: _t->onCallKwh(); break;
        case 3: _t->onResetRtu(); break;
        case 4: _t->onResetChan(); break;
        case 5: _t->onRTUSelect((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->onTimer(); break;
        case 7: _t->onAuto(); break;
        case 8: _t->onChanSelect((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->onMannal_Load_Para(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QFesOper::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QFesOper::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QFesOper,
      qt_meta_data_QFesOper, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QFesOper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QFesOper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QFesOper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QFesOper))
        return static_cast<void*>(const_cast< QFesOper*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QFesOper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
