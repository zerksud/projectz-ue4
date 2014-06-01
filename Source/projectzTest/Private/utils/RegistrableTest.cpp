#include "projectzTestPrivatePCH.h"

#include "utils/Registrable.h"

#include "utils/IUniqueIdRegistry.h"
#include "utils/Services.h"

namespace prz {
    namespace testing {

        class RegistrableTest : public ::testing::Test {
        protected:
            static const utl::ZIdType kSomeId = 42;
        };

        class RegistrableWithInstanceTest : public RegistrableTest {
        protected:
            utl::ZRegistrable mRegistrable;
        };

        TEST_F(RegistrableWithInstanceTest, Constructor_SetsNoId) {
            ASSERT_EQ(utl::ZRegistrable::kNoId, mRegistrable.GetId());
        }

        TEST_F(RegistrableWithInstanceTest, SetId_StoresPassedId) {
            mRegistrable.SetId(kSomeId);
            ASSERT_EQ(kSomeId, mRegistrable.GetId());
        }

        TEST_F(RegistrableWithInstanceTest, IsRegistered_ReturnsFalseForNoId) {
            ASSERT_FALSE(mRegistrable.IsRegistered());
        }

        TEST_F(RegistrableWithInstanceTest, IsRegistered_ReturnsTrueForExplicitlySetId) {
            mRegistrable.SetId(kSomeId);
            ASSERT_TRUE(mRegistrable.IsRegistered());
        }

    }
}
