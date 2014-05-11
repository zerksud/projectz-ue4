#pragma once

#include "ServiceManager.h"

namespace prz {
    namespace utils {
        class ZServices {
        public:
            static ZServiceManager& GetInstance() {
                static ZServiceManager instance;
                return instance;
            }

        private:
            ZServices();
            virtual ~ZServices();
        };
    }
}
