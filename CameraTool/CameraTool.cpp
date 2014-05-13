//------------------------------------------------------------------------------ 
// 
//  SimConnect Cockpit Camera Sample 
//  
//  Description: 
//              Press the < and > keys (actually the , and . keys, as there is 
//              no use of the shift key) to rotate the pilot's view 
//------------------------------------------------------------------------------ 

#include <windows.h> 
#include <tchar.h> 
#include <iostream> 
#include <strsafe.h> 

#include "SimConnect.h" 
#include "config.h"




int     quit = 0;
HANDLE  hSimConnect = NULL;


static enum GROUP_ID {
	GROUP0,
};

static enum EVENT_ID {
	EVENT_CAMERA_RIGHT,
	EVENT_CAMERA_LEFT,
	EVENT_CAMERA_UP,
	EVENT_CAMERA_DOWN,
	EVENT_CAMERA_Xp,
	EVENT_CAMERA_Xn
};

static enum INPUT_ID {
	INPUT0,
};


static float cameraBank = 0.0f;
static float cameraMoveForward = 0.0f;
static float cameraLeftRight = 0.0f;
float normalize180(float v)
{
	while (v < -180.0f) v += 360.0f;
	while (v > 180.0f) v -= 360.0f;
	return v;
}

void CALLBACK MyDispatchProcCC(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	HRESULT hr;

	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
		{
										 SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

										 switch (evt->uEventID)
										 {
											 case EVENT_CAMERA_RIGHT:

												 cameraBank = normalize180(cameraBank + 2.0f);

												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);

												 printf("\nOrientation = %f", cameraBank);
												 break;

											 case EVENT_CAMERA_LEFT:

												 cameraBank = normalize180(cameraBank - 2.0f);

												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);

												 printf("\nOrientation = %f", cameraBank);
												 break;
											 case EVENT_CAMERA_UP:
												 cameraMoveForward += 0.1;
												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);
												 printf("\nAvancement = %f", cameraMoveForward);
												 break;
											 case EVENT_CAMERA_DOWN:
												 cameraMoveForward -= 0.1;
												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);
												 printf("\nAvancement = %f", cameraMoveForward);
												 break;
											 case EVENT_CAMERA_Xp:
												 cameraLeftRight += 0.1;
												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);
												 printf("\nLeftRight= %f", cameraLeftRight);
												 break;
											 case EVENT_CAMERA_Xn:
												 cameraLeftRight -= 0.1;
												 hr = SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight, 0.0f, cameraMoveForward,
																					   SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);
												 printf("\nLeftRight = %f", cameraLeftRight);
												 break;


											 default:
												 break;
										 }
										 break;
		}

		case SIMCONNECT_RECV_ID_EXCEPTION:
		{
											 SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;

											 switch (except->dwException)
											 {
												 case SIMCONNECT_EXCEPTION_ERROR:
													 printf("\nCamera error");
													 break;

												 default:
													 printf("\nException");
													 break;
											 }
											 break;
		}

		case SIMCONNECT_RECV_ID_QUIT:
		{
										quit = 1;
										break;
		}

		default:
			break;
	}
}

void CockpitCamera()
{
	HRESULT hr;

	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Cockpit Camera", NULL, 0, 0, 0)))
	{
		printf("\nConnected...!");

		// Define private events 
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_RIGHT);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_LEFT);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_UP);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_DOWN);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_Xp);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_Xn);

		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_RIGHT);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_LEFT);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_UP);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_DOWN);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_Xp);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_Xn);

		hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

		// Map the keys , and . keys to the private events 
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+o", EVENT_CAMERA_RIGHT);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+u", EVENT_CAMERA_LEFT);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+i", EVENT_CAMERA_UP);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+k", EVENT_CAMERA_DOWN);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+l", EVENT_CAMERA_Xp);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "ctrl+w+j", EVENT_CAMERA_Xn);

		hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

		//position initialisation
		SimConnect_CameraSetRelative6DOF(hSimConnect, cameraLeftRight,0.0f, cameraMoveForward,
										 SIMCONNECT_CAMERA_IGNORE_FIELD, SIMCONNECT_CAMERA_IGNORE_FIELD, cameraBank);

		while (0 == quit)
		{
			SimConnect_CallDispatch(hSimConnect, MyDispatchProcCC, NULL);
			Sleep(1);
		}

		hr = SimConnect_Close(hSimConnect);
	}
}

using namespace safe;

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	safe::loadConfig();
	try{
		const Setting &camera = safe::cfg_Safe.lookup("camera");
		if (!camera.lookupValue("initAvancement", cameraMoveForward)) std::cerr << "setting 'moveForward' on 'camera' not found" << std::endl;
		if (!camera.lookupValue("initX", cameraLeftRight)) std::cerr << "setting 'initX' on 'camera' not found" << std::endl;
		if (!camera.lookupValue("initOrientation", cameraBank)) std::cerr << "setting 'bank' on 'camera' not found" << std::endl;
		

	}
	catch (SettingNotFoundException ex)
	{
		std::cerr << "setting 'camera' not found on " << configFile << std::endl;
	}


	CockpitCamera();

	system("pause");
	return 0;
}