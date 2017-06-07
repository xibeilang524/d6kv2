/****************************************************************************
** Meta object code from reading C++ file 'convertools.h'
**
** Created: Fri Apr 7 09:19:54 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../convertools.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'convertools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Convertools[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      29,   12,   12,   12, 0x0a,
      46,   12,   12,   12, 0x0a,
      66,   12,   12,   12, 0x0a,
      85,   12,   12,   12, 0x0a,
     103,   12,   12,   12, 0x0a,
     116,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Convertools[] = {
    "Convertools\0\0slot_open_dir()\0"
    "slot_open_file()\0slot_del_filelist()\0"
    "slot_search_file()\0slot_gdf_to_bgf()\0"
    "slot_about()\0slot_exit()\0"
};

void Convertools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Convertools *_t = static_cast<Convertools *>(_o);
        switch (_id) {
        case 0: _t->slot_open_dir(); break;
        case 1: _t->slot_open_file(); break;
        case 2: _t->slot_del_filelist(); break;
        case 3: _t->slot_search_file(); break;
        case 4: _t->slot_gdf_to_bgf(); break;
        case 5: _t->slot_about(); break;
        case 6: _t->slot_exit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Convertools::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Convertools::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Convertools,
      qt_meta_data_Convertools, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Convertools::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Convertools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Convertools::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Convertools))
        return static_cast<void*>(const_cast< Convertools*>(this));
    if (!strcmp(_clname, "Ui::convertbase"))
        return static_cast< Ui::convertbase*>(const_cast< Convertools*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Convertools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
