/**
*   Loads needed DirectX9 functions dynamically.
*   Allows an application to handle missing DirectX runtime instead of just not running at all.
*   I've only put the functions used by GWToolbox in here - add more as needed
*/
#include "stdafx.h"

#include <d3dx9_dynamic.h>
#include <Logger.h>
#include <Path.h>

HMODULE d3dx9Module = nullptr;

// Function addresses used within this project
typedef D3DXMATRIX* (WINAPI* D3DXMatrixTranslation_pt)(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z);
D3DXMatrixTranslation_pt D3DXMatrixTranslationPtr = nullptr;

typedef D3DXMATRIX* (WINAPI* D3DXMatrixRotationZ_pt)(D3DXMATRIX* pOut, FLOAT Angle);
D3DXMatrixRotationZ_pt D3DXMatrixRotationZPtr = nullptr;

typedef D3DXMATRIX* (WINAPI* D3DXMatrixScaling_pt)(D3DXMATRIX* pOut, FLOAT sx, FLOAT sy, FLOAT sz);
D3DXMatrixScaling_pt D3DXMatrixScalingPtr = nullptr;

typedef D3DXMATRIX* (WINAPI* D3DXMatrixMultiply_pt)(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM1, CONST D3DXMATRIX* pM2);
D3DXMatrixMultiply_pt D3DXMatrixMultiplyPtr = nullptr;

typedef HRESULT(WINAPI* D3DXCreateTextureFromFileW_pt)(LPDIRECT3DDEVICE9 pDevice,LPCWSTR pSrcFile,LPDIRECT3DTEXTURE9* ppTexture);
D3DXCreateTextureFromFileW_pt D3DXCreateTextureFromFileWPtr = nullptr;

typedef HRESULT(WINAPI* D3DXCreateTextureFromFileExW_pt)(
    LPDIRECT3DDEVICE9         pDevice,
    LPCWSTR                   pSrcFile,
    UINT                      Width,
    UINT                      Height,
    UINT                      MipLevels,
    DWORD                     Usage,
    D3DFORMAT                 Format,
    D3DPOOL                   Pool,
    DWORD                     Filter,
    DWORD                     MipFilter,
    D3DCOLOR                  ColorKey,
    D3DXIMAGE_INFO* pSrcInfo,
    PALETTEENTRY* pPalette,
    LPDIRECT3DTEXTURE9* ppTexture);
D3DXCreateTextureFromFileExW_pt D3DXCreateTextureFromFileExWPtr = nullptr;

typedef HRESULT(WINAPI* D3DXCreateTextureFromResourceA_pt)(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture);
D3DXCreateTextureFromResourceA_pt D3DXCreateTextureFromResourceAPtr = nullptr;

typedef HRESULT(WINAPI* D3DXCreateTextureFromResourceExA_pt)(
    LPDIRECT3DDEVICE9         pDevice,
    HMODULE                   hSrcModule,
    LPCSTR                    pSrcResource,
    UINT                      Width,
    UINT                      Height,
    UINT                      MipLevels,
    DWORD                     Usage,
    D3DFORMAT                 Format,
    D3DPOOL                   Pool,
    DWORD                     Filter,
    DWORD                     MipFilter,
    D3DCOLOR                  ColorKey,
    D3DXIMAGE_INFO* pSrcInfo,
    PALETTEENTRY* pPalette,
    LPDIRECT3DTEXTURE9* ppTexture);
D3DXCreateTextureFromResourceExA_pt D3DXCreateTextureFromResourceExAPtr = nullptr;

#define return_on_false( expr ) if ( !(expr) ) { return false; }

static bool MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName, LPVOID lpProc)
{
    LPVOID* lpOut = reinterpret_cast<LPVOID*>(lpProc);
    *lpOut = nullptr;
    if (hModule == nullptr)
        return false;
    FARPROC Proc = GetProcAddress(hModule, lpProcName);
    if (Proc == nullptr)
        return false;
    *lpOut = (LPVOID)((uintptr_t)Proc);
    return true;
}

