// Entity pool interface
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#define _SA_BEGIN         namespace SA {
#define _SA_END           }
#define _SA               ::SA::

_SA_BEGIN

union tPoolObjectFlags
{
    struct
    {
        unsigned char   uID : 7;
        bool            bIsFreeSlot : 1;
    }                   a;
    signed char         b;
};

template<class A, class B = A> class CPool
{
public:
    //typedef A base_type;
    using base_type = A;

    B*                  m_Objects;
    tPoolObjectFlags*   m_ByteMap;
    int                 m_Size;
    int                 m_nFirstFree;
    bool                m_bOwnsAllocations;
    bool                m_bLocked;


    // Default constructor for statically allocated pools
    CPool()
        : CPool(0)
    {}

    // Initializes pool
    CPool(int nSize, const char* pPoolName = nullptr) :
        m_Objects(nullptr),
        m_ByteMap(nullptr),
        m_nFirstFree(-1),
        m_Size(0),
        m_bOwnsAllocations(false)
    {
        if (nSize)
        {
            Init(nSize);
        }
    }

    ~CPool()
    {
        Flush();
    }

    // Initialises a pool with preallocated
    void Init(int nSize, void* pObjects = nullptr, void* pInfos = nullptr)
    {
        m_Objects = static_cast<B*>(operator new(sizeof(B) * nSize));
        m_ByteMap = static_cast<tPoolObjectFlags*>(operator new(nSize));

        m_Size = nSize;
        m_nFirstFree = -1;
        m_bOwnsAllocations = true;

        for (int i = 0; i < nSize; ++i)
        {
            m_ByteMap[i].a.bIsFreeSlot = true;
            m_ByteMap[i].a.uID = 0;
        }
    }

    // Shutdown pool
    void Flush()
    {
        if (m_bOwnsAllocations)
        {
            operator delete(m_Objects);
            operator delete(m_ByteMap);
        }

        m_Objects = nullptr;
        m_ByteMap = nullptr;
        m_Size = 0;
        m_nFirstFree = 0;
    }

    // Clears pool
    void Clear()
    {
        for (int i = 0; i < m_Size; i++)
            m_ByteMap[i].a.bIsFreeSlot = true;
    }

    // Returns pointer to object by index
    A* GetAt(int nIndex)
    {
        return nIndex >= 0 && nIndex < m_Size && !m_ByteMap[nIndex].a.bIsFreeSlot ? &m_Objects[nIndex] : nullptr;
    }

    // Returns if specified slot is free (0x404940)
    bool IsFreeSlotAtIndex(int idx)
    {
        return m_ByteMap[idx].a.bIsFreeSlot;
    }

    // Marks slot as free / used (0x404970)
    void SetNotFreeAt(int idx, char bIsSlotFree)
    {
        m_ByteMap[idx].a.bIsFreeSlot = bIsSlotFree;
    }

    // Allocates object
    A* New()
    {
        //if (m_bLocked)
            //return nullptr;

        bool bReachedTop = false;
        do
        {
            if (++m_nFirstFree >= m_Size)
            {
                if (bReachedTop)
                {
                    m_nFirstFree = -1;
                    return nullptr;
                }
                bReachedTop = true;
                m_nFirstFree = 0;
            }
        } while (!m_ByteMap[m_nFirstFree].a.bIsFreeSlot);
        m_ByteMap[m_nFirstFree].a.bIsFreeSlot = false;
        ++m_ByteMap[m_nFirstFree].a.uID;
        return &m_Objects[m_nFirstFree];
    }

    // Allocates object at a specific index from a SCM handle
    void New(int nHandle)
    {
        //if (m_bLocked)
            //return;

        nHandle >>= 8;

        m_ByteMap[nHandle].a.bIsFreeSlot = false;
        ++m_ByteMap[nHandle].a.uID;
        m_nFirstFree = 0;

        while (!m_ByteMap[m_nFirstFree].a.bIsFreeSlot)
            ++m_nFirstFree;
    }

    // Deallocates object
    void Delete(A* pObject)
    {
        //if (m_bLocked)
            //return;

        int nIndex = reinterpret_cast<B*>(pObject) - m_Objects;
        m_ByteMap[nIndex].a.bIsFreeSlot = true;
        if (nIndex < m_nFirstFree)
            m_nFirstFree = nIndex;
    }

    // Returns SCM handle for object
    int GetIndex(A* pObject)
    {
        return ((reinterpret_cast<B*>(pObject) - m_Objects) << 8) + m_ByteMap[reinterpret_cast<B*>(pObject) - m_Objects].b;
    }

    // Returns pointer to object by SCM handle
    A* AtHandle(int handle)
    {
        int nSlotIndex = handle >> 8;
        return nSlotIndex >= 0 && nSlotIndex < m_Size && m_ByteMap[nSlotIndex].b == (handle & 0xFF) ? &m_Objects[nSlotIndex] : nullptr;
    }

    // Get pool usage
    u32 GetNoOfUsedSpaces()
    {
        u32 usage = 0u;
        for (s32 i = 0; i < m_Size; ++i)
            if (!IsFreeSlotAtIndex(i)) ++usage;
        return usage;
    }

    // Gets pool size
    inline s32 GetSize() const
    {
        return m_Size;
    }

    // Lock the pool?
    inline void Lock(bool _bLock)
    {
        m_bLocked = _bLock;
    }

    inline size_t GetItemSize()
    {
        return sizeof(B);
    }

    /*template<class Func>
    inline void ForEach(Func function)
    {
        for (s32 i = 0; i < GetSize(); ++i)
        {
            if (GetAt(i) != nullptr)
            {
                function(GetAt(i));
            }
        }
    }*/

    using foreach_func_t = void(*)(base_type *pItem);
    inline void ForEach(foreach_func_t functor)
    {
        for (s32 i = 0; i != GetSize(); ++i)
        {
            if (GetAt(i) != nullptr)
            {
                functor(GetAt(i));
            }
        }
    }

    // Until we have string support
    /*inline Str GetUsage()
    {
        return Str::Format("%d / %d", GetNoOfUsedSpaces(), GetSize());
    }*/
};

VALIDATE_SIZE(CPool<s32>, 0x14);

_SA_END
