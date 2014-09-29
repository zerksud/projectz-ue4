#include "przCorePCH.h"
#include "ServiceManager.h"

namespace prz {
namespace utl {

ZServiceManager::ZServiceManager() {
}

ZServiceManager::~ZServiceManager() {
    for (auto i = mServiceRegisterOrder.rbegin(); i != mServiceRegisterOrder.rend(); ++i) {
        delete *i;
    }
}

}
}
