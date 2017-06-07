/****************************************************************************
** Meta object code from reading C++ file 'event_ui_set_dlg.h'
**
** Created: Fri Apr 7 09:21:02 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../event_ui_set_dlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_ui_set_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Event_ui_set_dlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      34,   17,   17,   17, 0x0a,
      53,   17,   17,   17, 0x0a,
      74,   69,   17,   17, 0x0a,
     121,   17,   17,   17, 0x0a,
     145,   69,   17,   17, 0x0a,
     170,   69,   17,   17, 0x0a,
     201,  195,   17,   17, 0x0a,
     252,   17,   17,   17, 0x0a,
     271,   17,   17,   17, 0x0a,
     289,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Event_ui_set_dlg[] = {
    "Event_ui_set_dlg\0\0slot_add_page()\0"
    "slot_modify_page()\0slot_del_page()\0"
    "item\0slot_double_clicked_pagelist(QTreeWidgetItem*)\0"
    "slot_refresh_pagelist()\0"
    "slot_refresh_pageCB(int)\0"
    "slot_refresh_filter(int)\0pItem\0"
    "slot_event_type_listview_clicked(QTreeWidgetItem*)\0"
    "slot_save_filter()\0slot_cancel_btn()\0"
    "show_page(int)\0"
};

void Event_ui_set_dlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Event_ui_set_dlg *_t = static_cast<Event_ui_set_dlg *>(_o);
        switch (_id) {
        case 0: _t->slot_add_page(); break;
        case 1: _t->slot_modify_page(); break;
        case 2: _t->slot_del_page(); break;
        case 3: _t->slot_double_clicked_pagelist((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->slot_refresh_pagelist(); break;
        case 5: _t->slot_refresh_pageCB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slot_refresh_filter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slot_event_type_listview_clicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->slot_save_filter(); break;
        case 9: _t->slot_cancel_btn(); break;
        case 10: _t->show_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Event_ui_set_dlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Event_ui_set_dlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Event_ui_set_dlg,
      qt_meta_data_Event_ui_set_dlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Event_ui_set_dlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Event_ui_set_dlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Event_ui_set_dlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Event_ui_set_dlg))
        return static_cast<void*>(const_cast< Event_ui_set_dlg*>(this));
    if (!strcmp(_clname, "Ui::QEventUISetBase"))
        return static_cast< Ui::QEventUISetBase*>(const_cast< Event_ui_set_dlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int Event_ui_set_dlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
