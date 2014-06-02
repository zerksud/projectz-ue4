#pragma once

#include "ServiceManager.h"

namespace prz {
    namespace utl {

        class ZServices {
        public:
            ZServices() = delete;
            virtual ~ZServices() = delete;

            static ZServiceManager& GetInstance() {
                static ZServiceManager instance;
                return instance;
            }
        };

    }
}
