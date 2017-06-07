/****************************************************************************
** Meta object code from reading C++ file 'QZoneMainWnd.h'
**
** Created: Fri Apr 7 09:20:26 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QZoneMainWnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QZoneMainWnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QZoneMainWnd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      24,   13,   13,   13, 0x0a,
      33,   13,   13,   13, 0x0a,
      42,   13,   13,   13, 0x0a,
      51,   13,   13,   13, 0x0a,
      63,   13,   13,   13, 0x0a,
      78,   13,   13,   13, 0x0a,
      93,   13,   13,   13, 0x0a,
     108,   13,   13,   13, 0x0a,
     126,   13,   13,   13, 0x0a,
     142,   13,   13,   13, 0x0a,
     156,   13,   13,   13, 0x0a,
     173,   13,   13,   13, 0x0a,
     184,   13,   13,   13, 0x0a,
     195,   13,   13,   13, 0x0a,
     210,   13,   13,   13, 0x0a,
     231,  225,   13,   13, 0x0a,
     268,   13,   13,   13, 0x0a,
     304,  299,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QZoneMainWnd[] = {
    "QZoneMainWnd\0\0onLogin()\0onExit()\0"
    "onSave()\0onLoad()\0onAddZone()\0"
    "onDeleteZone()\0onModifyZone()\0"
    "onSelAllZone()\0onNotSelAllZone()\0"
    "onFuzzySelObj()\0onSelAllObj()\0"
    "onNotSelAllObj()\0onAppSel()\0onAppAll()\0"
    "onUserAppSel()\0onUserAppAll()\0pItem\0"
    "onZoneCfgLVclicked(QTreeWidgetItem*)\0"
    "onCurrentPageChanged(QWidget*)\0item\0"
    "showRtuObj(QTreeWidgetItem*)\0"
};

void QZoneMainWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QZoneMainWnd *_t = static_cast<QZoneMainWnd *>(_o);
        switch (_id) {
        case 0: _t->onLogin(); break;
        case 1: _t->onExit(); break;
        case 2: _t->onSave(); break;
        case 3: _t->onLoad(); break;
        case 4: _t->onAddZone(); break;
        case 5: _t->onDeleteZone(); break;
        case 6: _t->onModifyZone(); break;
        case 7: _t->onSelAllZone(); break;
        case 8: _t->onNotSelAllZone(); break;
        case 9: _t->onFuzzySelObj(); break;
        case 10: _t->onSelAllObj(); break;
        case 11: _t->onNotSelAllObj(); break;
        case 12: _t->onAppSel(); break;
        case 13: _t->onAppAll(); break;
        case 14: _t->onUserAppSel(); break;
        case 15: _t->onUserAppAll(); break;
        case 16: _t->onZoneCfgLVclicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 17: _t->onCurrentPageChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 18: _t->showRtuObj((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QZoneMainWnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QZoneMainWnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QZoneMainWnd,
      qt_meta_data_QZoneMainWnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QZoneMainWnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QZoneMainWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QZoneMainWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QZoneMainWnd))
        return static_cast<void*>(const_cast< QZoneMainWnd*>(this));
    if (!strcmp(_clname, "Ui::QZoneWndBase"))
        return static_cast< Ui::QZoneWndBase*>(const_cast< QZoneMainWnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QZoneMainWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
