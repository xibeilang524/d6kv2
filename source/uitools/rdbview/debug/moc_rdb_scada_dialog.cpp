/****************************************************************************
** Meta object code from reading C++ file 'rdb_scada_dialog.h'
**
** Created: Fri Apr 7 09:20:29 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rdb_scada_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rdb_scada_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Rdb_scada_dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      30,   17,   17,   17, 0x08,
      48,   17,   17,   17, 0x08,
      67,   17,   17,   17, 0x08,
      86,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Rdb_scada_dialog[] = {
    "Rdb_scada_dialog\0\0slot_exit()\0"
    "slot_select_all()\0slot_select_none()\0"
    "slot_save_config()\0slot_table_select_change(int)\0"
};

void Rdb_scada_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Rdb_scada_dialog *_t = static_cast<Rdb_scada_dialog *>(_o);
        switch (_id) {
        case 0: _t->slot_exit(); break;
        case 1: _t->slot_select_all(); break;
        case 2: _t->slot_select_none(); break;
        case 3: _t->slot_save_config(); break;
        case 4: _t->slot_table_select_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Rdb_scada_dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Rdb_scada_dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Rdb_scada_dialog,
      qt_meta_data_Rdb_scada_dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Rdb_scada_dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Rdb_scada_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Rdb_scada_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Rdb_scada_dialog))
        return static_cast<void*>(const_cast< Rdb_scada_dialog*>(this));
    if (!strcmp(_clname, "Ui::Rdb_scada_dialog_base"))
        return static_cast< Ui::Rdb_scada_dialog_base*>(const_cast< Rdb_scada_dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Rdb_scada_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
