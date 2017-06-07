/****************************************************************************
** Meta object code from reading C++ file 'event_real_wnd.h'
**
** Created: Fri Apr 7 09:21:03 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../event_real_wnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_real_wnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Event_real_wnd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      28,   15,   15,   15, 0x0a,
      43,   15,   15,   15, 0x0a,
      57,   15,   15,   15, 0x0a,
      73,   15,   15,   15, 0x0a,
      94,   15,   15,   15, 0x0a,
     119,   15,   15,   15, 0x0a,
     138,   15,   15,   15, 0x0a,
     158,   15,   15,   15, 0x0a,
     175,   15,   15,   15, 0x0a,
     195,   15,   15,   15, 0x0a,
     215,   15,   15,   15, 0x0a,
     234,   15,   15,   15, 0x0a,
     253,  251,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Event_real_wnd[] = {
    "Event_real_wnd\0\0slot_exit()\0slot_sys_set()\0"
    "slot_ui_set()\0slot_top_most()\0"
    "slot_confirm_event()\0slot_confirm_all_event()\0"
    "slot_confirm_snd()\0slot_delete_event()\0"
    "slot_pause_btn()\0slot_show_page(int)\0"
    "slot_test_wav_btn()\0slot_stop_thread()\0"
    "slot_show_help()\0,\0"
    "slot_custom_event(Rsys_event*,int)\0"
};

void Event_real_wnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Event_real_wnd *_t = static_cast<Event_real_wnd *>(_o);
        switch (_id) {
        case 0: _t->slot_exit(); break;
        case 1: _t->slot_sys_set(); break;
        case 2: _t->slot_ui_set(); break;
        case 3: _t->slot_top_most(); break;
        case 4: _t->slot_confirm_event(); break;
        case 5: _t->slot_confirm_all_event(); break;
        case 6: _t->slot_confirm_snd(); break;
        case 7: _t->slot_delete_event(); break;
        case 8: _t->slot_pause_btn(); break;
        case 9: _t->slot_show_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slot_test_wav_btn(); break;
        case 11: _t->slot_stop_thread(); break;
        case 12: _t->slot_show_help(); break;
        case 13: _t->slot_custom_event((*reinterpret_cast< Rsys_event*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Event_real_wnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Event_real_wnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Event_real_wnd,
      qt_meta_data_Event_real_wnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Event_real_wnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Event_real_wnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Event_real_wnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Event_real_wnd))
        return static_cast<void*>(const_cast< Event_real_wnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Event_real_wnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
