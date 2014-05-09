#include "projectz.h"
#include "ServiceManager.h"

namespace prz {
    namespace utils {
        ServiceManager::ServiceManager() {
        }

        ServiceManager::~ServiceManager() {
            for (auto& kv : mServiceMap) {
                kv.second->destructor();
                delete kv.second;
            }
        }
    }
}
