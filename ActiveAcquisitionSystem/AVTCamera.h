#pragma once
#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
//#include <string>

class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera();
	AVTCamera(AVT::VmbAPI::CameraPtr avtCam);
	~AVTCamera();

	void setCameraPtr(AVT::VmbAPI::CameraPtr avtCam);
	bool loadSettings();
	bool loadSettings(std::string configPath);


	std::string getAnyStr()
	{
		if (pCam == NULL)
		{
			std::string ns = "no se cuando paso, ni como sucedio, lo unico que yo se es que me muero por tii.";
			printf("ns: %s\n", ns.c_str());
			return ns;
		}
		pCam->Open(VmbAccessModeFull);
		
		pCam->Close();
		return "any string";
	}
	AVT::VmbAPI::CameraPtr getAVTPointer() { 
		return pCam; }

	void setName(std::string n) {name = n; }
	std::string getName() { 
		const char* n = name.c_str();
		printf("%s:\n", n);

		return name; }

private:
//	VimbaSystem& sys;

	std::string name;
	std::string model;
	std::string serial;
	std::string interfaceID;//i.e. ETHERNET for GigE cameras

	std::string ipAddress;

	AVT::VmbAPI::CameraPtr pCam;
	
};

// TODO Check this out to shot the camera:
/*
void CSynchronousGrabDlg::OnBnClickedButtonStartstop()
{
VmbErrorType    err;
int             nRow = m_ListBoxCameras.GetCurSel();

if ( -1 < nRow )
{
// Get the frame
FramePtr pFrame;
err = m_pApiController->AcquireSingleImage( m_cameras[nRow], pFrame );
if ( VmbErrorSuccess == err )
{
// See if it is not corrupt
VmbFrameStatusType eReceiveStatus;
err = pFrame->GetReceiveStatus( eReceiveStatus );
if (    VmbErrorSuccess == err
&& VmbFrameStatusComplete == eReceiveStatus )
{
// Set up image for MFC picture box
if ( NULL != m_Image )
{
m_Image.Destroy();
}
m_Image.Create(  m_pApiController->GetWidth(),
-m_pApiController->GetHeight(),
NUM_COLORS * BIT_DEPTH );

VmbUchar_t *pBuffer;
VmbUchar_t *pColorBuffer = NULL;
err = pFrame->GetImage( pBuffer );
if ( VmbErrorSuccess == err )
{
VmbUint32_t nSize;
err = pFrame->GetImageSize( nSize );
if ( VmbErrorSuccess == err )
{
VmbPixelFormatType ePixelFormat = m_pApiController->GetPixelFormat();
if (    VmbPixelFormatMono8 == ePixelFormat
||    VmbPixelFormatBgr8 == ePixelFormat )
{
// Convert mono to bgr if necessary
if ( VmbPixelFormatMono8 == ePixelFormat )
{
pColorBuffer = new VmbUchar_t[ nSize * NUM_COLORS ];
MonoToBGR( pBuffer, pColorBuffer, nSize );
pBuffer = pColorBuffer;
}

// Copy it
// We need that because MFC might repaint the view after we have released the frame already
CopyToImage( pBuffer, &m_Image );

if ( NULL != pColorBuffer )
{
delete[] pColorBuffer;
pColorBuffer = NULL;
}

// Display it
RECT rect;
m_PictureBoxStream.GetWindowRect( &rect );
ScreenToClient( &rect );
InvalidateRect( &rect, false );
}
}
}
}
else
{
// If we receive an incomplete image we do nothing
err = VmbErrorOther;
}
}
Log( _TEXT("Acquire single image"), err );
}
else
{
Log( _TEXT( "Please select a camera." ) );
}
}
*/
