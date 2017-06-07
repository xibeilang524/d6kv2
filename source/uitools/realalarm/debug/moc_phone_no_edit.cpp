/****************************************************************************
** Meta object code from reading C++ file 'phone_no_edit.h'
**
** Created: Fri Apr 7 09:21:02 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../phone_no_edit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'phone_no_edit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Phone_no_edit[] = {

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
      27,   15,   14,   14, 0x0a,
      80,   72,   14,   14, 0x0a,
     121,   14,   14,   14, 0x0a,
     137,   14,   14,   14, 0x0a,
     156,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Phone_no_edit[] = {
    "Phone_no_edit\0\0item,column\0"
    "slot_refresh_desc_tree(QTreeWidgetItem*,int)\0"
    "phoneNo\0slot_refresh_selected_desc_tree(QString)\0"
    "slot_add_desc()\0slot_delete_desc()\0"
    "slot_load_desc()\0"
};

void Phone_no_edit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Phone_no_edit *_t = static_cast<Phone_no_edit *>(_o);
        switch (_id) {
        case 0: _t->slot_refresh_desc_tree((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->slot_refresh_selected_desc_tree((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->slot_add_desc(); break;
        case 3: _t->slot_delete_desc(); break;
        case 4: _t->slot_load_desc(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Phone_no_edit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Phone_no_edit::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Phone_no_edit,
      qt_meta_data_Phone_no_edit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Phone_no_edit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Phone_no_edit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Phone_no_edit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Phone_no_edit))
        return static_cast<void*>(const_cast< Phone_no_edit*>(this));
    if (!strcmp(_clname, "Ui::phoneNoEditDlg"))
        return static_cast< Ui::phoneNoEditDlg*>(const_cast< Phone_no_edit*>(this));
    return QDialog::qt_metacast(_clname);
}

int Phone_no_edit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
