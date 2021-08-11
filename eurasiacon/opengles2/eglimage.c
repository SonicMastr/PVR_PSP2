/******************************************************************************
 * Name         : eglimage.c
 *
 * Copyright    : 2008-2009 by Imagination Technologies Limited.
 *              : All rights reserved. No part of this software, either
 *              : material or conceptual may be copied or distributed,
 *              : transmitted, transcribed, stored in a retrieval system or
 *              : translated into any human or computer language in any form
 *              : by any means, electronic, mechanical, manual or otherwise,
 *              : or disclosed to third parties without the express written
 *              : permission of Imagination Technologies Limited,
 *              : Home Park Estate, King's Langley, Hertfordshire,
 *              : WD4 8LZ, U.K.
 *
 * Platform     : ANSI
 *
 * Description	: GL_OES_EGL_image extension
 *
 * $Log: eglimage.c $
 **************************************************************************/

#include "context.h"
#include "drveglext.h"
#include "texformat.h"
#include "twiddle.h"

#if defined(GLES2_EXTENSION_EGL_IMAGE)

/***********************************************************************************
 Function Name      : ReleaseImageFromTexture
 Inputs             : 
 Outputs            : 
 Returns            : 
 Description        : Unbinds an egl image from a texture
************************************************************************************/
IMG_INTERNAL IMG_VOID ReleaseImageFromTexture(GLES2Context *gc, GLES2Texture *psTex)
{
	IMG_UINT8 *pui8Dest;
	EGLImage *psEGLImage;
	IMG_UINT32 ui32InternalFormat, ui32BytesPerPixel;
	IMG_UINT32 ui32BufferWidth,ui32BufferHeight,ui32BufferStride;
	const GLES2TextureFormat *psTexFormat;

	psEGLImage = psTex->psEGLImageTarget;

	GLES_ASSERT(psEGLImage);

	ui32BufferWidth = psEGLImage->ui32Width;
	ui32BufferHeight = psEGLImage->ui32Height;
	ui32BufferStride = psEGLImage->ui32Stride;


	switch(psEGLImage->ePixelFormat)
	{
		case PVRSRV_PIXEL_FORMAT_RGB565:
		{
			psTexFormat = &TexFormatRGB565;

			ui32InternalFormat = GL_RGB;

			ui32BytesPerPixel = 2;

			break;
		}
		case PVRSRV_PIXEL_FORMAT_ARGB4444:
		{
			psTexFormat = &TexFormatARGB4444;

			ui32InternalFormat = GL_RGBA;

			ui32BytesPerPixel = 2;

			break;
		}
		case PVRSRV_PIXEL_FORMAT_ARGB1555:
		{
			psTexFormat = &TexFormatARGB1555;

			ui32InternalFormat = GL_RGBA;

			ui32BytesPerPixel = 2;

			break;
		}
		case PVRSRV_PIXEL_FORMAT_ARGB8888:
		{
			psTexFormat = &TexFormatARGB8888;

			ui32InternalFormat = GL_BGRA_EXT;

			ui32BytesPerPixel = 4;

			break;
		}
		case PVRSRV_PIXEL_FORMAT_ABGR8888:
		{
			psTexFormat = &TexFormatABGR8888;

			ui32InternalFormat = GL_RGBA;

			ui32BytesPerPixel = 4;

			break;
		}
		case PVRSRV_PIXEL_FORMAT_XBGR8888:
		{
			psTexFormat = &TexFormatXBGR8888;

			ui32InternalFormat = GL_RGB;

			ui32BytesPerPixel = 4;

			break;
		}
#if defined(EGL_IMAGE_COMPRESSED_GLES2)
		case PVRSRV_PIXEL_FORMAT_PVRTC2:
		{
			if( psEGLImage->bCompressedRGBOnly ) 
			{
				psTexFormat = &TexFormatPVRTC2RGB;
				ui32InternalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			}
			else
			{
				psTexFormat = &TexFormatPVRTC2RGBA;
				ui32InternalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			}

			ui32BytesPerPixel = 8;

			ui32BufferWidth = MAX(ui32BufferWidth >> 3, 1);
			ui32BufferStride = MAX(ui32BufferStride >> 3, 1);
			ui32BufferHeight = MAX(ui32BufferHeight >> 2, 1);
			break;
		}

		case PVRSRV_PIXEL_FORMAT_PVRTC4:
		{
			if( psEGLImage->bCompressedRGBOnly ) 
			{
				psTexFormat = &TexFormatPVRTC4RGB;
				ui32InternalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			}
			else
			{
				psTexFormat = &TexFormatPVRTC4RGBA;
				ui32InternalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
			}
			ui32BytesPerPixel = 8;

			ui32BufferWidth = MAX(ui32BufferWidth >> 2, 1);
			ui32BufferStride = MAX(ui32BufferStride >> 2, 1);
			ui32BufferHeight = MAX(ui32BufferHeight >> 2, 1);

			break;
		}
		case PVRSRV_PIXEL_FORMAT_PVRTCII2:
		{
			if( psEGLImage->bCompressedRGBOnly ) 
			{
				psTexFormat = &TexFormatPVRTCII2RGB;
				ui32InternalFormat = GL_RGB;
			}
			else
			{
				psTexFormat = &TexFormatPVRTCII2RGBA;
				ui32InternalFormat = GL_RGBA;
			}

			ui32BytesPerPixel = 8;

			ui32BufferWidth = MAX(ui32BufferWidth >> 3, 1);
			ui32BufferStride = MAX(ui32BufferStride >> 3, 1);
			ui32BufferHeight = MAX(ui32BufferHeight >> 2, 1);

			PVR_DPF((PVR_DBG_WARNING,"ReleaseImageFromTexture: PVRTCII2 EGLimage - no internal format enum for it"));

			break;
		}

		case PVRSRV_PIXEL_FORMAT_PVRTCII4:
		{
			if( psEGLImage->bCompressedRGBOnly ) 
			{
				psTexFormat = &TexFormatPVRTCII4RGB;
				ui32InternalFormat = GL_RGB;
			}
			else
			{
				psTexFormat = &TexFormatPVRTCII4RGBA;
				ui32InternalFormat = GL_RGBA;
			}

			ui32BytesPerPixel = 8;

			ui32BufferWidth = MAX(ui32BufferWidth >> 2, 1);
			ui32BufferStride = MAX(ui32BufferStride >> 2, 1);
			ui32BufferHeight = MAX(ui32BufferHeight >> 2, 1);

			PVR_DPF((PVR_DBG_WARNING,"ReleaseImageFromTexture: PVRTCII4 EGLimage - no internal format enum for it"));

			break;
		}
		case PVRSRV_PIXEL_FORMAT_PVRTCIII:
		{
			psTexFormat = &TexFormatETC1RGB;
			ui32InternalFormat = GL_ETC1_RGB8_OES;

			ui32BytesPerPixel = 8;

			ui32BufferWidth = MAX(ui32BufferWidth >> 2, 1);
			ui32BufferStride = MAX(ui32BufferStride >> 2, 1);
			ui32BufferHeight = MAX(ui32BufferHeight >> 2, 1);

			break;
		}
#endif /* defined(EGL_IMAGE_COMPRESSED_GLES2) */

		default:
		{
			PVR_DPF((PVR_DBG_ERROR,"ReleaseImageFromTexture: Unsupported pixel format"));

			return;
		}
	}

	pui8Dest = TextureCreateLevel(gc, psTex, 0, ui32InternalFormat, psTexFormat, psEGLImage->ui32Width, psEGLImage->ui32Height);

	if(pui8Dest)
	{
		if(psEGLImage->bTwiddled)
		{
			IMG_UINT32 ui32WidthLog2, ui32HeightLog2;

			ui32WidthLog2  = FloorLog2(psEGLImage->ui32Width);
			ui32HeightLog2 = FloorLog2(psEGLImage->ui32Height);

			switch(ui32BytesPerPixel)
			{
				case 2:
				{
					ReadBackTwiddle16bpp(pui8Dest, psEGLImage->pvLinSurfaceAddress, ui32WidthLog2, ui32HeightLog2, 
										psEGLImage->ui32Width, psEGLImage->ui32Height, psEGLImage->ui32Width);
					break;
				}
				case 4:
				{
					ReadBackTwiddle32bpp(pui8Dest, psEGLImage->pvLinSurfaceAddress, ui32WidthLog2, ui32HeightLog2, 
										 psEGLImage->ui32Width, psEGLImage->ui32Height, psEGLImage->ui32Width);

					break;
				}
				default:
				{
					PVR_DPF((PVR_DBG_ERROR,"ReleaseImageFromTexture: Invalid BytesPerPixel (%d)", ui32BytesPerPixel));

					break;
				}
			}
		}
		else
		{
			IMG_UINT8 *pui8Src;
			IMG_UINT32 i, ui32Size;

			ui32Size = ui32BufferWidth * ui32BytesPerPixel;

			pui8Src = (IMG_UINT8 *)psEGLImage->pvLinSurfaceAddress;

			for(i=0; i<ui32BufferHeight; i++)
			{
				GLES2MemCopy(pui8Dest, pui8Src, ui32Size);

				pui8Src  += ui32BufferStride;
				pui8Dest +=	ui32Size;
			}
		}
	}

	/* If the texture was live we must ghost it */
	if (KRM_IsResourceNeeded(&gc->psSharedState->psTextureManager->sKRM, &psTex->sResource))
	{
		TexMgrGhostTexture(gc, psTex);
	}
	else
	{
		KEGLUnbindImage(psEGLImage->hImage);
	}

	/* deassociate the image and the texture */
	psTex->psEGLImageTarget = IMG_NULL;

	gc->ui32NumEGLImageTextureTargetsBound--;

	TextureRemoveResident(gc, psTex);

	/* hmm... */
	psTex->ui32LevelsConsistent = GLES2_TEX_UNKNOWN;
}


