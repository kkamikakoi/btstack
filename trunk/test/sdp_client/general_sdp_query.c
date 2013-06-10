
//*****************************************************************************
//
// test rfcomm query tests
//
//*****************************************************************************

#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_parser.h"
#include <btstack/hci_cmds.h>
#include <btstack/run_loop.h>

#include "hci.h"
#include "btstack_memory.h"
#include "hci_dump.h"
#include "l2cap.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"


static uint8_t  sdp_test_record_list[] = { 
                                                                  0x36, 0x03, 0xDE, 0x35, 0x62,
0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x0A, 0x09, 0x00, 0x04, 0x35, 0x10, 0x35, 0x06, 0x19,
0x01, 0x00, 0x09, 0x00, 0x19, 0x35, 0x06, 0x19, 0x00, 0x19, 0x09, 0x01, 0x00, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x0D, 0x09,
0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x11, 0x41, 0x32, 0x44, 0x50, 0x20, 0x41, 0x75, 0x64, 0x69,
0x6F, 0x20, 0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x09, 0x03, 0x11, 0x09, 0x00, 0x01, 0x09, 0x07,
0x77, 0x1C, 0x6F, 0x6D, 0x98, 0xF2, 0x3C, 0x3A, 0x11, 0xD6, 0x95, 0x6A, 0x00, 0x03, 0x93, 0x53,
0xE8, 0x58, 0x35, 0x5D, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x0C, 0x09, 0x00, 0x04, 0x35,
0x10, 0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x17, 0x35, 0x06, 0x19, 0x00, 0x17, 0x09, 0x01,
0x00, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06,
0x19, 0x11, 0x0E, 0x09, 0x01, 0x03, 0x09, 0x01, 0x00, 0x25, 0x0C, 0x41, 0x56, 0x52, 0x43, 0x50,
0x20, 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x09, 0x03, 0x11, 0x09, 0x00, 0x01, 0x09, 0x07, 0x77,
0x1C, 0x6F, 0x6D, 0x98, 0xF2, 0x3C, 0x3A, 0x11, 0xD6, 0x95, 0x6A, 0x00, 0x03, 0x93, 0x53, 0xE8,
0x58, 0x35, 0x71, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x05, 0x09, 0x00, 0x04, 0x35, 0x11,
0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x0A, 0x35, 0x03, 0x19, 0x00,
0x08, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65,
0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11,
0x05, 0x09, 0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x10, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x4F, 0x62,
0x6A, 0x65, 0x63, 0x74, 0x20, 0x50, 0x75, 0x73, 0x68, 0x09, 0x03, 0x03, 0x35, 0x02, 0x08, 0xFF,
0x09, 0x07, 0x77, 0x1C, 0x6F, 0x6D, 0x98, 0xF2, 0x3C, 0x3A, 0x11, 0xD6, 0x95, 0x6A, 0x00, 0x03,
0x93, 0x53, 0xE8, 0x58, 0x35, 0x65, 0x09, 0x00, 0x01, 0x35, 0x06, 0x19, 0x11, 0x1F, 0x19, 0x12,
0x03, 0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03,
0x08, 0x02, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09,
0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09,
                                                                  
                                                                  0x35, 0x06, 0x19, 0x11, 0x1E,
0x09, 0x01, 0x05, 0x09, 0x01, 0x00, 0x25, 0x18, 0x48, 0x61, 0x6E, 0x64, 0x73, 0x20, 0x46, 0x72,
0x65, 0x65, 0x20, 0x41, 0x75, 0x64, 0x69, 0x6F, 0x20, 0x47, 0x61, 0x74, 0x65, 0x77, 0x61, 0x79,
0x09, 0x03, 0x01, 0x08, 0x00, 0x09, 0x03, 0x11, 0x09, 0x00, 0x00, 0x35, 0x80, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x01, 0x00, 0x25, 0x1A, 0x41, 0x70, 0x70, 0x6C, 0x65, 0x20,
0x4D, 0x61, 0x63, 0x69, 0x6E, 0x74, 0x6F, 0x73, 0x68, 0x20, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62,
0x75, 0x74, 0x65, 0x73, 0x09, 0x07, 0x80, 0x1C, 0xF0, 0x72, 0x2E, 0x20, 0x0F, 0x8B, 0x4E, 0x90,
0x8C, 0xC2, 0x1B, 0x46, 0xF5, 0xF2, 0xEF, 0xE2, 0x09, 0x07, 0x81, 0x25, 0x09, 0x3C, 0x75, 0x6E,
0x6B, 0x6E, 0x6F, 0x77, 0x6E, 0x3E, 0x09, 0x07, 0x82, 0x25, 0x0D, 0x4D, 0x61, 0x63, 0x42, 0x6F,
0x6F, 0x6B, 0x41, 0x69, 0x72, 0x34, 0x2C, 0x31, 0x09, 0x07, 0x83, 0x28, 0x01, 0x09, 0x07, 0x84,
0x25, 0x0D, 0x34, 0x2E, 0x31, 0x2E, 0x33, 0x66, 0x33, 0x20, 0x31, 0x31, 0x33, 0x34, 0x39, 0x09,
0x07, 0x85, 0x0A, 0x00, 0x00, 0x00, 0x03, 0x09, 0x07, 0x86, 0x19, 0x12, 0x34, 0x35, 0x73, 0x09,
0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x06, 0x09, 0x00, 0x04, 0x35, 0x11, 0x35, 0x03, 0x19, 0x01,
0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x0F, 0x35, 0x03, 0x19, 0x00, 0x08, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A,
0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x06, 0x09, 0x01, 0x00,
0x09, 0x01, 0x00, 0x25, 0x12, 0x4F, 0x42, 0x45, 0x58, 0x20, 0x46, 0x69, 0x6C, 0x65, 0x20, 0x54,
0x72, 0x61, 0x6E, 0x73, 0x66, 0x65, 0x72, 0x09, 0x03, 0x03, 0x35, 0x02, 0x08, 0xFF, 0x09, 0x07,
0x77, 0x1C, 0x6F, 0x6D, 0x98, 0xF2, 0x3C, 0x3A, 0x11, 0xD6, 0x95, 0x6A, 0x00, 0x03, 0x93, 0x53,
0xE8, 0x58, 0x35, 0x53, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x01, 0x09, 0x00, 0x04, 0x35,
0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x03, 0x09, 0x00, 0x05,
0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A,
0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x01, 0x09, 0x01, 0x00,
0x09, 0x01, 0x00, 0x25, 0x12, 0x42, 0x6C, 0x75, 0x65, 0x74, 0x6F,

                                                                  0x6F, 0x74, 0x68, 0x2D, 0x50,
0x44, 0x41, 0x2D, 0x53, 0x79, 0x6E, 0x63, 0x35, 0x59, 0x09, 0x00, 0x01, 0x35, 0x06, 0x19, 0x11,
0x12, 0x19, 0x12, 0x03, 0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05,
0x19, 0x00, 0x03, 0x08, 0x04, 0x09, 0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06,
0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08,
0x35, 0x06, 0x19, 0x11, 0x08, 0x09, 0x01, 0x02, 0x09, 0x01, 0x00, 0x25, 0x15, 0x48, 0x65, 0x61,
0x64, 0x73, 0x65, 0x74, 0x20, 0x41, 0x75, 0x64, 0x69, 0x6F, 0x20, 0x47, 0x61, 0x74, 0x65, 0x77,
0x61, 0x79, 0x35, 0x98, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x17, 0x09, 0x00, 0x04, 0x35,
0x1E, 0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x0F, 0x35, 0x14, 0x19, 0x00, 0x0F, 0x09, 0x01,
0x00, 0x35, 0x0C, 0x09, 0x08, 0x00, 0x09, 0x08, 0x06, 0x09, 0x86, 0xDD, 0x09, 0x88, 0x0B, 0x09,
0x00, 0x05, 0x35, 0x03, 0x19, 0x10, 0x02, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6E, 0x09,
0x00, 0x6A, 0x09, 0x01, 0x00, 0x09, 0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x17, 0x09,
0x01, 0x00, 0x09, 0x01, 0x00, 0x25, 0x1C, 0x47, 0x72, 0x6F, 0x75, 0x70, 0x20, 0x41, 0x64, 0x2D,
0x68, 0x6F, 0x63, 0x20, 0x4E, 0x65, 0x74, 0x77, 0x6F, 0x72, 0x6B, 0x20, 0x53, 0x65, 0x72, 0x76,
0x69, 0x63, 0x65, 0x09, 0x01, 0x01, 0x25, 0x18, 0x50, 0x41, 0x4E, 0x20, 0x47, 0x72, 0x6F, 0x75,
0x70, 0x20, 0x41, 0x64, 0x2D, 0x68, 0x6F, 0x63, 0x20, 0x4E, 0x65, 0x74, 0x77, 0x6F, 0x72, 0x6B,
0x09, 0x03, 0x0A, 0x09, 0x00, 0x01, 0x09, 0x03, 0x0B, 0x09, 0x00, 0x05
};


