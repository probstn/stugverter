/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<Plotter.h>)
#  include <Plotter.h>
#endif
#if __has_include(<Scope.h>)
#  include <Scope.h>
#endif
#if __has_include(<ScopeManager.h>)
#  include <ScopeManager.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_stuglab()
{
    qmlRegisterModule("stuglab", 254, 0);
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<Plotter>("stuglab", 254);
    qmlRegisterTypesAndRevisions<Scope>("stuglab", 254);
    qmlRegisterTypesAndRevisions<ScopeManager>("stuglab", 254);
    QT_WARNING_POP
    qmlRegisterModule("stuglab", 254, 254);
}

static const QQmlModuleRegistration stuglabRegistration("stuglab", qml_register_types_stuglab);
