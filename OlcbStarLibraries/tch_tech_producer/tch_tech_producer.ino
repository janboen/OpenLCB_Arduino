//==============================================================
// tch_tech_producer
// 32-channel OpenLCB producer board
//  TCH Tech 0.7.6     
//==============================================================

// next line for stand-alone compile
#include <Arduino.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "logging.h"

// The following lines are needed because the Arduino environment 
// won't search a library directory unless the library is included 
// from the top level file (this file)
#include <EEPROM.h>
#include <can.h>
#include "NodeID.h"
#include <ButtonLed.h>
 
NodeID nodeid(2,1,54,8,18,2);    // This node's default ID

/**
 * Get and put routines that 
 * use a test memory space.
 TCH Tech Ver 0.7.6
 */
const char configDefInfo[] PROGMEM = {
    60, 63, 120, 109, 108, 32, 118, 101, 114, 115, 105, 111, 110, 61, 39, 49, 46, 48, 39, 63, 62, 10, 60, 63, 120, 109, 108, 45, 115, 116, 121, 108, 101, 115, 104, 101, 101, 116, 32, 116, 121, 112, 101, 61, 39, 116, 101, 120, 116, 47, 120, 115, 108, 39, 32, 104, 114, 101, 102, 61, 39, 104, 116, 116, 112, 58, 47, 47, 111, 112, 101, 110, 108, 99, 98, 46, 111, 114, 103, 47, 116, 114, 117, 110, 107, 47, 112, 114, 111, 116, 111, 116, 121, 112, 101, 115, 47, 120, 109, 108, 47, 120, 115, 108, 116, 47, 99, 100, 105, 46, 120, 115, 108, 39, 63, 62, 10, 60, 99, 100, 105, 32, 120, 109, 108, 110, 115, 58,    // | <?xml version='1.0'?><?xml-stylesheet type='text/xsl' href='http://openlcb.org/trunk/prototypes/xml/xslt/cdi.xsl'?><cdi xmlns:|
   120, 115, 105, 61, 39, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 45, 105, 110, 115, 116, 97, 110, 99, 101, 39, 32, 120, 115, 105, 58, 110, 111, 78, 97, 109, 101, 115, 112, 97, 99, 101, 83, 99, 104, 101, 109, 97, 76, 111, 99, 97, 116, 105, 111, 110, 61, 39, 104, 116, 116, 112, 58, 47, 47, 111, 112, 101, 110, 108, 99, 98, 46, 111, 114, 103, 47, 116, 114, 117, 110, 107, 47, 112, 114, 111, 116, 111, 116, 121, 112, 101, 115, 47, 120, 109, 108, 47, 115, 99, 104, 101, 109, 97, 47, 99, 100,    // | xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:noNamespaceSchemaLocation='http://openlcb.org/trunk/prototypes/xml/schema/cd|
   105, 46, 120, 115, 100, 39, 62, 10, 10, 60, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 109, 97, 110, 117, 102, 97, 99, 116, 117, 114, 101, 114, 62, 84, 67, 72, 32, 84, 101, 99, 104, 110, 111, 108, 111, 103, 121, 60, 47, 109, 97, 110, 117, 102, 97, 99, 116, 117, 114, 101, 114, 62, 10, 32, 32, 32, 32, 60, 109, 111, 100, 101, 108, 62, 51, 50, 32, 73, 110, 112, 117, 116, 32, 79, 112, 101, 110, 76, 67, 66, 32, 80, 114, 111, 100, 117, 99, 101, 114, 32, 82, 101, 118, 32, 66, 60, 47, 109, 111, 100, 101, 108, 62, 10, 32, 32, 32,    // | i.xsd'><identification>    <manufacturer>TCH Technology</manufacturer>    <model>32 Input OpenLCB Producer Rev B</model>   |
   32, 60, 104, 97, 114, 100, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 49, 46, 48, 46, 49, 60, 47, 104, 97, 114, 100, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 115, 111, 102, 116, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 48, 46, 55, 46, 54, 60, 47, 115, 111, 102, 116, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 10, 60, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 62, 10, 10, 60, 115, 101, 103, 109, 101, 110, 116, 32, 111, 114, 105, 103, 105, 110, 61, 39, 48, 39, 32, 115, 112,    // |  <hardwareVersion>1.0.1</hardwareVersion>    <softwareVersion>0.7.6</softwareVersion></identification><segment origin='0' sp|
   97, 99, 101, 61, 39, 50, 53, 51, 39, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111, 117, 112, 32, 111, 102, 102, 115, 101, 116, 61, 39, 54, 53, 52, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 85, 115, 101, 114, 32, 73, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 76, 101, 116, 115, 32, 116, 104, 101, 32, 117, 115, 101, 114, 32, 97, 100, 100, 32, 104, 105, 115, 32, 111, 119, 110, 32, 100, 101, 115, 99,    // | ace='253'>    <group offset='654'>        <name>User Identification</name>        <description>Lets the user add his own desc|
   114, 105, 112, 116, 105, 111, 110, 60, 47, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 115, 116, 114, 105, 110, 103, 32, 115, 105, 122, 101, 61, 39, 50, 48, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 111, 100, 101, 32, 78, 97, 109, 101, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 115, 116, 114, 105, 110, 103, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 115, 116, 114, 105, 110, 103, 32, 115, 105, 122, 101, 61, 39, 51, 48, 39, 62,    // | ription</description>        <string size='20'>            <name>Node Name</name>        </string>        <string size='30'>|
   10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 111, 100, 101, 32, 68, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 115, 116, 114, 105, 110, 103, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 60, 47, 115, 101, 103, 109, 101, 110, 116, 62, 10, 10, 60, 115, 101, 103, 109, 101, 110, 116, 32, 111, 114, 105, 103, 105, 110, 61, 39, 49, 48, 39, 32, 115, 112, 97, 99, 101, 61, 39, 50, 53, 51, 39, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111,    // |             <name>Node Description</name>        </string>    </group></segment><segment origin='10' space='253'>    <gro|
   117, 112, 32, 114, 101, 112, 108, 105, 99, 97, 116, 105, 111, 110, 61, 39, 51, 50, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 73, 110, 112, 117, 116, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 112, 110, 97, 109, 101, 62, 73, 110, 112, 117, 116, 60, 47, 114, 101, 112, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 115, 116, 114, 105, 110, 103, 32, 115, 105, 122, 101, 61, 39, 49, 54, 39, 32, 111, 102, 102, 115, 101, 116, 61, 39, 55, 54, 48, 39, 62, 10, 32, 32, 32, 32, 32, 32,    // | up replication='32'>        <name>Inputs</name>        <repname>Input</repname>        <string size='16' offset='760'>      |
   32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 68, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 115, 116, 114, 105, 110, 103, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 32, 111, 102, 102, 115, 101, 116, 61, 39, 45, 55, 55, 52, 39, 32, 62, 60, 33, 45, 45, 32, 102, 111, 114, 119, 97, 114, 100, 32, 55, 54, 48, 44, 32, 97, 100, 100, 32, 49, 54, 44, 32, 98, 97, 99, 107, 32, 55, 54, 48, 44, 32, 116, 104, 101, 110, 32, 102, 111, 114, 119, 97, 114,    // |       <name>Description</name>        </string>        <eventid offset='-774' ><!-- forward 760, add 16, back 760, then forwar|
   100, 32, 50, 45, 45, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 65, 99, 116, 105, 118, 97, 116, 105, 111, 110, 32, 69, 118, 101, 110, 116, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 32, 111, 102, 102, 115, 101, 116, 61, 39, 50, 39, 32, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 73, 110, 97, 99, 116, 105, 118, 97, 116, 105, 111, 110,    // | d 2-->            <name>Activation Event</name>        </eventid>        <eventid offset='2' >            <name>Inactivation|
   32, 69, 118, 101, 110, 116, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 60, 47, 115, 101, 103, 109, 101, 110, 116, 62, 10, 10, 60, 115, 101, 103, 109, 101, 110, 116, 32, 111, 114, 105, 103, 105, 110, 61, 39, 48, 39, 32, 115, 112, 97, 99, 101, 61, 39, 50, 53, 51, 39, 62, 10, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 39, 52, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 82, 101, 115, 101,    // |  Event</name>        </eventid>    </group></segment><segment origin='0' space='253'>    <int size='4'>        <name>Rese|
   116, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 67, 111, 110, 116, 114, 111, 108, 115, 32, 114, 101, 108, 111, 97, 100, 105, 110, 103, 32, 97, 110, 100, 32, 99, 108, 101, 97, 114, 105, 110, 103, 32, 110, 111, 100, 101, 32, 109, 101, 109, 111, 114, 121, 46, 32, 66, 111, 97, 114, 100, 32, 109, 117, 115, 116, 32, 98, 101, 32, 114, 101, 115, 116, 97, 114, 116, 101, 100, 32, 102, 111, 114, 32, 116, 104, 105, 115, 32, 116, 111, 32, 116, 97, 107, 101, 32, 101, 102, 102, 101, 99, 116, 46, 60, 47, 100, 101, 115,    // | t</name>        <description>Controls reloading and clearing node memory. Board must be restarted for this to take effect.</des|
   99, 114, 105, 112, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 109, 97, 112, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 51, 57, 57, 56, 53, 55, 50, 50, 54, 49, 60, 47, 112, 114, 111, 112, 101, 114, 116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 40, 78, 111, 32, 114, 101, 115, 101, 116, 41, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114,    // | cription>        <map>            <relation><property>3998572261</property><value>(No reset)</value></relation>            <r|
   101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 48, 60, 47, 112, 114, 111, 112, 101, 114, 116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 82, 101, 115, 101, 116, 32, 97, 108, 108, 32, 116, 111, 32, 100, 101, 102, 97, 117, 108, 116, 115, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 51, 57, 57, 56, 53, 54, 49, 50, 50, 56, 60, 47, 112, 114, 111, 112, 101, 114,    // | elation><property>0</property><value>Reset all to defaults</value></relation>            <relation><property>3998561228</proper|
   116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 82, 101, 115, 101, 116, 32, 69, 118, 101, 110, 116, 73, 68, 115, 32, 116, 111, 32, 110, 101, 119, 32, 100, 101, 102, 97, 117, 108, 116, 115, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 109, 97, 112, 62, 10, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 60, 47, 115, 101, 103, 109, 101, 110, 116, 62, 10, 10, 60, 47, 99, 100, 105, 62, 10, 0
};
extern "C" {
const char SNII_const_data[] PROGMEM = "\001TCH Technology\000OpenLCB 32-Input Node Rev B\0001.0.1\0000.7.6";
}
#define SNII_var_data 654
#define SNII_var_offset 20

