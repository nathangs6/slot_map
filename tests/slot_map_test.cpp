#include <slot_map.hpp>
#include <gtest/gtest.h>

TEST(ConstructorTest, Simple) {
    slot_map<int> sm;
    ASSERT_EQ(sm.get_capacity(), 0);
    ASSERT_EQ(sm.get_size(), 0);
    int a = 1;
    sm.insert(a);
    ASSERT_EQ(sm.get_capacity(), 1);
    ASSERT_EQ(sm.get_size(), 1);
    int b = 2;
    sm.insert(b);
    ASSERT_EQ(sm.get_capacity(), 2);
    ASSERT_EQ(sm.get_size(), 2);
    int c = 3;
    sm.insert(c);
    ASSERT_EQ(sm.get_capacity(), 4);
    ASSERT_EQ(sm.get_size(), 3);
}

TEST(InsertGet, Simple) {
    slot_map<int> sm;
    int a = 1;
    key object1 = sm.insert(a);
    ASSERT_EQ(object1.ind, 0);
    ASSERT_EQ(object1.gen, 0);
    ASSERT_EQ(sm[object1], 1);
}

TEST(InsertGetDelete, Simple) {
    slot_map<int> sm;
    int a = 1;
    key object1 = sm.insert(a);
    ASSERT_EQ(object1.ind, 0);
    ASSERT_EQ(object1.gen, 0);
    ASSERT_EQ(sm[object1], 1);
    sm.remove(object1);
    EXPECT_THROW(sm[object1], std::invalid_argument);
}
