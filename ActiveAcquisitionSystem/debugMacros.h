#pragma once

#include <qdatetime.h>

#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QDateTime::currentDateTime().time().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QDateTime::currentDateTime().time().msecsSinceStartOfDay(),(msg), __LINE__)
/*
//TODO Review, qtime or qdatetime?
#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
*/