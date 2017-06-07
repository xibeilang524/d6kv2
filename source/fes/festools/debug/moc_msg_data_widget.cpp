/****************************************************************************
** Meta object code from reading C++ file 'msg_data_widget.h'
**
** Created: Fri Apr 7 09:05:20 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../msg_data_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msg_data_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_msg_data_widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   16,   16,   16, 0x0a,
      67,   16,   16,   16, 0x0a,
      78,   16,   16,   16, 0x0a,
      92,   16,   16,   16, 0x0a,
     103,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     128,   16,   16,   16, 0x0a,
     146,   16,   16,   16, 0x0a,
     171,   16,   16,   16, 0x0a,
     196,  185,   16,   16, 0x0a,
     219,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_msg_data_widget[] = {
    "msg_data_widget\0\0signal_show_raw_table(bool)\0"
    "on_analysis_msg_cbk()\0on_pause()\0"
    "on_continue()\0on_clean()\0on_save()\0"
    "on_auto_save()\0on_clear_search()\0"
    "on_search_text_changed()\0on_set_time()\0"
    "channel_no\0on_channel_change(int)\0"
    "on_timer()\0"
};

void msg_data_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        msg_data_widget *_t = static_cast<msg_data_widget *>(_o);
        switch (_id) {
        case 0: _t->signal_show_raw_table((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_analysis_msg_cbk(); break;
        case 2: _t->on_pause(); break;
        case 3: _t->on_continue(); break;
        case 4: _t->on_clean(); break;
        case 5: _t->on_save(); break;
        case 6: _t->on_auto_save(); break;
        case 7: _t->on_clear_search(); break;
        case 8: _t->on_search_text_changed(); break;
        case 9: _t->on_set_time(); break;
        case 10: _t->on_channel_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_timer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData msg_data_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject msg_data_widget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_msg_data_widget,
      qt_meta_data_msg_data_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &msg_data_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *msg_data_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *msg_data_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_msg_data_widget))
        return static_cast<void*>(const_cast< msg_data_widget*>(this));
    return QFrame::qt_metacast(_clname);
}

int msg_data_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void msg_data_widget::signal_show_raw_table(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
