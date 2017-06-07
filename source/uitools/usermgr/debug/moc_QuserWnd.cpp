/****************************************************************************
** Meta object code from reading C++ file 'QuserWnd.h'
**
** Created: Fri Apr 7 09:20:08 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QuserWnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QuserWnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QuserWnd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      20,    9,    9,    9, 0x0a,
      29,    9,    9,    9, 0x0a,
      38,    9,    9,    9, 0x0a,
      47,    9,    9,    9, 0x0a,
      59,    9,    9,    9, 0x0a,
      74,    9,    9,    9, 0x0a,
      84,    9,    9,    9, 0x0a,
      99,    9,    9,    9, 0x0a,
     117,    9,    9,    9, 0x0a,
     130,    9,    9,    9, 0x0a,
     145,    9,    9,    9, 0x0a,
     157,    9,    9,    9, 0x0a,
     169,    9,    9,    9, 0x0a,
     180,    9,    9,    9, 0x0a,
     192,    9,    9,    9, 0x0a,
     207,    9,    9,    9, 0x0a,
     222,    9,    9,    9, 0x0a,
     237,    9,    9,    9, 0x0a,
     252,    9,    9,    9, 0x0a,
     287,    9,    9,    9, 0x0a,
     324,  318,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QuserWnd[] = {
    "QuserWnd\0\0onLogin()\0onExit()\0onSave()\0"
    "onLoad()\0onAddRole()\0onDeleteRole()\0"
    "onApply()\0onSelAllRole()\0onNotSelAllRole()\0"
    "onreadAuth()\0onModifyNode()\0onAddNode()\0"
    "onDelNode()\0ViewAuth()\0onAddUser()\0"
    "onDeleteUser()\0onModifyUser()\0"
    "onSelAllAuth()\0onSelEptAuth()\0"
    "m_roleCfgLVclicked(QListViewItem*)\0"
    "onCurrentPageChanged(QWidget*)\0pItem\0"
    "GetAuthCheck(QTreeWidgetItem*)\0"
};

void QuserWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QuserWnd *_t = static_cast<QuserWnd *>(_o);
        switch (_id) {
        case 0: _t->onLogin(); break;
        case 1: _t->onExit(); break;
        case 2: _t->onSave(); break;
        case 3: _t->onLoad(); break;
        case 4: _t->onAddRole(); break;
        case 5: _t->onDeleteRole(); break;
        case 6: _t->onApply(); break;
        case 7: _t->onSelAllRole(); break;
        case 8: _t->onNotSelAllRole(); break;
        case 9: _t->onreadAuth(); break;
        case 10: _t->onModifyNode(); break;
        case 11: _t->onAddNode(); break;
        case 12: _t->onDelNode(); break;
        case 13: _t->ViewAuth(); break;
        case 14: _t->onAddUser(); break;
        case 15: _t->onDeleteUser(); break;
        case 16: _t->onModifyUser(); break;
        case 17: _t->onSelAllAuth(); break;
        case 18: _t->onSelEptAuth(); break;
        case 19: _t->m_roleCfgLVclicked((*reinterpret_cast< QListViewItem*(*)>(_a[1]))); break;
        case 20: _t->onCurrentPageChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 21: _t->GetAuthCheck((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QuserWnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QuserWnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QuserWnd,
      qt_meta_data_QuserWnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QuserWnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QuserWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QuserWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QuserWnd))
        return static_cast<void*>(const_cast< QuserWnd*>(this));
    if (!strcmp(_clname, "Ui::QuserWnd"))
        return static_cast< Ui::QuserWnd*>(const_cast< QuserWnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QuserWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
