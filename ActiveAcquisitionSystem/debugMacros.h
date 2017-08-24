#pragma once

#include <qdatetime.h>

#define LOGEXEC(msg,...) printf("\n[%d]::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());printf((msg),__VA_ARGS__);printf(" : %d\n", __LINE__)
#define LOGERR(msg,...) printf("\n[%d]:ERROR!!!::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());printf((msg),__VA_ARGS__);printf(" : %d\n", __LINE__)
