/****************************************************************************
** Meta object code from reading C++ file 'networkview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../ui/visualizers/networkview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkView_t {
    QByteArrayData data[13];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkView_t qt_meta_stringdata_NetworkView = {
    {
QT_MOC_LITERAL(0, 0, 11), // "NetworkView"
QT_MOC_LITERAL(1, 12, 9), // "cellMoved"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12), // "std::wstring"
QT_MOC_LITERAL(4, 36, 4), // "name"
QT_MOC_LITERAL(5, 41, 1), // "x"
QT_MOC_LITERAL(6, 43, 1), // "y"
QT_MOC_LITERAL(7, 45, 14), // "requestAddCell"
QT_MOC_LITERAL(8, 60, 17), // "requestAddSynapse"
QT_MOC_LITERAL(9, 78, 4), // "from"
QT_MOC_LITERAL(10, 83, 2), // "to"
QT_MOC_LITERAL(11, 86, 19), // "requestAddElectrode"
QT_MOC_LITERAL(12, 106, 8) // "cellName"

    },
    "NetworkView\0cellMoved\0\0std::wstring\0"
    "name\0x\0y\0requestAddCell\0requestAddSynapse\0"
    "from\0to\0requestAddElectrode\0cellName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       7,    2,   41,    2, 0x06 /* Public */,
       8,    2,   46,    2, 0x06 /* Public */,
      11,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int,    4,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    9,   10,
    QMetaType::Void, 0x80000000 | 3,   12,

       0        // eod
};

void NetworkView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->cellMoved((*reinterpret_cast< const std::wstring(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->requestAddCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->requestAddSynapse((*reinterpret_cast< const std::wstring(*)>(_a[1])),(*reinterpret_cast< const std::wstring(*)>(_a[2]))); break;
        case 3: _t->requestAddElectrode((*reinterpret_cast< const std::wstring(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkView::*)(const std::wstring & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkView::cellMoved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkView::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkView::requestAddCell)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkView::*)(const std::wstring & , const std::wstring & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkView::requestAddSynapse)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkView::*)(const std::wstring & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkView::requestAddElectrode)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_NetworkView.data,
    qt_meta_data_NetworkView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int NetworkView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void NetworkView::cellMoved(const std::wstring & _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkView::requestAddCell(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkView::requestAddSynapse(const std::wstring & _t1, const std::wstring & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkView::requestAddElectrode(const std::wstring & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
