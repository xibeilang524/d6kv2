/****************************************************************************
** Meta object code from reading C++ file 'trendcurveform.h'
**
** Created: Fri Apr 7 09:20:12 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../trendcurveform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trendcurveform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrendCurveForm[] = {

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
      24,   16,   15,   15, 0x0a,
      63,   55,   15,   15, 0x0a,
      99,   15,   15,   15, 0x08,
     129,   15,   15,   15, 0x08,
     143,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TrendCurveForm[] = {
    "TrendCurveForm\0\0selPara\0"
    "updateCurveData(selectorpara&)\0selpara\0"
    "selectorChanged(selectorcurvepara&)\0"
    "mouseSelected(QwtDoublePoint)\0"
    "moved(QPoint)\0selected(QwtPolygon)\0"
};

void TrendCurveForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TrendCurveForm *_t = static_cast<TrendCurveForm *>(_o);
        switch (_id) {
        case 0: _t->updateCurveData((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 1: _t->selectorChanged((*reinterpret_cast< selectorcurvepara(*)>(_a[1]))); break;
        case 2: _t->mouseSelected((*reinterpret_cast< const QwtDoublePoint(*)>(_a[1]))); break;
        case 3: _t->moved((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: _t->selected((*reinterpret_cast< const QwtPolygon(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TrendCurveForm::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TrendCurveForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TrendCurveForm,
      qt_meta_data_TrendCurveForm, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrendCurveForm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrendCurveForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrendCurveForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrendCurveForm))
        return static_cast<void*>(const_cast< TrendCurveForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int TrendCurveForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