#include "OlcbArduinoCAN.h"

/* *************************************************
 * EEPROM memory layout
 *     See NodeMemory.h for background info
 *
 * Internal data, not to be reset by user:
 *     0 - 3        Memory state flag
 *     4 - 5        Cycle count
 *     6 - 11       Node ID
 *
 * User configuration data:
 *     12 - 12+8*sizeof(Event)  EventID storage
 * 
 *     94 - 113     Node name (zero-term string, 20 bytes total)
 *     114 - 136     User comment (zero-term string, 24 bytes total)
 *     
 *     12+654 - 12+654+8*sizeof(Event)  Event description strings
 *
 **************************************************** */

#define LAST_EEPROM 12+1316+8*sizeof(Event)

const uint8_t getRead(uint32_t address, int space) {
  if (space == 0xFF) {
    // Configuration definition information
    return pgm_read_byte(configDefInfo+address);
  } else if (space == 0xFE) {
    // All memory reads from RAM starting at first location in this program
    return *(((uint8_t*)&rxBuffer)+address);
  } else if (space == 0xFD) {
    // Configuration space is entire EEPROM
    return EEPROM.read(address);
  } else if (space == 0xFC) { // 
    // used by ADCDI/SNII for constant data
    return pgm_read_byte(SNII_const_data+address);
  } else if (space == 0xFB) { // 
    // used by ADCDI/SNII for variable data
    return EEPROM.read(SNII_var_data+address);
  } else {
    // unknown space
    return 0; 
  }
}
void getWrite(uint32_t address, int space, uint8_t val) {
  if (space == 0xFE) {
    // All memory
    *(((uint8_t*)&rxBuffer)+address) = val;
  } else if (space == 0xFD) {
    // Configuration space
    EEPROM.write(address, val);
  } 
  // all other spaces not written
}
extern "C" {
uint8_t protocolIdentValue[6] = {0xD7,0x48,0,0,0,0};
}

