#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace projectz {
    namespace utils {

        typedef std::function<void(void*)> ObserverHandle;

        class NotificationCenter {
        public:
            static NotificationCenter& getInstance() {
                static NotificationCenter instance;
                return instance;
            };

            void addObserver(const std::string& name, void* observerOwner, ObserverHandle observerHandle);
            void removeObserver(const std::string& name, void* observerOwner);

            void postNotification(const std::string& name);
            void postNotification(const std::string& name, void* params);

        private:
            NotificationCenter();
            NotificationCenter(const NotificationCenter& other);
            NotificationCenter& operator=(const NotificationCenter& other);
            virtual ~NotificationCenter();

            struct Observer {
                void* observerOwner;
                ObserverHandle handle;

                Observer(void* observerPtr, ObserverHandle handlePtr) {
                    observerOwner = observerPtr;
                    handle = handlePtr;
                }
            };

            typedef std::vector<Observer> ObserverList;
            typedef std::unordered_map<std::string, ObserverList*> ObserverListTable;
            typedef std::pair<std::string, ObserverList*> ObserverListTablePair;

            ObserverListTable mObservers;
        };
    }
}
