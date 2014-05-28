#pragma once

#include "model/Direction.h"

namespace prz {
    namespace mdl {
        class ZMonster {
        public:
            ZMonster();
            virtual ~ZMonster();

            ZDirection& GetDirection();

        private:
            ZDirection mDirection;
        };
    }
}
