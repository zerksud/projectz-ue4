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
#include "Model/ZMonster.h"

#include "Utils/ZServices.h"
#include "Utils/IUniqueIdRegistry.h"
#include "Utils/LOG.h"

namespace prz {
namespace mdl {

ZMonster* ZMonster::CreateMonster() {
    ZMonster* monster = new ZMonster();

    utl::IUniqueIdRegistry* registry = GET_SERVICE(prz::utl::IUniqueIdRegistry);
    if (registry) {
        registry->AssignUniqueId(monster);
    }

    return monster;
}

void ZMonster::DestroyMonster(ZMonster** monster) {
    if (monster == nullptr) {
        LOGE("Can't destroy nullptr as ZMonster**");
        return;
    }

    if (*monster != nullptr && (*monster)->IsRegistered()) {
        utl::IUniqueIdRegistry* registry = GET_SERVICE(prz::utl::IUniqueIdRegistry);
        if (registry) {
            registry->ReleaseUniqueId(*monster);
        }
    }

    delete *monster;
    *monster = nullptr;
}

ZDirection& ZMonster::GetDirection() {
    return mDirection;
}

unsigned int ZMonster::GetViewDistance() const {
    return 8;
}

void ZMonster::UpdateFieldOfView(ZFieldOfView&& newFieldOfView) {
    mFieldOfView = newFieldOfView;
}

const ZFieldOfView& ZMonster::GetFieldOfView() const {
    return mFieldOfView;
}

}
}
