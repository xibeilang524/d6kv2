/*
 * Copyright 1993 Network Computing Devices, Inc.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 * 
 * THIS SOFTWARE IS PROVIDED 'AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * $NCDId: @(#)Aproto.h,v 1.34 1995/12/06 01:30:44 greg Exp $
 */

/* Portions derived from */
/*
 * $XConsortium: Xproto.h,v 1.86 92/10/18 16:33:19 rws Exp $
 */

/* Definitions for the X window system used by server and c bindings */

/*
 * This packet-construction scheme makes the following assumptions:
 * 
 * 1. The compiler is able to generate code which addresses one- and two-byte
 * quantities. In the worst case, this would be done with bit-fields.  If
 * bit-fields are used it may be necessary to reorder the request fields in
 * this file, depending on the order in which the machine assigns bit fields
 * to machine words.  There may also be a problem with sign extension, as K+R
 * specify that bitfields are always unsigned.
 * 
 * 2. 2- and 4-byte fields in packet structures must be ordered by hand such
 * that they are naturally-aligned, so that no compiler will ever insert
 * padding bytes.
 * 
 * 3. All packets are hand-padded to a multiple of 4 bytes, for the same reason.
 */

#ifndef NCD_AUDIO_PROTO_H
#define NCD_AUDIO_PROTO_H

/***********************************************************
Portions of the comments are
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <audio/Amd.h>

/*
 * Define constants for the sizes of the network packets.  The sz_ prefix is
 * used instead of something more descriptive so that the symbols are no more
 * than 32 characters in length (which causes problems for some compilers).
 */


/*
 * For the purpose of the structure definitions in this file, we must
 * redefine the following types in terms of Xmd.h's types, which may include
 * bit fields.  All of these are #undef'd at the end of this file, restoring
 * the definitions in X.h.
 */

#define AuDeviceID			CARD32
#define AuBucketID			CARD32
#define AuFlowID			CARD32
#define AuRadioID			CARD32
#define AuTime				CARD32


#define AU_DEFAULT_TCP_PORT		8000
#define AU_DEFAULT_DECNET_TASK		"AUDIO$"
#define AU_DEFAULT_UNIX_PATH		"/tmp/.sockets/audio"

#define auFalse				0
#define auTrue				1


/*
 * Request codes
 */
#define Au_ListDevices			1
#define Au_GetDeviceAttributes		2
#define Au_SetDeviceAttributes		3

#define Au_CreateBucket			4
#define Au_DestroyBucket		5
#define Au_ListBuckets			6
#define Au_GetBucketAttributes		7
#define Au_SetBucketAttributes		8

#define Au_CreateRadio			9
#define Au_DestroyRadio			10
#define Au_ListRadios			11
#define Au_GetRadioAttributes		12
#define Au_SetRadioAttributes		13

#define Au_CreateFlow			14
#define Au_DestroyFlow			15
#define Au_GetFlowAttributes		16
#define Au_SetFlowAttributes		17
#define Au_GetElements			18
#define Au_SetElements			19
#define Au_GetElementStates		20
#define Au_SetElementStates		21
#define Au_GetElementParameters		22
#define Au_SetElementParameters		23
#define Au_WriteElement			24
#define Au_ReadElement			25

#define Au_GrabComponent		26
#define Au_UngrabComponent		27
#define Au_SendEvent			28

#define Au_GetAllowedUsers		29
#define Au_SetAllowedUsers		30

#define Au_ListExtensions		31
#define Au_QueryExtension		32
#define Au_GetCloseDownMode		33
#define Au_SetCloseDownMode		34
#define Au_KillClient			35
#define Au_GetServerTime		36
#define Au_NoOperation			37

/*
 * Reply codes
 */
#define Au_Error			0
#define Au_Reply			1




/*****************************************************************************
 *				  DATA TYPES				     *
 *****************************************************************************/



#ifndef NO_AUDIO_REQUESTS

/*****************************************************************************
 *			       CONNECTION SETUP				     *
 *****************************************************************************/

typedef struct
{
    CARD8           byteOrder;
    BYTE            pad;
    CARD16          majorVersion B16;
    CARD16          minorVersion B16;
    CARD16          nbytesAuthProto B16;	/* Authorization protocol */
    CARD16          nbytesAuthString B16;	/* Authorization string */
    CARD16          pad1 B16;
}               auConnClientPrefix;
#define sz_auConnClientPrefix		12

