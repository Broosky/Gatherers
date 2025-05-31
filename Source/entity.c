/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Headers/ai.h"
#include "../Headers/assets.h"
#include "../Headers/constants.h"
#include "../Headers/entity.h"
#include "../Headers/enums.h"
#include "../Headers/globals.h"
#include "../Headers/heap_allocator.h"
#include "../Headers/log.h"
#include "../Headers/message.h"
#include "../Headers/misc.h"
#include "../Headers/path_finding.h"
#include "../Headers/Renderer/renderer.h"
#include "../Headers/settings.h"
#include "../Headers/Windows/windows_main.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Zero(ENTITY_T* p_Entity) {
	ZeroMemory(p_Entity, sizeof(ENTITY_T));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Create(FPOINT_T Location, ENTITY_TYPE_T eType, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, LOG_T* p_Log, RENDERER_T* p_Renderer, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If there are no manufacturing restrictions, and no entity overlap, allow creation.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (ENTITY_BuildIntersectionCheck(eType, p_Assets, p_Globals, p_Log, p_Renderer, p_Constants) && ENTITY_BuildResourcesCheck(eType, p_Globals, p_Log, p_Settings, p_Constants)) {
		ENTITY_T* p_Entity = (ENTITY_T*)HEAP_ALLOCATOR_Alloc(p_Globals->p_EntityAllocator, p_Log);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!p_Entity) {
			MISC_WriteOut(p_Log, LOG_SEVERITY_FATAL, "ENTITY_Create(): Allocation failed.\n");
			UINT8 ubDiscard = MAIN_FailFast(p_Globals, p_Log);
			return;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->usId = p_Globals->usIdStamp++;
		p_Entity->eType = eType;
		p_Entity->CenterPoint = Location;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// To avoid lines being drawn from each entity's center point to the client (0, 0) coordinate.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->MinorDestinationCenterPoint = p_Entity->CenterPoint;
		p_Entity->MajorDestinationCenterPoint = p_Entity->CenterPoint;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->ubIsAlive = 1;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (p_Entity->eType) {
		case ENTITY_TYPE_WORKER: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Entity->p_Picture = &p_Assets->Worker[0];
			p_Entity->MovementSpeed = (FDELTA_T){ p_Settings->fWorkerMoveSpeed, p_Settings->fWorkerMoveSpeed };
			p_Entity->ubIsMovable = 1;
			p_Entity->usState = ANIMATION_STATE_WORKER_NORMAL;
			break;
		}
		case ENTITY_TYPE_MINERAL: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Entity->p_Picture = &p_Assets->Mineral[0];
			p_Entity->iMineralCount = p_Settings->uiMineralsOnCreation;
			p_Entity->MovementSpeed = (FDELTA_T){ p_Settings->fMineralMoveSpeed, p_Settings->fMineralMoveSpeed };
			p_Entity->ubIsObstacle = 1;
			break;
		}
		case ENTITY_TYPE_COMMAND: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Entity->p_Picture = &p_Assets->Command[0];
			p_Entity->MovementSpeed = (FDELTA_T){ p_Settings->fCommandMoveSpeed, p_Settings->fCommandMoveSpeed };
			p_Entity->ubIsObstacle = 1;
			p_Entity->usState = ANIMATION_STATE_COMMAND_CREATING;
			break;
		}
		case ENTITY_TYPE_SUPPLY: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Entity->p_Picture = &p_Assets->Supply[0];
			p_Entity->MovementSpeed = (FDELTA_T){ p_Settings->fSupplyMoveSpeed, p_Settings->fSupplyMoveSpeed };
			p_Entity->ubIsObstacle = 1;
			p_Entity->usState = ANIMATION_STATE_SUPPLY_NORMAL;
			break;
		}
		case ENTITY_TYPE_REFINERY: {
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Entity->p_Picture = &p_Assets->Refinery[0];
			p_Entity->iGasCount = p_Settings->uiGasOnCreation;
			p_Entity->MovementSpeed = (FDELTA_T){ p_Settings->fRefineryMoveSpeed, p_Settings->fRefineryMoveSpeed };
			p_Entity->ubIsObstacle = 1;
			break;
		}
		default: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_Create(): Unknown entity type.\n");
		}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->Size.fDx = (*p_Entity->p_Picture).Bitmap.bmWidth;
		p_Entity->Size.fDy = (*p_Entity->p_Picture).Bitmap.bmHeight;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->HalfSize.fDx = (*p_Entity->p_Picture).Bitmap.bmWidth >> 1;
		p_Entity->HalfSize.fDy = (*p_Entity->p_Picture).Bitmap.bmHeight >> 1;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->Location.fX = p_Entity->CenterPoint.fX - p_Entity->HalfSize.fDx;
		p_Entity->Location.fY = p_Entity->CenterPoint.fY - p_Entity->HalfSize.fDy;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Entity->fRadius = (p_Entity->HalfSize.fDx + p_Entity->HalfSize.fDy) / 2.0f;
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
		ENTITY_SortToFront(SORT_ORDER_OBSTACLES_TO_FRONT, p_Globals, p_Log);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_BuildResourcesCheck(ENTITY_TYPE_T eType, GLOBALS_T* p_Globals, LOG_T* p_Log, SETTINGS_T* p_Settings, CONSTANTS_T* p_Constants) {
	const FPOINT_T Position = { 15.0f, 85.0f };
	ENTITY_REQUIREMENTS_T EntityRequirements = { 0 };
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (eType) {
	case ENTITY_TYPE_COMMAND: {
		EntityRequirements = (ENTITY_REQUIREMENTS_T){
			// Cost
			p_Settings->usCostOfCommandMinerals,
			p_Settings->usCostOfCommandGas,
			// Supply
			0,
			p_Settings->usSupplyFromCommand,
			p_Settings->usSupplyFromCommand
		};
		break;
	}
	case ENTITY_TYPE_WORKER: {
		EntityRequirements = (ENTITY_REQUIREMENTS_T){
			// Cost
			p_Settings->usCostOfWorkerMinerals,
			p_Settings->usCostOfWorkerGas,
			// Supply
			p_Settings->usSupplyUsedWorker,
			0,
			-p_Settings->usSupplyUsedWorker
		};
		break;
	}
	case ENTITY_TYPE_SUPPLY: {
		EntityRequirements = (ENTITY_REQUIREMENTS_T){
			// Cost
			p_Settings->usCostOfDepotMinerals,
			p_Settings->usCostOfDepotGas,
			// Supply
			0,
			p_Settings->usSupplyFromDepot,
			p_Settings->usSupplyFromDepot
		};
		break;
	}
	case ENTITY_TYPE_REFINERY: {
		EntityRequirements = (ENTITY_REQUIREMENTS_T){
			// Cost
			p_Settings->usCostOfRefineryMinerals,
			p_Settings->usCostOfRefineryGas,
			// Supply
			0,
			0,
			0
		};
		break;
	}
	case ENTITY_TYPE_MINERAL: {
		return 1;
	}
	default:
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_Restrict(): Unknown entity type.\n");
		return 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (EntityRequirements.iSupplyNeeded > (p_Globals->iTotalSupply - p_Globals->iCurrentSupply)) {
		MESSAGE_Create("Not enough supplies!", Position, MESSAGE_TYPE_GENERAL_WARNING, p_Globals, p_Constants, p_Log);
		return 0;
	}
	if (p_Globals->iMineralCount < EntityRequirements.iMinMinerals) {
		MESSAGE_Create("Not enough minerals!", Position, MESSAGE_TYPE_GENERAL_WARNING, p_Globals, p_Constants, p_Log);
		return 0;
	}
	if (p_Globals->iGasCount < EntityRequirements.iMinGas) {
		MESSAGE_Create("Not enough gas!", Position, MESSAGE_TYPE_GENERAL_WARNING, p_Globals, p_Constants, p_Log);
		return 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	p_Globals->iMineralCount -= EntityRequirements.iMinMinerals;
	p_Globals->iGasCount -= EntityRequirements.iMinGas;
	p_Globals->iCurrentSupply += (EntityRequirements.iSupplyDelta < 0) ? -EntityRequirements.iSupplyDelta : 0;
	p_Globals->iTotalSupply += (EntityRequirements.iSupplyDelta > 0) ? EntityRequirements.iSupplyDelta : 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_BuildIntersectionCheck(ENTITY_TYPE_T eType, ASSETS_T* p_Assets, GLOBALS_T* p_Globals, LOG_T* p_Log, RENDERER_T* p_Renderer, CONSTANTS_T* p_Constants) {
	FDELTA_T Size = { 0.0f, 0.0f };
	FPOINT_T Position = { 15.0f, 85.0f };
	FDELTA_T AdditionalBuffer = { 0.0f, 0.0f };
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RECT ClientArea = p_Renderer->GetClientArea(p_Renderer);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (eType) {
	case ENTITY_TYPE_WORKER: {
		Size.fDx = p_Assets->Worker[0].Bitmap.bmWidth;
		Size.fDy = p_Assets->Worker[0].Bitmap.bmHeight;
		break;
	}
	case ENTITY_TYPE_MINERAL: {
		Size.fDx = p_Assets->Mineral[0].Bitmap.bmWidth;
		Size.fDy = p_Assets->Mineral[0].Bitmap.bmHeight;
		break;
	}
	case ENTITY_TYPE_COMMAND: {
		Size.fDx = p_Assets->Command[0].Bitmap.bmWidth;
		Size.fDy = p_Assets->Command[0].Bitmap.bmHeight;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdditionalBuffer.fDx = p_Assets->Command[0].Bitmap.bmWidth >> 1;
		AdditionalBuffer.fDy = p_Assets->Command[0].Bitmap.bmHeight >> 1;
		break;
	}
	case ENTITY_TYPE_SUPPLY: {
		Size.fDx = p_Assets->Supply[0].Bitmap.bmWidth;
		Size.fDy = p_Assets->Supply[0].Bitmap.bmHeight;
		break;
	}
	case ENTITY_TYPE_REFINERY: {
		Size.fDx = p_Assets->Refinery[0].Bitmap.bmWidth;
		Size.fDy = p_Assets->Refinery[0].Bitmap.bmHeight;
		break;
	}
	default: {
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_Overlap(): Unknown entity type.\n");
	}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FPOINT_T EntityALocation = {
		p_Globals->Mouse.iX - (Size.fDx / 2.0f + AdditionalBuffer.fDx),
		p_Globals->Mouse.iY - (Size.fDy / 2.0f + AdditionalBuffer.fDy)
	};
	FDELTA_T EntityADelta = {
		EntityALocation.fX + Size.fDx + AdditionalBuffer.fDx * 2.0f,
		EntityALocation.fY + Size.fDy + AdditionalBuffer.fDy * 2.0f
	};
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check for screen out-of-bounds.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (EntityALocation.fX < 0.0f || EntityADelta.fDx > ClientArea.right ||
		EntityALocation.fY < 0.0f || EntityADelta.fDy > ClientArea.bottom) {
		MESSAGE_Create("You can't build there!", Position, MESSAGE_TYPE_GENERAL_WARNING, p_Globals, p_Constants, p_Log);
		return 0;
	}
	else {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Check for intersections with other entities.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IDELTA_T BufferWorkerLess = {
			p_Assets->Worker[0].Bitmap.bmWidth,
			p_Assets->Worker[0].Bitmap.bmHeight
		};
		IDELTA_T BufferWorkerMore = {
			BufferWorkerLess.iDx << 1,
			BufferWorkerLess.iDy << 1,
		};
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IDELTA_T BufferCommandLess = {
			p_Assets->Command[0].Bitmap.bmWidth >> 1,
			p_Assets->Command[0].Bitmap.bmHeight >> 1
		};
		IDELTA_T BufferCommandMore = {
			p_Assets->Command[0].Bitmap.bmWidth,
			p_Assets->Command[0].Bitmap.bmHeight
		};
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		FPOINT_T EntityBLocation = { 0.0f, 0.0f };
		FDELTA_T EntityBDelta = { 0.0f, 0.0f };
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ENTITY_T* p_Current = p_Globals->p_RootEntity;
		while (p_Current) {
			if (p_Current->eType == ENTITY_TYPE_COMMAND) {
				EntityBLocation.fX = p_Current->Location.fX - BufferWorkerLess.iDx - BufferCommandLess.iDx;
				EntityBLocation.fY = p_Current->Location.fY - BufferWorkerLess.iDy - BufferCommandLess.iDy;

				EntityBDelta.fDx = EntityBLocation.fX + p_Current->Size.fDx + BufferWorkerMore.iDx + BufferCommandMore.iDx;
				EntityBDelta.fDy = EntityBLocation.fY + p_Current->Size.fDy + BufferWorkerMore.iDy + BufferCommandMore.iDy;
			}
			else {
				EntityBLocation.fX = p_Current->Location.fX - BufferWorkerLess.iDx;
				EntityBLocation.fY = p_Current->Location.fY - BufferWorkerLess.iDy;

				EntityBDelta.fDx = EntityBLocation.fX + p_Current->Size.fDx + BufferWorkerMore.iDx;
				EntityBDelta.fDy = EntityBLocation.fY + p_Current->Size.fDy + BufferWorkerMore.iDy;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if ((EntityALocation.fX < EntityBDelta.fDx) &&
				(EntityADelta.fDx > EntityBLocation.fX) &&
				(EntityALocation.fY < EntityBDelta.fDy) &&
				(EntityADelta.fDy > EntityBLocation.fY)
				) {
				MESSAGE_Create("You can't build there!", Position, MESSAGE_TYPE_GENERAL_WARNING, p_Globals, p_Constants, p_Log);
				return 0;
			}
			p_Current = (ENTITY_T*)p_Current->p_Next;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// No intersections exist.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Pause(ENTITY_T* p_Entity, float fTime, SETTINGS_T* p_Settings) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// When the entity is paused, reset the pause counter for each call.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	p_Entity->fPauseCount = 0.0f;
	p_Entity->ubIsPaused = 1;
	p_Entity->fPauseTime = fTime * p_Settings->fEngineFPS / 1000.0f;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_CollidedWith(ENTITY_T* p_Source, ENTITY_T* p_Destination, SETTINGS_T* p_Settings) {
	UINT8 ubHasCollided = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float fDistance = PATH_CalculateDistance(p_Source, p_Destination);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (fDistance <= p_Source->fRadius + p_Settings->fCollisionBuffer + p_Destination->fRadius + p_Settings->fCollisionBuffer) {
		p_Source->ubIsInMotion = 0;
		ubHasCollided = 1;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return ubHasCollided;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function will redefine the manufacturing statistics when an entity is deleted.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Redefine(ENTITY_TYPE_T eType, GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	switch (eType) {
	case ENTITY_TYPE_WORKER: {
		p_Globals->iCurrentSupply -= p_Settings->usSupplyUsedWorker;
		break;
	}
	case ENTITY_TYPE_COMMAND: {
		p_Globals->iTotalSupply -= p_Settings->usSupplyFromCommand;
		break;
	}
	case ENTITY_TYPE_SUPPLY: {
		p_Globals->iTotalSupply -= p_Settings->usSupplyFromDepot;
		break;
	}
	case ENTITY_TYPE_MINERAL: {
		break;
	}
	case ENTITY_TYPE_REFINERY: {
		break;
	}
	default: {
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_Redefine(): Unknown entity type.\n");
	}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_SelectAll(GLOBALS_T* p_Globals) {
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		p_Current->ubIsSelected = 1;
		p_Current = (ENTITY_T*)p_Current->p_Next;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteAll(GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		ENTITY_T* p_Temp = p_Current;
		p_Current = (ENTITY_T*)p_Current->p_Next;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Re-calculate manufacturing statistics.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ENTITY_Redefine(p_Temp->eType, p_Globals, p_Settings, p_Log);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		p_Temp->ubIsAlive = 0;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Temp, p_Log);
	}
	p_Globals->p_RootEntity = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_DeleteSelected(GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	ENTITY_T* p_Previous = NULL;
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		if (p_Current->ubIsSelected) {
			if (p_Current == p_Globals->p_RootEntity) {
				p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Re-calculate manufacturing statistics.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
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
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
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
void __cdecl ENTITY_DeleteSpecific(ENTITY_T* p_Entity, GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	ENTITY_T* p_Previous = NULL;
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		if (p_Current == p_Entity) {
			if (p_Current == p_Globals->p_RootEntity) {
				p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Re-calculate manufacturing statistics.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
				break;
			}
			else {
				p_Previous->p_Next = (ENTITY_T*)p_Current->p_Next;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Re-calculate manufacturing statistics.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
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
void __cdecl ENTITY_DeleteEntityType(ENTITY_TYPE_T eType, GLOBALS_T* p_Globals, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	ENTITY_T* p_Previous = NULL;
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		if (p_Current->eType == eType) {
			if (p_Current == p_Globals->p_RootEntity) {
				p_Globals->p_RootEntity = (ENTITY_T*)p_Current->p_Next;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Re-calculate manufacturing statistics.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
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
				ENTITY_Redefine(p_Current->eType, p_Globals, p_Settings, p_Log);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				p_Current->ubIsAlive = 0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				HEAP_ALLOCATOR_Free(p_Globals->p_EntityAllocator, p_Current, p_Log);
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
// Double pointer for original pointer manupulations (advancing).
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_Sort(ENTITY_T** pp_Previous, ENTITY_T** pp_Current, GLOBALS_T* p_Globals) {
	ENTITY_T* p_Current = *pp_Current;
	ENTITY_T* p_Previous = *pp_Previous;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (p_Current == p_Globals->p_RootEntity) {
		*pp_Previous = p_Current;
		*pp_Current = (ENTITY_T*)p_Current->p_Next;
	}
	else {
		ENTITY_T* p_Ahead = (ENTITY_T*)p_Current->p_Next;
		ENTITY_T* p_SavedRoot = p_Globals->p_RootEntity;
		p_Globals->p_RootEntity = p_Current;
		p_Current->p_Next = (ENTITY_T*)p_SavedRoot;
		p_Previous->p_Next = (ENTITY_T*)p_Ahead;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Adjusting ahead to make sorting linear.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		*pp_Current = p_Ahead;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_SkipSort(ENTITY_T** pp_Previous, ENTITY_T** pp_Current) {
	*pp_Previous = *pp_Current;
	*pp_Current = (ENTITY_T*)(*pp_Current)->p_Next;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 __cdecl ENTITY_ConsiderSortToFront(ENTITY_T* p_Entity, SORT_ORDER_T eCondition, LOG_T* p_Log) {
	UINT8 ubShouldSort = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (eCondition) {
	case SORT_ORDER_WORKERS_TO_FRONT: {
		ubShouldSort = p_Entity->eType == ENTITY_TYPE_WORKER;
		break;
	}
	case SORT_ORDER_MINERALS_TO_FRONT: {
		ubShouldSort = p_Entity->eType == ENTITY_TYPE_MINERAL;
		break;
	}
	case SORT_ORDER_COMMANDS_TO_FRONT: {
		ubShouldSort = p_Entity->eType == ENTITY_TYPE_COMMAND;
		break;
	}
	case SORT_ORDER_SUPPLIES_TO_FRONT: {
		ubShouldSort = p_Entity->eType == ENTITY_TYPE_SUPPLY;
		break;
	}
	case SORT_ORDER_REFINERIES_TO_FRONT: {
		ubShouldSort = p_Entity->eType == ENTITY_TYPE_REFINERY;
		break;
	}
	case SORT_ORDER_OBSTACLES_TO_FRONT: {
		ubShouldSort = p_Entity->ubIsObstacle;
		break;
	}
	default:
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_ConsiderSortToFront(): Unknown case.\n");
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return ubShouldSort;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_SortToFront(SORT_ORDER_T eCondition, GLOBALS_T* p_Globals, LOG_T* p_Log) {
	ENTITY_T* p_Previous = NULL;
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		if (ENTITY_ConsiderSortToFront(p_Current, eCondition, p_Log)) {
			ENTITY_Sort(&p_Previous, &p_Current, p_Globals);
		}
		else {
			ENTITY_SkipSort(&p_Previous, &p_Current);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	p_Globals->eEntitySortState = eCondition;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_PrintList(GLOBALS_T* p_Globals, LOG_T* p_Log) {
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
	while (p_Current) {
		switch (p_Current->eType) {
		case ENTITY_TYPE_WORKER: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Type: ENTITY_TYPE_WORKER\n");
			break;
		}
		case ENTITY_TYPE_MINERAL: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Type: ENTITY_TYPE_MINERAL\n");
			break;
		}
		case ENTITY_TYPE_COMMAND: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Type: ENTITY_TYPE_COMMAND\n");
			break;
		}
		case ENTITY_TYPE_SUPPLY: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Type: ENTITY_TYPE_SUPPLY\n");
			break;
		}
		case ENTITY_TYPE_REFINERY: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "Type: ENTITY_TYPE_REFINERY\n");
			break;
		}
		default: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_PrintList(): Unknown entity type.\n");
		}
		}
		p_Current = (ENTITY_T*)p_Current->p_Next;
	}
	MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_PrintClosestEntitiesList(AI_CLOSEST_T* p_ClosestEntities, LOG_T* p_Log) {
	MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
	if (p_ClosestEntities) {
		USHORT usI = 0;
		while (p_ClosestEntities[usI].p_Entity) {
			ENTITY_T* p_Entity = p_ClosestEntities[usI].p_Entity;
			MISC_WriteOutParams(p_Log, LOG_SEVERITY_TRACE, "Closest Entity [%hu]: ID: %hu, P: %p, Distance: %0.2f\n", usI, p_Entity->usId, p_Entity, p_ClosestEntities[usI].fDistance);
			usI++;
		}
	}
	MISC_WriteOut(p_Log, LOG_SEVERITY_TRACE, "--------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_HandleAnimation(ENTITY_T* p_Entity, ASSETS_T* p_Assets, SETTINGS_T* p_Settings, LOG_T* p_Log) {
	switch (p_Entity->eType) {
	case ENTITY_TYPE_SUPPLY: {
		ENTITY_AnimateSupply(p_Entity, p_Log);
		break;
	}
	case ENTITY_TYPE_REFINERY: {
		ENTITY_AnimateRefinery(p_Entity, p_Log);
		break;
	}
	case ENTITY_TYPE_WORKER: {
		ENTITY_AnimateWorker(p_Entity, p_Log);
		break;
	}
	case ENTITY_TYPE_MINERAL: {
		ENTITY_AnimateMineral(p_Entity, p_Log);
		break;
	}
	case ENTITY_TYPE_COMMAND: {
		ENTITY_AnimateCommand(p_Entity, p_Log);
		break;
	}
	default: {
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_HandleAnimation(): Unknown entity type.\n");
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
			switch (p_Current->eType) {
			case ENTITY_TYPE_WORKER: {
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
void __cdecl ENTITY_ReceiveHotReload(SETTINGS_T* p_Settings, GLOBALS_T* p_Globals, LOG_T* p_Log) {
	ENTITY_T* p_Current = p_Globals->p_RootEntity;
	while (p_Current) {
		switch (p_Current->eType) {
		case ENTITY_TYPE_WORKER: {
			float fDivisor = 1.0f;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (p_Current->ubIsCarryingMinerals) {
				fDivisor = p_Settings->fWorkerMineralDivisor;
			}
			else if (p_Current->ubIsCarryingGas) {
				fDivisor = p_Settings->fWorkerGasDivisor;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			p_Current->MovementSpeed = (FDELTA_T){ p_Settings->fWorkerMoveSpeed / fDivisor, p_Settings->fWorkerMoveSpeed / fDivisor };
			break;
		}
		case ENTITY_TYPE_MINERAL: {
			p_Current->MovementSpeed = (FDELTA_T){ p_Settings->fMineralMoveSpeed, p_Settings->fMineralMoveSpeed };
			break;
		}
		case ENTITY_TYPE_COMMAND: {
			p_Current->MovementSpeed = (FDELTA_T){ p_Settings->fCommandMoveSpeed, p_Settings->fCommandMoveSpeed };
			break;
		}
		case ENTITY_TYPE_SUPPLY: {
			p_Current->MovementSpeed = (FDELTA_T){ p_Settings->fSupplyMoveSpeed, p_Settings->fSupplyMoveSpeed };
			break;
		}
		case ENTITY_TYPE_REFINERY: {
			p_Current->MovementSpeed = (FDELTA_T){ p_Settings->fRefineryMoveSpeed, p_Settings->fRefineryMoveSpeed };
			break;
		}
		default: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_ReceiveHotReload(): Unknown entity type.\n");
		}
		}
		p_Current = (ENTITY_T*)p_Current->p_Next;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_AnimateSupply(ENTITY_T* p_Entity, LOG_T* p_Log) {
	switch (p_Entity->usState) {
	case ANIMATION_STATE_SUPPLY_NORMAL: {
		p_Entity->usCurrentFrame++;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (p_Entity->usCurrentFrame > 5) {
			p_Entity->usCurrentFrame = 0;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (p_Entity->usCurrentFrame) {
		case 0: { p_Entity->p_Picture = &p_Assets->Supply[0]; break; } // Original.
		case 1: { p_Entity->p_Picture = &p_Assets->Supply[1]; break; } // Top light bright.
		case 2: { p_Entity->p_Picture = &p_Assets->Supply[2]; break; } // Top light brighter.
		case 3: { p_Entity->p_Picture = &p_Assets->Supply[3]; break; } // Top light brightest.
		case 4: { p_Entity->p_Picture = &p_Assets->Supply[4]; break; } // Top light brighter.
		case 5: { p_Entity->p_Picture = &p_Assets->Supply[5]; break; } // Top light bright.
		default: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateSupply(): Unknown frame.\n");
		}
		}
		break;
	}
	default: {
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateSupply(): Unknown entity state.\n");
	}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_AnimateRefinery(ENTITY_T* p_Entity, LOG_T* p_Log) {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_AnimateWorker(ENTITY_T* p_Entity, LOG_T* p_Log) {
	switch (p_Entity->usState) {
	case ANIMATION_STATE_WORKER_NORMAL: { p_Entity->p_Picture = &p_Assets->Worker[0]; break; } // Normal.
	case ANIMATION_STATE_WORKER_MINERALS: { p_Entity->p_Picture = &p_Assets->Worker[1]; break; } // Holding minerals.
	case ANIMATION_STATE_WORKER_GAS: { p_Entity->p_Picture = &p_Assets->Worker[2]; break; } // Holding gas.
	default: {
		MISC_WriteOutParams(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateWorker(): Unknown entity state. %hu\n", p_Entity->usState);
	}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_AnimateMineral(ENTITY_T* p_Entity, LOG_T* p_Log) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Animation frame depends on the percentage of minerals remaining.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if ((p_Entity->iMineralCount / (float)p_Settings->uiMineralsOnCreation) > 0.75f) {
		p_Entity->p_Picture = &p_Assets->Mineral[0];
	}
	else if ((p_Entity->iMineralCount / (float)p_Settings->uiMineralsOnCreation) > 0.50f) {
		p_Entity->p_Picture = &p_Assets->Mineral[1];
	}
	else if ((p_Entity->iMineralCount / (float)p_Settings->uiMineralsOnCreation) > 0.25f) {
		p_Entity->p_Picture = &p_Assets->Mineral[2];
	}
	else if ((p_Entity->iMineralCount / (float)p_Settings->uiMineralsOnCreation) > 0.0f) {
		p_Entity->p_Picture = &p_Assets->Mineral[3];
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// The mineral count is <= 0.0f.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else {
		p_Entity->p_Picture = &p_Assets->Mineral[4];
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl ENTITY_AnimateCommand(ENTITY_T* p_Entity, LOG_T* p_Log) {
	switch (p_Entity->usState) {
	case ANIMATION_STATE_COMMAND_NORMAL: {
		p_Entity->p_Picture = &p_Assets->Command[0]; // Normal.
		break;
	}
	case ANIMATION_STATE_COMMAND_CREATING: {
		p_Entity->usCurrentFrame++;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (p_Entity->usCurrentFrame > 23) {
			p_Entity->usCurrentFrame = 0;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateCommand(): Unknown frame.\n");
		}
		}
		break;
	}
	case ANIMATION_STATE_COMMAND_RECEIVE: {
		p_Entity->usCurrentFrame++;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (p_Entity->usCurrentFrame > 5) {
			p_Entity->usCurrentFrame = 0;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (p_Entity->usCurrentFrame) {
		case 0: { p_Entity->p_Picture = &p_Assets->Command[0]; break; } // Original.
		case 1: { p_Entity->p_Picture = &p_Assets->Command[1]; break; } // All lights bright.
		case 2: { p_Entity->p_Picture = &p_Assets->Command[2]; break; } // All lights brighter.
		case 3: { p_Entity->p_Picture = &p_Assets->Command[3]; break; } // All lights brightest.
		case 4: { p_Entity->p_Picture = &p_Assets->Command[4]; break; } // All lights brighter.
		case 5: { p_Entity->p_Picture = &p_Assets->Command[5]; break; } // All lights bright.
		default: {
			MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateCommand(): Unknown frame.\n");
		}
		}
		break;
	}
	default: {
		MISC_WriteOut(p_Log, LOG_SEVERITY_WARNING, "ENTITY_AnimateCommand(): Unknown entity state.\n");
	}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
