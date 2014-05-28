#include "projectzCorePrivatePCH.h"

#include "utils/Registrable.h"

namespace prz {
    namespace utils {

        ZRegistrable::ZRegistrable() : mId(kNoId) {
        }

        ZRegistrable::~ZRegistrable() {
        }

        ZIdType ZRegistrable::GetId() const {
            return mId;
        }

        void ZRegistrable::SetId(ZIdType id) {
            mId = id;
        }

        bool ZRegistrable::IsRegistered() const {
            return mId != kNoId;
        }

    }
}