typedef struct
{
    AUBOOL          success;
    BYTE            lengthReason;		/* bytes in string following
						 * if failure */
    CARD16          majorVersion B16;
    CARD16          minorVersion B16;
    CARD16          length B16;			/* 1/4 additional bytes in
						 * setup info */
}               auConnSetupPrefix;
#define sz_auConnSetupPrefix		8


typedef struct
{
    CARD32          release B32;
    CARD32          ridBase B32;
    CARD32          ridMask B32;
    CARD16          minSampleRate B16;
    CARD16          maxSampleRate B16;
    CARD16          nbytesVendor B16;		/* bytes in vendor string */
    CARD16          maxRequestSize B16;
    CARD8           maxTracks;
    CARD8           numFormats;
    CARD8           numElementTypes;
    CARD8           numWaveForms;
    CARD8	    numActions;
    CARD8           numDevices;
    CARD8           numBuckets;
    CARD8           numRadios;
    /* vendor string */
    /* formats */
    /* element types */
    /* wave forms */
    /* actions */
    /* device attributes */
    /* bucket attributes */
    /* radio attributes */
}               auConnSetup;
#define sz_auConnSetup			28

typedef struct
{
    CARD8           type;			/* AuStringLatin1, ... */
    BYTE            pad;
    BYTE            pad1;
    BYTE            pad2;
    CARD32          len B32;			/* length in bytes */
    /* string */
}               auString;
#define sz_auString 			8

/*
 * Attributes used for Devices, Buckets, and Radios
 */
typedef struct
{
    CARD32          value_mask B32;		/* what is present */
    CARD32          changable_mask B32;		/* what can be changed */
    AuID            id B32;			/* used to reference in flows */
    CARD8           kind;			/* what type of object is it */
    CARD8           use;			/* importable, exportable,
						 * etc. */
    CARD8           format;			/* current value */
    CARD8           num_tracks;			/* 1 for mono, 2 for stereo,
						 * etc. */
    CARD32          access B32;			/* who can access */
    auString        description;		/* description of device */
}               auCommonPart;
#define sz_auCommonPart			(20 + sz_auString)


/*
 * Device attributes - these are physical devices attached to the server.
 */
typedef struct
{
    CARD32          location B32;		/* mask of bits */
    CARD32          gain B32;			/* 16.16% */
    CARD16          min_sample_rate B16;
    CARD16          max_sample_rate B16;
    CARD8           line_mode;			/* high vs. low amp circuit */
    CARD8           num_children;		/* number of subdevices, if
						 * any */
    CARD16          pad B16;
    /* children */
}               auDevicePart;
#define sz_auDevicePart			16

typedef struct
{
    auCommonPart    common;
    auDevicePart    device;
}               auDeviceAttributes;
#define sz_auDeviceAttributes		(sz_auCommonPart + sz_auDevicePart)

/*
 * Bucket attributes - these are server storage for sound data.
 */
typedef struct
{
    CARD16          sample_rate B16;		/* current value */
    CARD16          pad B16;
    CARD32          num_samples B32;		/* max number in this device */
}               auBucketPart;
#define sz_auBucketPart			8

typedef struct
{
    auCommonPart    common;
    auBucketPart    bucket;
}               auBucketAttributes;
#define sz_auBucketAttributes		(sz_auCommonPart + sz_auBucketPart)

#ifdef notdef
/*
 * Radio attributes - these are special devices for LAN broadcast audio data
 */
typedef struct
{
    CARD32          station B32;		/* which one you are
						 * listening to */
}               auRadioPart;
#define sz_auRadioPart			4

typedef struct
{
    auCommonPart    common;
    auRadioPart     radio;
}               auRadioAttributes;
#define sz_auRadioAttributes		(sz_auCommonPart + sz_auRadioPart)

#endif						/* notdef */


/*****************************************************************************
 *			     REQUESTS and REPLIES			     *
 *****************************************************************************/

typedef struct
{
    CARD8           reqType;
    CARD8           data;			/* meaning depends upon req
						 * type */
    CARD16          length B16;			/* 4-byte quants, including
						 * header */
}               auReq;
#define sz_auReq			4

/*
 * ResourceReq is used for any request which has a resource ID (or Atom or
 * Time) as its one and only argument.
 */
typedef struct
{
    CARD8           reqType;
    BYTE            pad;
    CARD16          length B16;
    AuID            id B32;			/* a Flow, Buffer, or Device */
}               auResourceReq;
#define sz_auResourceReq		8


/*
 * GenericReply is the common format of all replies.  The "data" items are
 * specific to each individual reply type.
 */
