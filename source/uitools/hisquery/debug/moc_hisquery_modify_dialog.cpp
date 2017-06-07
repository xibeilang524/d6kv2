/****************************************************************************
** Meta object code from reading C++ file 'hisquery_modify_dialog.h'
**
** Created: Fri Apr 7 09:20:43 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hisquery_modify_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hisquery_modify_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Event_modify_dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x08,
      55,   21,   20,   20, 0x08,
      84,   21,   20,   20, 0x08,
     106,   21,   20,   20, 0x08,
     130,   20,   20,   20, 0x08,
     150,   20,   20,   20, 0x08,
     170,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Event_modify_dialog[] = {
    "Event_modify_dialog\0\0item\0"
    "slot_event_sort_changed(int)\0"
    "slot_event_type_changed(int)\0"
    "slot_rtu_changed(int)\0slot_group_changed(int)\0"
    "slot_modify_event()\0slot_delete_event()\0"
    "slot_add_event()\0"
};

void Event_modify_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Event_modify_dialog *_t = static_cast<Event_modify_dialog *>(_o);
        switch (_id) {
        case 0: _t->slot_event_sort_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slot_event_type_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slot_rtu_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slot_group_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_modify_event(); break;
        case 5: _t->slot_delete_event(); break;
        case 6: _t->slot_add_event(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Event_modify_dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Event_modify_dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Event_modify_dialog,
      qt_meta_data_Event_modify_dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Event_modify_dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Event_modify_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Event_modify_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Event_modify_dialog))
        return static_cast<void*>(const_cast< Event_modify_dialog*>(this));
    if (!strcmp(_clname, "Ui::Event_modify_dialog_base"))
        return static_cast< Ui::Event_modify_dialog_base*>(const_cast< Event_modify_dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Event_modify_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
