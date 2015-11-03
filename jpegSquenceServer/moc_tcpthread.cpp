/****************************************************************************
** Meta object code from reading C++ file 'tcpthread.h'
**
** Created: Tue Nov 3 16:55:31 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tcpthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TcpThread[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TcpThread[] = {
    "TcpThread\0\0imageInfo(QImage&)\0RecvdFile()\0"
};

const QMetaObject TcpThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TcpThread,
      qt_meta_data_TcpThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TcpThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TcpThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TcpThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TcpThread))
        return static_cast<void*>(const_cast< TcpThread*>(this));
    return QThread::qt_metacast(_clname);
}

int TcpThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: imageInfo((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: RecvdFile(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TcpThread::imageInfo(QImage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
