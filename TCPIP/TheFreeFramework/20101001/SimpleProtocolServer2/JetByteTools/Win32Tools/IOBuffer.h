#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_IO_BUFFER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_IO_BUFFER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IOBuffer.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 - 2003 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
// class member is a reference 
//lint -esym(1725, CIOBuffer::m_allocator)       
//lint -esym(1725, InOrderBufferList::m_criticalSection)   
// 
// Private constructor
//lint -esym(1704, CIOBuffer::CIOBuffer)
//lint -esym(1704, Allocator::Allocator)
//
// No default constructor
//lint -esym(1712, CIOBuffer)
//lint -esym(1712, Allocator)
//lint -esym(1712, InOrderBufferList)
//
// Base class destructor not virtual
//lint -esym(1509, Node)
//lint -esym(1509, COpaqueUserData)
//
// Base class has no destructor
//lint -esym(1510, _OVERLAPPED)
//
// Symbol hides global operator
//lint -esym(1737, CIOBuffer::operator new)
//
// 
//lint -e537 repeated include winsock2.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4786)  // identifier was truncated to '255' characters 
                                 // in the debug information

#include "WinsockWrapper.h"

#include "CriticalSection.h" 
#include "tstring.h"

#include "NodeList.h"
#include "OpaqueUserData.h"

#include <map>

#pragma warning(disable: 4200) // nonstandard extension used : zero-sized array in struct/union

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CIOBuffer
///////////////////////////////////////////////////////////////////////////////

class CIOBuffer : 
   public OVERLAPPED, 
   public CNodeList::Node, 
   public COpaqueUserData
{
   public :

      class Allocator;

      friend class Allocator;

      class InOrderBufferList;

      WSABUF *GetWSABUF() const;

      size_t GetUsed() const;

      size_t GetSize() const;

      const BYTE *GetBuffer() const;

      void SetupZeroByteRead();

      void SetupRead();

      void SetupWrite();
      
      void AddData(
         const char * const pData,
         size_t dataLength);

      void AddData(
         const BYTE * const pData,
         size_t dataLength);

      void AddData(
         BYTE data);

      void Use(
         size_t dataUsed);

      CIOBuffer *SplitBuffer(
         size_t bytesToRemove);

      CIOBuffer *AllocateNewBuffer() const;

      void ConsumeAndRemove(
         size_t bytesToRemove);

      void Empty();

      void AddRef();
      void Release();

      size_t GetOperation() const;
      
      void SetOperation(
         size_t operation);

      size_t GetSequenceNumber() const;
      
      void SetSequenceNumber(
         size_t sequenceNumber);

   private :

      size_t m_operation;
      size_t m_sequenceNumber;

      WSABUF m_wsabuf;

      Allocator &m_allocator;

      long m_ref;
      const size_t m_size;
      size_t m_used;
      BYTE m_buffer[0];      // start of the actual buffer, must remain the last
                             // data member in the class.

   private :

      static void *operator new(size_t objSize, size_t bufferSize);
      static void operator delete(void *pObj, size_t bufferSize);
      
      CIOBuffer(
         Allocator &m_allocator,
         size_t size);

      // No copies do not implement
      CIOBuffer(const CIOBuffer &rhs);
      CIOBuffer &operator=(const CIOBuffer &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// CIOBuffer::Allocator
///////////////////////////////////////////////////////////////////////////////

class CIOBuffer::Allocator
{
   public :

      friend class CIOBuffer;

      explicit Allocator(
         size_t bufferSize,
         size_t maxFreeBuffers);

      virtual ~Allocator();

      CIOBuffer *Allocate();

      size_t GetBufferSize() const;

   protected :

      void Flush();

   private :

      void Release(
         CIOBuffer *pBuffer);

      virtual void OnBufferCreated() {}
      virtual void OnBufferAllocated() {}
      virtual void OnBufferReleased() {}
      virtual void OnBufferDestroyed() {}

      void DestroyBuffer(
         CIOBuffer *pBuffer);

      const size_t m_bufferSize;

      typedef TNodeList<CIOBuffer> BufferList;
      
      BufferList m_freeList;
      BufferList m_activeList;
      
      const size_t m_maxFreeBuffers;

      CCriticalSection m_criticalSection;

      // No copies do not implement
      Allocator(const Allocator &rhs);
      Allocator &operator=(const Allocator &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// CIOBuffer::InOrderBufferList
///////////////////////////////////////////////////////////////////////////////

class CIOBuffer::InOrderBufferList
{
   public:

      explicit InOrderBufferList(
         CCriticalSection &lock);

      ~InOrderBufferList();

      void AddBuffer(
         CIOBuffer *pBuffer);

      void ProcessBuffer();

      CIOBuffer *ProcessAndGetNext();

      CIOBuffer *GetNext();

      CIOBuffer *GetNext(
         CIOBuffer *pBuffer);

      void Reset();

      bool Empty() const;

   private :

      size_t m_next;
   
      typedef std::map<size_t, CIOBuffer *> BufferSequence;

      BufferSequence m_list;

      CCriticalSection &m_criticalSection;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

#endif // JETBYTE_TOOLS_WIN32_IO_BUFFER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IOBuffer.h
///////////////////////////////////////////////////////////////////////////////
