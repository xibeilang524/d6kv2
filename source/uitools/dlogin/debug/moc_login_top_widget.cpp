/****************************************************************************
** Meta object code from reading C++ file 'login_top_widget.h'
**
** Created: Fri Apr 7 09:20:08 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../login_top_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login_top_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Login_top_widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      37,   17,   17,   17, 0x08,
      48,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Login_top_widget[] = {
    "Login_top_widget\0\0slot_change_skin()\0"
    "slot_min()\0slot_close()\0"
};

void Login_top_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Login_top_widget *_t = static_cast<Login_top_widget *>(_o);
        switch (_id) {
        case 0: _t->slot_change_skin(); break;
        case 1: _t->slot_min(); break;
        case 2: _t->slot_close(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Login_top_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Login_top_widget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Login_top_widget,
      qt_meta_data_Login_top_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Login_top_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Login_top_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Login_top_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Login_top_widget))
        return static_cast<void*>(const_cast< Login_top_widget*>(this));
    return QFrame::qt_metacast(_clname);
}

int Login_top_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
