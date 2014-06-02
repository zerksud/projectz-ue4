#pragma once

#include "model/Direction.h"
#include "utils/Registrable.h"

namespace prz {
    namespace mdl {

        class ZMonster : public utl::ZRegistrable {
        public:
            static ZMonster CreateMonster();

            ZMonster(const ZMonster& other) = default;
            ZMonster& operator=(const ZMonster& other) = default;
            virtual ~ZMonster() = default;

            ZDirection& GetDirection();

        private:
            ZMonster() = default;
            ZDirection mDirection;
        };

    }
}
