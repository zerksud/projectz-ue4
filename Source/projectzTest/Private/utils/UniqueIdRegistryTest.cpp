#include "projectzTestPrivatePCH.h"

#include "utils/UniqueIdRegistry.h"

namespace prz {
    namespace testing {

        class UniqueIdRegistryTest : public ::testing::Test {
        protected:
            static const utils::ZIdType kSomeId = 42;
        };

        class UniqueIdRegistryWithInstanceTest : public UniqueIdRegistryTest {
        protected:
            utils::ZUniqueIdRegistry mRegistry;
            utils::ZRegistrable mRegistrable;
        };

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_AssignsUniqueId) {
            EXPECT_TRUE(mRegistry.AssignUniqueId(&mRegistrable));
            ASSERT_NE(utils::ZRegistrable::kNoId, mRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_SecondRegistrationOfTheSameObjectIsNotPermitted) {
            mRegistry.AssignUniqueId(&mRegistrable);

            ASSERT_FALSE(mRegistry.AssignUniqueId(&mRegistrable));
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, AssignUniqueId_ReusesPreviouslyReleasedId) {
            mRegistry.AssignUniqueId(&mRegistrable);
            utils::ZIdType id = mRegistrable.GetId();

            mRegistry.ReleaseUniqueId(&mRegistrable);
            mRegistry.AssignUniqueId(&mRegistrable);

            ASSERT_EQ(id, mRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryTest, AssignUniqueId_FailsIfAllIdsAreUsed) {
            const utils::ZIdType minId = utils::ZRegistrable::kNoId + 1;
            const utils::ZIdType maxId = kSomeId;
            utils::ZUniqueIdRegistry registry(maxId);
            utils::ZRegistrable registrable;

            for (utils::ZIdType i = minId; i < maxId - 1; ++i) {
                registry.AssignUniqueId(&registrable);
                registrable.SetId(utils::ZRegistrable::kNoId);
            }

            EXPECT_TRUE(registry.AssignUniqueId(&registrable));
            registrable.SetId(utils::ZRegistrable::kNoId);
            ASSERT_FALSE(registry.AssignUniqueId(&registrable));
        }

        TEST_F(UniqueIdRegistryTest, AssignUniqueId_AssignsIdReleasedAfterAllIdsWereUsed) {
            const utils::ZIdType minId = utils::ZRegistrable::kNoId + 2;
            const utils::ZIdType maxId = kSomeId;
            utils::ZUniqueIdRegistry registry(maxId);
            utils::ZRegistrable firstRegistrable, anotherRegistrable;

            registry.AssignUniqueId(&firstRegistrable);

            for (utils::ZIdType i = minId; i < maxId; ++i) {
                registry.AssignUniqueId(&anotherRegistrable);
                anotherRegistrable.SetId(utils::ZRegistrable::kNoId);
            }

            utils::ZRegistrable newRegistrable;
            EXPECT_FALSE(registry.AssignUniqueId(&newRegistrable));

            EXPECT_TRUE(registry.ReleaseUniqueId(&firstRegistrable));
                        
            EXPECT_TRUE(registry.AssignUniqueId(&newRegistrable));
            ASSERT_NE(utils::ZRegistrable::kNoId, newRegistrable.GetId());
        }

        TEST_F(UniqueIdRegistryWithInstanceTest, ReleaseUniqueId_ReleasesUniqueId) {
            mRegistry.AssignUniqueId(&mRegistrable);
            EXPECT_TRUE(mRegistry.ReleaseUniqueId(&mRegistrable));

            ASSERT_EQ(utils::ZRegistrable::kNoId, mRegistrable.GetId());
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
            utils::ZIdType id = mRegistrable.GetId();

            mRegistry.ReleaseUniqueId(&mRegistrable);
            mRegistrable.SetId(id);

            ASSERT_FALSE(mRegistry.ReleaseUniqueId(&mRegistrable));
        }

    }
}
