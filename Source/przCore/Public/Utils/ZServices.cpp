#include "przCorePCH.h"
#include "Utils/ZServices.h"

namespace prz {
namespace utl {

ZServiceManager& ZServices::GetInstance() {
    static ZServiceManager instance;
    return instance;
}

}
}