/***********************************************************************************
 Function Name      : EGLImageTargetTexture2DOES
 Inputs             : target, image
 Outputs            : -
 Returns            : -
 Description        : Sets up a texture from an EGL image
************************************************************************************/
GL_API_EXT void GL_APIENTRY glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	EGLImage *psEGLImage;
	GLES2Texture *psTex;
	IMG_UINT32 ui32Target;

	__GLES2_GET_CONTEXT();

	PVR_DPF((PVR_DBG_CALLTRACE,"glEGLImageTargetTexture2DOES"));

	GLES2_TIME_START(GLES2_TIMES_glEGLImageTargetTexture2DOES);

	switch(target)
	{
		case GL_TEXTURE_2D:
			ui32Target = GLES2_TEXTURE_TARGET_2D;
			break;
#if defined(GLES2_EXTENSION_EGL_IMAGE_EXTERNAL)
		case GL_TEXTURE_EXTERNAL_OES:
			ui32Target = GLES2_TEXTURE_TARGET_STREAM;
			break;
#endif
		default:
			SetError(gc, GL_INVALID_ENUM);
			GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);
			return;
	}

	/* Check if image is a valid eglImageOES object */
	if(!KEGLGetImageSource(image, &psEGLImage))
	{
		PVR_DPF((PVR_DBG_WARNING,"glEGLImageTargetTexture2DOES: Invalid name"));

		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);

		return;
	}

	/* The source must not be too large */
	if((psEGLImage->ui32Width > GLES2_MAX_TEXTURE_SIZE) || (psEGLImage->ui32Height > GLES2_MAX_TEXTURE_SIZE))
	{
		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);

		return;
	}

	psTex = gc->sTexture.apsBoundTexture[gc->sState.sTexture.ui32ActiveTexture][ui32Target];

	if(psTex->psEGLImageTarget)
	{
		/* If the texture was live we must ghost it */
		if (KRM_IsResourceNeeded(&gc->psSharedState->psTextureManager->sKRM, &psTex->sResource))
		{
			TexMgrGhostTexture(gc, psTex);
		}
		else
		{
			KEGLUnbindImage(psTex->psEGLImageTarget->hImage);
		}

		gc->ui32NumEGLImageTextureTargetsBound--;
	}
	else if(psTex->psEGLImageSource)
	{
		if (KRM_IsResourceNeeded(&gc->psSharedState->psTextureManager->sKRM, &psTex->sResource))
		{
			TexMgrGhostTexture(gc, psTex);
		}
		else
		{
			/* Inform EGL that the source is being orphaned */
			KEGLUnbindImage(psTex->psEGLImageSource->hImage);

			psTex->psMemInfo = IMG_NULL;
			psTex->psEGLImageSource = IMG_NULL;
		}
	}
	else if (psTex->psMemInfo)
	{
		if(KRM_IsResourceNeeded(&gc->psSharedState->psTextureManager->sKRM, &psTex->sResource))
		{
			if(TexMgrGhostTexture(gc, psTex) != IMG_TRUE)
			{
				PVR_DPF((PVR_DBG_ERROR,"glEGLImageTargetTexture2DOES: Can't ghost the texture"));
				
				GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);

				return;
			}
		}
		else
		{
#if (defined(DEBUG) || defined(TIMING))
			ui32TextureMemCurrent -= psTex->psMemInfo->uAllocSize;
#endif
			GLES2FREEDEVICEMEM_HEAP(gc, psTex->psMemInfo);

			psTex->psMemInfo = IMG_NULL;
		}
	}

	psTex->psEGLImageTarget = psEGLImage;
		
	if(TextureCreateImageLevel(gc, psTex) != IMG_TRUE)
	{
		psTex->psEGLImageTarget = IMG_NULL;

		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);

		return;
	}

	psTex->bResidence = IMG_TRUE;

	KEGLBindImage(psTex->psEGLImageTarget->hImage);

	gc->ui32NumEGLImageTextureTargetsBound++;

	GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetTexture2DOES);
}	


