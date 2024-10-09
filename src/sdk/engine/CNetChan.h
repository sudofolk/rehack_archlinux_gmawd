#pragma once
#include "../client/CViewSetup.h"

// Auto reconstructed from vtable block @ 0x004F4CE0
// from "engine.dylib", by ida_vtables.idc
// Modified VTable dumper script obviously by t.me/Gaztoof.
class CNetChan
{
public:
	/*0*/	virtual const char* GetName(void)const = 0;
	/*1*/	virtual const char* GetAddress(void)const = 0;
	/*2*/	virtual float GetTime(void)const = 0;
	/*3*/	virtual float GetTimeConnected(void)const = 0;
	/*4*/	virtual int GetBufferSize(void)const = 0;
	/*5*/	virtual int GetDataRate(void)const = 0;
	/*6*/	virtual bool IsLoopback(void)const = 0;
	/*7*/	virtual bool IsTimingOut(void)const = 0;
	/*8*/	virtual bool IsPlayback(void)const = 0;
	/*9*/	virtual float GetLatency(int)const = 0;
	/*10*/	virtual float GetAvgLatency(int)const = 0;
	/*11*/	virtual float GetAvgLoss(int)const = 0;
	/*12*/	virtual float GetAvgChoke(int)const = 0;
	/*13*/	virtual float GetAvgData(int)const = 0;
	/*14*/	virtual float GetAvgPackets(int)const = 0;
	/*15*/	virtual int GetTotalData(int)const = 0;
	/*16*/	virtual int GetSequenceNr(int)const = 0;
	/*17*/	virtual bool IsValidPacket(int, int)const = 0;
	/*18*/	virtual float GetPacketTime(int, int)const = 0;
	/*19*/	virtual int GetPacketBytes(int, int, int)const = 0;
	/*20*/	virtual bool GetStreamProgress(int, int*, int*)const = 0;
	/*21*/	virtual float GetTimeSinceLastReceived(void)const = 0;
	/*22*/	virtual float GetCommandInterpolationAmount(int, int)const = 0;
	/*23*/	virtual float GetPacketResponseLatency(int, int, int*, int*)const = 0;
	/*24*/	virtual void GetRemoteFramerate(float*, float*)const = 0;
	/*25*/	virtual float GetTimeoutSeconds(void)const = 0;
	// every following index = index+1, i'm lazy to update them.
	/*27*/	virtual void* destr1() = 0;
	/*28*/	virtual void* SetDataRate(float) = 0;
	/*29*/	virtual void* RegisterMessage(void*) = 0;
	/*30*/	virtual void* StartStreaming(unsigned int) = 0;
	/*31*/	virtual void* ResetStreaming(void) = 0;
	/*32*/	virtual void* SetTimeout(float) = 0;
	/*33*/	virtual void* SetDemoRecorder(void*) = 0;
	/*34*/	virtual void* SetChallengeNr(unsigned int) = 0;
	/*35*/	virtual void* Reset(void) = 0;
	/*36*/	virtual void* Clear(void) = 0;
	/*37*/	virtual void* Shutdown(char const*) = 0;
	/*38*/	virtual void* ProcessPlayback(void) = 0;
	/*39*/	virtual void* ProcessStream(void) = 0;
	/*40*/	virtual void* ProcessPacket(void*, bool) = 0;
	/*41*/	virtual void* SendNetMsg(void*, bool, bool) = 0;
	/*42*/	virtual void* SendData(void*, bool) = 0;
	/*43*/	virtual void* SendFile(char const*, unsigned int) = 0;
	/*44*/	virtual void* DenyFile(unsigned int) = 0;
	/*45*/	virtual void* RequestFile_OLD(char const*, unsigned int) = 0;
	/*46*/	virtual void SetChoked(void) = 0; // m_outSequenceNr++; m_nChokedPackets;
	/*47*/	virtual void* SendDatagram(void*) = 0;
	/*48*/	virtual void* Transmit(bool) = 0;
	/*49*/	virtual void* GetRemoteAddress(void)const = 0;
	/*50*/	virtual void* GetMsgHandler(void)const = 0;
	/*51*/	virtual void* GetDropNumber(void)const = 0;
	/*52*/	virtual void* GetSocket(void)const = 0;
	/*53*/	virtual void* GetChallengeNr(void)const = 0;
	/*54*/	virtual void* GetSequenceData(int&, int&, int&) = 0;
	/*55*/	virtual void* SetSequenceData(int, int, int) = 0;
	/*56*/	virtual void* UpdateMessageStats(int, int) = 0;
	/*57*/	virtual void* CanPacket(void)const = 0;
	/*58*/	virtual void* IsOverflowed(void)const = 0;
	/*59*/	virtual void* IsTimedOut(void)const = 0;
	/*60*/	virtual void* HasPendingReliableData(void) = 0;
	/*61*/	virtual void* SetFileTransmissionMode(bool) = 0;
	/*62*/	virtual void* SetCompressionMode(bool) = 0;
	/*63*/	virtual void* RequestFile(void*, unsigned int) = 0;
	/*64*/	virtual void* SetMaxBufferSize(bool, int, bool) = 0;
	/*65*/	virtual void* IsNull(void)const = 0;
	/*66*/	virtual void* GetNumBitsWritten(bool) = 0;
	/*67*/	virtual void* SetInterpolationAmount(float) = 0;
	/*68*/	virtual void* SetRemoteFramerate(float, float) = 0;
	/*69*/	virtual void* SetMaxRoutablePayloadSize(int) = 0;
	/*70*/	virtual void* GetMaxRoutablePayloadSize(void) = 0;
	/*71*/	virtual void* GetProtocolVersion(void) = 0;

	// https://i.imgur.com/PxwCKE9.png

	int unknown;

	// last send outgoing sequence number
	int			m_nOutSequenceNr;
	// last received incoming sequnec number
	int			m_nInSequenceNr;
	// last received acknowledge outgoing sequnce number
	int			m_nOutSequenceNrAck;
	// state of outgoing reliable data (0/1) flip flop used for loss detection
	int			m_nOutReliableState;
	// state of incoming reliable data
	int			m_nInReliableState;


	int			m_nChokedPackets; //number of choked packets

};

class INetChannelInfo
{
public:

	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};
