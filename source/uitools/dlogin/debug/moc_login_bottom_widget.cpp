/****************************************************************************
** Meta object code from reading C++ file 'login_bottom_widget.h'
**
** Created: Fri Apr 7 09:20:08 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../login_bottom_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login_bottom_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Login_bottom_widget[] = {

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
      27,   21,   20,   20, 0x0a,
      55,   20,   20,   20, 0x0a,
      69,   20,   20,   20, 0x0a,
      89,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Login_bottom_widget[] = {
    "Login_bottom_widget\0\0index\0"
    "slot_user_type_changed(int)\0slot_ok_btn()\0"
    "slot_register_btn()\0slot_find_password_btn()\0"
};

void Login_bottom_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Login_bottom_widget *_t = static_cast<Login_bottom_widget *>(_o);
        switch (_id) {
        case 0: _t->slot_user_type_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slot_ok_btn(); break;
        case 2: _t->slot_register_btn(); break;
        case 3: _t->slot_find_password_btn(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Login_bottom_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Login_bottom_widget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Login_bottom_widget,
      qt_meta_data_Login_bottom_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Login_bottom_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Login_bottom_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Login_bottom_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Login_bottom_widget))
        return static_cast<void*>(const_cast< Login_bottom_widget*>(this));
    return QFrame::qt_metacast(_clname);
}

int Login_bottom_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
