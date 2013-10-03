#include <stdio.h>
#include <time.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <unistd.h>

#include "util.h"
#include "porting.h"

static int _gMetadataVersion = 1;
static int _gDiscoveryMode = 0;
static int _gInstanceId = 1;
char *nativeGetXAddrs()
{
	char pTmp[1024] = {0};
	
	sprintf(pTmp, "http://%s:80/onvif/device_service", getMyIpString());
	return CopyString(pTmp);
}

char *nativeGetEndpointAddress()
{
	char pTmp[1024]={0};
	
	sprintf(pTmp, "urn:uuid:00075f74-9ef6-f69e-745f-%s", getMyMacAddress());
	
	// It is RECOMMENDED that the balue of this element be a stable globally-unique identifier (GUID) base URN[RFC 4122]
	// If the value of this element is not a network-resolvable transport address, 
	// such tansport address(es) are converyed in a separate d:XAddrs element
	
	// "urn:uuid:98190dc2-0890-4ef8-ac9a-5940995e6119" is a example of wsdd-discovery-1.1-spec-cs-01.pdf
	return CopyString(pTmp);
}

char *nativeGetTypes()
{
	// For old version, return "dn:NetworkVideoTransmitter"
	//return CopyString("tds:Device");
	return CopyString("dn:NetworkVideoTransmitter");
}

char *nativeGetScopesItem()
{
	// TODO: the scopes may change, we should reload it every time user invoke this function
	return CopyString("\
onvif://www.onvif.org/type/audio_encoder \
onvif://www.onvif.org/type/video_encoder \
onvif://www.onvif.org/name/albert \
onvif://www.onvif.org/hardware/undefined \
onvif://www.onvif.org/location/ \
onvif://www.onvif.org/Profile/Streaming");
}

char *nativeGetMessageId()
{
	char pTmp[] = "urn:uuid:73948edc-3204-4455-bae2-7c7d0ff6c37c";
	int vLen = strlen(pTmp)-1;
	
	// TODO: fix me
	pTmp[vLen] += _gInstanceId;
	
	return CopyString(pTmp);
	
	// TODO: should return different MessageId
	//return CopyString("urn:uuid:73948edc-3204-4455-bae2-7c7d0ff6c37c");
}

char *nativeGetTo()
{
	// In an ad hoc mode, it MUST be "urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01"
	return CopyString("urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01");
	
	// If this is a response message
	// CopyString("http://www.w3.org/2006/08/addressing/anonymous");
	
	// In a managed mode, it MUST be the [address] property of the Endpoint Reference of the Discovery Proxy.
}

int nativeGetInstanceId()
{
	return _gInstanceId++;
}

int nativeGetMetadataVersion()
{
	return _gMetadataVersion;
}

void nativeIncreaseMetadataVersion()
{
	_gMetadataVersion++;
}

int nativeGetDiscoveryMode()
{
	return _gDiscoveryMode;
}

void nativeChangeDiscoveryMode(char Mode)
{
	if(Mode=='0')
	{
		printf("NONDISCOVERABLE !! \n");
		_gDiscoveryMode = NONDISCOVERABLE;
	}
	else
	{
			printf("DISCOVERABLE !! \n");
		_gDiscoveryMode = DISCOVERABLE;
	}
}