typedef struct
{
    BYTE            type;			/* Au_Reply */
    BYTE            data1;			/* depends on reply type */
    CARD16          sequenceNumber B16;		/* of last request received */
    CARD32          length B32;			/* 4 byte quant beyond
						 * GenericReply */
    CARD32          data00 B32;
    CARD32          data01 B32;
    CARD32          data02 B32;
    CARD32          data03 B32;
    CARD32          data04 B32;
    CARD32          data05 B32;
}               auGenericReply;


/*
 * see down below EVENTS for the auReply object.
 */


/* CreateFlow uses a ResourceReq */
/* DestroyFlow uses a ResourceReq */
/* GetDeviceAttributes uses a ResourceReq */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    CARD32          pad6 B32;
    /* DEVICEATTRIBUTES */
}               auGetDeviceAttributesReply;
#define sz_auGetDeviceAttributesReply	32

/* SetDeviceAttributes uses a ResourceReq */

/* CreateBucket uses a ResourceReq */
/* DestroyBucket uses a ResourceReq */
/* GetBucketAttributes uses a ResourceReq */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    CARD32          pad6 B32;
    /* BUCKETATTRIBUTES */
}               auGetBucketAttributesReply;
#define sz_auGetBucketAttributesReply	32

/* ListBuckets uses a Req */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          num_buckets B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    /* BUCKETATTRIBUTES */
}               auListBucketsReply;
#define sz_auListBucketsReply	32

/* ListDevices uses a Req */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          num_devices B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    /* DEVICEATTRIBUTES */
}               auListDevicesReply;
#define sz_auListDevicesReply	32

/* GetElements uses a ResourceReq */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    AUBOOL          clocked;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    AuFlowID        flow B32;
    CARD32          num_elements B32;
    CARD32          pad B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    /* LISTofELEMENTS */
}               auGetElementsReply;
#define sz_auGetElementsReply	32


/* ELEMENTS */

/* ACTIONS */
typedef struct
{
    AuFlowID        flow B32;			/* target flow to affect */
    CARD8           element_num;		/* target element to affect */
    CARD8           trigger_state;		/* state to trigger action */
    CARD8           trigger_prev_state;		/* previous state to trigger
						 * action */
    CARD8           trigger_reason;		/* reason to trigger action */
    CARD8           action;			/* what to do */
    CARD8           new_state;			/* new state for target */
    CARD16          pad B16;
}               auElementAction;
#define sz_auElementAction 		12

typedef struct
{
    CARD32          num_actions B32;
    /* LISTofACTIONS */
}               auElementActionList;
#define sz_auElementActionList		4

/* IMPORTS */

typedef struct
{
    CARD16          type B16;			/* AuElementTypeImportClient */
    CARD16          sample_rate B16;
    CARD8           format;
    CARD8           num_tracks;
    AUBOOL          discard;
    CARD8           pad;
    CARD32          max_samples B32;
    CARD32          low_water_mark B32;
    auElementActionList actions;
}               auElementImportClient;
#define sz_auElementImportClient	(16 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeImportDevice */
    CARD16          sample_rate B16;
    CARD32          num_samples B32;
    AuDeviceID      device B32;
    auElementActionList actions;
}               auElementImportDevice;
#define sz_auElementImportDevice	(12 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeImportBucket */
    CARD16          sample_rate B16;
    CARD32          num_samples B32;
    AuBucketID      bucket B32;
    CARD32          offset B32;
    auElementActionList actions;
}               auElementImportBucket;
#define sz_auElementImportBucket	(16 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeImportWaveForm */
    CARD16          sample_rate B16;
    CARD32          num_samples B32;
    CARD8           wave_form;
    CARD8           pad;
    CARD8           pad1;
    CARD8           pad2;
    CARD32          frequency B32;
    auElementActionList actions;
}               auElementImportWaveForm;
#define sz_auElementImportWaveForm		(16 + sz_auElementActionList)

/* OPS */

typedef struct
{
    CARD8           element_num;
    CARD8           track;
    CARD16          pad B16;
}               auInputTrack;
#define sz_auInputTrack			4

typedef struct
{
    CARD16          type B16;			/* AuElementTypeBundle */
    CARD16          num_inputs B16;
    /* LISTofTRACKS */
}               auElementBundle;
#define sz_auElementBundle		4

typedef struct
{
    CARD16          type B16;			/* AuElementTypeMultiplyConsta
						 * nt */
    CARD16          input B16;
    CARD32          constant B32;		/* 16.16 */
}               auElementMultiplyConstant;
#define sz_auElementMultiplyConstant	8

