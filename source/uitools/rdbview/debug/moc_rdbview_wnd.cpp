/****************************************************************************
** Meta object code from reading C++ file 'rdbview_wnd.h'
**
** Created: Fri Apr 7 09:20:27 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rdbview_wnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rdbview_wnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Rdbview_wnd[] = {

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
      13,   12,   12,   12, 0x08,
      36,   12,   12,   12, 0x08,
      62,   12,   12,   12, 0x08,
      75,   12,   12,   12, 0x08,
      92,   12,   12,   12, 0x08,
     108,   12,   12,   12, 0x08,
     125,   12,   12,   12, 0x08,
     141,   12,   12,   12, 0x08,
     169,  163,   12,   12, 0x08,
     206,   12,   12,   12, 0x08,
     225,   12,   12,   12, 0x08,
     247,   12,   12,   12, 0x08,
     262,   12,   12,   12, 0x08,
     287,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Rdbview_wnd[] = {
    "Rdbview_wnd\0\0slot_sql_checkbox(int)\0"
    "slot_change_button_type()\0slot_timer()\0"
    "slot_read_data()\0slot_file_out()\0"
    "slot_scada_set()\0slot_ycyx_set()\0"
    "slot_read_data_info()\0pItem\0"
    "slot_select_change(QTreeWidgetItem*)\0"
    "slot_sql_connect()\0slot_sql_disconnect()\0"
    "slot_sql_run()\0slot_rdb_select_change()\0"
    "slot_sql_change()\0"
};

void Rdbview_wnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Rdbview_wnd *_t = static_cast<Rdbview_wnd *>(_o);
        switch (_id) {
        case 0: _t->slot_sql_checkbox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slot_change_button_type(); break;
        case 2: _t->slot_timer(); break;
        case 3: _t->slot_read_data(); break;
        case 4: _t->slot_file_out(); break;
        case 5: _t->slot_scada_set(); break;
        case 6: _t->slot_ycyx_set(); break;
        case 7: _t->slot_read_data_info(); break;
        case 8: _t->slot_select_change((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->slot_sql_connect(); break;
        case 10: _t->slot_sql_disconnect(); break;
        case 11: _t->slot_sql_run(); break;
        case 12: _t->slot_rdb_select_change(); break;
        case 13: _t->slot_sql_change(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Rdbview_wnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Rdbview_wnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Rdbview_wnd,
      qt_meta_data_Rdbview_wnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Rdbview_wnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Rdbview_wnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Rdbview_wnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Rdbview_wnd))
        return static_cast<void*>(const_cast< Rdbview_wnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Rdbview_wnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
