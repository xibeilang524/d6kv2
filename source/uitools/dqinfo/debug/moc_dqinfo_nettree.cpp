/****************************************************************************
** Meta object code from reading C++ file 'dqinfo_nettree.h'
**
** Created: Fri Apr 7 09:20:31 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dqinfo_nettree.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dqinfo_nettree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QNetTree[] = {

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
      10,    9,    9,    9, 0x08,
      34,   20,    9,    9, 0x08,
      70,    9,    9,    9, 0x08,
      82,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QNetTree[] = {
    "QNetTree\0\0onTimer()\0server,column\0"
    "serverClicked(QTreeWidgetItem*,int)\0"
    "setMaster()\0setMainSrv()\0"
};

void QNetTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QNetTree *_t = static_cast<QNetTree *>(_o);
        switch (_id) {
        case 0: _t->onTimer(); break;
        case 1: _t->serverClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setMaster(); break;
        case 3: _t->setMainSrv(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QNetTree::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QNetTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_QNetTree,
      qt_meta_data_QNetTree, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QNetTree::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QNetTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QNetTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QNetTree))
        return static_cast<void*>(const_cast< QNetTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int QNetTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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
