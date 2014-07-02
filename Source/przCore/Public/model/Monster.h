#pragma once

#include "model/Direction.h"
#include "utils/Registrable.h"

namespace prz {
    namespace mdl {

        class ZMonster : public utl::ZRegistrable {
        public:
            static ZMonster* CreateMonster();

            ZMonster(const ZMonster& other) = delete;
            ZMonster& operator=(const ZMonster& other) = delete;
            virtual ~ZMonster() = default;

            ZDirection& GetDirection();

        private:
            ZMonster() = default;
            ZDirection mDirection;
        };

    }
}
