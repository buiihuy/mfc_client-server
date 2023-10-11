#pragma once

#include<vector>
#include "ClientSocket.h"

struct channel {
	char channelName[20];
	std::vector<CClientSocket*> channelClient;
};