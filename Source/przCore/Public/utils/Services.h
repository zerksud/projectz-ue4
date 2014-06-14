#pragma once

#include "ServiceManager.h"

namespace prz {
    namespace utl {

        class PRZCORE_API ZServices {
        public:
            ZServices() = delete;
            virtual ~ZServices() = delete;

            static ZServiceManager& GetInstance();
        };

    }
}