// Loads the d3dx9 module, then assigns the addresses needed.
bool Loadd3dx9(){
    static bool IsLoaded = false;
    if (IsLoaded) {
        return true; // Only load the dll once
    }
    IsLoaded = true;
    char d3dx9name[MAX_PATH];

    std::filesystem::path dllpath;
    if (!PathGetDocumentsPath(dllpath, L"GWToolboxpp\\d3dx9_43.dll") || !std::filesystem::exists(dllpath)) {
        d3dx9Module = LoadLibrary("d3dx9_43.dll");
    } else {
        d3dx9Module = LoadLibraryW(dllpath.wstring().c_str());
    }
    if (!d3dx9Module) {
        return false;
    }

    Log::Log("Loaded DirectX module successfully: %s\n", d3dx9name);

    // Add function definitions as applicable
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXMatrixTranslation", &D3DXMatrixTranslationPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXMatrixRotationZ", &D3DXMatrixRotationZPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXMatrixScaling", &D3DXMatrixScalingPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXMatrixMultiply", &D3DXMatrixMultiplyPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXCreateTextureFromFileW", &D3DXCreateTextureFromFileWPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXCreateTextureFromFileExW", &D3DXCreateTextureFromFileExWPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXCreateTextureFromResourceA", &D3DXCreateTextureFromResourceAPtr));
    return_on_false(MyGetProcAddress(d3dx9Module, "D3DXCreateTextureFromResourceExA", &D3DXCreateTextureFromResourceExAPtr));
    return true;
}
D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z) {
    Loadd3dx9();
    return D3DXMatrixTranslationPtr(pOut, x, y, z);
}
D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle) {
    Loadd3dx9();
    return D3DXMatrixRotationZPtr(pOut,Angle);
}
D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, FLOAT sx, FLOAT sy, FLOAT sz) {
    Loadd3dx9();
    return D3DXMatrixScalingPtr(pOut, sx, sy, sz);
}
D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM1, CONST D3DXMATRIX* pM2) {
    Loadd3dx9();
    return D3DXMatrixMultiplyPtr(pOut, pM1, pM2);
}
HRESULT WINAPI D3DXCreateTextureFromFileW(LPDIRECT3DDEVICE9 pDevice,LPCWSTR pSrcFile,LPDIRECT3DTEXTURE9* ppTexture) {
    Loadd3dx9();
    return D3DXCreateTextureFromFileWPtr(pDevice, pSrcFile, ppTexture);
}
HRESULT WINAPI D3DXCreateTextureFromFileExW(
    LPDIRECT3DDEVICE9         pDevice,
    LPCWSTR                   pSrcFile,
    UINT                      Width,
    UINT                      Height,
    UINT                      MipLevels,
    DWORD                     Usage,
    D3DFORMAT                 Format,
    D3DPOOL                   Pool,
    DWORD                     Filter,
    DWORD                     MipFilter,
    D3DCOLOR                  ColorKey,
    D3DXIMAGE_INFO* pSrcInfo,
    PALETTEENTRY* pPalette,
    LPDIRECT3DTEXTURE9* ppTexture) {
    Loadd3dx9();
    return D3DXCreateTextureFromFileExWPtr(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
}



HRESULT WINAPI D3DXCreateTextureFromResourceA(LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCSTR pSrcResource, LPDIRECT3DTEXTURE9* ppTexture) {
    Loadd3dx9();
    return D3DXCreateTextureFromResourceAPtr(pDevice, hSrcModule, pSrcResource, ppTexture);
}
HRESULT WINAPI
D3DXCreateTextureFromResourceExA(
    LPDIRECT3DDEVICE9         pDevice,
    HMODULE                   hSrcModule,
    LPCSTR                    pSrcResource,
    UINT                      Width,
    UINT                      Height,
    UINT                      MipLevels,
    DWORD                     Usage,
    D3DFORMAT                 Format,
    D3DPOOL                   Pool,
    DWORD                     Filter,
    DWORD                     MipFilter,
    D3DCOLOR                  ColorKey,
    D3DXIMAGE_INFO* pSrcInfo,
    PALETTEENTRY* pPalette,
    LPDIRECT3DTEXTURE9* ppTexture) {
    Loadd3dx9();
    return D3DXCreateTextureFromResourceExAPtr(pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
}