/***********************************************************************************
 Function Name      : EGLImageTargetRenderbufferStorageOES
 Inputs             : target, image
 Outputs            : -
 Returns            : -
 Description        : Sets up a render buffer's storage from an EGL image
************************************************************************************/
GL_API_EXT void GL_APIENTRY glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image)
{
	GLES2RenderBuffer *psRenderBuffer;
	EGLImage *psEGLImage;

	__GLES2_GET_CONTEXT();

	PVR_DPF((PVR_DBG_CALLTRACE,"glEGLImageTargetRenderbufferStorageOES"));

	GLES2_TIME_START(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

	if(target!=GL_RENDERBUFFER)
	{
		SetError(gc, GL_INVALID_ENUM);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

		return;
	}

	/* Check if image is a valid eglImageOES object */
	if(!KEGLGetImageSource(image, &psEGLImage))
	{
		PVR_DPF((PVR_DBG_WARNING,"glEGLImageTargetRenderbufferStorageOES: Invalid name"));

		SetError(gc, GL_INVALID_VALUE);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

		return;
	}

	/* The value of GL_MAX_RENDERBUFFER_SIZE is equal to GLES2_MAX_TEXTURE_SIZE */
	if((psEGLImage->ui32Width > GLES2_MAX_TEXTURE_SIZE) || (psEGLImage->ui32Height > GLES2_MAX_TEXTURE_SIZE))
	{
		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

		return;
	}

	psRenderBuffer = gc->sFrameBuffer.psActiveRenderBuffer;

	if(!psRenderBuffer)
	{
		PVR_DPF((PVR_DBG_WARNING,"glEGLImageTargetRenderbufferStorageOES: No active render buffer"));

		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

		return;
	}

	if(psRenderBuffer->psEGLImageSource)
	{
		KEGLUnbindImage(psRenderBuffer->psEGLImageSource->hImage);

		psRenderBuffer->psMemInfo = IMG_NULL;

		psRenderBuffer->psEGLImageSource = IMG_NULL;
	}
	else if(psRenderBuffer->psEGLImageTarget)
	{
		KEGLUnbindImage(psRenderBuffer->psEGLImageTarget->hImage);
	}

	psRenderBuffer->psEGLImageTarget = psEGLImage;

	if(!SetupRenderbufferFromEGLImage(gc, psRenderBuffer))
	{
		psRenderBuffer->psEGLImageTarget = IMG_NULL;

		SetError(gc, GL_INVALID_OPERATION);

		GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);

		return;
	}

	KEGLBindImage(psRenderBuffer->psEGLImageTarget->hImage);

	GLES2_TIME_STOP(GLES2_TIMES_glEGLImageTargetRenderbufferStorageOES);
}	


