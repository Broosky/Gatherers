/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ENUMS_H_
#define _ENUMS_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum LOG_SEVERITY {
    LOG_SEVERITY_UNKNOWN = 0,
    LOG_SEVERITY_TRACE,
    LOG_SEVERITY_INFO,
    LOG_SEVERITY_WARNING,
    LOG_SEVERITY_ERROR,
    LOG_SEVERITY_FATAL
} LOG_SEVERITY_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SORT_ORDER {
    SORT_ORDER_UNKNOWN = 0,
    SORT_ORDER_WORKERS_TO_FRONT,
    SORT_ORDER_MINERALS_TO_FRONT,
    SORT_ORDER_COMMANDS_TO_FRONT,
    SORT_ORDER_SUPPLIES_TO_FRONT,
    SORT_ORDER_REFINERIES_TO_FRONT,
    SORT_ORDER_OBSTACLES_TO_FRONT
} SORT_ORDER_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum ENTITY_TYPE {
    ENTITY_TYPE_UNKNOWN = 0,
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_WORKER,
    ENTITY_TYPE_MINERAL,
    ENTITY_TYPE_COMMAND,
    ENTITY_TYPE_SUPPLY,
    ENTITY_TYPE_REFINERY,
    ENTITY_TYPE_OBSTACLE
} ENTITY_TYPE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum ANIMATION_STATE {
    ANIMATION_STATE_UNKNOWN = 0,
    ANIMATION_STATE_WORKER_NORMAL,
    ANIMATION_STATE_WORKER_MINERALS,
    ANIMATION_STATE_WORKER_GAS,
    ANIMATION_STATE_COMMAND_NORMAL,
    ANIMATION_STATE_COMMAND_RECEIVE,
    ANIMATION_STATE_COMMAND_CREATING,
    ANIMATION_STATE_SUPPLY_NORMAL
} ANIMATION_STATE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum TRANSFORM_TYPE {
    TRANSFORM_TYPE_UNKNOWN = 0,
    TRANSFORM_TYPE_ROTATE,
    TRANSFORM_TYPE_SCALE
} TRANSFORM_TYPE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum MESSAGE_TYPE {
    MESSAGE_TYPE_UNKNOWN = 0,
    MESSAGE_TYPE_GAIN_MINERALS,
    MESSAGE_TYPE_GAIN_GAS,
    MESSAGE_TYPE_BONUS_MINERALS,
    MESSAGE_TYPE_BONUS_GAS,
    MESSAGE_TYPE_GENERAL_WARNING
} MESSAGE_TYPE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum COMMAND_CARD {
    COMMAND_CARD_UNKNOWN = 0,
    COMMAND_CARD_BLANK
} COMMAND_CARD_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum PROCESS_TYPE {
    PROCESS_TYPE_UNKNOWN = 0,
    PROCESS_TYPE_ENTITY,
    PROCESS_TYPE_MESSAGE
} PROCESS_TYPE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum RENDERER_TYPE {
    RENDERER_TYPE_WIREFRAME = 0,
    RENDERER_TYPE_SINGLE_BUFFER,
    RENDERER_TYPE_DOUBLE_BUFFER,
    RENDERER_TYPE_SDL,
    RENDERER_TYPE_DIRECT2D,
    // Must be after the valid renderer types.
    RENDERER_TYPE_COUNT,
    RENDERER_TYPE_UNKNOWN
} RENDERER_TYPE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum TRANSLATION_ZONE {
    // Enum value sequencing matters.
    // Corner cases first.
    TRANSLATION_ZONE_TOP_LEFT = 0,
    TRANSLATION_ZONE_TOP_RIGHT,
    TRANSLATION_ZONE_BOTTOM_RIGHT,
    TRANSLATION_ZONE_BOTTOM_LEFT,
    // Border cases.
    TRANSLATION_ZONE_TOP,
    TRANSLATION_ZONE_RIGHT,
    TRANSLATION_ZONE_BOTTOM,
    TRANSLATION_ZONE_LEFT,
    // Must be after the valid translation zones.
    TRANSLATION_ZONE_COUNT,
    TRANSLATION_ZONE_UNKNOWN,
} TRANSLATION_ZONE_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum BLITTER_COLOR {
    BLITTER_COLOR_LIGHT_BLUE,
    BLITTER_COLOR_BLUE,
    BLITTER_COLOR_GREEN,
    BLITTER_COLOR_ORANGE,
    BLITTER_COLOR_PURPLE,
    BLITTER_COLOR_PINK,
    BLITTER_COLOR_RED,
    BLITTER_COLOR_WHITE,
    BLITTER_COLOR_YELLOW,
    // Must be after the valid blitter colours.
    BLITTER_COLOR_COUNT
} BLITTER_COLOR_T;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
