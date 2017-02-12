// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "przCorePCH.h"
#include "Utils/ZUniqueIdRegistry.h"

#include "Utils/LOG.h"

namespace prz {
namespace utl {

ZUniqueIdRegistry::ZUniqueIdRegistry(ZIdType maxId) :
mMaxId(maxId),
    mNextFreeId(ZRegistrable::kNoId + 1) {
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

}
}
