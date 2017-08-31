#pragma once

#include <qdatetime.h>

#define LOGEXEC(msg,...) std::printf("\n[%d]::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());std::printf((msg),__VA_ARGS__);std::printf(" : %d", __LINE__)
#define LOGERR(msg,...) std::printf("\n[%d]:ERROR!!!::", QDateTime::currentDateTime().time().msecsSinceStartOfDay());std::printf((msg),__VA_ARGS__);std::printf(" : %d", __LINE__)