typedef struct
{
    CARD16          type B16;			/* AuElementTypeAddConstant */
    CARD16          input B16;
    CARD32          constant B32;		/* 16.16 */
}               auElementAddConstant;
#define sz_auElementAddConstant		8

typedef struct
{
    CARD16          type B16;			/* AuElementTypeSum */
    CARD16          num_inputs B16;
    /* LISTofCARD16 */
}               auElementSum;
#define sz_auElementSum			4

/* EXPORTS */

typedef struct
{
    CARD16          type B16;			/* AuElementTypeExportClient */
    CARD16          sample_rate B16;
    CARD16          input B16;
    CARD16          pad B16;
    CARD8           format;
    CARD8           num_tracks;
    AUBOOL          discard;
    CARD8           pad1;
    CARD32          max_samples B32;
    CARD32          high_water_mark B32;
    auElementActionList actions;
}               auElementExportClient;
#define sz_auElementExportClient	(20 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeExportDevice */
    CARD16          sample_rate B16;
    CARD16          input B16;
    CARD16          pad B16;
    CARD32          num_samples B32;
    AuDeviceID      device B32;
    auElementActionList actions;
}               auElementExportDevice;
#define sz_auElementExportDevice	(16 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeExportBucket */
    CARD16          input B16;
    CARD32          num_samples B32;
    AuBucketID      bucket B32;
    CARD32          offset B32;
    auElementActionList actions;
}               auElementExportBucket;
#define sz_auElementExportBucket	(16 + sz_auElementActionList)

typedef struct
{
    CARD16          type B16;			/* AuElementTypeExportMonitor */
    CARD16          event_rate B16;
    CARD16          input B16;
    CARD16          pad B16;
    CARD8           format;
    CARD8           num_tracks;
    CARD16          pad1 B16;
}               auElementExportMonitor;
#define sz_auElementExportMonitor	12

#define sz_auElementMAX			sz_auElementExportClient

typedef union					/* generic element */
{
    CARD16          type B16;
    auElementImportClient importclient;
    auElementImportDevice importdevice;
    auElementImportBucket importbucket;
    auElementImportWaveForm importwaveform;
#ifdef NOTYET
    auElementImportRadio importradio;
#endif						/* NOTYET */
    auElementBundle bundle;
    auElementMultiplyConstant multiplyconstant;
    auElementAddConstant addconstant;
    auElementSum    sum;
    auElementExportClient exportclient;
    auElementExportDevice exportdevice;
    auElementExportBucket exportbucket;
#ifdef NOTYET
    auElementExportRadio exportradio;
#endif						/* NOTYET */
    auElementExportMonitor exportmonitor;
}               auElement;
#define sz_auElement			sz_auElementMAX

typedef struct
{
    CARD8           reqType;
    AUBOOL          clocked;
    CARD16          length B16;
    AuFlowID        flow B32;
    CARD32          numElements B32;
    /* LISTofELEMENTS */
}               auSetElementsReq;
#define sz_auSetElementsReq		12

/* KillClient uses a ResourceReq */

typedef struct
{
    AuFlowID        flow B32;
    CARD8           element_num;
    CARD8           state;
    CARD16          pad B16;
}               auElementState;
#define sz_auElementState		8

typedef struct
{
    CARD8           reqType;
    CARD8           pad;
    CARD16          length B16;
    CARD32          numStates B32;
    /* LISTofELEMENTSTATES */
}               auSetElementStatesReq;
#define sz_auSetElementStatesReq 	8

typedef struct
{
    CARD8           reqType;
    CARD8           pad;
    CARD16          length B16;
    CARD32          numStates B32;
    /* LISTofELEMENTSTATES */
}               auGetElementStatesReq;
#define sz_auGetElementStatesReq 	8

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          numStates B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    /* LISTofELEMENTSTATES */
}               auGetElementStatesReply;
#define sz_auGetElementStatesReply		32

typedef struct
{
    CARD8           reqType;
    CARD8           element_num;
    CARD16          length B16;
    AuFlowID        flow B32;
    CARD32          num_bytes B32;
    CARD8           state;
    CARD8           pad;
    CARD8           pad1;
    CARD8           pad2;
    /* LISTofBYTE */
}               auWriteElementReq;
#define sz_auWriteElementReq		16

