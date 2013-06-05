#pragma once

#include <QtCore/qglobal.h>
#include <QGraphicsItem>

#if defined(SRC_LIBRARY)
#  define SRCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SRCSHARED_EXPORT Q_DECL_IMPORT
#endif

#ifndef GRAPHICS_TYPE_ZERO
#define GRAPHICS_TYPE_ZERO QGraphicsItem::UserType
#endif
namespace GraphicsPlot
{
    enum GraphicsType
    {
        DekartPlot = GRAPHICS_TYPE_ZERO+1,
        DataType = GRAPHICS_TYPE_ZERO+2
    };
};
