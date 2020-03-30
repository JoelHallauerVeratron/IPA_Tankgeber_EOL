//#include <windows.h>
#include <windows.h>
#include <cvintwrk.h>
#include <ansi_c.h>
#include <userint.h>


#include "NetworkState.h"

#define PROD_SERVER_IP "192.168.20.6"
#define PNL_NETSTAT_UIR_FILE_NAME "NetworkState.uir"
#define PNL_NETSTAT_PANEL_UNDEFINED (-1)

int iPanelNetworkStatus_m=PNL_NETSTAT_PANEL_UNDEFINED;

BOOL bNetState_IsAvailable_g(void)
{
  int iRc;
  int iAvailable;

  iRc=InetPing (PROD_SERVER_IP, &iAvailable,100);
  if(iRc!=0)
  {
		printf("InetPing() failed %d <%s>\n",iRc,InetGetErrorMessage (iRc));
		return(FALSE);
  }
	return (iAvailable);
}

void vNetState_Show_g(void)
{
  if(iPanelNetworkStatus_m==PNL_NETSTAT_PANEL_UNDEFINED)
  {
    if((iPanelNetworkStatus_m=LoadPanel(0,PNL_NETSTAT_UIR_FILE_NAME,PNL_NETSTA))<0)
    {
      printf("Failed to load %s\n",PNL_NETSTAT_UIR_FILE_NAME);
      return;
    }
  }

  DisplayPanel(iPanelNetworkStatus_m);

}

void vNetState_Hide_g(void)
{
  if(iPanelNetworkStatus_m==PNL_NETSTAT_PANEL_UNDEFINED)
    return;

  HidePanel(iPanelNetworkStatus_m);

}
