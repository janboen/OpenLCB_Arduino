#ifndef __OLCB_DATAGRAM_HANDLER__
#define __OLCB_DATAGRAM_HANDLER__

#include "OLCB_Handler.h"

#define DATAGRAM_ACK_TIMEOUT 2000
#define DATAGRAM_ERROR_ABORTED 0x1001
#define DATAGRAM_ERROR_ACK_TIMEOUT 0x1002

// A mix-in class for handling the datagram protocol.

class OLCB_Datagram_Handler : public OLCB_Handler
{
 public:
  OLCB_Datagram_Handler() : OLCB_Handler(), _rxDatagramBufferFree(true), _txDatagramBufferFree(true), _initialized(false), _sentTime(0), _txFlag(false), _loc(0) {}
  
  void setLink(OLCB_Link *newLink);
  void setNID(OLCB_NodeID *newNID);
  
  virtual bool verifyNID(OLCB_NodeID *nid)
  {
    if((*nid) == (*NID))
    {
//      Serial.println("Someone wants to know my name!");
      return true;
    }
    return false;
  }
  
  virtual bool handleFrame(OLCB_Buffer *frame);
  
  virtual void update(void);
  
  //This method, and maybe the next, must be fleshed out in a derived class to be of much use.
  virtual bool processDatagram(void) {return false;}
  virtual void datagramResult(bool accepted, uint16_t errorcode) {return;}
  
  bool sendDatagram(OLCB_Datagram *datagram);
  bool isDatagramSent(void);
  
 protected:
 
 //TODO condense all these bools into a bitfield
  uint16_t _sentTime;
  bool _initialized;
  bool _txFlag;
  bool _rxDatagramBufferFree;
  bool _txDatagramBufferFree;
  uint8_t _loc;
  
  OLCB_Datagram *_rxDatagramBuffer;
  OLCB_Datagram *_txDatagramBuffer;
  
};

#endif