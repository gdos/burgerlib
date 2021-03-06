/***************************************

	Static position texturing shader

	Copyright (c) 1995-2016 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!
	
***************************************/

#ifndef __BREFFECTSTATICPOSITIONTEXTURE_H__
#define __BREFFECTSTATICPOSITIONTEXTURE_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRMATRIX4D_H__
#include "brmatrix4d.h"
#endif

#ifndef __BRDISPLAY_H__
#include "brdisplay.h"
#endif

#ifndef __BREFFECT_H__
#include "breffect.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRDISPLAYDIRECTX9_H__)
#include "brdisplaydirectx9.h"
#endif

#ifndef __BRDISPLAYOPENGL_H__
#include "brdisplayopengl.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRWINDOWSTYPES_H__)
#include "brwindowstypes.h"
#endif

#if defined(BURGER_XBOX360) && !defined(__BRXBOX360TYPES_H__)
#include "brxbox360types.h"
#endif

/* BEGIN */
namespace Burger {
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
class EffectPositionTextureDX9 : public EffectDX9 {
	BURGER_RTTI_IN_CLASS();
public:
	EffectPositionTextureDX9(DisplayDirectX9 *pDisplay,const Word *pVertexMembers);
	void BURGER_API SetProjection(const Matrix4D_t *pMatrix);
};
#endif
#if defined(BURGER_OPENGL_SUPPORTED)
class EffectPositionTextureOpenGL : public EffectOpenGL {
	BURGER_RTTI_IN_CLASS();
protected:
	Int m_iEffectMatrix;		///< Index for the Matrix
public:
	EffectPositionTextureOpenGL(Display *pDisplay,const Word *pVertexMembers);
	void BURGER_API SetProjection(const Matrix4D_t *pMatrix);
};
#endif
#if defined(BURGER_XBOX360) || defined(DOXYGEN)
class EffectPositionTexture : public Effect {
	BURGER_RTTI_IN_CLASS();
public:
	EffectPositionTexture(Display *pDisplay,const Word *pVertexMembers);
	void BURGER_API SetProjection(const Matrix4D_t *pMatrix);
};
#endif

#if defined(BURGER_WINDOWS)
typedef EffectPositionTextureDX9 EffectPositionTexture;
#elif defined(BURGER_OPENGL_SUPPORTED)
typedef EffectPositionTextureOpenGL EffectPositionTexture;
#endif
}
/* END */

#endif
