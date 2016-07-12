#ifndef TRACKINGCOLOREDOBJECTS_H
#define TRACKINGCOLOREDOBJECTS_H

#include "TrackingColoredObjects_global.h"
#include <interfaces.h>

/*!
 * \brief The TrackingColoredObjects class
 * this is a exapmle how to create your own library for tracking with opencv and
 * qtpk
 */
class TRACKINGCOLOREDOBJECTSSHARED_EXPORT TrackingColoredObjects: public QObject,NodeLibrary{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)//Declaration of used interface
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.TrackingColoredObjects/1.0" FILE "TrackingColoredObjects.json")
#endif
public:
    TrackingColoredObjects();
    /*!
     * \brief getLibraryName returns the userfriendly library name
     * \return
     */
    virtual QString getLibraryName();
    /*!
     * \brief registrate classes in the plugin
     *  is called in when the plugin is loaded to make avalaible dynamic classes construction
     * \param filename the filename of the plugin
     */
    virtual void registrate(QString filename);
};

#endif // TRACKINGCOLOREDOBJECTS_H
