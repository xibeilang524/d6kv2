/****************************************************************************
** Meta object code from reading C++ file 'fesstat.h'
**
** Created: Fri Apr 7 09:05:16 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fesstat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fesstat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QFesStat[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      20,    9,    9,    9, 0x08,
      34,    9,    9,    9, 0x08,
      47,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QFesStat[] = {
    "QFesStat\0\0onTimer()\0onTableMode()\0"
    "onListMode()\0onExit()\0"
};

void QFesStat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QFesStat *_t = static_cast<QFesStat *>(_o);
        switch (_id) {
        case 0: _t->onTimer(); break;
        case 1: _t->onTableMode(); break;
        case 2: _t->onListMode(); break;
        case 3: _t->onExit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QFesStat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QFesStat::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QFesStat,
      qt_meta_data_QFesStat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QFesStat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QFesStat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QFesStat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QFesStat))
        return static_cast<void*>(const_cast< QFesStat*>(this));
    return QDialog::qt_metacast(_clname);
}

int QFesStat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
