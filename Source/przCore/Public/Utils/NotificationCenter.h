#pragma once

#include "Platform.h"

#include "Utils/StandartLibrary/UnorderedMap.h"
#include "Utils/StandartLibrary/Utility.h"
#include "Utils/StandartLibrary/Vector.h"
#include "Utils/INotificationCenter.h"

namespace prz {
namespace utl {

class PRZCORE_API ZNotificationCenter : public INotificationCenter {
public:
    ZNotificationCenter() = default;
    ZNotificationCenter(const ZNotificationCenter& other) = delete;
    ZNotificationCenter& operator=(const ZNotificationCenter& other) = delete;
    virtual ~ZNotificationCenter();

    virtual bool AddObserver(const ZString& name, void* observerOwner, ZNotificationEventHandler handler) override;
    virtual bool RemoveObserver(const ZString& name, void* observerOwner) override;

    virtual bool PostNotification(const ZString& name) override;
    virtual bool PostNotification(const ZString& name, const ZDictionary& dict) override;

private:
    struct ZObserver {
        void* observerOwner;
        ZNotificationEventHandler handler;

        ZObserver(void* pObserverOwner, ZNotificationEventHandler pHandler) {
            this->observerOwner = pObserverOwner;
            this->handler = pHandler;
        }
    };

    typedef utl::ZVector<ZObserver> ZObserverList;
    typedef utl::ZUnorderedMap<utl::ZString, ZObserverList*> ZObserverListTable;
    typedef utl::ZPair<ZString, ZObserverList*> ZObserverListTablePair;

    ZObserverListTable mObservers;
};

}
}
