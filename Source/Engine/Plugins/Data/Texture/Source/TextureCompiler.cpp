//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 1/28/2018.
//

#include <Data/TextureCompiler.h>
#include <IL/il.h>
#include <Base/DefaultLogs.h>
#include <Data/BaseTextureAsset.h>
#include <Core/Memory.h>
#include <compressonator/Compressonator.h>
#include <Core/Math.h>
#include <Async/Group.h>
#include <Async/Async.h>
#include <Core/Timer.h>
#include <Platform/User.h>

DJINN_NS2(Data);

/// Read callbacks
namespace {
    /// Open
    ILAPIENTRY ILHANDLE _ILReadOpen(ILconst_string) {
        return nullptr;
    }

    /// Close
    ILAPIENTRY void _ILReadClose(Core::IStream *) {
        ;
    }

    /// Eof
    ILAPIENTRY ILboolean _ILReadEof(Core::IStream *stream) {
        return static_cast<ILboolean>(stream->IsEOS());
    }

    /// Getc
    ILAPIENTRY ILint _ILReadGetc(Core::IStream *stream) {
        char ch;
        stream->Read(ch);
        return ch;
    }

    /// Read
    ILAPIENTRY ILint _ILReadRead(void *buffer, ILuint stride, ILuint count, Core::IStream *stream) {
        return static_cast<ILint>(stream->Read(buffer, stride * count));
    }

    /// Seek
    ILAPIENTRY ILint _ILReadSeek(Core::IStream *stream, ILint num, ILint mode) {
        switch (mode) {
            case SEEK_SET:
                stream->SetPosition(static_cast<USize>(num));
                return 0;
            case SEEK_CUR:
                stream->SetPosition(stream->GetPosition() + num);
                return 0;
            case SEEK_END:
                stream->SetPosition(stream->GetSize() + num);
                return 0;
            default:
                return 1;
        }
    }

    /// Tell
    ILAPIENTRY ILint _ILReadTell(Core::IStream *stream) {
        return static_cast<ILint>(stream->GetPosition());
    }
}

/// Write callbacks
namespace {
    Platform::Mutex mutex;
}

/// Compressonator
namespace {
    CMP_FORMAT AsCMPFormat(ILint formatType, ILint imageType) {
        switch (imageType) {
            case IL_BYTE: {
                switch (formatType) {
                    case IL_RGB: {
                        return CMP_FORMAT_RGB_888;
                    }
                    case IL_RGBA: {
                        return CMP_FORMAT_RGBA_8888;
                    }
                    case IL_BGR: {
                        return CMP_FORMAT_BGR_888;
                    }
                    case IL_BGRA: {
                        return CMP_FORMAT_BGRA_8888;
                    }
                }
                break;
            }
            case IL_UNSIGNED_BYTE: {
                switch (formatType) {
                    case IL_RGB: {
                        return CMP_FORMAT_RGB_888;
                    }
                    case IL_RGBA: {
                        return CMP_FORMAT_RGBA_8888;
                    }
                    case IL_BGR: {
                        return CMP_FORMAT_BGR_888;
                    }
                    case IL_BGRA: {
                        return CMP_FORMAT_BGRA_8888;
                    }
                }
                break;
            }
            case IL_FLOAT: {
                switch (formatType) {
                    case IL_RGB: {
                        return CMP_FORMAT_RGB_32F;
                    }
                    case IL_RGBA: {
                        return CMP_FORMAT_RGBA_32F;
                    }
                    case IL_BGR: {
                        return CMP_FORMAT_BGR_32F;
                    }
                    case IL_BGRA: {
                        return CMP_FORMAT_BGRA_32F;
                    }
                }
                break;
            }
        }
        return CMP_FORMAT_Unknown;
    }
}

namespace {
    template<typename T, UInt32 N>
    struct FilterVector {
        FilterVector() = default;

        template<typename U>
        FilterVector(const FilterVector<U, N> &other) {
            for (UInt32 i = 0; i < N; i++) {
                elements[i] = other.elements[i];
            }
        }