typedef struct
{
    CARD8           reqType;
    CARD8           element_num;
    CARD16          length B16;
    AuFlowID        flow B32;
    CARD32          num_bytes B32;
}               auReadElementReq;
#define sz_auReadElementReq		12

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;
    CARD32          num_bytes B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
    /* LISTofBYTE */
}               auReadElementReply;
#define sz_auReadElementReply		32

typedef struct
{
    AuFlowID        flow B32;
    CARD8           element_num;
    CARD8           num_parameters;
    CARD16          pad B16;
    /* LISTofCARD32 */
}               auElementParameters;
#define sz_auElementParameters 		8

typedef struct
{
    CARD8           reqType;
    CARD8           pad;
    CARD16          length B16;
    CARD32          numParameters B32;
    /* LISTofPARAMETERS */
}               auSetElementParametersReq;
#define sz_auSetElementParametersReq	8

typedef struct
{
    CARD8           reqType;
    BYTE            mode;
    CARD16          length B16;
}               auSetCloseDownModeReq;
#define sz_auSetCloseDownModeReq 	4


/* GetCloseDownMode just uses an empty request */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           closeDownMode;
    CARD16          sequenceNumber B16;
    CARD32          length B32;			/* 0 */
    CARD32          pad0 B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
}               auGetCloseDownModeReply;
#define sz_auGetCloseDownModeReply	32

/* GetServerTime just uses an empty request */

typedef struct
{
    BYTE            type;			/* Au_Reply */
    CARD8           pad;
    CARD16          sequenceNumber B16;
    CARD32          length B32;			/* 0 */
    AuTime          time B32;
    CARD32          pad1 B32;
    CARD32          pad2 B32;
    CARD32          pad3 B32;
    CARD32          pad4 B32;
    CARD32          pad5 B32;
}               auGetServerTimeReply;
#define sz_auGetServerTime Reply	32

#endif						/* NO_AUDIO_REQUESTS */


#ifndef NO_AUDIO_EVENTS
/*****************************************************************************
 *				    EVENTS				     *
 *****************************************************************************/

/*
 * auEvent - all event packets are 32 bytes long
 */
typedef struct _auEvent
{
    union
    {
	struct
	{
	    BYTE            type;
	    BYTE            detail;
	    CARD16          sequenceNumber B16;
	    AuTime          time B32;
	}               u;
	struct
	{
	    CARD32          pad B32;		/* padding for common data */
	    CARD32          pad1 B32;

	    AuFlowID        flow B32;
	    CARD16          element_num B16;
	    CARD16          kind B16;
	    CARD16          prev_state B16;
	    CARD16          cur_state B16;
	    CARD16          reason B16;
	    CARD16          pad2 B16;
	    CARD32          num_bytes B32;
	}               elementNotify;
	struct
	{
	    CARD32          pad B32;		/* padding for common data */
	    CARD32          pad1 B32;

	    AuFlowID        flow B32;
	    CARD16          element_num B16;
	    CARD8           format;
	    CARD8           num_tracks;
	    CARD16          count B16;
	    CARD16          num_fields B16;
	    CARD32          data B32;
	    CARD32          data1 B32;
	    CARD32          data2 B32;
	}               monitorNotify;
    }               u;
}               auEvent;
#define sz_auEvent			32

/*
 * auError - errors are just special types of event packets.
 */
typedef struct _auError
{
    BYTE            type;			/* Au_Error */
    BYTE            errorCode;
    CARD16          sequenceNumber B16;
    AuTime          time B32;
    CARD32          resourceID B32;
    CARD16          minorCode B16;
    CARD8           majorCode;
    BYTE            pad;
    CARD32          data0 B32;
    CARD32          data1 B32;
    CARD32          data2 B32;
    CARD32          data3 B32;
}               auError;
#define sz_auError			32

#endif						/* NO_AUDIO_EVENTS */


#ifndef NO_AUDIO_REPLIES
/*****************************************************************************
 *				    CLEANUP				     *
 *****************************************************************************/

/*
 * AuReply is the union of all the replies above whose "fixed part" fits in
 * 32 bytes.  It does NOT include ...
 */
typedef union
{
    auGenericReply  generic;
    auGetCloseDownModeReply getCloseDownMode;
    auError         error;
    auEvent         event;
}               auReply;
#define sz_auReply			32

#endif						/* NO_AUDIO_REPLIES */

/* restore these definitions back to the typedefs in X.h */
#undef AuDevice
#undef AuBuffer
#undef AuFlow
#undef AuTime

#endif						/* NCD_AUDIO_PROTO_H */
