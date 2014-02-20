
#pragma once

#include "wgl.h"
#include <cstdlib>
#include <stdio.h>
#include <memory.h>

// If Windows.h hasn't been included, or if we're on a non-Windows platform
#ifndef _WINDEF_

    #include <errno.h>

    typedef uint8_t BYTE;
    typedef uint32_t  UINT;
    typedef uint32_t DWORD;
    typedef int errno_t;

    inline errno_t fopen_s(FILE** pFile, const char* filename, const char *mode)
    {
        (*pFile) = fopen(filename, mode);
        if (!*pFile) {
            return errno;
        }

        return 0;
    }

    inline size_t fread_s(void* buffer, size_t bufferSize, size_t elementSize, size_t count, FILE *stream)
    {
        return fread(buffer, elementSize, count, stream);
    }
    
#endif

#define dds_imax(_a, _b) (((_a) > (_b)) ? (_a) : (_b))

#ifndef MAKEFOURCC
#define MAKEFOURCC(a, b, c, d) \
                  (((d & 0xFF) << 24) \
                |  ((c & 0xFF) << 16) \
                |  ((b & 0xFF) <<  8) \
                |  ((a & 0xFF) <<  0))
#endif

struct TextureDetails
{
    DWORD dwWidth;
    DWORD dwHeight;

    GLsizei szMipMapCount;
    void* pPixels;
    GLsizei* pSizes;

    GLenum glFormat;

    bool bCompressed;

    TextureDetails() { memset(this, 0, sizeof(*this)); }
    ~TextureDetails()
    {
        if (pPixels) 
            free(pPixels);
        if (pSizes)
            free(pSizes);
    }

    GLint MipMapHeight(GLint mipMap) const { return dds_imax(1, dwHeight >> mipMap); }
    GLint MipMapWidth(GLint mipMap) const { return dds_imax(1, dwWidth >> mipMap); }
};

// ------------------------------------------------------------------------------------------------
// API
bool readDDSFromFile(const char* _pFilename, TextureDetails* _outDetails);

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Implementation
struct DDS_PIXELFORMAT 
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

// ------------------------------------------------------------------------------------------------
struct DDS_HEADER 
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwHeight;
    DWORD           dwWidth;
    DWORD           dwPitchOrLinearSize;
    DWORD           dwDepth;
    DWORD           dwMipMapCount;
    DWORD           dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    DWORD           dwCaps;
    DWORD           dwCaps2;
    DWORD           dwCaps3;
    DWORD           dwCaps4;
    DWORD           dwReserved2;

    DDS_HEADER()
    {
        memset(this, 0, sizeof(*this));
    }
};

