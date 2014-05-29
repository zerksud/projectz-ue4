#include "projectzCorePrivatePCH.h"
#include "ServiceManager.h"

namespace prz {
    namespace utl {
        ZServiceManager::ZServiceManager() {
        }

        ZServiceManager::~ZServiceManager() {
            for (auto& kv : mServiceMap) {
                delete kv.second;
            }
        }
    }
}
