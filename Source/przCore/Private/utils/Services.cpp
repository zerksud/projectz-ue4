#include "przCorePrivatePCH.h"
#include "utils/Services.h"

namespace prz {
    namespace utl {

        ZServiceManager& ZServices::GetInstance() {
            static ZServiceManager instance;
            return instance;
        }

    }
}
