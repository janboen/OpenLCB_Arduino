#ifndef __OLCB_LINK_H__
#define __OLCB_LINK_H__

#include <stdint.h>
#include "Arduino.h"
#include "can.h"
#include "OLCB_Event.h"
#include "OLCB_Virtual_Node.h"
#include "OLCB_Datagram.h"
#include "OLCB_Stream.h"
#include "OLCB_NodeID.h"

class OLCB_Virtual_Node;

/* An abstract base class representing a physical link to the outside world */

class OLCB_Link
{
 public:
  OLCB_Link() : _handlers(0) {}
  
  virtual bool initialize(void) {return false;} //called once, returns true if init succeeded.
    
  virtual void update(void); //called repeatedly  
  virtual void addVNode(OLCB_Virtual_Node *vnode);
  virtual void removeVNode(OLCB_Virtual_Node *vnode);
  
  virtual bool sendVerifiedNID(OLCB_NodeID *nid)  {return false;}
  virtual bool sendVerifyNID(OLCB_NodeID *src, OLCB_NodeID *request)  {return false;}
  virtual bool sendVerifyNID(OLCB_NodeID *src) {return false;} //global version
    
  virtual bool sendIdent(void) {return false;}
    
  virtual bool sendPCER(OLCB_Event *event) {return true;}
  
  virtual uint8_t sendDatagramFragment(OLCB_Datagram *datagram, uint8_t start) {return 0;}
  virtual bool ackDatagram(OLCB_NodeID *source, OLCB_NodeID *dest) {return true;}
  virtual bool nakDatagram(OLCB_NodeID *source, OLCB_NodeID *dest, int reason) {return true;}
  
  virtual bool sendStream(OLCB_Stream *stream) {return true;}
  //Not sure that this is how streams should work at all!
  
  //TODO!!
  virtual bool sendConsumerIdentified(OLCB_Event *event) {return false;}
  virtual bool sendLearnEvent(OLCB_Event *event) {return false;}
  virtual bool sendProducerIdentified(OLCB_Event *event) {return false;}
  
 protected:
  OLCB_Virtual_Node* _handlers;
};

#endif //__OLCB_LINK_H__
