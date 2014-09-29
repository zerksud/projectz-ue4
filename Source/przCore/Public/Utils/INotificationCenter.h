#pragma once

#include <functional>

#include "Utils/Dictionary.h"

namespace prz {
namespace utl {

class INotificationCenter {
public:
    virtual ~INotificationCenter() = default;

    typedef std::function<void(const ZDictionary& dict)> ZNotificationEventHandler;

    virtual bool AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) = 0;
    virtual bool RemoveObserver(const std::string& name, void* observerOwner) = 0;

    virtual bool PostNotification(const std::string& name) = 0;
    virtual bool PostNotification(const std::string& name, const ZDictionary& dict) = 0;
};

}
}
