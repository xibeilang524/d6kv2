/****************************************************************************
** Meta object code from reading C++ file 'hisdatamodel.h'
**
** Created: Fri Apr 7 09:20:13 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../hisdatamodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hisdatamodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HisDataModel[] = {

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
      22,   14,   13,   13, 0x0a,
      61,   53,   13,   13, 0x0a,
      92,   53,   13,   13, 0x0a,
     133,   13,   13,   13, 0x0a,
     167,   13,  155,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HisDataModel[] = {
    "HisDataModel\0\0selPara\0"
    "updateModelData(selectorpara&)\0selpara\0"
    "selectorChanged(selectorpara&)\0"
    "selectorCurveChanged(selectorcurvepara&)\0"
    "updateModelDataToDB()\0MainWindow*\0"
    "mainWindow()\0"
};

void HisDataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HisDataModel *_t = static_cast<HisDataModel *>(_o);
        switch (_id) {
        case 0: _t->updateModelData((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 1: _t->selectorChanged((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 2: _t->selectorCurveChanged((*reinterpret_cast< selectorcurvepara(*)>(_a[1]))); break;
        case 3: _t->updateModelDataToDB(); break;
        case 4: { MainWindow* _r = _t->mainWindow();
            if (_a[0]) *reinterpret_cast< MainWindow**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HisDataModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HisDataModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_HisDataModel,
      qt_meta_data_HisDataModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HisDataModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HisDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HisDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HisDataModel))
        return static_cast<void*>(const_cast< HisDataModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int HisDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
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
