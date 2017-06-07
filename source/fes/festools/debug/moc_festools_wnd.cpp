/****************************************************************************
** Meta object code from reading C++ file 'festools_wnd.h'
**
** Created: Fri Apr 7 09:05:21 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../festools_wnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'festools_wnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_festools_wnd[] = {

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
      16,   14,   13,   13, 0x08,
      65,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_festools_wnd[] = {
    "festools_wnd\0\0,\0"
    "on_current_channel_changed(QTreeWidgetItem*,int)\0"
    "on_show_raw_table(bool)\0"
};

void festools_wnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        festools_wnd *_t = static_cast<festools_wnd *>(_o);
        switch (_id) {
        case 0: _t->on_current_channel_changed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->on_show_raw_table((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData festools_wnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject festools_wnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_festools_wnd,
      qt_meta_data_festools_wnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &festools_wnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *festools_wnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *festools_wnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_festools_wnd))
        return static_cast<void*>(const_cast< festools_wnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int festools_wnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
