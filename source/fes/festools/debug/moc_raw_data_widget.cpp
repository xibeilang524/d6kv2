/****************************************************************************
** Meta object code from reading C++ file 'raw_data_widget.h'
**
** Created: Fri Apr 7 09:05:20 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../raw_data_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raw_data_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_raw_data_widget[] = {

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
      17,   16,   16,   16, 0x08,
      37,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_raw_data_widget[] = {
    "raw_data_widget\0\0on_tab_changed(int)\0"
    "on_timer()\0"
};

void raw_data_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        raw_data_widget *_t = static_cast<raw_data_widget *>(_o);
        switch (_id) {
        case 0: _t->on_tab_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_timer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData raw_data_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject raw_data_widget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_raw_data_widget,
      qt_meta_data_raw_data_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &raw_data_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *raw_data_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *raw_data_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_raw_data_widget))
        return static_cast<void*>(const_cast< raw_data_widget*>(this));
    return QFrame::qt_metacast(_clname);
}

int raw_data_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
