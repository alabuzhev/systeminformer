/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Authors:
 *
 *     jxy-s   2022
 *     dmex    2022
 *
 */

#ifndef POOLMON_H
#define POOLMON_H

typedef enum _POOLTAG_TREE_ITEM_TYPE
{
    TPOOLTAG_TREE_ITEM_TYPE_NONE,
    TPOOLTAG_TREE_ITEM_TYPE_OBJECT,
    TPOOLTAG_TREE_ITEM_TYPE_DRIVER,
} POOLTAG_TREE_ITEM_TYPE;

typedef struct _POOL_TAG_LIST_ENTRY
{
    LIST_ENTRY ListEntry;

    union
    {
        UCHAR Tag[4];
        ULONG TagUlong;
    };

    PPH_STRING BinaryNameString;
    PPH_STRING DescriptionString;
} POOL_TAG_LIST_ENTRY, *PPOOL_TAG_LIST_ENTRY;

typedef struct _POOL_ITEM
{
    ULONG TagUlong;
    WCHAR TagString[5];

    PPH_STRING BinaryNameString;
    PPH_STRING DescriptionString;
    POOLTAG_TREE_ITEM_TYPE Type;

    PH_UINT64_DELTA PagedAllocsDelta;
    PH_UINT64_DELTA PagedFreesDelta;
    PH_UINT64_DELTA PagedCurrentDelta;
    PH_UINT64_DELTA PagedTotalSizeDelta;
    PH_UINT64_DELTA NonPagedAllocsDelta;
    PH_UINT64_DELTA NonPagedFreesDelta;
    PH_UINT64_DELTA NonPagedCurrentDelta;
    PH_UINT64_DELTA NonPagedTotalSizeDelta;
} POOL_ITEM, *PPOOL_ITEM;

typedef enum _POOLTAG_TREE_COLUMN_ITEM_NAME
{
    TREE_COLUMN_ITEM_TAG,
    TREE_COLUMN_ITEM_DRIVER,
    TREE_COLUMN_ITEM_DESCRIPTION,
    TREE_COLUMN_ITEM_PAGEDALLOC,
    TREE_COLUMN_ITEM_PAGEDFREE,
    TREE_COLUMN_ITEM_PAGEDCURRENT,
    TREE_COLUMN_ITEM_PAGEDTOTAL,
    TREE_COLUMN_ITEM_NONPAGEDALLOC,
    TREE_COLUMN_ITEM_NONPAGEDFREE,
    TREE_COLUMN_ITEM_NONPAGEDCURRENT,
    TREE_COLUMN_ITEM_NONPAGEDTOTAL,
    TREE_COLUMN_ITEM_MAXIMUM
} POOLTAG_TREE_COLUMN_ITEM_NAME;

typedef struct _POOLTAG_ROOT_NODE
{
    PH_TREENEW_NODE Node;
    PH_STRINGREF TextCache[TREE_COLUMN_ITEM_MAXIMUM];

    ULONG TagUlong;
    PPOOL_ITEM PoolItem;

    PPH_STRING PagedAllocsDeltaString;
    PPH_STRING PagedFreesDeltaString;
    PPH_STRING PagedLiveDeltaString;
    PPH_STRING PagedCurrentDeltaString;
    PPH_STRING PagedTotalSizeDeltaString;
    PPH_STRING NonPagedAllocsDeltaString;
    PPH_STRING NonPagedFreesDeltaString;
    PPH_STRING NonPagedLiveDeltaString;
    PPH_STRING NonPagedCurrentDeltaString;
    PPH_STRING NonPagedTotalSizeDeltaString;
} POOLTAG_ROOT_NODE, *PPOOLTAG_ROOT_NODE;

typedef struct _POOLTAG_CONTEXT
{
    HWND ParentWindowHandle;
    HWND SearchboxHandle;
    HWND TreeNewHandle;
    PH_LAYOUT_MANAGER LayoutManager;
    ULONG ProcessesUpdatedCount;
    PH_CALLBACK_REGISTRATION ProcessesUpdatedCallbackRegistration;

    PPH_STRING SearchboxText;
    PH_TN_FILTER_SUPPORT FilterSupport;
    PPH_TN_FILTER_ENTRY TreeFilterEntry;

    PH_QUEUED_LOCK PoolTagListLock;
    PPH_LIST PoolTagDbList;
    PPH_HASHTABLE PoolTagDbHashtable;

    ULONG TreeNewSortColumn;
    PH_SORT_ORDER TreeNewSortOrder;
    PPH_HASHTABLE NodeHashtable;
    PPH_LIST NodeList;
    PPH_LIST NodeRootList;
} POOLTAG_CONTEXT, *PPOOLTAG_CONTEXT;

// pooldialog.c

typedef struct _BIGPOOLTAG_CONTEXT
{
    HWND WindowHandle;
    HWND ListViewHandle;
    PH_LAYOUT_MANAGER LayoutManager;

    union
    {
        UCHAR Tag[4];
        ULONG TagUlong;
    };
    WCHAR TagString[5];

} BIGPOOLTAG_CONTEXT, *PBIGPOOLTAG_CONTEXT;

VOID EtShowBigPoolDialog(
    _In_ PPOOL_ITEM PoolItem
    );

VOID EtLoadSettingsTreeList(
    _Inout_ PPOOLTAG_CONTEXT Context
    );

VOID EtSaveSettingsTreeList(
    _Inout_ PPOOLTAG_CONTEXT Context
    );

VOID EtInitializePoolTagTree(
    _Inout_ PPOOLTAG_CONTEXT Context
    );

VOID EtDeletePoolTagTree(
    _In_ PPOOLTAG_CONTEXT Context
    );

PPOOLTAG_ROOT_NODE EtFindPoolTagNode(
    _In_ PPOOLTAG_CONTEXT Context,
    _In_ ULONG PoolTag
    );

PPOOLTAG_ROOT_NODE EtAddPoolTagNode(
    _Inout_ PPOOLTAG_CONTEXT Context,
    _In_ PPOOL_ITEM PoolItem
    );

VOID EtUpdatePoolTagNode(
    _In_ PPOOLTAG_CONTEXT Context,
    _In_ PPOOLTAG_ROOT_NODE Node
    );

struct _PH_TN_FILTER_SUPPORT*
NTAPI
EtGetFilterSupportTreeList(
    VOID
    );

VOID EtCopyPoolTagTree(
    _In_ PPOOLTAG_CONTEXT Context
    );

PPOOLTAG_ROOT_NODE EtGetSelectedPoolTagNode(
    _In_ PPOOLTAG_CONTEXT Context
    );

// pooldb.c

VOID EtLoadPoolTagDatabase(
    _In_ PPOOLTAG_CONTEXT Context
    );

VOID EtFreePoolTagDatabase(
    _In_ PPOOLTAG_CONTEXT Context
    );

VOID EtUpdatePoolTagBinaryName(
    _In_ PPOOLTAG_CONTEXT Context,
    _In_ PPOOL_ITEM PoolEntry,
    _In_ ULONG TagUlong
    );

#endif