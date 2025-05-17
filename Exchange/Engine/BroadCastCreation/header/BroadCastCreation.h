/*
 * BroadCastCreation.h
 *
 *  Created on: Apr 19, 2025
 *      Author: mukesh
 */

#ifndef BROADCASTCREATION_H_
#define BROADCASTCREATION_H_


#include<iostream>
#include<vector>
#include<utility>
#include<string>

std::vector<std::pair<std::string, double>> nifty50 = {
    {"ADANIENT", 2300},
    {"ADANIPORTS", 1100},
    {"APOLLOHOSP", 5100},
    {"ASIANPAINT", 2900},
    {"AXISBANK", 1000},
    {"BAJAJ-AUTO", 5500},
    {"BAJFINANCE", 7000},
    {"BAJAJFINSV", 1600},
    {"BPCL", 450},
    {"BHARTIARTL", 900},
    {"BRITANNIA", 4600},
    {"CIPLA", 1250},
    {"COALINDIA", 370},
    {"DIVISLAB", 3700},
    {"DRREDDY", 6200},
    {"EICHERMOT", 3800},
    {"GRASIM", 1900},
    {"HCLTECH", 1350},
    {"HDFCBANK", 1550},
    {"HDFCLIFE", 580},
    {"HEROMOTOCO", 3600},
    {"HINDALCO", 600},
    {"HINDUNILVR", 2600},
    {"ICICIBANK", 1000},
    {"INDUSINDBK", 1400},
    {"INFY", 1450},
    {"ITC", 430},
    {"JSWSTEEL", 850},
    {"KOTAKBANK", 1750},
    {"LT", 3600},
    {"LTIM", 5100},
    {"M&M", 1800},
    {"MARUTI", 11500},
    {"NESTLEIND", 25000},
    {"NTPC", 340},
    {"ONGC", 230},
    {"POWERGRID", 280},
    {"RELIANCE", 2650},
    {"SBILIFE", 1400},
    {"SBIN", 800},
    {"SUNPHARMA", 1500},
    {"TATACONSUM", 1000},
    {"TATAMOTORS", 950},
    {"TATASTEEL", 120},
    {"TCS", 3700},
    {"TECHM", 1350},
    {"TITAN", 3100},
    {"ULTRACEMCO", 8700},
    {"UPL", 600},
    {"WIPRO", 550}
};

#pragma pack(push, 1)
struct StockMarketData
{
	char cstrSymbol[10];
	double dstrPrice;
	int    istrVolume;
	uint64_t istrTimestamp;
};
#pragma pack(pop)

uint64_t get_current_timestamp();
void update_price(double &price);






#endif /* BROADCASTCREATION_H_ */
