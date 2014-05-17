#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "utils/Dictionary.h"

namespace prz {
    namespace testing {
        class DictionaryTestWithInstance : public ::testing::Test {
        protected:
            static const char* kSomeKey;
            static const char* kAnotherKey;
            static const int kSomeInt = 42;
            static const int kAnotherInt = 1337;
            static const bool kSomeBool = true;

            utils::ZDictionary dict;
        };

        const char* DictionaryTestWithInstance::kSomeKey = "some_key";
        const char* DictionaryTestWithInstance::kAnotherKey = "another_key";

        utils::ZDictionary makeDictWithValue(utils::ZDictionary dict, const char* key, int value) {
            dict[key] = value;

            return dict;
        }

        TEST_F(DictionaryTestWithInstance, IntCanBeStoredAndRetrievedByKey) {
            dict[kSomeKey] = kSomeInt;

            ASSERT_EQ(kSomeInt, dict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, BoolCanBeStoredAndRetrievedByKey) {
            dict[kSomeKey] = kSomeBool;

            ASSERT_EQ(kSomeBool, dict[kSomeKey].AsBool());
        }

        TEST_F(DictionaryTestWithInstance, DictionaryCanBeStoredAndRetrievedByKey) {
            dict[kSomeKey] = utils::ZDictionary();
            dict[kSomeKey][kAnotherKey] = kSomeInt;

            ASSERT_EQ(kSomeInt, dict[kSomeKey][kAnotherKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, CopyConstructor_CopiesDictionary) {
            dict[kSomeKey] = kSomeInt;
            utils::ZDictionary anotherDict(dict);

            ASSERT_EQ(kSomeInt, anotherDict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, CopyConstructor_CreatesIndependentCopyOfDictionary) {
            dict[kSomeKey] = kSomeInt;
            utils::ZDictionary anotherDict = dict;
            dict[kSomeKey] = kSomeBool;

            ASSERT_EQ(kSomeInt, anotherDict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, MoveConstructor_MovesDictionary) {
            dict = makeDictWithValue(utils::ZDictionary(), kSomeKey, kSomeInt);

            ASSERT_EQ(kSomeInt, dict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, AssignmentOperator_CopiesDictionary) {
            dict[kSomeKey] = kSomeInt;
            utils::ZDictionary anotherDict;
            anotherDict = dict;

            ASSERT_EQ(kSomeInt, anotherDict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, AssignmentOperator_CreatesIndependentCopyOfDictionary) {
            dict[kSomeKey] = kSomeInt;
            utils::ZDictionary anotherDict;
            anotherDict = dict;
            dict[kSomeKey] = kSomeBool;

            ASSERT_EQ(kSomeInt, anotherDict[kSomeKey].AsInt());
        }

        TEST_F(DictionaryTestWithInstance, ArraySubscriptOperator_RetrievesDictByExactKey) {
            dict[kSomeKey] = kSomeInt;
            dict[kAnotherKey] = kAnotherInt;

            ASSERT_EQ(kSomeInt, dict[kSomeKey].AsInt());
            ASSERT_EQ(kAnotherInt, dict[kAnotherKey].AsInt());
        }
    }
}