#ifndef DXGI_FORMAT_DEFINED
// ------------------------------------------------------------------------------------------------
enum DXGI_FORMAT 
{ 
    DXGI_FORMAT_UNKNOWN                     = 0,
    DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
    DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
    DXGI_FORMAT_R32G32B32A32_UINT           = 3,
    DXGI_FORMAT_R32G32B32A32_SINT           = 4,
    DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
    DXGI_FORMAT_R32G32B32_FLOAT             = 6,
    DXGI_FORMAT_R32G32B32_UINT              = 7,
    DXGI_FORMAT_R32G32B32_SINT              = 8,
    DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
    DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
    DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
    DXGI_FORMAT_R16G16B16A16_UINT           = 12,
    DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
    DXGI_FORMAT_R16G16B16A16_SINT           = 14,
    DXGI_FORMAT_R32G32_TYPELESS             = 15,
    DXGI_FORMAT_R32G32_FLOAT                = 16,
    DXGI_FORMAT_R32G32_UINT                 = 17,
    DXGI_FORMAT_R32G32_SINT                 = 18,
    DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
    DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
    DXGI_FORMAT_R10G10B10A2_UINT            = 25,
    DXGI_FORMAT_R11G11B10_FLOAT             = 26,
    DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
    DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    DXGI_FORMAT_R8G8B8A8_UINT               = 30,
    DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
    DXGI_FORMAT_R8G8B8A8_SINT               = 32,
    DXGI_FORMAT_R16G16_TYPELESS             = 33,
    DXGI_FORMAT_R16G16_FLOAT                = 34,
    DXGI_FORMAT_R16G16_UNORM                = 35,
    DXGI_FORMAT_R16G16_UINT                 = 36,
    DXGI_FORMAT_R16G16_SNORM                = 37,
    DXGI_FORMAT_R16G16_SINT                 = 38,
    DXGI_FORMAT_R32_TYPELESS                = 39,
    DXGI_FORMAT_D32_FLOAT                   = 40,
    DXGI_FORMAT_R32_FLOAT                   = 41,
    DXGI_FORMAT_R32_UINT                    = 42,
    DXGI_FORMAT_R32_SINT                    = 43,
    DXGI_FORMAT_R24G8_TYPELESS              = 44,
    DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    DXGI_FORMAT_R8G8_TYPELESS               = 48,
    DXGI_FORMAT_R8G8_UNORM                  = 49,
    DXGI_FORMAT_R8G8_UINT                   = 50,
    DXGI_FORMAT_R8G8_SNORM                  = 51,
    DXGI_FORMAT_R8G8_SINT                   = 52,
    DXGI_FORMAT_R16_TYPELESS                = 53,
    DXGI_FORMAT_R16_FLOAT                   = 54,
    DXGI_FORMAT_D16_UNORM                   = 55,
    DXGI_FORMAT_R16_UNORM                   = 56,
    DXGI_FORMAT_R16_UINT                    = 57,
    DXGI_FORMAT_R16_SNORM                   = 58,
    DXGI_FORMAT_R16_SINT                    = 59,
    DXGI_FORMAT_R8_TYPELESS                 = 60,
    DXGI_FORMAT_R8_UNORM                    = 61,
    DXGI_FORMAT_R8_UINT                     = 62,
    DXGI_FORMAT_R8_SNORM                    = 63,
    DXGI_FORMAT_R8_SINT                     = 64,
    DXGI_FORMAT_A8_UNORM                    = 65,
    DXGI_FORMAT_R1_UNORM                    = 66,
    DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
    DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
    DXGI_FORMAT_BC1_TYPELESS                = 70,
    DXGI_FORMAT_BC1_UNORM                   = 71,
    DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
    DXGI_FORMAT_BC2_TYPELESS                = 73,
    DXGI_FORMAT_BC2_UNORM                   = 74,
    DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
    DXGI_FORMAT_BC3_TYPELESS                = 76,
    DXGI_FORMAT_BC3_UNORM                   = 77,
    DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
    DXGI_FORMAT_BC4_TYPELESS                = 79,
    DXGI_FORMAT_BC4_UNORM                   = 80,
    DXGI_FORMAT_BC4_SNORM                   = 81,
    DXGI_FORMAT_BC5_TYPELESS                = 82,
    DXGI_FORMAT_BC5_UNORM                   = 83,
    DXGI_FORMAT_BC5_SNORM                   = 84,
    DXGI_FORMAT_B5G6R5_UNORM                = 85,
    DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
    DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
    DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
    DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    DXGI_FORMAT_BC6H_TYPELESS               = 94,
    DXGI_FORMAT_BC6H_UF16                   = 95,
    DXGI_FORMAT_BC6H_SF16                   = 96,
    DXGI_FORMAT_BC7_TYPELESS                = 97,
    DXGI_FORMAT_BC7_UNORM                   = 98,
    DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
    DXGI_FORMAT_AYUV                        = 100,
    DXGI_FORMAT_Y410                        = 101,
    DXGI_FORMAT_Y416                        = 102,
    DXGI_FORMAT_NV12                        = 103,
    DXGI_FORMAT_P010                        = 104,
    DXGI_FORMAT_P016                        = 105,
    DXGI_FORMAT_420_OPAQUE                  = 106,
    DXGI_FORMAT_YUY2                        = 107,
    DXGI_FORMAT_Y210                        = 108,
    DXGI_FORMAT_Y216                        = 109,
    DXGI_FORMAT_NV11                        = 110,
    DXGI_FORMAT_AI44                        = 111,
    DXGI_FORMAT_IA44                        = 112,
    DXGI_FORMAT_P8                          = 113,
    DXGI_FORMAT_A8P8                        = 114,
    DXGI_FORMAT_B4G4R4A4_UNORM              = 115,
    DXGI_FORMAT_FORCE_UINT                  = 0xffffffffUL
};
#endif

// ------------------------------------------------------------------------------------------------
const DWORD DDPF_ALPHAPIXELS    = 0x1;
const DWORD DDPF_ALPHA	        = 0x2;
const DWORD DDPF_FOURCC	        = 0x4; 
const DWORD DDPF_RGB	        = 0x40;
const DWORD DDPF_YUV	        = 0x200;
const DWORD DDPF_LUMINANCE	    = 0x20000;

