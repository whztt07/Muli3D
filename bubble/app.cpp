
#include "app.h"
#include "../libappframework/include/input.h"
#include "../libappframework/include/graphics.h"
#include "../libappframework/include/resmanager.h"

#ifdef WIN32
#ifdef _DEBUG
	#pragma comment( lib, "../libappframework/lib/libappframeworkd.lib" )
#else
	#pragma comment( lib, "../libappframework/lib/libappframework.lib" )
#endif
#pragma comment( lib, "../libappframework/libpng/libpng.lib" )
#pragma comment( lib, "../libappframework/zlib/zlib.lib" )
#endif

#ifdef __amigaos4__
#	include <proto/intuition.h>
#endif

#include "mycamera.h"
#include "bubble.h"

bool CApp::bCreateWorld()
{
	m_pCamera = 0;
	m_hBubble = 0;

	// Create and setup camera ------------------------------------------------
	m_pCamera = new CMyCamera( pGetGraphics() );
	if( !m_pCamera->bCreateRenderCamera( iGetWindowWidth(), iGetWindowHeight() ) )
		return false;

	m_pCamera->CalculateProjection( M3D_PI / 6.0f, 1000.0f, 1.0f );

	m_pCamera->SetPosition( vector3( 0, 0, -100 ) );
	m_pCamera->SetLookAt( vector3( 0, 0, 0 ), vector3( 0, 1, 0 ) );
	m_pCamera->CalculateView();

	pGetScene()->SetClearColor( vector4( 0, 0, 0.5f, 0 ) );

	// Register bubble-entity and create an instance --------------------------
	pGetScene()->RegisterEntityType( "bubble", CBubble::pCreate );
	m_hBubble = pGetScene()->hCreateEntity( "bubble" );
	if( !m_hBubble )
		return false;

	CBubble *pBubble = (CBubble *)pGetScene()->pGetEntity( m_hBubble );
	if( !pBubble->bInitialize( 20.0f, 16, 16 ) )
		return false;

	return true;
}

void CApp::DestroyWorld()
{
	pGetScene()->ReleaseEntity( m_hBubble );
	SAFE_DELETE( m_pCamera );
}

void CApp::FrameMove()
{
#ifdef WIN32
	if( pGetInput()->bKeyDown( DIK_ESCAPE ) )
		PostQuitMessage( 0 );
#endif

	if( iGetFrameIdent() % 12 == 0 )
	{
		#ifdef __amigaos4__
		static
		#endif
		char szCaption[256];
		sprintf( szCaption, "Bubble, FPS: %3.1f", fGetFPS() );
		#ifdef WIN32
		SetWindowText( hGetWindowHandle(), szCaption );
		#endif
		#ifdef LINUX_X11
		XStoreName( (Display *)pGetDisplay(), hGetWindowHandle(), szCaption );
		#endif
		#ifdef __amigaos4__
		IIntuition->SetWindowTitles( hGetWindowHandle(), szCaption, szCaption );
		#endif
	}

	if( pGetInput()->bButtonDown( 0 ) )
	{
		// rotate camera around the bubble
		static float32 fRotX = 0.0f, fRotY = 0.0f;

		int32 iDeltaX, iDeltaY;

		pGetInput()->GetMovement( &iDeltaX, &iDeltaY );
		fRotX += -0.1f * iDeltaX * fGetInvFPS();
		fRotY += 0.1f * iDeltaY * fGetInvFPS();

		const float32 fCamPosX = -100.0f * sinf( fRotX );
		const float32 fCamPosY = -50.0f * sinf( fRotY );
		const float32 fCamPosZ = -100.0f * cosf( fRotX );
		m_pCamera->SetPosition( vector3( fCamPosX, fCamPosY, fCamPosZ ) );
		m_pCamera->SetLookAt( vector3( 0, 0, 0 ), vector3( 0, 1, 0 ) );
		m_pCamera->CalculateView();
	}
}

void CApp::RenderWorld()
{
	if( m_pCamera )
	{
		m_pCamera->BeginRender();
		m_pCamera->ClearToSceneColor();
		m_pCamera->RenderPass( -1 );
		m_pCamera->EndRender( true );
	}
}
