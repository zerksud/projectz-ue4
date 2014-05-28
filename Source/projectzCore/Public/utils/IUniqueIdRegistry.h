#pragma once

#include "utils/Registrable.h"

namespace prz {
    namespace utils {

        class IUniqueIdRegistry {
        public:
            virtual bool AssignUniqueId(ZRegistrable* object) = 0;
            virtual bool ReleaseUniqueId(ZRegistrable* object) = 0;
        };

    }
}
