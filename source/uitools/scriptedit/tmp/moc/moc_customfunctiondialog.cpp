/****************************************************************************
** Meta object code from reading C++ file 'customfunctiondialog.h'
**
** Created: Fri Apr 7 09:20:30 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../customfunctiondialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customfunctiondialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CustomFunctionDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   22,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CustomFunctionDialog[] = {
    "CustomFunctionDialog\0\0button\0"
    "buttonClicked(QAbstractButton*)\0"
};

void CustomFunctionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CustomFunctionDialog *_t = static_cast<CustomFunctionDialog *>(_o);
        switch (_id) {
        case 0: _t->buttonClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CustomFunctionDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CustomFunctionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CustomFunctionDialog,
      qt_meta_data_CustomFunctionDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CustomFunctionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CustomFunctionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CustomFunctionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomFunctionDialog))
        return static_cast<void*>(const_cast< CustomFunctionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CustomFunctionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
