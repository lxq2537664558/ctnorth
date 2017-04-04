// Static entity
// Author(s):       iFarbod <>
//                  GTA Modding Community <http://gtamodding.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Placeable.hpp"
#include "EntityFlags.hpp"

namespace ctn::SA
{

class CEntity : public CPlaceable
{
public:
    union
    {
        struct RwObject* m_rwObject; // +0x18
        struct RpClump* m_rpClump;   // +0x18
        struct RpAtomic* m_rpAtomic; // +0x18
    };
    union
    {
        struct
        {
            // 0  0x00000001 Set if this entity is a big building
            u32 m_usesCollision : 1;
            //  1  0x00000002 Has object been processed by a ProcessEntityCollision function?
            u32 m_collisionProcessed : 1;
            //  2  0x00000004 Is entity static?
            u32 m_isStatic : 1;
            //  3  0x00000008 Has entity processed some contact forces?
            u32 m_hasContacted : 1; //
            //  4  0x00000010 Is entity stuck ?
            u32 m_isStuck : 1;
            //  5  0x00000020 Is entity in a collision free safe position?
            u32 m_inSafePosition : 1;
            //  6  0x00000040 Was entity control processing postponed?
            u32 m_wasPostponed : 1;
            //  7  0x00000080 Is the entity visible?
            u32 m_isVisible : 1;

            //  8  0x00000100  Set if this entity is a big building
            u32 m_isBigBuilding : 1;
            //  9  0x00000200  Use damaged LOD models for objects with applicable damage
            u32 m_renderDamaged : 1;
            // 10  0x00000400  Don't let the streaming remove this
            u32 m_streamingDontDelete : 1;
            // 11  0x00000800  Remove this entity next time it should be processed
            u32 m_removeFromWorld : 1;
            // 12  0x00001000  Has collided with a building (changes subsequent collisions)
            u32 m_hasHitWall : 1;
            // 13  0x00002000  Don't delete me because I'm being rendered
            u32 m_imBeingRendered : 1;
            // 14  0x00004000  Draw object last
            u32 m_drawLast : 1;
            // 15  0x00008000  Fade entity because it is far away
            u32 m_distanceFade : 1;

            // 16  0x00010000  Don't cast shadows on this object
            u32 m_dontCastShadowsOn : 1;
            // 17  0x00020000  Offscreen flag. This can only be trusted when it is set to true.
            u32 m_offscreen : 1;
            // 18  0x00040000  This is used by script created entities - they are static until the collision is loaded
            // below them
            u32 m_isStaticWaitingForCollision : 1;
            // 19  0x00080000  Tell the streaming not to stream me
            u32 m_dontStream : 1;
            // 20  0x00100000  This object is underwater; change drawing order
            u32 m_underwater : 1;
            // 21  0x00200000  Object has prerender effects attached to it
            u32 m_hasPreRenderEffects : 1;
            // 22  0x00400000  Whether or not the building is temporary (i.e. can be created and deleted more than once)
            u32 m_isTempBuilding : 1;
            // 23  0x00800000  Don't update the animation hierarchy this frame
            u32 m_dontUpdateHierarchy : 1;

            // 24  0x01000000  Entity is roadsign and has some 2deffect text stuff to be rendered
            u32 m_hasRoadsignText : 1;
            // 25  0x02000000  Displays super low LOD?
            u32 m_displayedSuperLowLOD : 1;
            // 26  0x04000000  Set object has been generated by procedural object generator
            u32 m_isProcObject : 1;
            // 27  0x08000000  Has backface culling on
            u32 m_backfaceCulled : 1;
            // 28  0x10000000  Light object with directional lights
            u32 m_directionalLight : 1;
            // 29  0x20000000  Set that this object is unimportant, if streaming is having problems
            u32 m_unimportantStream : 1;
            // 30  0x40000000  Is this model part of a tunnel
            u32 m_tunnel : 1;
            // 31  0x80000000  This model should be rendered from within and outside of the tunnel
            u32 m_tunnelTransition : 1;
        };

        u32 m_entityFlags; // +0x1C
    };

    u16 m_randomSeed;
    u16 m_modelIndex;
    void* m_references;    // CReference*
    void* m_streamingLink; // CLink<CEntity*>*
    s16 m_scanCode;
    u8 m_iplSlot;
    u8 m_areaCode;

    union
    {
        class CEntity* m_lodEntity;
        s32 m_lodIndex;
    };

    u8 m_numLodChildren;
    u8 m_numLodChildrenRendered;

    union
    {
        u8 m_typeStatus;

        struct
        {
            u8 m_type : 3;
            u8 m_status : 5;
        };
    };
};

VALIDATE_SIZE(CEntity, 0x38);

}