/***********************************************************************************
 Function Name      : ReleaseImageFromRenderbuffer
 Inputs             : gc, psRenderbuffer
 Outputs            : psRenderbuffer
 Returns            : -
 Description        : Unbinds an egl image from a renderbuffer
************************************************************************************/
IMG_INTERNAL IMG_VOID ReleaseImageFromRenderbuffer(GLES2Context *gc, GLES2RenderBuffer *psRenderBuffer)
{
	PVR_UNREFERENCED_PARAMETER(gc);

	KEGLUnbindImage(psRenderBuffer->psEGLImageTarget->hImage);

	psRenderBuffer->psEGLImageTarget = IMG_NULL;
}

#endif /* defined(GLES2_EXTENSION_EGL_IMAGE) */



#if defined(EGL_EXTENSION_KHR_IMAGE)

#if defined(GLES2_EXTENSION_EGL_IMAGE)
/***********************************************************************************
 Function Name      : GLESGetImageSourceParams
 Inputs             : hContext, ui32Source, ui32Name, ui32Level
 Outputs            : psEGLImage
 Returns            : Success/Failure
 Description        : Retrieves source parameters for an EGL image
************************************************************************************/
IMG_INTERNAL IMG_EGLERROR GLESGetImageSource(EGLContextHandle hContext, IMG_UINT32 ui32Source, IMG_UINT32 ui32Name, IMG_UINT32 ui32Level, EGLImage *psEGLImage)
{
	GLES2Context *gc = (GLES2Context *)hContext;
	IMG_UINT32 ui32Stride;

	switch(ui32Source)
	{
		case EGL_GL_TEXTURE_2D_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
		{
			IMG_UINT32 ui32OffsetInBytes, ui32BytesPerTexel, ui32TopUsize, ui32TopVsize, ui32OverloadTexLayout;
			PVRSRV_PIXEL_FORMAT ePixelFormat;
			GLES2TextureParamState *psParams;
			GLES2NamesArray *psNamesArray;
			GLES2MipMapLevel *psLevel;
		    GLES2Texture *psTex;
#if defined(EGL_IMAGE_COMPRESSED_GLES2)
			IMG_BOOL bCompressedRGBOnly = IMG_FALSE;
#endif

			/* Default texture is not allowed */
			if (ui32Name==0)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Invalid name"));

				return IMG_EGL_BAD_PARAMETER;
			}
			
			psNamesArray = gc->psSharedState->apsNamesArray[GLES2_NAMETYPE_TEXOBJ];

			psTex = (GLES2Texture *)NamedItemAddRef(psNamesArray, ui32Name);

			/* Is name a valid texture */
			if (psTex==IMG_NULL)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Invalid name"));

				return IMG_EGL_BAD_PARAMETER;
			}

			NamedItemDelRef(gc, psNamesArray, (GLES2NamedItem*)psTex);

			if(psTex->psEGLImageSource || psTex->psEGLImageTarget)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Texture source is an image sibling"));

				return IMG_EGL_BAD_ACCESS;
			}

			/* Make sure the texture isn't tiled */
			if((psTex->ui32LevelsConsistent==GLES2_TEX_CONSISTENT) && ((psTex->sState.aui32StateWord1[0] & EURASIA_PDS_DOUTT1_TEXTYPE_TILED) != 0))
			{
				/* IsTextureConsistent() will unload and set the texture to be stride */
				psTex->ui32LevelsConsistent = GLES2_TEX_UNKNOWN;

			}

			ui32OverloadTexLayout = GLES2_HINT_OVERLOAD_TEX_LAYOUT_STRIDE<<GLES2_HINT_OVERLOAD_TEX_LAYOUT_NONPOW2_SHIFT;

			/* Is texture consistent */
			if(IsTextureConsistent(gc, psTex, ui32OverloadTexLayout, IMG_FALSE) != GLES2_TEX_CONSISTENT)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Inconsistent texture"));

				psTex->psEGLImageSource = IMG_NULL;

				return IMG_EGL_BAD_PARAMETER;
			}

			psTex->psEGLImageSource = psEGLImage;

			if(ui32Level>(psTex->ui32NumLevels-1))
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Mipmap level out of range"));

				psTex->psEGLImageSource = IMG_NULL;

				return IMG_EGL_BAD_MATCH;
			}

			/* Need to make texture resident to get pointers */
			if(!TextureMakeResident(gc, psTex))
			{
				PVR_DPF((PVR_DBG_ERROR,"GLESGetImageSource: Can't make texture resident"));

				psTex->psEGLImageSource = IMG_NULL;

				return IMG_EGL_OUT_OF_MEMORY;
			}

			psLevel  = &psTex->psMipLevel[ui32Level];
			psParams = &psTex->sState;

