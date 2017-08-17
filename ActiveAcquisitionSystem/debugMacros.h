#pragma once

#include <qdatetime.h>

#define LOGEXEC(msg,...) printf("\n[%d]::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());printf((msg),__VA_ARGS__);printf(" : %d\n", __LINE__)
#define LOGERR(msg,...) printf("\n[%d]:ERROR!!!::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());printf((msg),__VA_ARGS__);printf(" : %d\n", __LINE__)
/*
//TODO Review, qtime or qdatetime?
#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
*/