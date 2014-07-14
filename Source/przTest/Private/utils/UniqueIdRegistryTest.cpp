#include "przTestPrivatePCH.h"

#include "utils/UniqueIdRegistry.h"

namespace prz {
    namespace testing {

        class UniqueIdRegistryTest : public ::testing::Test {
        protected:
            static const utl::ZIdType kSomeId = 42;
        };

        class UniqueIdRegistryWithInstanceTest : public UniqueIdRegistryTest {
        protected:
            utl::ZUniqueIdRegistry mRegistry;
            utl::ZRegistrable mRegistrable;
        };

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_AssignsUniqueId) {
            EXPECT_TRUE(mRegistry.AssignUniqueId(&mRegistrable));
            ASSERT_NE(utl::ZRegistrable::kNoId, mRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_SecondRegistrationOfTheSameObjectIsNotPermitted) {
            mRegistry.AssignUniqueId(&mRegistrable);

            ASSERT_FALSE(mRegistry.AssignUniqueId(&mRegistrable));
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_ReusesPreviouslyReleasedId) {
            mRegistry.AssignUniqueId(&mRegistrable);
            utl::ZIdType id = mRegistrable.GetId();

            mRegistry.ReleaseUniqueId(&mRegistrable);
            mRegistry.AssignUniqueId(&mRegistrable);

            ASSERT_EQ(id, mRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryTest, AssignUniqueId_FailsIfAllIdsAreUsed) {
            const utl::ZIdType minId = utl::ZRegistrable::kNoId + 1;
            const utl::ZIdType maxId = kSomeId;
            utl::ZUniqueIdRegistry registry(maxId);
            utl::ZRegistrable registrable;

            for (utl::ZIdType i = minId; i < maxId - 1; ++i) {
                registry.AssignUniqueId(&registrable);
                registrable.SetId(utl::ZRegistrable::kNoId);
            }

            EXPECT_TRUE(registry.AssignUniqueId(&registrable));
            registrable.SetId(utl::ZRegistrable::kNoId);
            ASSERT_FALSE(registry.AssignUniqueId(&registrable));
        }

        TEST_F(UniqueIdRegistryTest, AssignUniqueId_AssignsIdReleasedAfterAllIdsWereUsed) {
            const utl::ZIdType minId = utl::ZRegistrable::kNoId + 2;
            const utl::ZIdType maxId = kSomeId;
            utl::ZUniqueIdRegistry registry(maxId);
            utl::ZRegistrable firstRegistrable, anotherRegistrable;

            registry.AssignUniqueId(&firstRegistrable);

            for (utl::ZIdType i = minId; i < maxId; ++i) {
                registry.AssignUniqueId(&anotherRegistrable);
                anotherRegistrable.SetId(utl::ZRegistrable::kNoId);
            }

            utl::ZRegistrable newRegistrable;
            EXPECT_FALSE(registry.AssignUniqueId(&newRegistrable));

            EXPECT_TRUE(registry.ReleaseUniqueId(&firstRegistrable));
                        
            EXPECT_TRUE(registry.AssignUniqueId(&newRegistrable));
            ASSERT_NE(utl::ZRegistrable::kNoId, newRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, ReleaseUniqueId_ReleasesUniqueId) {
            mRegistry.AssignUniqueId(&mRegistrable);
            EXPECT_TRUE(mRegistry.ReleaseUniqueId(&mRegistrable));

            ASSERT_EQ(utl::ZRegistrable::kNoId, mRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, ReleaseUniqueId_ReleasingOfNoIdIsNotPermitted) {
            ASSERT_FALSE(mRegistry.ReleaseUniqueId(&mRegistrable));
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, ReleaseUniqueId_ReleasingOfNotAssignedIdIsNotPermitted) {
            mRegistrable.SetId(kSomeId);

            ASSERT_FALSE(mRegistry.ReleaseUniqueId(&mRegistrable));
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, ReleaseUniqueId_ReleasingOfAlreadyReleasedIdIsNotPermitted) {
            mRegistry.AssignUniqueId(&mRegistrable);
            utl::ZIdType id = mRegistrable.GetId();

            mRegistry.ReleaseUniqueId(&mRegistrable);
            mRegistrable.SetId(id);

            ASSERT_FALSE(mRegistry.ReleaseUniqueId(&mRegistrable));
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, GetAssignedUniqueIdCount_ReturnsZeroInitially) {
            ASSERT_EQ(0, mRegistry.GetAssignedUniqueIdCount());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, GetAssignedUniqueIdCount_ReturnsExactNumberOfRegisteredIds) {
            mRegistry.AssignUniqueId(&mRegistrable);
            EXPECT_EQ(1, mRegistry.GetAssignedUniqueIdCount());
            mRegistry.ReleaseUniqueId(&mRegistrable);
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, GetAssignedUniqueIdCount_ReturnsExactNumberOfRegisteredIdsWhenSomeWereAlreadyReleased) {
            utl::ZRegistrable anotherRegistrable;
            mRegistry.AssignUniqueId(&mRegistrable);
            mRegistry.AssignUniqueId(&anotherRegistrable);

            mRegistry.ReleaseUniqueId(&mRegistrable);

            EXPECT_EQ(1, mRegistry.GetAssignedUniqueIdCount());
            mRegistry.ReleaseUniqueId(&anotherRegistrable);
        }

    }
}