        template<typename U>
        FilterVector operator+(const FilterVector<U, N> &other) {
            FilterVector<T, N> vector;
            for (UInt32 i = 0; i < N; i++) {
                vector.elements[i] = elements[i] + other.elements[i];
            }
            return vector;
        }

        template<typename U>
        FilterVector &operator+=(const FilterVector<U, N> &other) {
            for (UInt32 i = 0; i < N; i++) {
                elements[i] += other.elements[i];
            }
            return *this;
        }

        FilterVector operator/(double f) {
            FilterVector<T, N> vector;
            for (UInt32 i = 0; i < N; i++) {
                vector.elements[i] = elements[i] / f;
            }
            return vector;
        }

        T elements[N] = {};
    };

    template<typename T, UInt32 N>
    USize Filter(const GHA::Size &sourceSize, FilterVector<T, N> *source, const GHA::Size &destSize, FilterVector<T, N> *dest) {
        double fdx = static_cast<double>(sourceSize.width) / static_cast<double>(destSize.width);
        double fdy = static_cast<double>(sourceSize.height) / static_cast<double>(destSize.height);
        double fdz = static_cast<double>(sourceSize.depth) / static_cast<double>(destSize.depth);

        USize i = 0;
        for (USize y = 0; y < destSize.height; y++) {
            for (USize x = 0; x < destSize.width; x++) {
                for (USize z = 0; z < destSize.depth; z++, i++) {
                    /*FilterVector<double, N> accum;
                    accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) (x * fdx), (USize) Math::Min((Int64) y * fdy + 1, sourceSize.height - 1), (USize) (z * fdz))];
                    accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) (x * fdx), (USize) Math::Max(0, (Int64) y * fdy - 1), (USize) (z * fdz))];
                    accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) Math::Min((Int64) x * fdx + 1, sourceSize.width - 1), (USize) (y * fdy), (USize) (z * fdz))];
                    accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) Math::Max(0, (Int64) x * fdx - 1), (USize) (y * fdy), (USize) (z * fdz))];
                    if (sourceSize.depth != 1) {
                        accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) (x * fdx), (USize) (y * fdy), (USize) Math::Clamp((Int64) z * fdz + 1, 0, sourceSize.depth - 1))];
                        accum += source[Math::To1D(sourceSize.width, sourceSize.depth, (USize) (x * fdx), (USize) (y * fdy), (USize) Math::Max(0, (Int64) z * fdz - 1))];
                        dest[i] = accum / 6.0;
                    } else {
                        dest[i] = accum / 4.0;
                    }*/
                    dest[i] = source[Math::To1D(sourceSize.width, sourceSize.depth, static_cast<USize> (x * fdx), static_cast<USize> (y * fdy), static_cast<USize> (z * fdz))];
                }
            }
        }

        return i * N * sizeof(T);
    }

