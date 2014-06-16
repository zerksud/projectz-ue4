#pragma once

#include <limits>
#include <unordered_set>

#include "Core.h"

#include "utils/IUniqueIdRegistry.h"

namespace prz {
    namespace utl {

        class PRZCORE_API ZUniqueIdRegistry : public IUniqueIdRegistry {
        public:
            ZUniqueIdRegistry(ZIdType maxId = std::numeric_limits<ZIdType>::max());
            ZUniqueIdRegistry(const ZUniqueIdRegistry& other) = delete;
            ZUniqueIdRegistry& operator=(const ZUniqueIdRegistry& other) = delete;
            virtual ~ZUniqueIdRegistry();

            bool AssignUniqueId(ZRegistrable* object) override;
            bool ReleaseUniqueId(ZRegistrable* object) override;

        private:
            typedef std::unordered_set<ZIdType> ZReleasedIdSet;

            ZIdType mMaxId;
            ZIdType mNextFreeId;
            ZReleasedIdSet mReleasedIds;
        };

    }
}
