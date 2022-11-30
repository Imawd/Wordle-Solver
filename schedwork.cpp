

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int day,
    int needMet
);

bool isValid (const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //1. What are we recursing over? 
    // We recurse over the given avail schedule and assign employees a day based on AvailabilityMatrix return true if successful else return false

    //2. When do we recurse?
    //Try each combination of employee and recurse only if combination is valid with regard to maxShifts

    //3. Base Case?
    //dailyNeed is met and last day is done
		if (avail.empty()) return true;

		else sched.push_back(vector<Worker_T>());

		return scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int day,
    int needMet)
{
    //base case: no more days left to fill and daily needs of the last day are filled
    if (day == avail.size()) {
        return true;
    }

		//day is completely filled, move to next day through recursion
    if (needMet == dailyNeed) {
				// cout << "New Day: " << day + 1 << endl;
				if (day != avail.size() - 1) sched.push_back(vector<Worker_T>());
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, day+1, 0) == true) return true;
				else sched.pop_back();
    }

    //day still has empty slots 
    else {
        for (Worker_T i = 0; i < (unsigned int)avail[day].size(); i++) {
						// check if worker is available in timeslot and if worker already has had shift
						if (avail[day][i] == 1 && find(sched[day].begin(), sched[day].end(), i) == sched[day].end()) sched[day].push_back(i);
            //if above condition is invalid, entirely skip over worker for the day
						else continue;
            //check if schedule is valid and if so, recurse 
            if (isValid(avail, dailyNeed, maxShifts, sched)) {
                if (scheduleHelper(avail, dailyNeed, maxShifts, sched, day, needMet + 1) == true) return true;
								else {
                  //backtrace if worker is in wrong place
									sched[day].pop_back();
								}
            }
						else {
              //backtrace if worker place is invalid
							sched[day].pop_back(); 
						}
        }
    }

		return false;
}

//check if schedule is valid
bool isValid (const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched)
{
		map<Worker_T, int> workers;
    for (int i = 0; i < sched.size(); i++) {
			for (int j = 0; j < sched[i].size(); j++) {
					Worker_T workerID = sched[i][j];

					if (workerID == INVALID_ID) return false;

          //fill worker in map on finding first shift and set no of shifts to 1
					if (workers.find(workerID) == workers.end()) {
							workers.insert(std::make_pair(workerID, 1));
					}
					else {
							//increment no of shifts if worker is already entered into map
							workers[workerID]++;
					}
			}

      //for each worker, check if no of shifts is within maxshifts bound
			for (int k = 0; k < sched[i].size(); k++) {
					if (workers[sched[i][k]] > maxShifts) return false;
			}
    }
    return true;
}

