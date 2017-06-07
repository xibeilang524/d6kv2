/****************************************************************************
** Meta object code from reading C++ file 'event_sys_set_dlg.h'
**
** Created: Fri Apr 7 09:21:03 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../event_sys_set_dlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'event_sys_set_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QEventSysSetDlg[] = {

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
      22,   17,   16,   16, 0x0a,
      65,   17,   16,   16, 0x0a,
      83,   16,   16,   16, 0x0a,
      90,   16,   16,   16, 0x0a,
     101,   16,   16,   16, 0x0a,
     111,   16,   16,   16, 0x0a,
     125,  123,   16,   16, 0x0a,
     167,  123,   16,   16, 0x0a,
     217,   16,   16,   16, 0x0a,
     238,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QEventSysSetDlg[] = {
    "QEventSysSetDlg\0\0item\0"
    "onDoubleClickedColorList(QListWidgetItem*)\0"
    "onEventColor(int)\0onOk()\0onCancel()\0"
    "onColUp()\0onColDown()\0,\0"
    "onReportTreeClicked(QTreeWidgetItem*,int)\0"
    "onReportTreeListViewClicked(QTreeWidgetItem*,int)\0"
    "onReportFileChoose()\0"
    "onPhoneNoEdit(QTableWidgetItem*)\0"
};

void QEventSysSetDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QEventSysSetDlg *_t = static_cast<QEventSysSetDlg *>(_o);
        switch (_id) {
        case 0: _t->onDoubleClickedColorList((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->onEventColor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onOk(); break;
        case 3: _t->onCancel(); break;
        case 4: _t->onColUp(); break;
        case 5: _t->onColDown(); break;
        case 6: _t->onReportTreeClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onReportTreeListViewClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->onReportFileChoose(); break;
        case 9: _t->onPhoneNoEdit((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QEventSysSetDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QEventSysSetDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QEventSysSetDlg,
      qt_meta_data_QEventSysSetDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QEventSysSetDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QEventSysSetDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QEventSysSetDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QEventSysSetDlg))
        return static_cast<void*>(const_cast< QEventSysSetDlg*>(this));
    if (!strcmp(_clname, "Ui::QEventSysSetBase"))
        return static_cast< Ui::QEventSysSetBase*>(const_cast< QEventSysSetDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QEventSysSetDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
