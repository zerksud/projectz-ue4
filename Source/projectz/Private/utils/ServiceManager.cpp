#include "projectz.h"
#include "ServiceManager.h"

namespace prz {
    namespace utils {
        ServiceManager::ServiceManager() {
        }

        ServiceManager::~ServiceManager() {
            for (ServiceMap::iterator i = mServiceMap.begin(), end = mServiceMap.end(); i != end; ++i) {
                i->second->destructor();
                delete i->second;
            }
        }
    }
}
