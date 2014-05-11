#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace prz {
    namespace utils {

        typedef std::function<void(void*)> ZObserverHandle;

        class ZNotificationCenter {
        public:
            ZNotificationCenter();
            ZNotificationCenter(const ZNotificationCenter& other);
            ZNotificationCenter& operator=(const ZNotificationCenter& other);
            virtual ~ZNotificationCenter();

            void AddObserver(const std::string& name, void* observerOwner, ZObserverHandle observerHandle);
            void RemoveObserver(const std::string& name, void* observerOwner);

            void PostNotification(const std::string& name);
            void PostNotification(const std::string& name, void* params);

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
