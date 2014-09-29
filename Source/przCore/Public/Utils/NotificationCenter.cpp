#include "przCorePCH.h"
#include "NotificationCenter.h"

#include <algorithm>

#include "Utils/LOG_ANSI.h"

namespace prz {
    namespace utl {

        ZNotificationCenter::ZNotificationCenter() {
        }

        ZNotificationCenter::~ZNotificationCenter() {
            for (const ZObserverListTablePair& e : mObservers) {
                ZObserverList* list = e.second;
                delete list;
            }
        }

        bool ZNotificationCenter::AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) {
            if (name.empty()) {
                LOGE("Can't add observer for empty notification");
                return false;
            }

            if (!observerOwner) {
                LOGE("Can't add nullptr observer for notification %s", name.c_str());
                return false;
            }

            if (!handler) {
                LOGE("Can't add observer with handler without callable target for notification %s", name.c_str());
                return false;
            }

            ZObserverList* list;
            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                list = new ZObserverList();
                mObservers[name] = list;
            } else {
                list = pos->second;
            }

            ZObserverList::const_iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const ZObserver& observer) {
                return (observerOwner == observer.observerOwner);
            });

            if (listPos != list->end()) {
                LOGE("Can't add already added observer for %s", name.c_str());
                return false;
            }

            list->push_back(ZObserver(observerOwner, handler));
            return true;
        }

        bool ZNotificationCenter::RemoveObserver(const std::string& name, void* observerOwner) {
            ZObserverList* list;
            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                LOGE("Can't remove not added observer for %s", name.c_str());
                return false;
            } else {
                list = pos->second;
            }

            ZObserverList::iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const ZObserver& observer) {
                return (observerOwner == observer.observerOwner);
            });

            if (listPos == list->end()) {
                LOGE("Can't remove not added observer for %s", name.c_str());
                return false;
            }

            list->erase(listPos);
            return true;
        }

        bool ZNotificationCenter::PostNotification(const std::string& name) {
            return PostNotification(name, ZDictionary());
        }

        bool ZNotificationCenter::PostNotification(const std::string& name, const ZDictionary& dict) {
            if (name.empty()) {
                LOGE("Can't post empty notification");
                return false;
            }

            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos != mObservers.end()) {
                ZObserverList* list = pos->second;
                for (ZObserver& observer : *list) {
                    observer.handler(dict);
                }
            }

            return true;
        }
    }
}
