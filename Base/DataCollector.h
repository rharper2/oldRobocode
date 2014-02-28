/*
 *  DataCollector.h
 *  test2
 *
 *  Created by Robin Harper on 17/11/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef dataCollectorH
#define dataCollectorH

// using namespace std;



class dataCollector {
public:
 static dataCollector* Instance();
 long codonsRanOut;
 long invalidAsNoCodons;
 long terminatingExpressionLength;
 long invalidAsExpressionLength;
 
 int childrenBetterThanBest;
 int childrenBetterThanBestAfterMutation;
 int childrenBetterThanBestRuinedByMutation;
 int childrenBetterThanBestKilledByMutation;
 int childrenBetterThanBestMadeByMutation;
 int childrenBetterThan20Average;
 int childrenBetterThan20AverageAfterMutation;
 int childrenBetterThan20AverageRuinedByMutation;
 int childrenBetterThan20AverageKilledByMutation;
 int childrenBetterThan20AverageMadeByMutation;
 int childrenBetterThanBestParent;
 int childrenBetterThanBestParentAfterMutation;
 int childrenBetterThanBestParentRuinedByMutation;
 int childrenBetterThanBestParentKilledByMutation;
 int childrenBetterThanBestParentMadeByMutation;
 int childrenBetterThanaParent;
 int childrenBetterThanaParentAfterMutation;
 int childrenBetterThanaParentRuinedByMutation;
 int childrenBetterThanaParentKilledByMutation;
 int childrenBetterThanaParentMadeByMutation;
 
 void resetCounters();
protected:
  dataCollector();
private:
  static dataCollector *_instance;
};
#endif



