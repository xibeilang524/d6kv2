/****************************************************************************
** Meta object code from reading C++ file 'variantcurveform.h'
**
** Created: Fri Apr 7 09:20:10 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../variantcurveform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'variantcurveform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VariantCurveForm[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   18,   17,   17, 0x0a,
      65,   57,   17,   17, 0x0a,
     101,   17,   17,   17, 0x08,
     128,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VariantCurveForm[] = {
    "VariantCurveForm\0\0selPara\0"
    "updateCurveData(selectorpara&)\0selpara\0"
    "selectorChanged(selectorcurvepara&)\0"
    "mouseMoved(QwtDoublePoint)\0"
    "selected(QwtPolygon)\0"
};

void VariantCurveForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VariantCurveForm *_t = static_cast<VariantCurveForm *>(_o);
        switch (_id) {
        case 0: _t->updateCurveData((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 1: _t->selectorChanged((*reinterpret_cast< selectorcurvepara(*)>(_a[1]))); break;
        case 2: _t->mouseMoved((*reinterpret_cast< const QwtDoublePoint(*)>(_a[1]))); break;
        case 3: _t->selected((*reinterpret_cast< const QwtPolygon(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VariantCurveForm::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VariantCurveForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VariantCurveForm,
      qt_meta_data_VariantCurveForm, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VariantCurveForm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VariantCurveForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VariantCurveForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VariantCurveForm))
        return static_cast<void*>(const_cast< VariantCurveForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int VariantCurveForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
