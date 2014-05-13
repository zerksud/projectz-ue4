#pragma once

#include <functional>

namespace prz {
    namespace utils {
        class INotificationCenter {
        public:
            typedef std::function<void(const void*)> ZNotificationEventHandler;

            virtual bool AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) = 0;
            virtual bool RemoveObserver(const std::string& name, void* observerOwner) = 0;

            virtual bool PostNotification(const std::string& name) = 0;
            virtual bool PostNotification(const std::string& name, const void* params) = 0;
        };
    }
}
