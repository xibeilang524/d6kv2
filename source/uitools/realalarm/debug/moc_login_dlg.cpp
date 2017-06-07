/****************************************************************************
** Meta object code from reading C++ file 'login_dlg.h'
**
** Created: Fri Apr 7 09:21:02 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../login_dlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Login_dlg[] = {

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
      16,   11,   10,   10, 0x0a,
      39,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Login_dlg[] = {
    "Login_dlg\0\0item\0onUserTypeChanged(int)\0"
    "onOk()\0"
};

void Login_dlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Login_dlg *_t = static_cast<Login_dlg *>(_o);
        switch (_id) {
        case 0: _t->onUserTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onOk(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Login_dlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Login_dlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Login_dlg,
      qt_meta_data_Login_dlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Login_dlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Login_dlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Login_dlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Login_dlg))
        return static_cast<void*>(const_cast< Login_dlg*>(this));
    if (!strcmp(_clname, "Ui::QLoginBase"))
        return static_cast< Ui::QLoginBase*>(const_cast< Login_dlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int Login_dlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
