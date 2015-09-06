#pragma once

#include "Utils/ZDictionary.h"
#include "Utils/StandardLibrary/Functional.h"

namespace prz {
namespace utl {

class INotificationCenter {
public:
    virtual ~INotificationCenter() = default;

    typedef utl::ZFunction<void(const ZDictionary& dict)> ZNotificationEventHandler;

    virtual bool AddObserver(const ZString& name, void* observerOwner, ZNotificationEventHandler handler) = 0;
    virtual bool RemoveObserver(const ZString& name, void* observerOwner) = 0;

    virtual bool PostNotification(const ZString& name) = 0;
    virtual bool PostNotification(const ZString& name, const ZDictionary& dict) = 0;
};

}
}
