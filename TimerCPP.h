/*
* TimerCPP.h
*
* Created: 2016-06-24 4:07:41 PM
*  Author: slemay
*/


#ifndef TIMERCPP_H_
#define TIMERCPP_H_

#ifndef INC_FREERTOS_H
#error "include FreeRTOS.h must appear in source files before include timers.h"
#endif

/*lint -e537 This headers are only multiply included if the application code
happens to also be including task.h. */
#include "timers.h"
/*lint +e956 */

	class Timer {

		public:

		Timer(void (*timertask)(void *)) {
			timerTaskHolder = timertask;
		}

		virtual ~Timer() {
			if(handle){
				xTimerDelete(handle, 0);
			}
			return;
		}
		
		TimerHandle_t getHandle() const {return handle;}

		TimerHandle_t timerCreate(const char * const pcTimerName, const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID) {
			handle = xTimerCreate(pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, timerTaskHolder);
		}
		
		BaseType_t timerDelete(TickType_t xBlockTimer) {
			xTimerDelete(handle, xBlockTimer);
		}

		BaseType_t timerStart(TickType_t xBlockTimer) {
			xTimerStart(handle, xBlockTimer);
		}

		BaseType_t timerStop(TickType_t xTickToWait) {
			xTimerStop(handle, xTickToWait);
		}

		BaseType_t timerReset(TickType_t xBlockTimer) {
			xTimerReset(handle, xBlockTimer);
		}
		
		BaseType_t timerChangePeriod(TickType_t xNewPeriod, TickType_t xBlockTime) {
			xTimerChangePeriod(handle, xNewPeriod, xBlockTime);
		}

		BaseType_t isTimerActive() {
			xTimerIsTimerActive(handle);
		}
		
		private:

		TimerHandle_t handle;
		void (*timerTaskHolder)(void *);

		#if __cplusplus < 201101L
		Timer(Timer const&);      ///< We are not copyable.
		void operator =(Timer const&);  ///< We are not assignable.
		#else
		Timer(Timer const&) = delete;      ///< We are not copyable.
		void operator =(Timer const&) = delete;  ///< We are not assignable.
		#endif // __cplusplus

	};

	class TimerClass : public Timer {
		public:
		
		TimerClass() :
		Timer(&timertask) {
			
		}
		
		virtual void timer() = 0;
		
		private:
		
		static void timertask(void* param) {
			static_cast<TimerClass *>(param)->timer();
		}
	};


	#endif /* TIMERCPP_H_ */