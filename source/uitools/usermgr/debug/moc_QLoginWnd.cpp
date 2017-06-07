/****************************************************************************
** Meta object code from reading C++ file 'QLoginWnd.h'
**
** Created: Fri Apr 7 09:20:11 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QLoginWnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QLoginWnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QLoginWnd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      26,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QLoginWnd[] = {
    "QLoginWnd\0\0onCbxType(int)\0onBtnOK()\0"
};

void QLoginWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QLoginWnd *_t = static_cast<QLoginWnd *>(_o);
        switch (_id) {
        case 0: _t->onCbxType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onBtnOK(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QLoginWnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QLoginWnd::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QLoginWnd,
      qt_meta_data_QLoginWnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QLoginWnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QLoginWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QLoginWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QLoginWnd))
        return static_cast<void*>(const_cast< QLoginWnd*>(this));
    return QDialog::qt_metacast(_clname);
}

int QLoginWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
