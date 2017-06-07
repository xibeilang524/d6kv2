/****************************************************************************
** Meta object code from reading C++ file 'hisquery_wnd.h'
**
** Created: Fri Apr 7 09:20:43 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hisquery_wnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hisquery_wnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Hisquery_wnd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      27,   13,   13,   13, 0x08,
      47,   13,   13,   13, 0x08,
      67,   13,   13,   13, 0x08,
      79,   13,   13,   13, 0x08,
      99,   13,   13,   13, 0x08,
     123,   13,   13,   13, 0x08,
     152,   13,   13,   13, 0x08,
     176,   13,   13,   13, 0x08,
     193,   13,   13,   13, 0x08,
     212,   13,   13,   13, 0x08,
     228,   13,   13,   13, 0x08,
     260,  247,   13,   13, 0x08,
     317,  311,   13,   13, 0x08,
     357,  311,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Hisquery_wnd[] = {
    "Hisquery_wnd\0\0slot_print()\0"
    "slot_event_output()\0slot_event_delete()\0"
    "slot_exit()\0slot_today_button()\0"
    "slot_yesterday_button()\0"
    "slot_toolbar_search_button()\0"
    "slot_event_type_query()\0slot_dev_query()\0"
    "slot_fuzzy_query()\0slot_all_type()\0"
    "slot_all_station()\0pItem,column\0"
    "slot_event_type_tree_clicked(QTreeWidgetItem*,int)\0"
    "pItem\0slot_show_station_dev(QTreeWidgetItem*)\0"
    "slot_modify_event(QTableWidgetItem*)\0"
};

void Hisquery_wnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Hisquery_wnd *_t = static_cast<Hisquery_wnd *>(_o);
        switch (_id) {
        case 0: _t->slot_print(); break;
        case 1: _t->slot_event_output(); break;
        case 2: _t->slot_event_delete(); break;
        case 3: _t->slot_exit(); break;
        case 4: _t->slot_today_button(); break;
        case 5: _t->slot_yesterday_button(); break;
        case 6: _t->slot_toolbar_search_button(); break;
        case 7: _t->slot_event_type_query(); break;
        case 8: _t->slot_dev_query(); break;
        case 9: _t->slot_fuzzy_query(); break;
        case 10: _t->slot_all_type(); break;
        case 11: _t->slot_all_station(); break;
        case 12: _t->slot_event_type_tree_clicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->slot_show_station_dev((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->slot_modify_event((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Hisquery_wnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Hisquery_wnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Hisquery_wnd,
      qt_meta_data_Hisquery_wnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Hisquery_wnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Hisquery_wnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Hisquery_wnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Hisquery_wnd))
        return static_cast<void*>(const_cast< Hisquery_wnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Hisquery_wnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