#if !defined(D3D10_APPEND_ALIGNED_ELEMENT)
// ------------------------------------------------------------------------------------------------
enum D3D10_RESOURCE_DIMENSION 
{ 
  D3D10_RESOURCE_DIMENSION_UNKNOWN    = 0,
  D3D10_RESOURCE_DIMENSION_BUFFER     = 1,
  D3D10_RESOURCE_DIMENSION_TEXTURE1D  = 2,
  D3D10_RESOURCE_DIMENSION_TEXTURE2D  = 3,
  D3D10_RESOURCE_DIMENSION_TEXTURE3D  = 4
};
#endif

// ------------------------------------------------------------------------------------------------
struct DDS_HEADER_DXT10 
{
  DXGI_FORMAT              dxgiFormat;
  D3D10_RESOURCE_DIMENSION resourceDimension;
  UINT                     miscFlag;
  UINT                     arraySize;
  UINT                     reserved;
};

// ------------------------------------------------------------------------------------------------
struct DDS_FILEHEADER
{
    DWORD           dwMagicNumber;
    DDS_HEADER      header;

    DDS_FILEHEADER() { memset(this, 0, sizeof(*this)); }
};

inline bool readDDSFromFile_DXT1(const DDS_HEADER& _header, FILE* _file, TextureDetails* _outDetails);

const DWORD kDDS_MagicNumber = MAKEFOURCC('D', 'D', 'S', ' ');
const DWORD kDDS_DX10 = MAKEFOURCC('D', 'X', '1', '0');

// ------------------------------------------------------------------------------------------------
inline bool readDDSFromFile(const char* _pFilename, TextureDetails* _outDetails)
{
    FILE* myFile = NULL;
    fopen_s(&myFile, _pFilename, "rb");

    if (!myFile) {
        return false;
    }

    bool success = false;
    DDS_FILEHEADER fileHeader;
    
    if (fread_s(&fileHeader, sizeof(fileHeader), sizeof(fileHeader), 1, myFile) != 1) {
        goto cleanup;
    }
    
    // Sanity check magic values.
    if (fileHeader.dwMagicNumber != kDDS_MagicNumber || fileHeader.header.dwSize != 124 || fileHeader.header.ddspf.dwSize != 32) {
        goto cleanup;
    }

    if ((fileHeader.header.ddspf.dwFlags & DDPF_FOURCC) != 0)
    {
        if (fileHeader.header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '1')) {
            success = readDDSFromFile_DXT1(fileHeader.header, myFile, _outDetails);
        }
    }

    if (success) {
        _outDetails->dwHeight = fileHeader.header.dwHeight;
        _outDetails->dwWidth = fileHeader.header.dwWidth;
    }
    
cleanup:
    fclose(myFile);
    return success;
}

// ------------------------------------------------------------------------------------------------
inline bool readDDSFromFile_DXT1(const DDS_HEADER& _header, FILE* _file, TextureDetails* _outDetails)
{
    const DWORD kBlockSizeBytes = 8;
    const DWORD kColsPerBlock = 4;
    const DWORD kRowsPerBlock = 4;

    size_t totalSize = 0;
    DWORD mipmaps = dds_imax(1, _header.dwMipMapCount);

    _outDetails->szMipMapCount = mipmaps;
    _outDetails->pSizes = (GLsizei*)malloc(mipmaps * sizeof(GLsizei));

    assert(sizeof(BYTE) == 1);

    DWORD mipWidth = _header.dwWidth;
    DWORD mipHeight = _header.dwHeight;

    for (DWORD mip = 0; mip < mipmaps; ++mip) {
        DWORD pitchBlocks = (mipWidth + (kColsPerBlock - 1)) / kColsPerBlock;
        DWORD heightBlocks = (mipHeight + (kRowsPerBlock - 1)) / kRowsPerBlock;
        GLsizei mipmapSize = pitchBlocks * heightBlocks * kBlockSizeBytes;
        _outDetails->pSizes[mip] = mipmapSize;

        totalSize += mipmapSize;
        mipWidth = dds_imax(1, mipWidth >> 1);
        mipHeight = dds_imax(1, mipHeight >> 1);
    }

    BYTE* buffer = (BYTE*)malloc(totalSize);
    size_t bytesRead = fread_s(buffer, totalSize, 1, totalSize, _file);
    if (bytesRead != totalSize) {
        free(buffer);
        buffer = 0;
        return false;
    }

    _outDetails->pPixels = buffer;
    _outDetails->bCompressed = true;
    _outDetails->glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    
    return true;
}

