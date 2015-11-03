/****************************************************************************
** Meta object code from reading C++ file 'tcpdialog.h'
**
** Created: Tue Nov 3 16:55:32 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tcpdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcpdialog[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tcpdialog[] = {
    "tcpdialog\0\0DisplayImage(QImage&)\0"
};

const QMetaObject tcpdialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_tcpdialog,
      qt_meta_data_tcpdialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcpdialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcpdialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcpdialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcpdialog))
        return static_cast<void*>(const_cast< tcpdialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int tcpdialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DisplayImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
