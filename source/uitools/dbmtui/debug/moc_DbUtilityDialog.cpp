/****************************************************************************
** Meta object code from reading C++ file 'DbUtilityDialog.h'
**
** Created: Fri Apr 7 09:20:48 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DbUtilityDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DbUtilityDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DbUtilityDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      22,   13,   13,   13, 0x0a,
      29,   13,   13,   13, 0x0a,
      43,   37,   13,   13, 0x0a,
      63,   13,   13,   13, 0x0a,
      74,   13,   13,   13, 0x0a,
      85,   13,   13,   13, 0x0a,
     108,  102,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DbUtilityDlg[] = {
    "DbUtilityDlg\0\0begin()\0exit()\0about()\0"
    "index\0tab_selChanged(int)\0importDb()\0"
    "exportDb()\0userSelChanged()\0state\0"
    "slotChekmdlChanged(int)\0"
};

void DbUtilityDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DbUtilityDlg *_t = static_cast<DbUtilityDlg *>(_o);
        switch (_id) {
        case 0: _t->begin(); break;
        case 1: _t->exit(); break;
        case 2: _t->about(); break;
        case 3: _t->tab_selChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->importDb(); break;
        case 5: _t->exportDb(); break;
        case 6: _t->userSelChanged(); break;
        case 7: _t->slotChekmdlChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DbUtilityDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DbUtilityDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DbUtilityDlg,
      qt_meta_data_DbUtilityDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DbUtilityDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DbUtilityDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DbUtilityDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DbUtilityDlg))
        return static_cast<void*>(const_cast< DbUtilityDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int DbUtilityDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
