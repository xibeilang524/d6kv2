/****************************************************************************
** Meta object code from reading C++ file 'dataselectortoolbar.h'
**
** Created: Fri Apr 7 09:20:14 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dataselectortoolbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataselectortoolbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataSelectorToolBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,   20,   20,   20, 0x0a,
      77,   69,   20,   20, 0x08,
     108,   69,   20,   20, 0x08,
     137,   69,   20,   20, 0x08,
     161,   69,   20,   20, 0x08,
     187,   69,   20,   20, 0x08,
     212,   69,   20,   20, 0x08,
     239,   69,   20,   20, 0x08,
     271,  266,   20,   20, 0x08,
     296,  266,   20,   20, 0x08,
     321,   20,   20,   20, 0x08,
     341,   20,   20,   20, 0x08,
     367,  361,   20,   20, 0x08,
     396,  361,   20,   20, 0x08,
     428,   20,   20,   20, 0x08,
     438,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DataSelectorToolBar[] = {
    "DataSelectorToolBar\0\0"
    "selectorChanged(selectorpara&)\0"
    "dataSrcChanged()\0checked\0"
    "variantCycleDayTriggered(bool)\0"
    "trendCycleDayTriggered(bool)\0"
    "cycleDayTriggered(bool)\0"
    "cycleMonthTriggered(bool)\0"
    "cycleYearTriggered(bool)\0"
    "generalDateTriggered(bool)\0"
    "segmentDateTriggered(bool)\0date\0"
    "dateEdit_1Changed(QDate)\0"
    "dateEdit_2Changed(QDate)\0timeEdit_1Changed()\0"
    "timeEdit_2Changed()\0index\0"
    "boxDataTypeIndexChanged(int)\0"
    "boxDayMinCycleIndexChanged(int)\0"
    "insComp()\0delComp()\0"
};

void DataSelectorToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataSelectorToolBar *_t = static_cast<DataSelectorToolBar *>(_o);
        switch (_id) {
        case 0: _t->selectorChanged((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 1: _t->dataSrcChanged(); break;
        case 2: _t->variantCycleDayTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->trendCycleDayTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->cycleDayTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->cycleMonthTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->cycleYearTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->generalDateTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->segmentDateTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->dateEdit_1Changed((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 10: _t->dateEdit_2Changed((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 11: _t->timeEdit_1Changed(); break;
        case 12: _t->timeEdit_2Changed(); break;
        case 13: _t->boxDataTypeIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->boxDayMinCycleIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->insComp(); break;
        case 16: _t->delComp(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataSelectorToolBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataSelectorToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_DataSelectorToolBar,
      qt_meta_data_DataSelectorToolBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataSelectorToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataSelectorToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataSelectorToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataSelectorToolBar))
        return static_cast<void*>(const_cast< DataSelectorToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int DataSelectorToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void DataSelectorToolBar::selectorChanged(selectorpara & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CurveSelectorToolBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   58,   21,   21, 0x08,
      88,   58,   21,   21, 0x08,
     105,   58,   21,   21, 0x08,
     120,   58,   21,   21, 0x08,
     137,   58,   21,   21, 0x08,
     156,   58,   21,   21, 0x08,
     175,   58,   21,   21, 0x08,
     194,   58,   21,   21, 0x08,
     215,   58,   21,   21, 0x08,
     233,   58,   21,   21, 0x08,
     250,   58,   21,   21, 0x08,
     267,   58,   21,   21, 0x08,
     287,  283,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CurveSelectorToolBar[] = {
    "CurveSelectorToolBar\0\0"
    "selectorChanged(selectorcurvepara&)\0"
    "checked\0attrAntialiased(bool)\0"
    "attrFitted(bool)\0showGrid(bool)\0"
    "showSymbol(bool)\0setLineStyle(bool)\0"
    "setDotsStyle(bool)\0setSetpStyle(bool)\0"
    "SetSticksStyle(bool)\0nullValZero(bool)\0"
    "nullValAvg(bool)\0nullValFix(bool)\0"
    "yAxisAuto(bool)\0val\0yAxisRangeChanged(int)\0"
};

void CurveSelectorToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CurveSelectorToolBar *_t = static_cast<CurveSelectorToolBar *>(_o);
        switch (_id) {
        case 0: _t->selectorChanged((*reinterpret_cast< selectorcurvepara(*)>(_a[1]))); break;
        case 1: _t->attrAntialiased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->attrFitted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->showGrid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->showSymbol((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setLineStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setDotsStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setSetpStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->SetSticksStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->nullValZero((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->nullValAvg((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->nullValFix((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->yAxisAuto((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->yAxisRangeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CurveSelectorToolBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CurveSelectorToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_CurveSelectorToolBar,
      qt_meta_data_CurveSelectorToolBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CurveSelectorToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CurveSelectorToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CurveSelectorToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CurveSelectorToolBar))
        return static_cast<void*>(const_cast< CurveSelectorToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int CurveSelectorToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CurveSelectorToolBar::selectorChanged(selectorcurvepara & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
