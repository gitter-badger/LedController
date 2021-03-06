#include "Move_multi.hpp"

static byte in_array[4] = {
    B00000001,
    B00001000,
    B00100000,
    B10000000
};

static void test_left_right_multi(controller_configuration& conf){
    try{
        LedController control = LedController(conf);

        control.moveLeft(in_array,nullptr);
        byte* out = new byte[4];
        control.moveRight(in_array,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

static void test_right_left_multi(controller_configuration& conf){
    try{
        LedController control = LedController(conf);

        control.moveRight(in_array,nullptr);
        byte* out = new byte[4];
        control.moveLeft(in_array,&out);
        for(unsigned int i = 0; i < 4;i++){
            TEST_ASSERT_EQUAL(in_array[i],out[i]);
        }
        delete out;
    }
    CATCH_STD
    #ifndef ARDUINO
    CATCH_FAKEIT
    #endif
}

static void test_left_right_default_multi(){
    test_left_right_multi(get_multi_conf());
}
static void test_right_left_default_multi(){
    test_right_left_multi(get_multi_conf());
}
static void test_up_down_default_multi(){
    test_up_down(get_multi_conf());
}
static void test_down_up_default_multi(){
    test_down_up(get_multi_conf());
}

static void test_left_right_SPI_multi(){
    test_left_right_multi(get_multi_conf_SPI());
}
static void test_right_left_SPI_multi(){
    test_right_left_multi(get_multi_conf_SPI());
}
static void test_up_down_SPI_multi(){
    test_up_down(get_multi_conf_SPI());
}
static void test_down_up_SPI_multi(){
    test_down_up(get_multi_conf_SPI());
}

void run_move_multi_tests(){
    RUN_TEST(test_left_right_default_multi);
    RUN_TEST(test_right_left_default_multi);
    RUN_TEST(test_up_down_default_multi);
    RUN_TEST(test_down_up_default_multi);
    
    RUN_TEST(test_left_right_SPI_multi);
    RUN_TEST(test_right_left_SPI_multi);
    RUN_TEST(test_up_down_SPI_multi);
    RUN_TEST(test_down_up_SPI_multi);
}