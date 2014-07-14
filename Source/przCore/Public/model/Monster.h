#pragma once

#include "model/Direction.h"
#include "utils/Registrable.h"

namespace prz {
    namespace mdl {

        class ZMonster : public utl::ZRegistrable {
        public:
            static ZMonster* CreateMonster();
            static void DestroyMonster(ZMonster** monster);

            ZMonster(const ZMonster& other) = delete;
            ZMonster& operator=(const ZMonster& other) = delete;

            ZDirection& GetDirection();

        private:
            ZMonster() = default;
            virtual ~ZMonster() = default;
            ZDirection mDirection;
        };

    }
}
