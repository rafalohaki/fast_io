#pragma once

#include"../fast_io.h"
#include<QIODevice>
#include<QFile>
#include<QFileDevice>
#include<QString>
#include<QStringRef>
#if defined(QT_VERSION) && defined(QT_VERSION_CHECK)
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
#include<QStringView>
#endif
#endif

#include"qt_impl/qstring.h"
#include"qt_impl/qiodevice.h"
#include"qt_impl/qfile.h"