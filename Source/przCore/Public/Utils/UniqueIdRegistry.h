#pragma once

#include "Platform.h"

#include "Utils/StandardLibrary/Limits.h"
#include "Utils/StandardLibrary/UnorderedSet.h"
#include "Utils/IUniqueIdRegistry.h"

namespace prz {
namespace utl {

class PRZCORE_API ZUniqueIdRegistry : public IUniqueIdRegistry {
public:
    ZUniqueIdRegistry(ZIdType maxId = utl::ZNumericLimits<ZIdType>::max());
    ZUniqueIdRegistry(const ZUniqueIdRegistry& other) = delete;
    ZUniqueIdRegistry& operator=(const ZUniqueIdRegistry& other) = delete;
    virtual ~ZUniqueIdRegistry() = default;

    bool AssignUniqueId(ZRegistrable* object) override;
    bool ReleaseUniqueId(ZRegistrable* object) override;

    unsigned int GetAssignedUniqueIdCount() const override;

private:
    typedef utl::ZUnorderedSet<ZIdType> ZReleasedIdSet;

    ZIdType mMaxId;
    ZIdType mNextFreeId;
    ZReleasedIdSet mReleasedIds;
};

}
}
