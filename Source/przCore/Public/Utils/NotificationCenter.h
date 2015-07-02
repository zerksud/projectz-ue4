#pragma once

#include <unordered_map>
#include <vector>

#include "Platform.h"

#include "Utils/INotificationCenter.h"

namespace prz {
namespace utl {

class PRZCORE_API ZNotificationCenter : public INotificationCenter {
public:
    ZNotificationCenter() = default;
    ZNotificationCenter(const ZNotificationCenter& other) = delete;
    ZNotificationCenter& operator=(const ZNotificationCenter& other) = delete;
    virtual ~ZNotificationCenter();

    virtual bool AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) override;
    virtual bool RemoveObserver(const std::string& name, void* observerOwner) override;

    virtual bool PostNotification(const std::string& name) override;
    virtual bool PostNotification(const std::string& name, const ZDictionary& dict) override;

private:
    struct ZObserver {
        void* observerOwner;
        ZNotificationEventHandler handler;

        ZObserver(void* pObserverOwner, ZNotificationEventHandler pHandler) {
            this->observerOwner = pObserverOwner;
            this->handler = pHandler;
        }
    };

    typedef std::vector<ZObserver> ZObserverList;
    typedef std::unordered_map<std::string, ZObserverList*> ZObserverListTable;
    typedef std::pair<std::string, ZObserverList*> ZObserverListTablePair;

    ZObserverListTable mObservers;
};

}
}