#if defined(SGX_FEATURE_TAG_POT_TWIDDLE)
			ui32TopUsize = 1U << ((psParams->aui32StateWord1[0] & ~EURASIA_PDS_DOUTT1_USIZE_CLRMSK) >> EURASIA_PDS_DOUTT1_USIZE_SHIFT);
			ui32TopVsize = 1U << ((psParams->aui32StateWord1[0] & ~EURASIA_PDS_DOUTT1_VSIZE_CLRMSK) >> EURASIA_PDS_DOUTT1_VSIZE_SHIFT);
#else
			ui32TopUsize = 1U + ((psParams->aui32StateWord1[0] & ~EURASIA_PDS_DOUTT1_WIDTH_CLRMSK)  >> EURASIA_PDS_DOUTT1_WIDTH_SHIFT);
			ui32TopVsize = 1U + ((psParams->aui32StateWord1[0] & ~EURASIA_PDS_DOUTT1_HEIGHT_CLRMSK) >> EURASIA_PDS_DOUTT1_HEIGHT_SHIFT);
#endif

			ui32BytesPerTexel = psTex->psFormat->ui32TotalBytesPerTexel;

			ui32OffsetInBytes = ui32BytesPerTexel * GetMipMapOffset(ui32Level, ui32TopUsize, ui32TopVsize);

			/* PRQA S 3355 1 */ /* Override QAC suggestion as the condition may be false when ui32Source is EGL_GL_TEXTURE_2D_KHR. */
			if((ui32Source>=EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR) && (ui32Source<=EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR))
			{
				IMG_UINT32 ui32FaceOffset, ui32Face;
				
				if(psTex->ui32TextureTarget!=GLES2_TEXTURE_TARGET_CEM)
				{
					PVR_DPF((PVR_DBG_ERROR,"GLESGetImageSource: CEM source requested from non-CEM texture"));

					psTex->psEGLImageSource = IMG_NULL;

					return IMG_EGL_BAD_PARAMETER;
				}

				ui32Face = ui32Source - EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR;

				ui32FaceOffset = ui32BytesPerTexel * GetMipMapOffset(psTex->ui32NumLevels, ui32TopUsize, ui32TopVsize);

				if(psTex->ui32HWFlags & GLES2_MIPMAP)
				{
					if(((ui32BytesPerTexel == 1) && (ui32TopUsize > EURASIA_TAG_CUBEMAP_NO_ALIGN_SIZE_8BPP)) ||
						(ui32TopUsize > EURASIA_TAG_CUBEMAP_NO_ALIGN_SIZE_16_32BPP))
					{
						ui32FaceOffset = ALIGNCOUNT(ui32FaceOffset, EURASIA_TAG_CUBEMAP_FACE_ALIGN);
					}
				}

				ui32OffsetInBytes += (ui32FaceOffset * ui32Face);
			}

			switch(psTex->psFormat->ePixelFormat)
			{
				case PVRSRV_PIXEL_FORMAT_ABGR8888:
				{
					if(psTex->psFormat==&TexFormatABGR8888)
					{
						/* RGBA 8888 */
						ePixelFormat = PVRSRV_PIXEL_FORMAT_ABGR8888;
					}
					else
					{
						/* RGB 888 (RGBX 8888) */
						ePixelFormat = PVRSRV_PIXEL_FORMAT_XBGR8888;
					}

					break;
				}
#if defined(GLES2_EXTENSION_TEXTURE_FORMAT_BGRA8888)
				case PVRSRV_PIXEL_FORMAT_ARGB8888:
#endif
				case PVRSRV_PIXEL_FORMAT_ARGB1555:
				case PVRSRV_PIXEL_FORMAT_ARGB4444:
				case PVRSRV_PIXEL_FORMAT_RGB565:
				{
					ePixelFormat = psTex->psFormat->ePixelFormat;

					break;
				}
#if defined(EGL_IMAGE_COMPRESSED_GLES2)
				case PVRSRV_PIXEL_FORMAT_PVRTC2:
				case PVRSRV_PIXEL_FORMAT_PVRTC4:
				case PVRSRV_PIXEL_FORMAT_PVRTCII2:
				case PVRSRV_PIXEL_FORMAT_PVRTCII4:
				case PVRSRV_PIXEL_FORMAT_PVRTCIII:
				{
					ePixelFormat = psTex->psFormat->ePixelFormat;

					if(psTex->psFormat == &TexFormatPVRTC2RGBA ||
					   psTex->psFormat == &TexFormatPVRTC4RGBA ||
					   psTex->psFormat == &TexFormatPVRTCII2RGBA ||
					   psTex->psFormat == &TexFormatPVRTCII4RGBA) 
					{
						bCompressedRGBOnly = IMG_FALSE;
					}
					else if(psTex->psFormat == &TexFormatPVRTC2RGB ||
					   		psTex->psFormat == &TexFormatPVRTC4RGB ||
							psTex->psFormat == &TexFormatPVRTCII2RGB ||
						    psTex->psFormat == &TexFormatPVRTCII4RGB ||
							psTex->psFormat == &TexFormatETC1RGB) 
					{
						bCompressedRGBOnly = IMG_TRUE;
					}
					else 
					{
						PVR_DPF((PVR_DBG_ERROR,"GLESGetImageSource: Unknown compressed format"));
					}
		
					break;
				}
#endif /* defined(EGL_IMAGE_COMPRESSED_GLES2) */
				default:
				{
					PVR_DPF((PVR_DBG_ERROR,"GLESGetImageSource: Format mismatch with texture"));

					psTex->psEGLImageSource = IMG_NULL;

					return IMG_EGL_GENERIC_ERROR;
				}
			}

			psEGLImage->ui32Width			 = psLevel->ui32Width;
			psEGLImage->ui32Height			 = psLevel->ui32Height;
			psEGLImage->ePixelFormat		 = ePixelFormat;