// Events this node can produce or consume, used by PCE and loaded from EEPROM by NM
    Event events[] = {
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event()
   };
int eventNum = 64;

// output drivers
ButtonLed p2(2, LOW);//1
ButtonLed p3(3, LOW);
ButtonLed p4(4, LOW);
ButtonLed p5(5, LOW);
ButtonLed p6(6, LOW);
ButtonLed p7(7, LOW);
ButtonLed p8(8, LOW);
ButtonLed p9(9, LOW);//8
ButtonLed p10(10, LOW);
ButtonLed p11(11, LOW);
ButtonLed p12(12, LOW);
ButtonLed p13(13, LOW);
ButtonLed p14(14, LOW);
ButtonLed p17(17, LOW);
ButtonLed p18(18, LOW);
ButtonLed p19(19, LOW);//16
ButtonLed p22(22, LOW);
ButtonLed p23(23, LOW);
ButtonLed p24(24, LOW);
ButtonLed p25(25, LOW);
ButtonLed p26(26, LOW);
ButtonLed p27(27, LOW);
ButtonLed p28(28, LOW);
ButtonLed p29(29, LOW);//24
ButtonLed p30(30, LOW);
ButtonLed p31(31, LOW);
ButtonLed p32(32, LOW);
ButtonLed p33(33, LOW);
ButtonLed p34(34, LOW);
ButtonLed p35(35, LOW);
ButtonLed p36(36, LOW);
ButtonLed p37(37, LOW);//32


