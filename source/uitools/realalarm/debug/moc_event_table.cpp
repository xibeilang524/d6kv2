/****************************************************************************
** Meta object code from reading C++ file 'event_table.h'
**
** Created: Fri Apr 7 09:21:03 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../event_table.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_table.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Event_table[] = {

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
      13,   12,   12,   12, 0x0a,
      39,   37,   12,   12, 0x0a,
      75,   37,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Event_table[] = {
    "Event_table\0\0slot_pop_menu(QAction*)\0"
    ",\0right_clicked(QTreeWidgetItem*,int)\0"
    "slot_current_page_changed(QTreeWidgetItem*,int)\0"
};

void Event_table::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Event_table *_t = static_cast<Event_table *>(_o);
        switch (_id) {
        case 0: _t->slot_pop_menu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->right_clicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->slot_current_page_changed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Event_table::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Event_table::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_Event_table,
      qt_meta_data_Event_table, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Event_table::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Event_table::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Event_table::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Event_table))
        return static_cast<void*>(const_cast< Event_table*>(this));
    if (!strcmp(_clname, "Event_filter"))
        return static_cast< Event_filter*>(const_cast< Event_table*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int Event_table::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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
