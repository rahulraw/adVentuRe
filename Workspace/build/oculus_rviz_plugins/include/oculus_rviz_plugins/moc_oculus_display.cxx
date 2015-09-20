/****************************************************************************
** Meta object code from reading C++ file 'oculus_display.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/oculus_rviz_plugins/include/oculus_rviz_plugins/oculus_display.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oculus_display.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_oculus_rviz_plugins__OculusDisplay[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x09,
      58,   35,   35,   35, 0x09,
      82,   35,   35,   35, 0x09,
      99,   35,   35,   35, 0x09,
     129,  120,   35,   35, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_oculus_rviz_plugins__OculusDisplay[] = {
    "oculus_rviz_plugins::OculusDisplay\0\0"
    "onFullScreenChanged()\0onPredictionDtChanged()\0"
    "onPubTfChanged()\0onFollowCamChanged()\0"
    "newCount\0onScreenCountChanged(int)\0"
};

void oculus_rviz_plugins::OculusDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OculusDisplay *_t = static_cast<OculusDisplay *>(_o);
        switch (_id) {
        case 0: _t->onFullScreenChanged(); break;
        case 1: _t->onPredictionDtChanged(); break;
        case 2: _t->onPubTfChanged(); break;
        case 3: _t->onFollowCamChanged(); break;
        case 4: _t->onScreenCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData oculus_rviz_plugins::OculusDisplay::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject oculus_rviz_plugins::OculusDisplay::staticMetaObject = {
    { &rviz::Display::staticMetaObject, qt_meta_stringdata_oculus_rviz_plugins__OculusDisplay,
      qt_meta_data_oculus_rviz_plugins__OculusDisplay, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &oculus_rviz_plugins::OculusDisplay::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *oculus_rviz_plugins::OculusDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *oculus_rviz_plugins::OculusDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_oculus_rviz_plugins__OculusDisplay))
        return static_cast<void*>(const_cast< OculusDisplay*>(this));
    if (!strcmp(_clname, "Ogre::RenderTargetListener"))
        return static_cast< Ogre::RenderTargetListener*>(const_cast< OculusDisplay*>(this));
    typedef rviz::Display QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int oculus_rviz_plugins::OculusDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef rviz::Display QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