#define ShortBlinkOn   0x00010001L
#define ShortBlinkOff  0xFFFEFFFEL

long patterns[] = {
  ShortBlinkOff,ShortBlinkOn,//1
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,//8
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,//16
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,//24
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn,
  ShortBlinkOff,ShortBlinkOn//32
};

ButtonLed* buttons[] = {&p2,&p2,&p3,&p3,&p4,&p4,&p5,&p5,&p6,&p6,&p7,&p7,&p8,&p8,&p9,&p9,&p10,&p10,&p11,&p11,&p12,&p12,&p13,&p13,&p14,&p14,&p17,&p17,&p18,&p18,&p19,&p19,&p22,&p22,&p23,&p23,&p24,&p24,&p25,&p25,&p26,&p26,&p27,&p27,&p28,&p28,&p29,&p29,&p30,&p30,&p31,&p31,&p32,&p32,&p33,&p33,&p34,&p34,&p35,&p35,&p36,&p36,&p37,&p37,};


ButtonLed blue(42, LOW);
ButtonLed gold(43, LOW);

void pceCallback(int index){
  // invoked when an event is consumed; drive pins as needed
  // from index
  //
  // sample code uses inverse of low bit of pattern to drive pin all on or all off
  // (pattern is mostly one way, blinking the other, hence inverse)
  //
  buttons[index]->on(patterns[index]&0x1 ? 0x0L : ~0x0L );
}

NodeMemory nm(0);  // allocate from start of EEPROM
void store() { nm.store(&nodeid, events, eventNum); }

PCE pce(events, eventNum, &txBuffer, &nodeid, pceCallback, store, &link);

// Set up Blue/Gold configuration

BG bg(&pce, buttons, patterns, eventNum, &blue, &gold, &txBuffer);

bool states[] = {false, false, false, false,false, false, false, false, false, false, false, false,
false, false, false, false,false, false, false, false, false, false, false, false,
false, false, false, false,false, false, false, false};
void produceFromInputs() {
  // called from loop(), this looks at pins and 
  // and decides which events to fire.
  // with pce.produce(i);
  // The first event of each pair is sent on button down,
  // and second on button up.
  for (int i = 0; i<eventNum/2; i++) {
    if (states[i] != buttons[i*2]->state) {
      states[i] = buttons[i*2]->state;
      if (states[i]) {
        pce.produce(i*2);
      } else {
        pce.produce(i*2+1);
      }
    }
  }
}

/**
 * Setup does initial configuration
 */
void setup()
{
  // set up serial comm; may not be space for this!
  //delay(250);Serial.begin(BAUD_RATE);logstr("\nOlcbBasicNode 32 input producer\n");
  
  // read OpenLCB from EEPROM
 
  //nm.forceInitAll(); // uncomment if need to go back to initial EEPROM state
  nm.setup(&nodeid, events, eventNum, (uint8_t*) 0, (uint16_t)0, (uint16_t)LAST_EEPROM);  
  
  // set event types, now that IDs have been loaded from configuration
  for (int i=0; i<eventNum; i++) {
      pce.newEvent(i,true,false); // produce, consume
  }
 
 
Olcb_setup();  
  // Initialize OpenLCB CAN link controller
  //link.reset();
}

void loop() {
    bool activity = Olcb_loop();
    if (activity) {
        // blink blue to show that the frame was received
        blue.blink(0x1);
    }
       // blink gold to show the frame was transmitted
    if (OpenLcb_can_active) {
        gold.blink(0x1);
        OpenLcb_can_active = false;
    }
    // handle the status lights  
    blue.process();
    gold.process();
}
