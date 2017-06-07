/****************************************************************************
** Meta object code from reading C++ file 'selectorwidget.h'
**
** Created: Fri Apr 7 09:20:12 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../selectorwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectorwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SelectorWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      47,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   15,   15,   15, 0x08,
      74,   15,   15,   15, 0x08,
      90,   15,   15,   15, 0x08,
     115,  107,   15,   15, 0x08,
     136,  107,   15,   15, 0x08,
     173,  156,   15,   15, 0x08,
     232,  220,   15,   15, 0x08,
     271,  266,   15,   15, 0x08,
     304,  107,   15,   15, 0x08,
     325,  107,   15,   15, 0x08,
     350,   15,   15,   15, 0x08,
     374,  369,   15,   15, 0x08,
     399,  266,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SelectorWidget[] = {
    "SelectorWidget\0\0selectorChanged(selectorpara&)\0"
    "dataSrcChanged()\0refresh()\0stationYcType()\0"
    "stationKwhType()\0checked\0singleSelector(bool)\0"
    "mutilSelector(bool)\0current,previous\0"
    "updateCurve(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "item,column\0updateCurve(QTreeWidgetItem*,int)\0"
    "item\0updateNodeInfo(QTreeWidgetItem*)\0"
    "showFindWidget(bool)\0showNodeInfoWidget(bool)\0"
    "findEnterPressed()\0text\0"
    "findTextChanged(QString)\0"
    "showRtuDetailObj(QTreeWidgetItem*)\0"
};

void SelectorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SelectorWidget *_t = static_cast<SelectorWidget *>(_o);
        switch (_id) {
        case 0: _t->selectorChanged((*reinterpret_cast< selectorpara(*)>(_a[1]))); break;
        case 1: _t->dataSrcChanged(); break;
        case 2: _t->refresh(); break;
        case 3: _t->stationYcType(); break;
        case 4: _t->stationKwhType(); break;
        case 5: _t->singleSelector((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->mutilSelector((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->updateCurve((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 8: _t->updateCurve((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->updateNodeInfo((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->showFindWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->showNodeInfoWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->findEnterPressed(); break;
        case 13: _t->findTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->showRtuDetailObj((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SelectorWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SelectorWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SelectorWidget,
      qt_meta_data_SelectorWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SelectorWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SelectorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SelectorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SelectorWidget))
        return static_cast<void*>(const_cast< SelectorWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SelectorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void SelectorWidget::selectorChanged(selectorpara & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SelectorWidget::dataSrcChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
