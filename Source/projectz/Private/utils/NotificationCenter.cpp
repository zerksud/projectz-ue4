#include "projectz.h"

#include <algorithm>

#include "NotificationCenter.h"

namespace prz {
    namespace utils {

        ZNotificationCenter::ZNotificationCenter() {
        }

        ZNotificationCenter::~ZNotificationCenter() {
            for (const ZObserverListTablePair& e : mObservers) {
                ZObserverList* list = e.second;
                delete list;
            }
        }

        void ZNotificationCenter::AddObserver(const std::string& name, void* observerOwner, ZObserverHandle observerHandle) {
            ZObserverList* list;
            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                list = new ZObserverList();
                mObservers[name] = list;
            } else {
                list = pos->second;
            }

            ZObserverList::const_iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const ZObserver& handle) {
                return (observerOwner == handle.observerOwner);
            });

            if (listPos != list->end()) {
                //LOGE("Can't add already added observer for %s", name.c_str());
                return;
            }

            list->push_back(ZObserver(observerOwner, observerHandle));
        }

        void ZNotificationCenter::RemoveObserver(const std::string& name, void* observerOwner) {
            ZObserverList* list;
            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                //LOGE("Can't remove not added observer for %s", name.c_str());
                return;
            } else {
                list = pos->second;
            }

            ZObserverList::iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const ZObserver& handle) {
                return (observerOwner == handle.observerOwner);
            });

            if (listPos == list->end()) {
                //LOGE("Can't remove not added observer for %s", name.c_str());
                return;
            }

            list->erase(listPos);
        }

        void ZNotificationCenter::PostNotification(const std::string& name) {
            PostNotification(name, nullptr);
        }

        void ZNotificationCenter::PostNotification(const std::string& name, void* params) {
            ZObserverListTable::iterator pos = mObservers.find(name);
            if (pos != mObservers.end()) {
                ZObserverList* list = pos->second;
                for (ZObserver& observer : *list) {
                    observer.handle(params);
                }
            }
        }
    }
}
