#include "projectzTestPrivatePCH.h"

#include "utils/Registrable.h"

namespace prz {
    namespace testing {

        class RegistrableTest : public ::testing::Test {
            protected:
                static const utl::ZIdType kSomeId = 42;

                utl::ZRegistrable mRegistrable;
        };

        TEST_F(RegistrableTest, Constructor_SetsNoId) {
            ASSERT_EQ(utl::ZRegistrable::kNoId, mRegistrable.GetId());
        }

        TEST_F(RegistrableTest, SetId_StoresPassedId) {
            mRegistrable.SetId(kSomeId);
            ASSERT_EQ(kSomeId, mRegistrable.GetId());
        }

        TEST_F(RegistrableTest, IsRegistered_ReturnsFalseForNoId) {
            ASSERT_FALSE(mRegistrable.IsRegistered());
        }

        TEST_F(RegistrableTest, IsRegistered_ReturnsTrueForExplicitlySetId) {
            mRegistrable.SetId(kSomeId);
            ASSERT_TRUE(mRegistrable.IsRegistered());
        }

    }
}
