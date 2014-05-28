#pragma once

namespace prz {
    namespace utils {

        typedef unsigned int ZIdType;

        class ZRegistrable {
        public:
            static const ZIdType kNoId = 0;

            ZRegistrable();
            ZRegistrable(const ZRegistrable& other) = delete;
            virtual ~ZRegistrable();

            ZIdType GetId() const;
            void SetId(ZIdType id);
            bool IsRegistered() const;

        private:
            ZIdType mId;
        };
    }
}