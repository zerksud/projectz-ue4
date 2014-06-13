#pragma once

#include "Core.h"

namespace prz {
    namespace mdl {

        class PRZCORE_API ZPlayerNavigation {
        public:
            ZPlayerNavigation() = delete;
            ~ZPlayerNavigation() = delete;

            static const char* kTurnLeftNotification;
            static const char* kMoveForwardNotification;
            static const char* kTurnRightNotification;
            static const char* kStrafeLeftNotification;
            static const char* kMoveBackwardNotification;
            static const char* kStrafeRightNotification;
        };

    }
}