#if (EURASIA_TAG_STRIDE_THRESHOLD > 0)
			if(psLevel->ui32Width < EURASIA_TAG_STRIDE_THRESHOLD)
			{
				ui32Stride = (psLevel->ui32Width + (EURASIA_TAG_STRIDE_ALIGN0 - 1)) & ~(EURASIA_TAG_STRIDE_ALIGN0 - 1);
			}
			else
#endif
			{			
				ui32Stride = (psLevel->ui32Width + (EURASIA_TAG_STRIDE_ALIGN1 - 1)) & ~(EURASIA_TAG_STRIDE_ALIGN1 - 1);
			}

			ui32Stride *= ui32BytesPerTexel;

			psEGLImage->ui32Stride		 	 = ui32Stride;
			psEGLImage->pvLinSurfaceAddress	 = (IMG_VOID*)(((IMG_UINTPTR_T)psTex->psMemInfo->pvLinAddr) + ui32OffsetInBytes);
			psEGLImage->ui32HWSurfaceAddress = psTex->psMemInfo->sDevVAddr.uiAddr + ui32OffsetInBytes;
			psEGLImage->psMemInfo			 = psTex->psMemInfo;

#if defined(EGL_IMAGE_COMPRESSED_GLES2)
			psEGLImage->bCompressedRGBOnly = bCompressedRGBOnly;
