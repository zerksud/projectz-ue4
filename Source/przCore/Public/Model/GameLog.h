#pragma once

#include "Utils/StandardLibrary/List.h"
#include "Utils/StandardLibrary/String.h"

namespace prz {
namespace mdl {

class ZGameLog {
public:
    ZGameLog();
    virtual ~ZGameLog() = default;

    void Clear();
    void Log(const char* format, ...);
    void SetHistoryMaxSize(unsigned int size);

    typedef utl::ZList<utl::ZString> LogHistory;
    const utl::ZString& GetHistory(const utl::ZString& delimeter = "\n");

private:
    void AddMessage(const utl::ZString& message);

    LogHistory mHistory;
    unsigned int mHistoryMaxSize;

    bool mCachedHistoryIsValid;
    utl::ZString mCachedHistory;
};

}
}
