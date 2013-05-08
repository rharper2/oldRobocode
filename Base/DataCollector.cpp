/*
 *  DataCollector.cpp
 *  test2
 *
 *  Created by Robin Harper on 17/11/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "DataCollector.h"
#include <iostream>

using namespace std;
dataCollector *dataCollector::_instance = 0;

dataCollector *dataCollector::Instance() {
 if (_instance ==0) {
  _instance = new dataCollector();
//  cout << "DC: Instance created.\n";
 }
 return _instance;
}

dataCollector::dataCollector() {
 resetCounters();
}

void dataCollector::resetCounters() {
 codonsRanOut = invalidAsNoCodons = terminatingExpressionLength = invalidAsExpressionLength = 0;
 childrenBetterThanBest=0;
 childrenBetterThanBestAfterMutation=0;
 childrenBetterThanBestRuinedByMutation=0;
 childrenBetterThanBestKilledByMutation=0;
 childrenBetterThanBestMadeByMutation=0;
 childrenBetterThan20Average=0;
 childrenBetterThan20AverageAfterMutation=0;
 childrenBetterThan20AverageRuinedByMutation=0;
 childrenBetterThan20AverageKilledByMutation=0;
 childrenBetterThan20AverageMadeByMutation=0;
 childrenBetterThanBestParent=0;
 childrenBetterThanBestParentAfterMutation=0;
 childrenBetterThanBestParentRuinedByMutation=0;
 childrenBetterThanBestParentKilledByMutation=0;
 childrenBetterThanBestParentMadeByMutation=0;
 childrenBetterThanaParent=0;
 childrenBetterThanaParentAfterMutation=0;
 childrenBetterThanaParentRuinedByMutation=0;
 childrenBetterThanaParentKilledByMutation=0;
 childrenBetterThanaParentMadeByMutation=0;

 
 
 
// cout << "DC:Counters reset.\n";
}

