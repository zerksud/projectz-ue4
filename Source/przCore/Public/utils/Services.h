#pragma once

#include "utils/ServiceManager.h"

#define REGISTER_SERVICE(typeName, instance) \
    (prz::utl::ZServices::GetInstance().Register< ::typeName >(instance, #typeName))

#define GET_SERVICE(typeName) \
    (prz::utl::ZServices::GetInstance().Get< ::typeName >(#typeName))

#define UNREGISTER_SERVICE(typeName) \
    (prz::utl::ZServices::GetInstance().Unregister< ::typeName >(#typeName))

namespace prz {
    namespace utl {

        class PRZCORE_API ZServices {
        public:
            ZServices() = delete;
            virtual ~ZServices() = delete;

            static ZServiceManager& GetInstance();
        };

    }
}