#endif /* defined(EGL_IMAGE_COMPRESSED_GLES2) */

			if((psParams->aui32StateWord1[0] & ~EURASIA_PDS_DOUTT1_TEXTYPE_CLRMSK) == EURASIA_PDS_DOUTT1_TEXTYPE_STRIDE)
			{
				psEGLImage->bTwiddled = IMG_FALSE;
			}
			else
			{
				psEGLImage->bTwiddled = IMG_TRUE;
			}

			break;
		}
		case EGL_GL_RENDERBUFFER_KHR:
		{
			GLES2RenderBuffer *psRenderBuffer;
			PVRSRV_PIXEL_FORMAT ePixelFormat;
			GLES2NamesArray *psNamesArray;
			IMG_UINT32 ui32BytesPerPixel;

			/* Default renderbuffer is not allowed */
			if (ui32Name==0)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Invalid name"));

				return IMG_EGL_BAD_PARAMETER;
			}

			psNamesArray = gc->psSharedState->apsNamesArray[GLES2_NAMETYPE_RENDERBUFFER];

			psRenderBuffer = (GLES2RenderBuffer *) NamedItemAddRef(psNamesArray, ui32Name);

			/* Is name a valid renderbuffer */
			if (psRenderBuffer==IMG_NULL)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Invalid name"));

				return IMG_EGL_BAD_PARAMETER;
			}

			NamedItemDelRef(gc, psNamesArray, (GLES2NamedItem*)psRenderBuffer);

			if(psRenderBuffer->psEGLImageSource || psRenderBuffer->psEGLImageTarget)
			{
				PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Renderbuffer is an image sibling"));

				return IMG_EGL_BAD_ACCESS;
			}

			switch(psRenderBuffer->eRequestedFormat)
			{
				case GL_RGB565:
				{
					ePixelFormat = PVRSRV_PIXEL_FORMAT_RGB565;

					ui32BytesPerPixel = 2;

					break;
				}
				case GL_RGBA4:
				{
					ePixelFormat = PVRSRV_PIXEL_FORMAT_ARGB4444;

					ui32BytesPerPixel = 2;

					break;
				}
				case GL_RGB5_A1:
				{
					ePixelFormat = PVRSRV_PIXEL_FORMAT_ARGB1555;

					ui32BytesPerPixel = 2;

					break;
				}
				case GL_RGBA8_OES:
				{
					ePixelFormat = PVRSRV_PIXEL_FORMAT_ABGR8888;

					ui32BytesPerPixel = 4;

					break;
				}
				case GL_RGB8_OES:
				{
					ePixelFormat = PVRSRV_PIXEL_FORMAT_XBGR8888;

					ui32BytesPerPixel = 4;

					break;
				}
				default:
				{
					PVR_DPF((PVR_DBG_ERROR,"GLESGetImageSource: Format mismatch with RenderbufferStorage"));

					return IMG_EGL_GENERIC_ERROR;
				}
			}

