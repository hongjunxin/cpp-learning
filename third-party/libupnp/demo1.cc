#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upnp/upnp.h>

// 更多 demo 参考 libupnp upnp/sample 目录

// event 指向一个结构体，该结构体的类型由 event_type 决定
int discovery_callback(Upnp_EventType event_type, const void *event, void *cookie)
{
    if (event_type == UPNP_DISCOVERY_SEARCH_RESULT) {
        UpnpDiscovery *d_event = (UpnpDiscovery*) event;

		IXML_Document *DescDoc = NULL;
		const char *location = NULL;
		int errCode = UpnpDiscovery_get_ErrCode(d_event);

		// if (errCode != UPNP_E_SUCCESS) {
        //     printf("UpnpDiscovery_get_ErrCode code %d\n", errCode);
        //     return errCode;
		// }

		printf("ErrCode     =  %d\n"
				 "Expires     =  %d\n"
				 "DeviceId    =  %s\n"
				 "DeviceType  =  %s\n"
				 "ServiceType =  %s\n"
				 "ServiceVer  =  %s\n"
				 "Location    =  %s\n"
				 "OS          =  %s\n"
				 "Date        =  %s\n"
				 "Ext         =  %s\n"
                 "\n",
			UpnpDiscovery_get_ErrCode(d_event),
			UpnpDiscovery_get_Expires(d_event),
			UpnpString_get_String(
				UpnpDiscovery_get_DeviceID(d_event)),
			UpnpString_get_String(
				UpnpDiscovery_get_DeviceType(d_event)),
			UpnpString_get_String(
				UpnpDiscovery_get_ServiceType(d_event)),
			UpnpString_get_String(
				UpnpDiscovery_get_ServiceVer(d_event)),
			UpnpString_get_String(
				UpnpDiscovery_get_Location(d_event)),
			UpnpString_get_String(UpnpDiscovery_get_Os(d_event)),
			UpnpString_get_String(UpnpDiscovery_get_Date(d_event)),
			UpnpString_get_String(UpnpDiscovery_get_Ext(d_event)));

    }
    
    // printf("Upnp_EventType: %d\n", event_type);

    return 0;
}

int main() {
    // Initialize libupnp
    UpnpInit2(NULL, 0);

    // Register a UPnP client and set the device discovery callback
    UpnpClient_Handle client_handle;
    UpnpRegisterClient(discovery_callback, NULL, &client_handle);

    // Search for UPnP devices
    UpnpSearchAsync(client_handle, 5, "upnp:rootdevice", NULL);

    // Wait for the discovery process to complete (you can implement your own logic for this)
    sleep(10);

    // Clean up and exit
    UpnpUnRegisterClient(client_handle);
    UpnpFinish();
    return 0;
}
