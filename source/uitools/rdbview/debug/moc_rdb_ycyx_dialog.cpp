/****************************************************************************
** Meta object code from reading C++ file 'rdb_ycyx_dialog.h'
**
** Created: Fri Apr 7 09:20:28 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rdb_ycyx_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rdb_ycyx_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Rdb_ycyx_dialog[] = {

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
      17,   16,   16,   16, 0x08,
      27,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Rdb_ycyx_dialog[] = {
    "Rdb_ycyx_dialog\0\0slot_ok()\0slot_cancel()\0"
};

void Rdb_ycyx_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Rdb_ycyx_dialog *_t = static_cast<Rdb_ycyx_dialog *>(_o);
        switch (_id) {
        case 0: _t->slot_ok(); break;
        case 1: _t->slot_cancel(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Rdb_ycyx_dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Rdb_ycyx_dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Rdb_ycyx_dialog,
      qt_meta_data_Rdb_ycyx_dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Rdb_ycyx_dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Rdb_ycyx_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Rdb_ycyx_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Rdb_ycyx_dialog))
        return static_cast<void*>(const_cast< Rdb_ycyx_dialog*>(this));
    if (!strcmp(_clname, "Ui::Rdb_ycyx_dialog_base"))
        return static_cast< Ui::Rdb_ycyx_dialog_base*>(const_cast< Rdb_ycyx_dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Rdb_ycyx_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
