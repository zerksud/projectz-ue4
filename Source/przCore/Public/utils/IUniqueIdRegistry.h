#pragma once

#include "utils/Registrable.h"

namespace prz {
    namespace utl {

        class IUniqueIdRegistry {
        public:
            virtual ~IUniqueIdRegistry() = default;

            virtual bool AssignUniqueId(ZRegistrable* object) = 0;
            virtual bool ReleaseUniqueId(ZRegistrable* object) = 0;

            virtual unsigned int GetAssignedUniqueIdCount() const = 0;
        };

    }
}
