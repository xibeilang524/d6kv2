/****************************************************************************
** Meta object code from reading C++ file 'outagequery.h'
**
** Created: Fri Apr 7 09:20:27 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../outagequery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'outagequery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPowerCutQuery[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      31,   15,   15,   15, 0x0a,
      48,   15,   15,   15, 0x0a,
      67,   15,   15,   15, 0x0a,
      82,   15,   15,   15, 0x0a,
      95,   15,   15,   15, 0x0a,
     112,   15,   15,   15, 0x0a,
     121,   15,   15,   15, 0x0a,
     130,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QPowerCutQuery[] = {
    "QPowerCutQuery\0\0onStQueryBtn()\0"
    "onAreaQueryBtn()\0onFeederQueryBtn()\0"
    "onAllAreaBtn()\0onAllStBtn()\0"
    "onAllFeederBtn()\0onRead()\0onExit()\0"
    "onQueryBtnClicked()\0"
};

void QPowerCutQuery::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPowerCutQuery *_t = static_cast<QPowerCutQuery *>(_o);
        switch (_id) {
        case 0: _t->onStQueryBtn(); break;
        case 1: _t->onAreaQueryBtn(); break;
        case 2: _t->onFeederQueryBtn(); break;
        case 3: _t->onAllAreaBtn(); break;
        case 4: _t->onAllStBtn(); break;
        case 5: _t->onAllFeederBtn(); break;
        case 6: _t->onRead(); break;
        case 7: _t->onExit(); break;
        case 8: _t->onQueryBtnClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPowerCutQuery::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPowerCutQuery::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QPowerCutQuery,
      qt_meta_data_QPowerCutQuery, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPowerCutQuery::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPowerCutQuery::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPowerCutQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPowerCutQuery))
        return static_cast<void*>(const_cast< QPowerCutQuery*>(this));
    if (!strcmp(_clname, "Ui::WindowBase"))
        return static_cast< Ui::WindowBase*>(const_cast< QPowerCutQuery*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QPowerCutQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
