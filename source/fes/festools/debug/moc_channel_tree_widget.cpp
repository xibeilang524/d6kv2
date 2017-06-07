/****************************************************************************
** Meta object code from reading C++ file 'channel_tree_widget.h'
**
** Created: Fri Apr 7 09:05:21 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../channel_tree_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'channel_tree_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_channel_tree_widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,   20,   20,   20, 0x08,
      87,   20,   20,   20, 0x08,
     105,   20,   20,   20, 0x08,
     119,   20,   20,   20, 0x08,
     135,   20,   20,   20, 0x08,
     154,   21,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_channel_tree_widget[] = {
    "channel_tree_widget\0\0,\0"
    "on_channel_select_changed(QTreeWidgetItem*,int)\0"
    "on_search_btn()\0on_call_alldata()\0"
    "on_call_kwh()\0on_check_time()\0"
    "on_init_terminal()\0"
    "on_channel_changed(QTreeWidgetItem*,int)\0"
};

void channel_tree_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        channel_tree_widget *_t = static_cast<channel_tree_widget *>(_o);
        switch (_id) {
        case 0: _t->on_channel_select_changed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->on_search_btn(); break;
        case 2: _t->on_call_alldata(); break;
        case 3: _t->on_call_kwh(); break;
        case 4: _t->on_check_time(); break;
        case 5: _t->on_init_terminal(); break;
        case 6: _t->on_channel_changed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData channel_tree_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject channel_tree_widget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_channel_tree_widget,
      qt_meta_data_channel_tree_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &channel_tree_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *channel_tree_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *channel_tree_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_channel_tree_widget))
        return static_cast<void*>(const_cast< channel_tree_widget*>(this));
    return QFrame::qt_metacast(_clname);
}

int channel_tree_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void channel_tree_widget::on_channel_select_changed(QTreeWidgetItem * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
