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

#pragma once

#include "Utils/ZRegistrable.h"

#include "Model/ZDirection.h"
#include "Model/ZFieldOfView.h"

namespace prz {
namespace mdl {

class ZMonster : public utl::ZRegistrable {
public:
    static ZMonster* CreateMonster();
    static void DestroyMonster(ZMonster** monster);

    ZMonster(const ZMonster& other) = delete;
    ZMonster& operator=(const ZMonster& other) = delete;

    ZDirection& GetDirection();

    unsigned int GetViewDistance() const;

    void UpdateFieldOfView(ZFieldOfView&& newFieldOfView);
    const ZFieldOfView& GetFieldOfView() const;
    
private:
    ZMonster() = default;
    virtual ~ZMonster() = default;
    ZDirection mDirection;
    ZFieldOfView mFieldOfView;
};

}
}
