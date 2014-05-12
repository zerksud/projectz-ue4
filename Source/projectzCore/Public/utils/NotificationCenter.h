#pragma once

#include <unordered_map>
#include <vector>

#include "utils/INotificationCenter.h"

namespace prz {
    namespace utils {
        class ZNotificationCenter : public INotificationCenter {
        public:
            ZNotificationCenter();
            ZNotificationCenter(const ZNotificationCenter& other);
            ZNotificationCenter& operator=(const ZNotificationCenter& other);
            virtual ~ZNotificationCenter();

            virtual void AddObserver(const std::string& name, void* observerOwner, ZObserverHandle observerHandle) override;
            virtual void RemoveObserver(const std::string& name, void* observerOwner) override;

            virtual void PostNotification(const std::string& name) override;
            virtual void PostNotification(const std::string& name, void* params) override;

        private:
            struct ZObserver {
                void* observerOwner;
                ZObserverHandle handle;

                ZObserver(void* observerPtr, ZObserverHandle handlePtr) {
                    observerOwner = observerPtr;
                    handle = handlePtr;
                }
            };

            typedef std::vector<ZObserver> ZObserverList;
            typedef std::unordered_map<std::string, ZObserverList*> ZObserverListTable;
            typedef std::pair<std::string, ZObserverList*> ZObserverListTablePair;

            ZObserverListTable mObservers;
        };
    }
}
