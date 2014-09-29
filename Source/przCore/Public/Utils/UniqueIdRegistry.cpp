#include "przCorePCH.h"

#include "Utils/UniqueIdRegistry.h"

#include "Utils/LOG_ANSI.h"

namespace prz {
    namespace utl {

        ZUniqueIdRegistry::ZUniqueIdRegistry(ZIdType maxId) :
            mMaxId(maxId),
            mNextFreeId(ZRegistrable::kNoId + 1) {
        }

        ZUniqueIdRegistry::~ZUniqueIdRegistry() {
        }

        bool ZUniqueIdRegistry::AssignUniqueId(ZRegistrable* object) {
            if (object->GetId() != ZRegistrable::kNoId) {
                LOGE("Can't register object with id = %d twice", object->GetId());
                return false;
            }

            if (!mReleasedIds.empty()) {
                ZIdType id = *mReleasedIds.begin();
                mReleasedIds.erase(id);
                object->SetId(id);
            } else if (mNextFreeId >= mMaxId) {
                LOGE("Can't create new Id cause all ids are used");
                return false;
            } else {
                object->SetId(mNextFreeId++);
            }

            return true;
        }

        bool ZUniqueIdRegistry::ReleaseUniqueId(ZRegistrable* object) {
            ZIdType id = object->GetId();

            if (id == ZRegistrable::kNoId) {
                LOGE("Can't relese empty id");
                return false;
            }

            if (id >= mNextFreeId) {
                LOGE("Can't release not assigned id = %d", id);
                return false;
            }

            if (mReleasedIds.count(id) > 0) {
                LOGE("Can't release already released id = %d", id);
                return false;
            }

            object->SetId(ZRegistrable::kNoId);
            mReleasedIds.insert(id);

            return true;
        }

        unsigned int ZUniqueIdRegistry::GetAssignedUniqueIdCount() const {
            return mNextFreeId - ZRegistrable::kNoId - 1 - mReleasedIds.size();
        }

    }
}