    USize Filter(GHA::Format format, const GHA::Size &sourceSize, void *source, const GHA::Size &destSize, void *dest) {
        switch (format) {
            case GHA::Format::eR8G8B8A8Srgb:
                return Filter<char, 4>(sourceSize, reinterpret_cast<FilterVector<char, 4> *>(source), destSize, reinterpret_cast<FilterVector<char, 4> *>( dest));
            case GHA::Format::eR8G8B8A8Snorm:
                return Filter<char, 4>(sourceSize, reinterpret_cast<FilterVector<char, 4> *>(source), destSize, reinterpret_cast<FilterVector<char, 4> *>( dest));
            case GHA::Format::eR8G8B8A8Unorm:
                return Filter<unsigned char, 4>(sourceSize, reinterpret_cast<FilterVector<unsigned char, 4> *>(source), destSize, reinterpret_cast<FilterVector<unsigned char, 4> *>( dest));
            case GHA::Format::eB8G8R8A8Unorm:
                return Filter<unsigned char, 4>(sourceSize, reinterpret_cast<FilterVector<unsigned char, 4> *>(source), destSize, reinterpret_cast<FilterVector<unsigned char, 4> *>( dest));
            case GHA::Format::eR32G32B32A32F:
                return Filter<float, 4>(sourceSize, reinterpret_cast<FilterVector<float, 4> *>(source), destSize, reinterpret_cast<FilterVector<float, 4> *>( dest));
            case GHA::Format::eR32G32B32A32I:
                return Filter<int, 4>(sourceSize, reinterpret_cast<FilterVector<int, 4> *>(source), destSize, reinterpret_cast<FilterVector<int, 4> *>( dest));
            case GHA::Format::eR32G32B32A32U:
                return Filter<unsigned int, 4>(sourceSize, reinterpret_cast<FilterVector<unsigned int, 4> *>(source), destSize, reinterpret_cast<FilterVector<unsigned int, 4> *>( dest));
            case GHA::Format::eR32G32B32F:
                return Filter<float, 3>(sourceSize, reinterpret_cast<FilterVector<float, 3> *>( source), destSize, reinterpret_cast<FilterVector<float, 3> *>( dest));
            case GHA::Format::eR32G32B32I:
                return Filter<int, 3>(sourceSize, reinterpret_cast<FilterVector<int, 3> *>(source), destSize, reinterpret_cast<FilterVector<int, 3> *>( dest));
            case GHA::Format::eR32G32B32U:
                return Filter<unsigned int, 3>(sourceSize, reinterpret_cast<FilterVector<unsigned int, 3> *>(source), destSize, reinterpret_cast<FilterVector<unsigned int, 3> *>( dest));
            case GHA::Format::eR32G32F:
                return Filter<float, 2>(sourceSize, reinterpret_cast<FilterVector<float, 2> *>( source), destSize, reinterpret_cast<FilterVector<float, 2> *>( dest));
            case GHA::Format::eR32G32I:
                return Filter<int, 2>(sourceSize, reinterpret_cast<FilterVector<int, 2> *>( source), destSize, reinterpret_cast<FilterVector<int, 2> *>( dest));
            case GHA::Format::eR32G32U:
                return Filter<unsigned int, 2>(sourceSize, reinterpret_cast<FilterVector<unsigned int, 2> *>(source), destSize, reinterpret_cast<FilterVector<unsigned int, 2> *>( dest));
            case GHA::Format::eR32F:
                return Filter<float, 1>(sourceSize, reinterpret_cast<FilterVector<float, 1> *>(source), destSize, reinterpret_cast<FilterVector<float, 1> *>( dest));
            case GHA::Format::eR32I:
                return Filter<int, 1>(sourceSize, reinterpret_cast<FilterVector<int, 1> *>(source), destSize, reinterpret_cast<FilterVector<int, 1> *>( dest));
            case GHA::Format::eR32U:
                return Filter<unsigned int, 1>(sourceSize, reinterpret_cast<FilterVector<unsigned int, 1> *>(source), destSize, reinterpret_cast<FilterVector<unsigned int, 1> *>( dest));
            case GHA::Format::eR8G8B8Snorm:
                return Filter<char, 3>(sourceSize, reinterpret_cast<FilterVector<char, 3> *>(source), destSize, reinterpret_cast<FilterVector<char, 3> *>( dest));
            case GHA::Format::eB8G8R8Snorm:
                return Filter<char, 3>(sourceSize, reinterpret_cast<FilterVector<char, 3> *>(source), destSize, reinterpret_cast<FilterVector<char, 3> *>( dest));
            case GHA::Format::eB8G8R8A8Snorm:
                return Filter<char, 4>(sourceSize, reinterpret_cast<FilterVector<char, 4> *>(source), destSize, reinterpret_cast<FilterVector<char, 4> *>( dest));
            case GHA::Format::eR8G8B8Unorm:
                return Filter<unsigned char, 3>(sourceSize, reinterpret_cast<FilterVector<unsigned char, 3> *>(source), destSize, reinterpret_cast<FilterVector<unsigned char, 3> *>(dest));
            case GHA::Format::eB8G8R8Unorm:
                return Filter<unsigned char, 3>(sourceSize, reinterpret_cast<FilterVector<unsigned char, 3> *>(source), destSize, reinterpret_cast<FilterVector<unsigned char, 3> *>(dest));
            default:
                return 0;
        }
    }

