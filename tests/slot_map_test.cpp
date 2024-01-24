#include <slot_map.hpp>
#include <gtest/gtest.h>
#include <iostream>

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

TEST(InsertGet, Many) {
    slot_map<int> sm;
    key o1 = sm.insert(1);
    ASSERT_EQ(o1.ind, 0);
    ASSERT_EQ(o1.gen, 0);
    ASSERT_EQ(sm[o1], 1);
    key o2 = sm.insert(2);
    ASSERT_EQ(o2.ind, 1);
    ASSERT_EQ(o2.gen, 0);
    ASSERT_EQ(sm[o2], 2);
    key o3 = sm.insert(3);
    ASSERT_EQ(o3.ind, 2);
    ASSERT_EQ(o3.gen, 0);
    ASSERT_EQ(sm[o3], 3);
    key o4 = sm.insert(4);
    ASSERT_EQ(o4.ind, 3);
    ASSERT_EQ(o4.gen, 0);
    ASSERT_EQ(sm[o4], 4);
    key* indices = sm.get_indices();
    int* data = sm.get_data();
    size_t* erase = sm.get_erase();
    ASSERT_EQ(indices[0].ind, 0);
    ASSERT_EQ(indices[1].ind, 1);
    ASSERT_EQ(data[0], 1);
    ASSERT_EQ(data[1], 2);
    ASSERT_EQ(erase[0], 0);
    ASSERT_EQ(erase[1], 1);
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

TEST(InsertGetDelete, ShiftedDataAfterDelete) {
    slot_map<int> sm;
    key o1 = sm.insert(1);
    key o2 = sm.insert(2);
    sm.remove(o1);
    key* indices = sm.get_indices();
    int* data = sm.get_data();
    size_t* erase = sm.get_erase();
    ASSERT_EQ(indices[1].ind, 0);
    ASSERT_EQ(data[0], 2);
    ASSERT_EQ(erase[0], 1);
    ASSERT_EQ(sm[o2], 2);
}

TEST(InsertDeleteCycle, Simple) {
    slot_map<int> sm;
    key o1 = sm.insert(1);
    key o2 = sm.insert(2);
    key o3 = sm.insert(3);
    sm.remove(o2);
    key o4 = sm.insert(4);
    key o5 = sm.insert(5);
    key o6 = sm.insert(6);
    ASSERT_EQ(sm[o1], 1);
    ASSERT_EQ(sm[o3], 3);
    ASSERT_EQ(sm[o4], 4);
    ASSERT_EQ(sm[o5], 5);
    ASSERT_EQ(sm[o6], 6);
}

TEST(Iterator, Simple) {
    slot_map<int> sm;
    key keys[3];
    keys[0] = sm.insert(1);
    keys[1] = sm.insert(2);
    keys[2] = sm.insert(3);
    int i = 0;
    for (slot_map_iterator<slot_map<int>> it = sm.begin(); it != sm.end(); it++) {
        ASSERT_EQ(sm[keys[i]], sm[*it]);
        i++;
    }
}

TEST(Iterator, WithRemoval) {
    slot_map<int> sm;
    key keys[6];
    keys[0] = sm.insert(1);
    keys[1] = sm.insert(2);
    keys[2] = sm.insert(3);
    keys[3] = sm.insert(4);
    keys[4] = sm.insert(5);
    keys[5] = sm.insert(6);
    sm.remove(keys[2]);
    keys[2] = keys[5];
    int i = 0;
    key k;
    for (slot_map_iterator<slot_map<int>> it = sm.begin(); it != sm.end(); it++) {
        k = *it;
        ASSERT_EQ(sm[keys[i]], sm.get_data_by_index(k));
        i++;
    }
}
