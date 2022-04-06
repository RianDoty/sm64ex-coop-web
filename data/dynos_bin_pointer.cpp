#include "dynos.cpp.h"
extern "C" {
#include "behavior_table.h"
#include "levels/scripts.h"
}

#define define_pointer(ptr) (const void *) #ptr, (const void *) ptr
static const void *sDynosPointers[] = {
    define_pointer(level_main_scripts_entry),
    define_pointer(script_func_global_1),
    define_pointer(script_func_global_2),
    define_pointer(script_func_global_3),
    define_pointer(script_func_global_4),
    define_pointer(script_func_global_5),
    define_pointer(script_func_global_6),
    define_pointer(script_func_global_7),
    define_pointer(script_func_global_8),
    define_pointer(script_func_global_9),
    define_pointer(script_func_global_10),
    define_pointer(script_func_global_11),
    define_pointer(script_func_global_12),
    define_pointer(script_func_global_13),
    define_pointer(script_func_global_14),
    define_pointer(script_func_global_15),
    define_pointer(script_func_global_16),
    define_pointer(script_func_global_17),
    define_pointer(script_func_global_18),
};

  /////////////
 // Writing //
/////////////

typedef Pair<String, u32> PointerData;
static PointerData GetDataFromPointer(const void* aPtr, GfxData* aGfxData) {
    // Lights
    for (auto& _Node : aGfxData->mLights) {
        if (&_Node->mData->l[0] == aPtr) { // Light *, not Lights1 *
            return { _Node->mName, 1 };
        }
        if (&_Node->mData->a == aPtr) { // Ambient *, not Lights1 *
            return { _Node->mName, 2 };
        }
    }

    for (auto& _Node : aGfxData->mLightTs) {
        if (&_Node->mData->col[0] == aPtr) {
            return { _Node->mName, 1 };
        }
        if (&_Node->mData->colc[0] == aPtr) {
            return { _Node->mName, 2 };
        }
        if (&_Node->mData->dir[0] == aPtr) {
            return { _Node->mName, 3 };
        }
    }

    for (auto& _Node : aGfxData->mAmbientTs) {
        if (&_Node->mData->col[0] == aPtr) {
            return { _Node->mName, 1 };
        }
        if (&_Node->mData->colc[0] == aPtr) {
            return { _Node->mName, 2 };
        }
    }

    // Textures
    for (auto& _Node : aGfxData->mTextures) {
        if (_Node == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Texture Lists
    for (auto& _Node : aGfxData->mTextureLists) {
        if (_Node == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Display lists
    for (auto& _Node : aGfxData->mDisplayLists) {
        if (_Node == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Geo layouts
    for (auto& _Node : aGfxData->mGeoLayouts) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Collisions
    for (auto& _Node : aGfxData->mCollisions) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Level scripts
    for (auto& _Node : aGfxData->mLevelScripts) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Macro objects
    for (auto& _Node : aGfxData->mMacroObjects) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Trajectories
    for (auto& _Node : aGfxData->mTrajectories) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Movtexs
    for (auto& _Node : aGfxData->mMovtexs) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // MovtexQCs
    for (auto& _Node : aGfxData->mMovtexQCs) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Rooms
    for (auto& _Node : aGfxData->mRooms) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
    }

    // Behaviors
    enum BehaviorId id = get_id_from_vanilla_behavior((const BehaviorScript*) aPtr);
    if (id >= 0 && id < id_bhv_max_count) {
        return { get_behavior_name_from_id(id), 0 };
    }

    // Vanilla Actors
    s32 actorCount = DynOS_Geo_GetActorCount();
    for (s32 i = 0; i < actorCount; i++) {
        if (DynOS_Geo_IsCustomActor(i)) { break; }
        if (aPtr == DynOS_Geo_GetActorLayout(i)) {
            return { DynOS_Geo_GetActorName(i), 0 };
        }
    }

    // Vanilla Lvl Geos
    s32 lvlGeoCount = DynOS_Lvl_GetGeoCount();
    for (s32 i = 0; i < lvlGeoCount; i++) {
        if (aPtr == DynOS_Lvl_GetGeoLayout(i)) {
            return { DynOS_Lvl_GetGeoName(i), 0 };
        }
    }

    // Vanilla Pointers
    s32 pointerCount = (s32) (sizeof(sDynosPointers) / (2 * sizeof(sDynosPointers[0])));
    for (s32 i = 0; i < pointerCount; i++) {
        const char* ptrName = (const char*)sDynosPointers[i * 2 + 0];
        const void* ptr = sDynosPointers[i * 2 + 1];
        if (ptr == aPtr) {
            return { ptrName, 0 };
        }
    }

    // Vertices
    String _VtxArrayName = "";
    uintptr_t _VtxArrayStart = 0;
    for (auto& _Node : aGfxData->mVertices) {
        if (_Node->mData == aPtr) {
            return { _Node->mName, 0 };
        }
        if ((uintptr_t)_Node->mData <= (uintptr_t)aPtr &&
            (uintptr_t)_Node->mData >= _VtxArrayStart) {
            _VtxArrayName = _Node->mName;
            _VtxArrayStart = (uintptr_t)_Node->mData;
        }
    }
    return { _VtxArrayName, (u32)((const Vtx*)aPtr - (const Vtx*)_VtxArrayStart) };
}

void DynOS_Pointer_Lua_Write(FILE* aFile, u32 index, GfxData* aGfxData) {
    String& token = aGfxData->mLuaTokenList[index];
    WriteBytes<u32>(aFile, LUA_VAR_CODE);
    token.Write(aFile);
}

void DynOS_Pointer_Write(FILE* aFile, const void* aPtr, GfxData* aGfxData) {

    // NULL
    if (!aPtr) {
        WriteBytes<u32>(aFile, 0);
        return;
    }

    // Lua variable
    for (s32 i = 0; i < aGfxData->mLuaPointerList.Count(); i++) {
        if (aPtr == aGfxData->mLuaPointerList[i]) {
            u32 index = *((u32*)aPtr);
            String& token = aGfxData->mLuaTokenList[index];
            WriteBytes<u32>(aFile, LUA_VAR_CODE);
            token.Write(aFile);
            return;
        }
    }

    // Geo function
    s32 _GeoFunctionIndex = DynOS_Geo_GetFunctionIndex(aPtr);
    if (_GeoFunctionIndex != -1) {
        WriteBytes<u32>(aFile, FUNCTION_CODE);
        WriteBytes<s32>(aFile, _GeoFunctionIndex);
        return;
    }

    // Lvl function
    s32 _LvlFunctionIndex = DynOS_Lvl_GetFunctionIndex(aPtr);
    if (_LvlFunctionIndex != -1) {
        WriteBytes<u32>(aFile, FUNCTION_CODE);
        WriteBytes<s32>(aFile, _LvlFunctionIndex);
        return;
    }

    // Pointer
    PointerData _PtrData = GetDataFromPointer(aPtr, aGfxData);
    WriteBytes<u32>(aFile, POINTER_CODE);
    _PtrData.first.Write(aFile);
    WriteBytes<u32>(aFile, _PtrData.second);
}

  /////////////
 // Reading //
/////////////

static void *GetPointerFromData(GfxData *aGfxData, const String &aPtrName, u32 aPtrData) {

    // Lights
    for (auto& _Node : aGfxData->mLights) {
        if (_Node->mName == aPtrName) {
            if (aPtrData == 1) {
                return (void *) &_Node->mData->l[0];
            }
            if (aPtrData == 2) {
                return (void *) &_Node->mData->a;
            }
            sys_fatal("Unknown Light type: %u", aPtrData);
        }
    }

    for (auto& _Node : aGfxData->mLightTs) {
        if (_Node->mName == aPtrName) {
            if (aPtrData == 1) {
                return (void *) &_Node->mData->col[0];
            }
            if (aPtrData == 2) {
                return (void *) &_Node->mData->colc[0];
            }
            if (aPtrData == 3) {
                return (void *) &_Node->mData->dir[0];
            }
            sys_fatal("Unknown Light type: %u", aPtrData);
        }
    }

    for (auto& _Node : aGfxData->mAmbientTs) {
        if (_Node->mName == aPtrName) {
            if (aPtrData == 1) {
                return (void *) &_Node->mData->col[0];
            }
            if (aPtrData == 2) {
                return (void *) &_Node->mData->colc[0];
            }
            sys_fatal("Unknown Light type: %u", aPtrData);
        }
    }

    // Textures
    for (auto& _Node : aGfxData->mTextures) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node;
        }
    }

    // Texture Lists
    for (auto& _Node : aGfxData->mTextureLists) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node;
        }
    }

    // Display lists
    for (auto &_Node : aGfxData->mDisplayLists) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Geo layouts
    for (auto &_Node : aGfxData->mGeoLayouts) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Vertices
    for (auto &_Node : aGfxData->mVertices) {
        if (_Node->mName == aPtrName) {
            return (void *) (_Node->mData + aPtrData);
        }
    }

    // Collisions
    for (auto &_Node : aGfxData->mCollisions) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Level scripts
    for (auto &_Node : aGfxData->mLevelScripts) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Macro objects
    for (auto &_Node : aGfxData->mMacroObjects) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Trajectories
    for (auto &_Node : aGfxData->mTrajectories) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Movtexs
    for (auto &_Node : aGfxData->mMovtexs) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // MovtexQCs
    for (auto &_Node : aGfxData->mMovtexQCs) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Rooms
    for (auto &_Node : aGfxData->mRooms) {
        if (_Node->mName == aPtrName) {
            return (void *) _Node->mData;
        }
    }

    // Behaviors
    enum BehaviorId id = get_id_from_behavior_name(aPtrName.begin());
    if (id >= 0 && id < id_bhv_max_count) {
        return (void*)get_behavior_from_id(id);
    }

    // Vanilla Actors
    s32 actorCount = DynOS_Geo_GetActorCount();
    for (s32 i = 0; i < actorCount; i++) {
        if (DynOS_Geo_IsCustomActor(i)) { break; }
        if (!strcmp(aPtrName.begin(), DynOS_Geo_GetActorName(i))) {
            return (void*)DynOS_Geo_GetActorLayout(i);
        }
    }

    // Vanilla Lvl Geos
    s32 lvlGeoCount = DynOS_Lvl_GetGeoCount();
    for (s32 i = 0; i < lvlGeoCount; i++) {
        if (!strcmp(aPtrName.begin(), DynOS_Lvl_GetGeoName(i))) {
            return (void*)DynOS_Lvl_GetGeoLayout(i);
        }
    }

    // Vanilla Pointers
    s32 pointerCount = (s32) (sizeof(sDynosPointers) / (2 * sizeof(sDynosPointers[0])));
    for (s32 i = 0; i < pointerCount; i++) {
        const char* ptrName = (const char*)sDynosPointers[i * 2 + 0];
        const void* ptr = sDynosPointers[i * 2 + 1];
        if (!strcmp(aPtrName.begin(), ptrName)) {
            return (void*)ptr;
        }
    }

    // Error
    sys_fatal("Pointer not found: %s", aPtrName.begin());
    return NULL;
}

void *DynOS_Pointer_Load(FILE *aFile, GfxData *aGfxData, u32 aValue, bool isLvl) {

    // LUAV
    if (aValue == LUA_VAR_CODE) {
        String token; token.Read(aFile);
        u32 index = aGfxData->mLuaTokenList.Count();
        aGfxData->mLuaTokenList.Add(token);
        return aGfxData->mLuaTokenList[index].begin();
    }

    // FUNC
    if (aValue == FUNCTION_CODE) {
        s32 _FunctionIndex = ReadBytes<s32>(aFile);
        return isLvl
             ? DynOS_Lvl_GetFunctionPointerFromIndex(_FunctionIndex)
             : DynOS_Geo_GetFunctionPointerFromIndex(_FunctionIndex);
    }

    // PNTR
    if (aValue == POINTER_CODE) {
        String _PtrName; _PtrName.Read(aFile);
        u32   _PtrData = ReadBytes<u32>(aFile);
        return GetPointerFromData(aGfxData, _PtrName, _PtrData);
    }

    // Not a pointer
    return NULL;
}