    USize Filter(CMP_FORMAT format, const GHA::Size &sourceSize, void *source, const GHA::Size &destSize, void *dest) {
        switch (format) {
            case CMP_FORMAT_RGBA_8888:
                return Filter<char, 4>(sourceSize, reinterpret_cast<FilterVector<char, 4> *>(source), destSize, reinterpret_cast<FilterVector<char, 4> *>( dest));
            case CMP_FORMAT_BGRA_8888:
                return Filter<char, 4>(sourceSize, reinterpret_cast<FilterVector<char, 4> *>(source), destSize, reinterpret_cast<FilterVector<char, 4> *>(dest));
            case CMP_FORMAT_RGB_888:
                return Filter<char, 3>(sourceSize, reinterpret_cast<FilterVector<char, 3> *>(source), destSize, reinterpret_cast<FilterVector<char, 3> *>(dest));
            case CMP_FORMAT_BGR_888:
                return Filter<char, 3>(sourceSize, reinterpret_cast<FilterVector<char, 3> *>(source), destSize, reinterpret_cast<FilterVector<char, 3> *>(dest));
            case CMP_FORMAT_RGBA_32F:
                return Filter<float, 4>(sourceSize, reinterpret_cast<FilterVector<float, 4> *>( source), destSize, reinterpret_cast<FilterVector<float, 4> *>( dest));
            case CMP_FORMAT_BGRA_32F:
                return Filter<float, 4>(sourceSize, reinterpret_cast<FilterVector<float, 4> *>(source), destSize, reinterpret_cast<FilterVector<float, 4> *>( dest));
            case CMP_FORMAT_RGB_32F:
                return Filter<float, 3>(sourceSize, reinterpret_cast<FilterVector<float, 3> *>(source), destSize, reinterpret_cast<FilterVector<float, 3> *>( dest));
            case CMP_FORMAT_BGR_32F:
                return Filter<float, 3>(sourceSize, reinterpret_cast<FilterVector<float, 3> *>(source), destSize, reinterpret_cast<FilterVector<float, 3> *>(dest));
            default:
                return 0;
        }
    }
}

TextureCompiler::TextureCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITextureCompiler(registry, outer, host) {

}

TextureCompiler::~TextureCompiler() {
    CMP_ShutdownBCLibrary();
    ilShutDown();
}

