#include "projectz.h"

#include <algorithm>

#include "NotificationCenter.h"

namespace prz {
    namespace utils {

        NotificationCenter::NotificationCenter() {
        }

        NotificationCenter::~NotificationCenter() {
            for (const ObserverListTablePair& e : mObservers) {
                ObserverList* list = e.second;
                delete list;
            }
        }

        void NotificationCenter::AddObserver(const std::string& name, void* observerOwner, ObserverHandle observerHandle) {
            ObserverList* list;
            ObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                list = new ObserverList();
                mObservers[name] = list;
            } else {
                list = pos->second;
            }

            ObserverList::const_iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const Observer& handle) {
                return (observerOwner == handle.observerOwner);
            });

            if (listPos != list->end()) {
                //LOGE("Can't add already added observer for %s", name.c_str());
                return;
            }

            list->push_back(Observer(observerOwner, observerHandle));
        }

        void NotificationCenter::RemoveObserver(const std::string& name, void* observerOwner) {
            ObserverList* list;
            ObserverListTable::iterator pos = mObservers.find(name);
            if (pos == mObservers.end()) {
                //LOGE("Can't remove not added observer for %s", name.c_str());
                return;
            } else {
                list = pos->second;
            }

            ObserverList::iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const Observer& handle) {
                return (observerOwner == handle.observerOwner);
            });

            if (listPos == list->end()) {
                //LOGE("Can't remove not added observer for %s", name.c_str());
                return;
            }

            list->erase(listPos);
        }

        void NotificationCenter::PostNotification(const std::string& name) {
            PostNotification(name, nullptr);
        }

        void NotificationCenter::PostNotification(const std::string& name, void* params) {
            ObserverListTable::iterator pos = mObservers.find(name);
            if (pos != mObservers.end()) {
                ObserverList* list = pos->second;
                for (Observer& observer : *list) {
                    observer.handle(params);
                }
            }
        }
    }
}
