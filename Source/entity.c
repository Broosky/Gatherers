/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/ai.h"
#include "../Headers/assets.h"
#include "../Headers/common.h"
#include "../Headers/constants.h"
#include "../Headers/entity.h"
#include "../Headers/globals.h"
#include "../Headers/message.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Zero(ENTITY_T* p_Entity) {
    ZeroMemory(p_Entity, sizeof(ENTITY_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Pass in the global structure. I use it more than once in this function.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Create(FPOINT_T Location, USHORT usType, ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If there are no manufacturing restrictions, and no entity overlap, allow creation.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!ENTITY_Overlap(usType, p_Assets, p_Globals) && !ENTITY_Restrict(usType, p_Globals)) {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_T* p_Entity = (ENTITY_T*)malloc(sizeof(ENTITY_T));
        p_Globals->iRunningHeap += sizeof(ENTITY_T);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_Zero(p_Entity);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->usId = p_Globals->usIdStamp++;
        p_Entity->usType = usType;
        p_Entity->CenterPoint = Location;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // To avoid lines being drawn from each entity's center point to the client (0, 0) coordinate.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->MinorDestinationCenterPoint = p_Entity->CenterPoint;
        p_Entity->MajorDestinationCenterPoint = p_Entity->CenterPoint;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->ubIsAlive = 1;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        switch (p_Entity->usType) {
        case ENTITY_WORKER: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Entity->p_Picture = &p_Assets->Worker[0];
            p_Entity->MovementSpeed = (FPOINT_T){ WORKER_MOVE_SPEED, WORKER_MOVE_SPEED };
            p_Entity->ubIsMovable = 1;
            p_Entity->usState = ANIMATE_WORKER_NORMAL;
            break;
        }
        case ENTITY_MINERAL: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Entity->p_Picture = &p_Assets->Mineral[0];
            p_Entity->iMineralCount = MINERALS_ON_CREATION;
            p_Entity->MovementSpeed = (FPOINT_T){ MINERAL_MOVE_SPEED, MINERAL_MOVE_SPEED };
            p_Entity->ubIsObstacle = 1;
            break;
        }
        case ENTITY_COMMAND: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Entity->p_Picture = &p_Assets->Command[0];
            p_Entity->MovementSpeed = (FPOINT_T){ COMMAND_MOVE_SPEED, COMMAND_MOVE_SPEED };
            p_Entity->ubIsObstacle = 1;
            p_Entity->usState = ANIMATE_COMMAND_CREATING;
            break;
        }
        case ENTITY_SUPPLY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Entity->p_Picture = &p_Assets->Supply[0];
            p_Entity->MovementSpeed = (FPOINT_T){ SUPPLY_MOVE_SPEED, SUPPLY_MOVE_SPEED };
            p_Entity->ubIsObstacle = 1;
            p_Entity->usState = ANIMATE_SUPPLY_NORMAL;
            break;
        }
        case ENTITY_REFINERY: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Entity->p_Picture = &p_Assets->Refinery[0];
            p_Entity->iGasCount = GAS_ON_CREATION;
            p_Entity->MovementSpeed = (FPOINT_T){ REFINERY_MOVE_SPEED, REFINERY_MOVE_SPEED };
            p_Entity->ubIsObstacle = 1;
            break;
        }
        default: {
            printf("ENTITY_Create(): Unknown entity type.\n");
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->Size.fX = (*p_Entity->p_Picture).Bitmap.bmWidth;
        p_Entity->Size.fY = (*p_Entity->p_Picture).Bitmap.bmHeight;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->HalfSize.fX = (*p_Entity->p_Picture).Bitmap.bmWidth >> 1;
        p_Entity->HalfSize.fY = (*p_Entity->p_Picture).Bitmap.bmHeight >> 1;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->Location.fX = p_Entity->CenterPoint.fX - p_Entity->HalfSize.fX;
        p_Entity->Location.fY = p_Entity->CenterPoint.fY - p_Entity->HalfSize.fY;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Entity->fRadius = (p_Entity->HalfSize.fX + p_Entity->HalfSize.fY) / 2.0f;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!p_Globals->p_RootEntity) {
            p_Entity->p_Next = NULL;
            p_Globals->p_RootEntity = p_Entity;
        }
        else {
            p_Entity->p_Next = (ENTITY_T*)p_Globals->p_RootEntity;
            p_Globals->p_RootEntity = p_Entity;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_SortToFront(OBSTACLES_TO_FRONT, p_Globals);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_Restrict(USHORT usType, GLOBALS_T* p_Globals) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // This function will return 0 if there are no manufacturing restrictions, otherwise it will return 1.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T Position = { 15.0f, 85.0f };
    switch (usType) {
    case ENTITY_COMMAND: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If I have enough minerals and gas, allow it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iMineralCount >= COST_OF_COMMAND_MINERALS) {
            if (p_Globals->iGasCount >= COST_OF_COMMAND_GAS) {
                p_Globals->iGasCount -= COST_OF_COMMAND_GAS;
                p_Globals->iMineralCount -= COST_OF_COMMAND_MINERALS;
                p_Globals->iTotalSupply += SUPPLY_FROM_COMMAND;
                return 0;
            }
            else {
                MESSAGE_Create("Not enough gas!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
                return 1;
            }
        }
        else {
            MESSAGE_Create("Not enough minerals!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
            return 1;
        }
    }
    case ENTITY_WORKER: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If I have enough minerals, gas, and supply, allow it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iTotalSupply - p_Globals->iCurrentSupply >= SUPPLY_USED_WORKER) {
            if (p_Globals->iMineralCount >= COST_OF_WORKER_MINERALS) {
                if (p_Globals->iGasCount >= COST_OF_WORKER_GAS) {
                    p_Globals->iGasCount -= COST_OF_WORKER_GAS;
                    p_Globals->iMineralCount -= COST_OF_WORKER_MINERALS;
                    p_Globals->iCurrentSupply += SUPPLY_USED_WORKER;
                    return 0;
                }
                else {
                    MESSAGE_Create("Not enough gas!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
                    return 1;
                }
            }
            else {
                MESSAGE_Create("Not enough minerals!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
                return 1;
            }
        }
        else {
            MESSAGE_Create("Not enough supplies!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
            return 1;
        }
    }
    case ENTITY_MINERAL: {
        return 0;
    }
    case ENTITY_SUPPLY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If I have enough minerals and gas, allow it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iMineralCount >= COST_OF_DEPOT_MINERALS) {
            if (p_Globals->iGasCount >= COST_OF_DEPOT_GAS) {
                p_Globals->iGasCount -= COST_OF_DEPOT_GAS;
                p_Globals->iMineralCount -= COST_OF_DEPOT_MINERALS;
                p_Globals->iTotalSupply += SUPPLY_FROM_DEPOT;
                return 0;
            }
            else {
                MESSAGE_Create("Not enough gas!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
                return 1;
            }
        }
        else {
            MESSAGE_Create("Not enough minerals!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
            return 1;
        }
    }
    case ENTITY_REFINERY: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If I have enough minerals and gas, allow it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Globals->iMineralCount >= COST_OF_REFINERY_MINERALS) {
            if (p_Globals->iGasCount >= COST_OF_REFINERY_GAS) {
                p_Globals->iGasCount -= COST_OF_REFINERY_GAS;
                p_Globals->iMineralCount -= COST_OF_REFINERY_MINERALS;
                return 0;
            }
            else {
                MESSAGE_Create("Not enough gas!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
                return 1;
            }
        }
        else {
            MESSAGE_Create("Not enough minerals!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
            return 1;
        }
    }
    default: {
        printf("ENTITY_Restrict(): Unknown entity type.\n");
        return 1;
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_Overlap(USHORT usType, ASSETS_T* p_Assets, GLOBALS_T* p_Globals) {
    FPOINT_T Position = { 15.0f, 85.0f };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T Size = { 0.0f, 0.0f };
    switch (usType) {
    case ENTITY_WORKER: {
        Size.fX = p_Assets->Worker[0].Bitmap.bmWidth;
        Size.fY = p_Assets->Worker[0].Bitmap.bmHeight;
        break;
    }
    case ENTITY_MINERAL: {
        Size.fX = p_Assets->Mineral[0].Bitmap.bmWidth;
        Size.fY = p_Assets->Mineral[0].Bitmap.bmHeight;
        break;
    }
    case ENTITY_COMMAND: {
        Size.fX = p_Assets->Command[0].Bitmap.bmWidth;
        Size.fY = p_Assets->Command[0].Bitmap.bmHeight;
        break;
    }
    case ENTITY_SUPPLY: {
        Size.fX = p_Assets->Supply[0].Bitmap.bmWidth;
        Size.fY = p_Assets->Supply[0].Bitmap.bmHeight;
        break;
    }
    case ENTITY_REFINERY: {
        Size.fX = p_Assets->Refinery[0].Bitmap.bmWidth;
        Size.fY = p_Assets->Refinery[0].Bitmap.bmHeight;
        break;
    }
    default: {
        printf("ENTITY_Overlap(): Unknown entity type.\n");
    }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T EntityALocation = { p_Globals->iMouseX - Size.fX / 2.0f, p_Globals->iMouseY - Size.fY / 2.0f };
    FPOINT_T EntityADelta = { EntityALocation.fX + Size.fX, EntityALocation.fY + Size.fY };
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_T* Current = p_Globals->p_RootEntity;
    while (Current) {
        FPOINT_T EntityBLocation = {
            Current->Location.fX - p_Assets->Worker[0].Bitmap.bmWidth,
            Current->Location.fY - p_Assets->Worker[0].Bitmap.bmHeight
        };
        FPOINT_T EntityBDelta = {
            EntityBLocation.fX + Current->Size.fX + (p_Assets->Worker[0].Bitmap.bmWidth << 1),
            EntityBLocation.fY + Current->Size.fY + (p_Assets->Worker[0].Bitmap.bmHeight << 1)
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (
            !(EntityALocation.fX > EntityBDelta.fX) &&
            !(EntityADelta.fX < EntityBLocation.fX) &&
            !(EntityALocation.fY > EntityBDelta.fY) &&
            !(EntityADelta.fY < EntityBLocation.fY)
            ) {
            MESSAGE_Create("You can't build there!", Position, MESSAGE_GENERAL_WARNING, p_Globals);
            return 1;
        }
        Current = (ENTITY_T*)Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // No overlap exists.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Pause(ENTITY_T* p_Entity, float fTime) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // When the entity is paused, reset the pause counter for each call.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Entity->fPauseCount = 0.0f;
    p_Entity->ubIsPaused = 1;
    p_Entity->fPauseTime = fTime * ENGINE_FPS / 1000.0f;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_CollidedWith(ENTITY_T* p_Source, ENTITY_T* p_Destination) {
    float fDistance = sqrtf(
        (p_Destination->CenterPoint.fX - p_Source->CenterPoint.fX) *
        (p_Destination->CenterPoint.fX - p_Source->CenterPoint.fX) +
        (p_Destination->CenterPoint.fY - p_Source->CenterPoint.fY) *
        (p_Destination->CenterPoint.fY - p_Source->CenterPoint.fY)
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (fDistance <= p_Source->fRadius + COLLISION_BUFFER + p_Destination->fRadius + COLLISION_BUFFER) {
        p_Source->ubIsInMotion = 0;
        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_WithinPoint(ENTITY_T* p_Inquirer, FPOINT_T Location) {
    if (
        Location.fX >= p_Inquirer->Location.fX &&
        Location.fX <= p_Inquirer->Location.fX + p_Inquirer->Size.fX &&
        Location.fY >= p_Inquirer->Location.fY &&
        Location.fY <= p_Inquirer->Location.fY + p_Inquirer->Size.fY
        ) {
        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_MoveTo(ENTITY_T* p_Source, ENTITY_T* p_Destination, GLOBALS_T* p_Globals) {
    ENTITY_MoveToPoint(p_Source, p_Destination->CenterPoint, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_MoveToPoint(ENTITY_T* p_Source, FPOINT_T CenterPoint, GLOBALS_T* p_Globals) {
    p_Source->ubIsInMotion = 1;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the absolute destination data. This is the major vector.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Source->MajorDestinationCenterPoint = CenterPoint;
    p_Source->MajorVector.fX = CenterPoint.fX - p_Source->CenterPoint.fX;
    p_Source->MajorVector.fY = CenterPoint.fY - p_Source->CenterPoint.fY;
    p_Source->MajorUnitVector.fX = p_Source->MajorVector.fX / sqrt(
        p_Source->MajorVector.fX * p_Source->MajorVector.fX +
        p_Source->MajorVector.fY * p_Source->MajorVector.fY
    );
    p_Source->MajorUnitVector.fY = p_Source->MajorVector.fY / sqrt(
        p_Source->MajorVector.fX * p_Source->MajorVector.fX +
        p_Source->MajorVector.fY * p_Source->MajorVector.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the path-finding destination data. These are the minor vectors needed to reach the major vector.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ENTITY_FindMinorVector(p_Source, p_Globals);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Future:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// - Extract a smaller subset of obstacles, by creating a new list of objects that lie upon the primary vector.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_FindMinorVector(ENTITY_T* p_Source, GLOBALS_T* p_Globals) {
    FPOINT_T CurrentPoint = p_Source->CenterPoint;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minor vector will initially equal the major vector. What if nothing is in the way?
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Source->MinorDestinationCenterPoint = p_Source->MajorDestinationCenterPoint;
    p_Source->MinorVector.fX = p_Source->MinorDestinationCenterPoint.fX - CurrentPoint.fX;
    p_Source->MinorVector.fY = p_Source->MinorDestinationCenterPoint.fY - CurrentPoint.fY;
    p_Source->MinorUnitVector.fX = p_Source->MinorVector.fX / sqrt(
        p_Source->MinorVector.fX * p_Source->MinorVector.fX +
        p_Source->MinorVector.fY * p_Source->MinorVector.fY
    );
    p_Source->MinorUnitVector.fY = p_Source->MinorVector.fY / sqrt(
        p_Source->MinorVector.fX * p_Source->MinorVector.fX +
        p_Source->MinorVector.fY * p_Source->MinorVector.fY
    );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Iterate along the vector to see if I need to change my course.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (
        abs(p_Source->MinorDestinationCenterPoint.fX - CurrentPoint.fX) >= SUFFICIENTLY_CLOSE ||
        abs(p_Source->MinorDestinationCenterPoint.fY - CurrentPoint.fY) >= SUFFICIENTLY_CLOSE
        ) {
        if (ENTITY_WithinPoint(p_Source, CurrentPoint)) {
            CurrentPoint.fX += p_Source->MinorUnitVector.fX;
            CurrentPoint.fY += p_Source->MinorUnitVector.fY;
            continue;
        }
        else {
            ENTITY_T* p_Current = p_Globals->p_RootEntity;
            while (p_Current && p_Current->ubIsObstacle) {
                if (ENTITY_WithinPoint(p_Current, CurrentPoint)) {
                    if (ENTITY_WithinPoint(p_Current, p_Source->MajorDestinationCenterPoint)) {
                        return;
                    }
                    else {
                        p_Source->MinorDestinationCenterPoint = ENTITY_MinorVectorHead(p_Source, p_Current, p_Globals);
                        /////////////////////////////////////////////////////////////////////////////////////////////////////
                        p_Source->MinorVector.fX = p_Source->MinorDestinationCenterPoint.fX - p_Source->CenterPoint.fX;
                        p_Source->MinorVector.fY = p_Source->MinorDestinationCenterPoint.fY - p_Source->CenterPoint.fY;
                        p_Source->MinorUnitVector.fX = p_Source->MinorVector.fX / sqrt(
                            p_Source->MinorVector.fX * p_Source->MinorVector.fX +
                            p_Source->MinorVector.fY * p_Source->MinorVector.fY
                        );
                        p_Source->MinorUnitVector.fY = p_Source->MinorVector.fY / sqrt(
                            p_Source->MinorVector.fX * p_Source->MinorVector.fX +
                            p_Source->MinorVector.fY * p_Source->MinorVector.fY
                        );
                        return;
                    }
                }
                else {
                    p_Current = (ENTITY_T*)p_Current->p_Next;
                }
            }
        }
        CurrentPoint.fX += p_Source->MinorUnitVector.fX;
        CurrentPoint.fY += p_Source->MinorUnitVector.fY;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPOINT_T __cdecl ENTITY_MinorVectorHead(ENTITY_T* p_Source, ENTITY_T* p_Current, GLOBALS_T* p_Globals) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Entities are rectangular.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FPOINT_T CornerPoints[4];
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CornerPoints[0].fX = p_Current->CenterPoint.fX - p_Current->HalfSize.fX - p_Source->HalfSize.fX;
    CornerPoints[0].fY = p_Current->CenterPoint.fY - p_Current->HalfSize.fY - p_Source->HalfSize.fY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Top right.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CornerPoints[1].fX = p_Current->CenterPoint.fX + p_Current->HalfSize.fX + p_Source->HalfSize.fX;
    CornerPoints[1].fY = p_Current->CenterPoint.fY - p_Current->HalfSize.fY - p_Source->HalfSize.fY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bottom left.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CornerPoints[2].fX = p_Current->CenterPoint.fX - p_Current->HalfSize.fX - p_Source->HalfSize.fX;
    CornerPoints[2].fY = p_Current->CenterPoint.fY + p_Current->HalfSize.fY + p_Source->HalfSize.fY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bottom right.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CornerPoints[3].fX = p_Current->CenterPoint.fX + p_Current->HalfSize.fX + p_Source->HalfSize.fX;
    CornerPoints[3].fY = p_Current->CenterPoint.fY + p_Current->HalfSize.fY + p_Source->HalfSize.fY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the distances to the points, and order the points from closest to farthest from the major destination.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubI;
    float fDistances[4];
    for (ubI = 0; ubI < 4; ubI++) {
        fDistances[ubI] = sqrtf(
            (CornerPoints[ubI].fX - p_Source->MajorDestinationCenterPoint.fX) *
            (CornerPoints[ubI].fX - p_Source->MajorDestinationCenterPoint.fX) +
            (CornerPoints[ubI].fY - p_Source->MajorDestinationCenterPoint.fY) *
            (CornerPoints[ubI].fY - p_Source->MajorDestinationCenterPoint.fY)
        );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UINT8 ubJ;
    for (ubI = 0; ubI < 4; ubI++) {
        for (ubJ = 0; ubJ < 3; ubJ++) {
            if (fDistances[ubJ] > fDistances[ubJ + 1]) {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Swap the distances.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                float fHolder = fDistances[ubJ + 1];
                fDistances[ubJ + 1] = fDistances[ubJ];
                fDistances[ubJ] = fHolder;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Swap the coordinates.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                FPOINT_T Holder = CornerPoints[ubJ + 1];
                CornerPoints[ubJ + 1] = CornerPoints[ubJ];
                CornerPoints[ubJ] = Holder;
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate bisections.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Points are ordered to distances from closest to farthest from the major destination point.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (ubI = 0; ubI < 4; ubI++) {
        UINT8 ubIsBisection = 0;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FPOINT_T Vector;
        FPOINT_T UnitVector;
        FPOINT_T EndPosition = CornerPoints[ubI];
        FPOINT_T CurrentPoint = p_Source->CenterPoint;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Vector.fX = EndPosition.fX - CurrentPoint.fX;
        Vector.fY = EndPosition.fY - CurrentPoint.fY;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        UnitVector.fX = Vector.fX / sqrt(Vector.fX * Vector.fX + Vector.fY * Vector.fY);
        UnitVector.fY = Vector.fY / sqrt(Vector.fX * Vector.fX + Vector.fY * Vector.fY);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (
            abs(EndPosition.fX - CurrentPoint.fX) >= SUFFICIENTLY_CLOSE ||
            abs(EndPosition.fY - CurrentPoint.fY) >= SUFFICIENTLY_CLOSE
            ) {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Bisection has occured. This vector is unreachable.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ENTITY_WithinPoint(p_Current, CurrentPoint)) {
                ubIsBisection = 1;
                break;
            }
            CurrentPoint.fX += UnitVector.fX;
            CurrentPoint.fY += UnitVector.fY;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This is the correct head to travel to, but it may contain additional obstacles.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (!ubIsBisection) {
            CurrentPoint = p_Source->CenterPoint;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            EndPosition = CornerPoints[ubI];
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Vector.fX = EndPosition.fX - CurrentPoint.fX;
            Vector.fY = EndPosition.fY - CurrentPoint.fY;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            UnitVector.fX = Vector.fX / sqrt(Vector.fX * Vector.fX + Vector.fY * Vector.fY);
            UnitVector.fY = Vector.fY / sqrt(Vector.fX * Vector.fX + Vector.fY * Vector.fY);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            while (
                abs(EndPosition.fX - CurrentPoint.fX) >= SUFFICIENTLY_CLOSE ||
                abs(EndPosition.fY - CurrentPoint.fY) >= SUFFICIENTLY_CLOSE
                ) {
                if (ENTITY_WithinPoint(p_Source, CurrentPoint)) {
                    CurrentPoint.fX += UnitVector.fX;
                    CurrentPoint.fY += UnitVector.fY;
                    continue;
                }
                else {
                    ENTITY_T* p_NewObstacle = p_Globals->p_RootEntity;
                    while (p_NewObstacle && p_NewObstacle->ubIsObstacle) {
                        if (ENTITY_WithinPoint(p_NewObstacle, CurrentPoint)) {
                            return ENTITY_MinorVectorHead(p_Source, p_NewObstacle, p_Globals);
                        }
                        p_NewObstacle = (ENTITY_T*)p_NewObstacle->p_Next;
                    }
                }
                CurrentPoint.fX += UnitVector.fX;
                CurrentPoint.fY += UnitVector.fY;
            }
            return CornerPoints[ubI];
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Should not usually get here. This means the entity was ordered to move while already inside an obstacle.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return CornerPoints[0];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_UpdatePosition(ENTITY_T* p_Entity, GLOBALS_T* p_Globals) {
    p_Entity->Location.fX += p_Entity->MinorUnitVector.fX * p_Entity->MovementSpeed.fX;
    p_Entity->Location.fY += p_Entity->MinorUnitVector.fY * p_Entity->MovementSpeed.fY;
    p_Entity->CenterPoint.fX = p_Entity->Location.fX + p_Entity->HalfSize.fX;
    p_Entity->CenterPoint.fY = p_Entity->Location.fY + p_Entity->HalfSize.fY;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Minor vectors...
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (
        abs(p_Entity->CenterPoint.fX - p_Entity->MinorDestinationCenterPoint.fX) <= SUFFICIENTLY_CLOSE &&
        abs(p_Entity->CenterPoint.fY - p_Entity->MinorDestinationCenterPoint.fY) <= SUFFICIENTLY_CLOSE
        ) {
        p_Entity->CenterPoint = p_Entity->MinorDestinationCenterPoint;
        p_Entity->Location.fX = p_Entity->MinorDestinationCenterPoint.fX - p_Entity->HalfSize.fX;
        p_Entity->Location.fY = p_Entity->MinorDestinationCenterPoint.fY - p_Entity->HalfSize.fY;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Major vector...
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (
            abs(p_Entity->CenterPoint.fX - p_Entity->MajorDestinationCenterPoint.fX) <= SUFFICIENTLY_CLOSE &&
            abs(p_Entity->CenterPoint.fY - p_Entity->MajorDestinationCenterPoint.fY) <= SUFFICIENTLY_CLOSE
            ) {
            p_Entity->CenterPoint = p_Entity->MajorDestinationCenterPoint;
            p_Entity->Location.fX = p_Entity->MajorDestinationCenterPoint.fX - p_Entity->HalfSize.fX;
            p_Entity->Location.fY = p_Entity->MajorDestinationCenterPoint.fY - p_Entity->HalfSize.fY;
            p_Entity->ubIsInMotion = 0;
        }
        else {
            ENTITY_FindMinorVector(p_Entity, p_Globals);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function will redefine the manufacturing statistics when an entity is deleted.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Redefine(USHORT usType, GLOBALS_T* p_Globals) {
    switch (usType) {
    case ENTITY_WORKER: {
        p_Globals->iCurrentSupply -= SUPPLY_USED_WORKER;
        break;
    }
    case ENTITY_COMMAND: {
        p_Globals->iTotalSupply -= SUPPLY_FROM_COMMAND;
        break;
    }
    case ENTITY_SUPPLY: {
        p_Globals->iTotalSupply -= SUPPLY_FROM_DEPOT;
        break;
    }
    case ENTITY_MINERAL: {
        break;
    }
    case ENTITY_REFINERY: {
        break;
    }
    default: {
        printf("ENTITY_Redefine(): Unknown entity type.\n");
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteAll(GLOBALS_T* p_Globals) {
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        ENTITY_T* p_Temp = p_Current;
        p_Current = (ENTITY_T*)p_Current->p_Next;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Re-calculate manufacturing statistics.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENTITY_Redefine(p_Temp->usType, p_Globals);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        p_Temp->ubIsAlive = 0;
        free(p_Temp);
        p_Globals->iRunningHeap -= sizeof(ENTITY_T);
    }
    p_Globals->p_RootEntity = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteSelected(GLOBALS_T* p_Globals) {
    ENTITY_T* p_Previous = NULL;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        if (p_Current->ubIsSelected) {
            if (p_Current == p_Globals->p_RootEntity) {
                p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current = p_Globals->p_RootEntity;
                continue;
            }
            else {
                ENTITY_T* p_Ahead = (ENTITY_T*)p_Current->p_Next;
                p_Previous->p_Next = (ENTITY_T*)p_Ahead;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current = p_Ahead;
            }
        }
        else {
            p_Previous = p_Current;
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleting one specific entity. When it is found, break out of the loop. Quick sort and binary search could be beneficial.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteSpecific(ENTITY_T* p_Entity, GLOBALS_T* p_Globals) {
    ENTITY_T* p_Previous = NULL;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        if (p_Current == p_Entity) {
            if (p_Current == p_Globals->p_RootEntity) {
                p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                break;
            }
            else {
                p_Previous->p_Next = (ENTITY_T*)p_Current->p_Next;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                break;
            }
        }
        else {
            p_Previous = p_Current;
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteEntityType(USHORT usType, GLOBALS_T* p_Globals) {
    ENTITY_T* p_Previous = NULL;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        if (p_Current->usType == usType) {
            if (p_Current == p_Globals->p_RootEntity) {
                p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current = p_Globals->p_RootEntity;
                continue;
            }
            else {
                ENTITY_T* p_Ahead = (ENTITY_T*)p_Current->p_Next;
                p_Previous->p_Next = (ENTITY_T*)p_Ahead;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Re-calculate manufacturing statistics.
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ENTITY_Redefine(p_Current->usType, p_Globals);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current->ubIsAlive = 0;
                free(p_Current);
                p_Globals->iRunningHeap -= sizeof(ENTITY_T);
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                p_Current = p_Ahead;
            }
        }
        else {
            p_Previous = p_Current;
            p_Current = (ENTITY_T*)p_Current->p_Next;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Using 'goto' statements in this function because I wanted to sort different entity types in one central place.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_SortToFront(USHORT usCondition, GLOBALS_T* p_Globals) {
    ENTITY_T* p_Previous = NULL;
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    while (p_Current) {
        switch (usCondition) {
        case WORKERS_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->usType == ENTITY_WORKER) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        case MINERALS_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->usType == ENTITY_MINERAL) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        case COMMANDS_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->usType == ENTITY_COMMAND) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        case SUPPLIES_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->usType == ENTITY_SUPPLY) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        case REFINERIES_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->usType == ENTITY_REFINERY) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        case OBSTACLES_TO_FRONT: {
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Current->ubIsObstacle) {
                goto Sort;
            }
            else {
                goto Next;
            }
        }
        default: {
            printf("ENTITY_SortToFront(): Invalid condition.\n");
            return;
        }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sort: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (p_Current == p_Globals->p_RootEntity) {
            p_Previous = p_Current;
            p_Current = (ENTITY_T*)p_Current->p_Next;
            continue;
        }
        else {
            ENTITY_T* p_Ahead = (ENTITY_T*)p_Current->p_Next;
            ENTITY_T* p_SavedRoot = p_Globals->p_RootEntity;
            p_Globals->p_RootEntity = p_Current;
            p_Current->p_Next = (ENTITY_T*)p_SavedRoot;
            p_Previous->p_Next = (ENTITY_T*)p_Ahead;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // To make sorting linear.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            p_Current = p_Ahead;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // To prevent fall-through.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            continue;
        }
        }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Next: {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    p_Previous = p_Current;
    p_Current = (ENTITY_T*)p_Current->p_Next;
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_PrintList(GLOBALS_T* p_Globals) {
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    printf("--------------------\n");
    while (p_Current) {
        switch (p_Current->usType) {
        case ENTITY_WORKER: {
            printf("Type: ENTITY_WORKER\n");
            break;
        }
        case ENTITY_MINERAL: {
            printf("Type: ENTITY_MINERAL\n");
            break;
        }
        case ENTITY_COMMAND: {
            printf("Type: ENTITY_COMMAND\n");
            break;
        }
        case ENTITY_SUPPLY: {
            printf("Type: ENTITY_SUPPLY\n");
            break;
        }
        case ENTITY_REFINERY: {
            printf("Type: ENTITY_REFINERY\n");
            break;
        }
        default: {
            printf("ENTITY_PrintList(): Unknown entity type.\n");
        }
        }
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
    printf("--------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_PrintClosestEntitiesList(AI_CLOSEST_T* p_ClosestEntities) {
    printf("--------------------\n");
    if (p_ClosestEntities) {
        USHORT usI = 0;
        while (p_ClosestEntities[usI].p_Entity) {
            ENTITY_T* p_Entity = p_ClosestEntities[usI].p_Entity;
            printf("Closest Entity [%d]: ID: %d, P: %p, Distance: %0.2f\n", usI, p_Entity->usId, p_Entity, p_ClosestEntities[usI].fDistance);
            usI++;
        }
    }
    printf("--------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Animate(ENTITY_T* p_Entity, ASSETS_T* p_Assets) {
    switch (p_Entity->usType) {
    case ENTITY_SUPPLY: {
        switch (p_Entity->usState) {
        case ANIMATE_SUPPLY_NORMAL: {
            p_Entity->usCurrentFrame++;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // I know there is only 6 frames in this animation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Entity->usCurrentFrame > 5) {
                p_Entity->usCurrentFrame = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (p_Entity->usCurrentFrame) {
            case 0: { p_Entity->p_Picture = &p_Assets->Supply[0]; break; } // Original.
            case 1: { p_Entity->p_Picture = &p_Assets->Supply[1]; break; } // Top light bright.
            case 2: { p_Entity->p_Picture = &p_Assets->Supply[2]; break; } // Top light brighter.
            case 3: { p_Entity->p_Picture = &p_Assets->Supply[3]; break; } // Top light brightest.
            case 4: { p_Entity->p_Picture = &p_Assets->Supply[4]; break; } // Top light brighter.
            case 5: { p_Entity->p_Picture = &p_Assets->Supply[5]; break; } // Top light bright.
            default: {
                printf("ENTITY_Animate(): Unknown frame.\n");
            }
            }
            break;
        }
        default: {
            printf("ENTITY_Animate(): Unknown entity state.\n");
        }
        }
        break;
    }
    case ENTITY_REFINERY: {
        break;
    }
    case ENTITY_WORKER: {
        switch (p_Entity->usState) {
        case ANIMATE_WORKER_NORMAL: { p_Entity->p_Picture = &p_Assets->Worker[0]; break; } // Normal.
        case ANIMATE_WORKER_MINERALS: { p_Entity->p_Picture = &p_Assets->Worker[1]; break; } // Holding minerals.
        case ANIMATE_WORKER_GAS: { p_Entity->p_Picture = &p_Assets->Worker[2]; break; } // Holding gas.
        default: {
            printf("ENTITY_Animate(): Unknown entity state. %d\n", p_Entity->usState);
        }
        }
        break;
    }
    case ENTITY_MINERAL: {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Animation frame depends on the percentage of minerals remaining.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ((p_Entity->iMineralCount / (float)MINERALS_ON_CREATION) > 0.75f) {
            p_Entity->p_Picture = &p_Assets->Mineral[0];
        }
        else if ((p_Entity->iMineralCount / (float)MINERALS_ON_CREATION) > 0.50f) {
            p_Entity->p_Picture = &p_Assets->Mineral[1];
        }
        else if ((p_Entity->iMineralCount / (float)MINERALS_ON_CREATION) > 0.25f) {
            p_Entity->p_Picture = &p_Assets->Mineral[2];
        }
        else if ((p_Entity->iMineralCount / (float)MINERALS_ON_CREATION) > 0.0f) {
            p_Entity->p_Picture = &p_Assets->Mineral[3];
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The mineral count is <= 0.0f.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else {
            p_Entity->p_Picture = &p_Assets->Mineral[4];
        }
        break;
    }
    case ENTITY_COMMAND: {
        switch (p_Entity->usState) {
        case ANIMATE_COMMAND_NORMAL: {
            p_Entity->p_Picture = &p_Assets->Command[0]; // Normal.
            break;
        }
        case ANIMATE_COMMAND_CREATING: {
            p_Entity->usCurrentFrame++;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // I know there is only 24 frames in this animation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Entity->usCurrentFrame > 23) {
                p_Entity->usCurrentFrame = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (p_Entity->usCurrentFrame) {
            case 0: { p_Entity->p_Picture = &p_Assets->Command[0];  break; } // Original.
            case 1: { p_Entity->p_Picture = &p_Assets->Command[6];  break; } // East light bright.
            case 2: { p_Entity->p_Picture = &p_Assets->Command[7];  break; } // East light brighter.
            case 3: { p_Entity->p_Picture = &p_Assets->Command[8];  break; } // East light brightest.
            case 4: { p_Entity->p_Picture = &p_Assets->Command[9];  break; } // East light brighter.
            case 5: { p_Entity->p_Picture = &p_Assets->Command[10]; break; } // East light bright.
            case 6: { p_Entity->p_Picture = &p_Assets->Command[0];  break; } // Original.
            case 7: { p_Entity->p_Picture = &p_Assets->Command[11]; break; } // South light bright.
            case 8: { p_Entity->p_Picture = &p_Assets->Command[12]; break; } // South light brighter.
            case 9: { p_Entity->p_Picture = &p_Assets->Command[13]; break; } // South light brightest,
            case 10: { p_Entity->p_Picture = &p_Assets->Command[14]; break; } // South light brighter.
            case 11: { p_Entity->p_Picture = &p_Assets->Command[15]; break; } // South light bright.
            case 12: { p_Entity->p_Picture = &p_Assets->Command[0];  break; } // Original.
            case 13: { p_Entity->p_Picture = &p_Assets->Command[16]; break; } // West light bright.
            case 14: { p_Entity->p_Picture = &p_Assets->Command[17]; break; } // West light brighter.
            case 15: { p_Entity->p_Picture = &p_Assets->Command[18]; break; } // West light brightest.
            case 16: { p_Entity->p_Picture = &p_Assets->Command[19]; break; } // West light brighter.
            case 17: { p_Entity->p_Picture = &p_Assets->Command[20]; break; } // West light bright,
            case 18: { p_Entity->p_Picture = &p_Assets->Command[0];  break; } // Original.
            case 19: { p_Entity->p_Picture = &p_Assets->Command[21]; break; } // Center light bright.
            case 20: { p_Entity->p_Picture = &p_Assets->Command[22]; break; } // Center light brighter.
            case 21: { p_Entity->p_Picture = &p_Assets->Command[23]; break; } // Center light brightest
            case 22: { p_Entity->p_Picture = &p_Assets->Command[24]; break; } // Center light brighter.
            case 23: { p_Entity->p_Picture = &p_Assets->Command[25]; break; } // Center light bright.
            default: {
                printf("ENTITY_Animate(): Unknown frame.\n");
            }
            }
            break;
        }
        case ANIMATE_COMMAND_RECEIVE: {
            p_Entity->usCurrentFrame++;
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // I know there is only 6 frames in this animation.
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (p_Entity->usCurrentFrame > 5) {
                p_Entity->usCurrentFrame = 0;
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            switch (p_Entity->usCurrentFrame) {
            case 0: { p_Entity->p_Picture = &p_Assets->Command[0]; break; } // Original.
            case 1: { p_Entity->p_Picture = &p_Assets->Command[1]; break; } // All lights bright.
            case 2: { p_Entity->p_Picture = &p_Assets->Command[2]; break; } // All lights brighter.
            case 3: { p_Entity->p_Picture = &p_Assets->Command[3]; break; } // All lights brightest.
            case 4: { p_Entity->p_Picture = &p_Assets->Command[4]; break; } // All lights brighter.
            case 5: { p_Entity->p_Picture = &p_Assets->Command[5]; break; } // All lights bright.
            default: {
                printf("ENTITY_Animate(): Unknown frame.\n");
            }
            }
            break;
        }
        default: {
            printf("ENTITY_Animate(): Unknown entity state.\n");
        }
        }
        break;
    }
    default: {
        printf("ENTITY_Animate(): Unknown entity type.\n");
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SELECTED_COUNT_T __cdecl ENTITY_GetSelectedEntityCounts(GLOBALS_T* p_Globals) {
    SELECTED_COUNT_T SelectedCount = { 0, 0 };
    ENTITY_T* p_Current = p_Globals->p_RootEntity;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Loop through all of the entities and count the workers selected.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (p_Current) {
        if (p_Current->ubIsSelected) {
            switch (p_Current->usType) {
            case ENTITY_WORKER: {
                SelectedCount.usSelectedWorkersCount++;
                SelectedCount.usSelectedAllCount++;
                break;
            }
            default: {
                SelectedCount.usSelectedOthersCount++;
                SelectedCount.usSelectedAllCount++;
                break;
            }
            }
        }
        p_Current = (ENTITY_T*)p_Current->p_Next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return SelectedCount;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