COM::Result TextureCompiler::Initialize() {
    ilInit();

    // Prepare file read callbacks
    ilSetRead(
            reinterpret_cast<fOpenRProc>(_ILReadOpen),
            reinterpret_cast<fCloseRProc>( _ILReadClose),
            reinterpret_cast<fEofProc>( _ILReadEof),
            reinterpret_cast<fGetcProc>( _ILReadGetc),
            reinterpret_cast<fReadProc>( _ILReadRead),
            reinterpret_cast<fSeekRProc>( _ILReadSeek),
            reinterpret_cast<fTellRProc>( _ILReadTell)
    );

    // Initialize
    /*if (CMP_InitializeBCLibrary() != BC_ERROR_NONE) {
        ErrorLog(this).Write("Failed to initialize bc library");
        return COM::kError;
    }*/

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result TextureCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = reinterpret_cast<TextureConfiguration *>(_config);

    Platform::LockGuard guard(mutex);

    // Diagnostic
    Core::Timer timer;
    InfoLog(this).Write("Started texture compilation job [", config->Hash(), "]");

    // Generate il image
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Switch type
    ILenum type;
    switch (config->sourceFormat) {
        case TextureSourceFormat::eJpg:
            type = IL_JPG;
            break;
        case TextureSourceFormat::ePng:
            type = IL_PNG;
            break;
        default:
            ErrorLog(this).Write("Invalid texture source format");
            return COM::kError;
    }

    // Load from stream
    if( !ilLoadF( type, stream ) ) {
        UInt32 error = ilGetError();
        switch( error ) {
            case IL_ILLEGAL_OPERATION: ErrorLog( this ).Write( "Currently no image bound to IL stack." ); break;
            case IL_COULD_NOT_OPEN_FILE: ErrorLog( this ).Write( "Could not open file.  File does not exist or is in use" ); break;
            case IL_INVALID_ENUM: ErrorLog( this ).Write( "Load type was an invalid value." ); break;
            case IL_INVALID_PARAM: ErrorLog( this ).Write( "File name, file, or memory lump was null." ); break;
            case IL_ILLEGAL_FILE_VALUE: ErrorLog( this ).Write( "Invalid value present in file data." ); break;
            case IL_OUT_OF_MEMORY: ErrorLog( this ).Write( "Could not allocate more memory -- we're out." ); break;
            case IL_LIB_JPEG_ERROR: ErrorLog( this ).Write( "JPEG load error" ); break;
            case IL_LIB_PNG_ERROR: ErrorLog( this ).Write( "PNG load error" ); break;
            default: ErrorLog(this).Write("Invalid or unsupported texture data");
        }

        return COM::kError;
    }

    // Create asset
    BaseTextureAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    // Set dimensions
    asset->size.width = static_cast<UInt32>(ilGetInteger(IL_IMAGE_WIDTH));
    asset->size.height = static_cast<UInt32>(ilGetInteger(IL_IMAGE_HEIGHT));
    asset->size.depth = static_cast<UInt32>(ilGetInteger(IL_IMAGE_DEPTH));

    // Get common properties
    ILenum imageType = static_cast<ILenum>(ilGetInteger(IL_IMAGE_TYPE));
    ILenum formatType = static_cast<ILenum>(ilGetInteger(IL_IMAGE_FORMAT));
    USize blobSize = static_cast<USize>(ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
    USize bbp = static_cast<USize>( ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

    // To cmp format
    CMP_FORMAT cmpFormat = CMP_FORMAT_Unknown;
    if (config->compressionMode != TextureCompressionMode::eNone) {
        if ((cmpFormat = AsCMPFormat(formatType, imageType)) == CMP_FORMAT_Unknown || asset->size.depth > 1) {
            WarningLog(this).Write("Texture compilation will not be compressed, incompatible format");
            cmpFormat = CMP_FORMAT_Unknown;
        } else if (asset->size.depth > 1) {
            WarningLog(this).Write("Texture compilation will not be compressed, 3d textures not supported");
            cmpFormat = CMP_FORMAT_Unknown;
        }
    }

    // Determine mip count
    UInt32 mipCount = config->generateMipMaps ? (Math::Floor(Math::Log2(Math::Max(
            asset->size.width, asset->size.height, asset->size.depth
    ))) + 1) : 1;

    // Preallocate
    asset->mipMapBlobs.Resize(mipCount);

    // Compression?
    if (cmpFormat != CMP_FORMAT_Unknown) {
        // Previous mip unprocessed data
        Core::Array<char> filteredData;

        // Source texture
        CMP_Texture source;
        source.dwSize = sizeof(CMP_Texture);
        source.nBlockWidth = 4;
        source.nBlockHeight = 4;
        source.nBlockDepth = 1;

        // Destination texture
        CMP_Texture dest;
        dest.dwSize = sizeof(CMP_Texture);
        dest.nBlockWidth = 4;
        dest.nBlockHeight = 4;
        dest.nBlockDepth = 1;

        // Compress options
        CMP_CompressOptions compressOptions = {};
        compressOptions.dwSize = sizeof(CMP_CompressOptions);
        compressOptions.nCompressionSpeed = static_cast<CMP_Speed>(CMP_Speed_Normal);
        compressOptions.dwnumThreads = (config->mode == TextureCompileMode::eExternal ? Platform::User::GetCoreCount() : 1);
        compressOptions.bDisableMultiThreading = (config->mode == TextureCompileMode::eInline);
        compressOptions.fquality = AMD_CODEC_QUALITY_DEFAULT;
        compressOptions.fInputDefog = AMD_CODEC_DEFOG_DEFAULT;
        compressOptions.fInputExposure = AMD_CODEC_EXPOSURE_DEFAULT;
        compressOptions.fInputKneeLow = AMD_CODEC_KNEELOW_DEFAULT;
        compressOptions.fInputKneeHigh = AMD_CODEC_KNEEHIGH_DEFAULT;
        compressOptions.fInputGamma = AMD_CODEC_GAMMA_DEFAULT;
        compressOptions.dwmodeMask = 0xCF;

        // Translate mode
        switch (config->compressionMode) {
            default:
                break;
            case TextureCompressionMode::eBC1:
                dest.format = CMP_FORMAT_BC1;
                asset->format = GHA::Format::eBC1;
                break;
            case TextureCompressionMode::eBC2:
                dest.format = CMP_FORMAT_BC2;
                asset->format = GHA::Format::eBC2;
                break;
            case TextureCompressionMode::eBC3:
                dest.format = CMP_FORMAT_BC3;
                asset->format = GHA::Format::eBC3;
                break;
            case TextureCompressionMode::eBC4:
                dest.format = CMP_FORMAT_BC4;
                asset->format = GHA::Format::eBC4;
                break;
            case TextureCompressionMode::eBC5:
                dest.format = CMP_FORMAT_BC5;
                asset->format = GHA::Format::eBC5;
                break;
            case TextureCompressionMode::eBC7:
                dest.format = CMP_FORMAT_BC7;
                asset->format = GHA::Format::eBC7;
                break;
            case TextureCompressionMode::eASTC:
                dest.format = CMP_FORMAT_ASTC;
                asset->format = GHA::Format::eASTC;
                break;
        }

        // Filter and compress mips
        GHA::Size lastMipSize = asset->size;
        GHA::Size mipSize = asset->size;
        for (UInt32 i = 0; i < mipCount; i++) {
            // Source texture
            source.dwWidth = mipSize.width;
            source.dwHeight = mipSize.height;
            source.dwPitch = static_cast<CMP_DWORD> (bbp * mipSize.width);
            source.format = static_cast<CMP_FORMAT>(cmpFormat);

            // Filter?
            Core::Array<char> tmpFilteredData;
            if (i == 0) {
                source.dwDataSize = static_cast<CMP_DWORD>(blobSize);
                source.pData = ilGetData();
            } else {
                tmpFilteredData.ResizePassive(mipSize.width * mipSize.height * mipSize.depth * bbp);
                source.dwDataSize = static_cast<CMP_DWORD>(Filter(
                        cmpFormat,
                        lastMipSize, (i == 1) ? static_cast<void *>(ilGetData()) : filteredData.Ptr(),
                        mipSize, tmpFilteredData.Ptr()
                ));
                source.pData = reinterpret_cast<CMP_BYTE *>(tmpFilteredData.Ptr());
                filteredData = tmpFilteredData;
            }

            // Destination texture
            dest.dwWidth = mipSize.width;
            dest.dwHeight = mipSize.height;
            dest.dwPitch = static_cast<CMP_DWORD>(bbp * mipSize.width);

            // End size
            dest.dwDataSize = CMP_CalculateBufferSize(&dest);

            // End data
            asset->mipMapBlobs[i].data.Resize(dest.dwDataSize);
            dest.pData = reinterpret_cast<UInt8 *>(asset->mipMapBlobs[i].data.Ptr());

            // Attempt to compress the texture
            CMP_ERROR error = CMP_ConvertTexture(&source, &dest, &compressOptions, nullptr, reinterpret_cast<CMP_DWORD_PTR>(registry), 0);
            if (error != CMP_OK) {
                const char *reason = "";
                switch (error) {
                    default:
                        break;
                    case CMP_ABORTED:
                        reason = "aborted";
                        break;
                    case CMP_ERR_INVALID_SOURCE_TEXTURE:
                        reason = "invalid source texture";
                        break;
                    case CMP_ERR_INVALID_DEST_TEXTURE:
                        reason = "invalid dest texture";
                        break;
                    case CMP_ERR_UNSUPPORTED_SOURCE_FORMAT:
                        reason = "unsupported source format";
                        break;
                    case CMP_ERR_UNSUPPORTED_DEST_FORMAT:
                        reason = "unsupported dest format";
                        break;
                    case CMP_ERR_UNSUPPORTED_GPU_ASTC_DECODE:
                        reason = "unsupported gpu astc decode";
                        break;
                    case CMP_ERR_SIZE_MISMATCH:
                        reason = "size mismatch";
                        break;
                    case CMP_ERR_UNABLE_TO_INIT_CODEC:
                        reason = "unable to init codec";
                        break;
                    case CMP_ERR_UNABLE_TO_INIT_DECOMPRESSLIB:
                        reason = "unable to init decompresslib";
                        break;
                    case CMP_ERR_UNABLE_TO_INIT_COMPUTELIB:
                        reason = "unable to init computelib";
                        break;
                    case CMP_ERR_GENERIC:
                        reason = "generic";
                        break;
                }
                ErrorLog(this).Write("Amd compressonator failed: ", reason);
                return COM::kError;
            }

            // Calculate new size
            lastMipSize = mipSize;
            mipSize.width = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.width) / 2.0)));
            mipSize.height = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.height) / 2.0)));
            mipSize.depth = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.depth) / 2.0)));
        }
    } else {
        // Reverse (Fx. BGR) formats are not supported, and 24 bits formats are converted due to lack of support on most cards
        if (formatType != IL_RGBA) {
            if (!ilConvertImage(IL_RGBA, imageType)) {
                ErrorLog(this).Write("Could not convert texture into a compatible format");
                return COM::kError;
            }

            // Repool
            formatType = static_cast<ILenum>(ilGetInteger(IL_IMAGE_FORMAT));
            blobSize = static_cast<USize>(ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
            bbp = static_cast<USize>(ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));
        }

        // Determine format
        switch (imageType) {
            case IL_BYTE: {
                asset->format = GHA::Format::eR8G8B8A8Snorm;
                break;
            }
            case IL_UNSIGNED_BYTE: {
                asset->format = GHA::Format::eR8G8B8A8Unorm;
                break;
            }
            case IL_INT: {
                asset->format = GHA::Format::eR32G32B32A32I;
                break;
            }
            case IL_UNSIGNED_INT: {
                asset->format = GHA::Format::eR32G32B32A32U;
                break;
            }
            case IL_SHORT: {
                asset->format = GHA::Format::eR16G16B16A16F;
                break;
            }
            case IL_UNSIGNED_SHORT: {
                asset->format = GHA::Format::eR16G16B16A16U;
                break;
            }
            case IL_FLOAT: {
                asset->format = GHA::Format::eR32G32B32A32F;
                break;
            }
        }

        // Process mips
        GHA::Size lastMipSize = asset->size;
        GHA::Size mipSize = asset->size;
        for (UInt32 i = 0; i < mipCount; i++) {
            // Filter
            if (i == 0) {
                asset->mipMapBlobs[i].data.Resize(blobSize);
                ilCopyPixels(0, 0, 0, asset->size.width, asset->size.height, asset->size.depth, formatType, imageType, asset->mipMapBlobs[i].data.Ptr());
            } else {
                asset->mipMapBlobs[i].data.Resize(mipSize.width * mipSize.height * mipSize.depth * bbp);
                Filter(
                        asset->format,
                        lastMipSize, asset->mipMapBlobs[i - 1].data.Ptr(),
                        mipSize, asset->mipMapBlobs[i].data.Ptr()
                );
            }

            // Calculate new size
            lastMipSize = mipSize;
            mipSize.width = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.width) / 2.0)));
            mipSize.height = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.height) / 2.0)));
            mipSize.depth = static_cast<UInt32>(Math::Max(1, Math::Floor(static_cast<double>(mipSize.depth) / 2.0)));
        }
    }

    // Unbind and free
    ilBindImage(0);
    ilDeleteImage(imageID);

    // Diagnostic
    InfoLog(this).Write("Texture compilation finished (", timer.Delta(), "s)");

    // OK
    *out = asset;
    return COM::kOK;
}


