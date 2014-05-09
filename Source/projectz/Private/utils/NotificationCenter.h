#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace prz {
    namespace utils {

        typedef std::function<void(void*)> ObserverHandle;

        class NotificationCenter {
        public:
            static NotificationCenter& GetInstance() {
                static NotificationCenter instance;
                return instance;
            };

            void AddObserver(const std::string& name, void* observerOwner, ObserverHandle observerHandle);
            void RemoveObserver(const std::string& name, void* observerOwner);

            void PostNotification(const std::string& name);
            void PostNotification(const std::string& name, void* params);

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
