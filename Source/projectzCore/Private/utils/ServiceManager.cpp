#include "projectzCorePrivatePCH.h"
#include "ServiceManager.h"

namespace prz {
    namespace utils {
        ZServiceManager::ZServiceManager() {
        }

        ZServiceManager::~ZServiceManager() {
            for (auto& kv : mServiceMap) {
                kv.second->destructor();
                delete kv.second;
            }
        }
    }
}
