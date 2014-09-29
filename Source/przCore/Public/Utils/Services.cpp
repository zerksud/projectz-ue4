#include "przCorePCH.h"
#include "Utils/Services.h"

namespace prz {
    namespace utl {

        ZServiceManager& ZServices::GetInstance() {
            static ZServiceManager instance;
            return instance;
        }

    }
}