#if (EURASIA_TAG_STRIDE_THRESHOLD > 0)
			if(psRenderBuffer->ui32Width < EURASIA_TAG_STRIDE_THRESHOLD)
			{
				ui32Stride = (psRenderBuffer->ui32Width + (EURASIA_TAG_STRIDE_ALIGN0 - 1)) & ~(EURASIA_TAG_STRIDE_ALIGN0 - 1);
			}
			else
#endif
			{			
				ui32Stride = (psRenderBuffer->ui32Width + (EURASIA_TAG_STRIDE_ALIGN1 - 1)) & ~(EURASIA_TAG_STRIDE_ALIGN1 - 1);
			}

			ui32Stride *= ui32BytesPerPixel;

			psEGLImage->ui32Width			 = psRenderBuffer->ui32Width;
			psEGLImage->ui32Height			 = psRenderBuffer->ui32Height;
			psEGLImage->ePixelFormat		 = ePixelFormat;
			psEGLImage->ui32Stride		 	 = ui32Stride;
			psEGLImage->pvLinSurfaceAddress	 = psRenderBuffer->psMemInfo->pvLinAddr;
			psEGLImage->ui32HWSurfaceAddress = psRenderBuffer->psMemInfo->sDevVAddr.uiAddr;
			psEGLImage->psMemInfo			 = psRenderBuffer->psMemInfo;

			psEGLImage->bTwiddled			 = IMG_FALSE;

			psRenderBuffer->psEGLImageSource = psEGLImage;

			break;
		}
		default:
		{
			PVR_DPF((PVR_DBG_WARNING,"GLESGetImageSource: Invalid source"));

			return IMG_EGL_BAD_PARAMETER;
		}
	}

	return IMG_EGL_NO_ERROR;
}	

#else  /* defined(GLES2_EXTENSION_EGL_IMAGE) */

IMG_INTERNAL IMG_EGLERROR GLESGetImageSource(EGLContextHandle hContext, IMG_UINT32 ui32Source, IMG_UINT32 ui32Name, IMG_UINT32 ui32Level, EGLImage *psEGLImage)
{
	return IMG_EGL_GENERIC_ERROR;
}	

#endif /* defined(GLES2_EXTENSION_EGL_IMAGE) */

#endif /* defined(EGL_EXTENSION_KHR_IMAGE) */

/******************************************************************************
 End of file (eglimage.c)
******************************************************************************/
