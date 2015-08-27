#pragma once

#include "Utils/Registrable.h"

#include "Model/Direction.h"
#include "Model/FieldOfView.h"

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
