/****************************************************************************
** Meta object code from reading C++ file 'msgframe.h'
**
** Created: Fri Apr 7 09:05:17 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../msgframe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msgframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMsgFrame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      21,   10,   10,   10, 0x0a,
      34,   10,   10,   10, 0x0a,
      44,   10,   10,   10, 0x0a,
      53,   10,   10,   10, 0x0a,
      64,   10,   10,   10, 0x0a,
      74,   10,   10,   10, 0x0a,
      95,   10,   10,   10, 0x0a,
     111,   10,   10,   10, 0x0a,
     121,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMsgFrame[] = {
    "QMsgFrame\0\0onPause()\0onContinue()\0"
    "onClean()\0onSave()\0autoSave()\0onTimer()\0"
    "onSearchTextChange()\0onClearSearch()\0"
    "onCheck()\0setTime()\0"
};

void QMsgFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMsgFrame *_t = static_cast<QMsgFrame *>(_o);
        switch (_id) {
        case 0: _t->onPause(); break;
        case 1: _t->onContinue(); break;
        case 2: _t->onClean(); break;
        case 3: _t->onSave(); break;
        case 4: _t->autoSave(); break;
        case 5: _t->onTimer(); break;
        case 6: _t->onSearchTextChange(); break;
        case 7: _t->onClearSearch(); break;
        case 8: _t->onCheck(); break;
        case 9: _t->setTime(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QMsgFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMsgFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QMsgFrame,
      qt_meta_data_QMsgFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMsgFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMsgFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMsgFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMsgFrame))
        return static_cast<void*>(const_cast< QMsgFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int QMsgFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
