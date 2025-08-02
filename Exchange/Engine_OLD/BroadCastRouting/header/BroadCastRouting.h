/*
 * BroadCastRouting.h
 *
 *  Created on: Apr 19, 2025
 *      Author: mukesh
 */

#ifndef BROADCASTROUTING_H_
#define BROADCASTROUTING_H_

#include<iostream>
#pragma pack(push, 1)
struct StockMarketData
{
	char cstrSymbol[10];
	double dstrPrice;
	int    istrVolume;
	uint64_t istrTimestamp;
};
#pragma pack(pop)





#endif /* BROADCASTROUTING_H_ */
