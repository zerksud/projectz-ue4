#include "przCorePCH.h"
#include "ZServiceManager.h"

namespace prz {
namespace utl {

ZServiceManager::~ZServiceManager() {
    for (auto i = mServiceRegisterOrder.rbegin(); i != mServiceRegisterOrder.rend(); ++i) {
        delete *i;
    }
}

}
}