uint16_t attribute_id = -1;
uint16_t record_id = -1;

uint8_t * attribute_value = NULL;
int       attribute_value_buffer_size = 1000;

void assertBuffer(int size){
    if (size > attribute_value_buffer_size){
        attribute_value_buffer_size *= 2;
        attribute_value = (uint8_t *) realloc(attribute_value, attribute_value_buffer_size);
    }
}


static void handle_general_sdp_parser_event(sdp_parser_event_t * event){
    sdp_parser_attribute_value_event_t * ve;
    sdp_parser_complete_event_t * ce;

    switch (event->type){
        case SDP_PARSER_ATTRIBUTE_VALUE:
            ve = (sdp_parser_attribute_value_event_t*) event;
            
            // handle new record
            if (ve->record_id != record_id){
                record_id = ve->record_id;
            }

            // buffer data
            assertBuffer(ve->attribute_length);
            attribute_value[ve->data_offset] = ve->data;
            
            break;
        case SDP_PARSER_COMPLETE:
            ce = (sdp_parser_complete_event_t*) event;
            printf("General query done with status %d.\n", ce->status);
            break;
    }
}


TEST_GROUP(SDPClient){
    void setup(){
        attribute_value_buffer_size = 1000;
        attribute_value = (uint8_t*) malloc(attribute_value_buffer_size);
        record_id = -1;
        sdp_parser_init();
        sdp_parser_register_callback(handle_general_sdp_parser_event);
    }
};


TEST(SDPClient, QueryRFCOMMWithMacOSXData){
    uint16_t expected_last_attribute_id = 0xffff;
    uint16_t expected_last_record_id = 8;
    uint8_t  expected_attribute_value[3] = {0x09, 0x00, 0x05};

    sdp_parser_handle_chunk(sdp_test_record_list, de_get_len(sdp_test_record_list));
    
    CHECK_EQUAL(expected_last_attribute_id, attribute_id);
    CHECK_EQUAL(expected_last_record_id, record_id);

    uint16_t i;
    for (i=0; i<sizeof(expected_attribute_value); i++){
        CHECK_EQUAL(expected_attribute_value[i], attribute_value[i]);
    }
}


int main (int argc, const char * argv[]){
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
