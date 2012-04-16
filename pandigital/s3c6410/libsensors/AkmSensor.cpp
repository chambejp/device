/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>

#include <linux/akm8973.h>

#include <cutils/log.h>

#include "AkmSensor.h"

/*****************************************************************************/

AkmSensor::AkmSensor()
/* trm : SensorBase(AKM_DEVICE_NAME, "compass"),*/
: SensorBase("/sys/devices/platform/angle-switch.0/angle_switch_status", "angle-switch"),
      mEnabled(0),
      mPendingMask(0),
      mInputReader(32)
{
    memset(mPendingEvents, 0, sizeof(mPendingEvents));

    mPendingEvents[Accelerometer].version = sizeof(sensors_event_t);
    mPendingEvents[Accelerometer].sensor = ID_A;
    mPendingEvents[Accelerometer].type = SENSOR_TYPE_ACCELEROMETER;
    mPendingEvents[Accelerometer].acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;

/*   mPendingEvents[MagneticField].version = sizeof(sensors_event_t);
    mPendingEvents[MagneticField].sensor = ID_M;
    mPendingEvents[MagneticField].type = SENSOR_TYPE_MAGNETIC_FIELD;
    mPendingEvents[MagneticField].magnetic.status = SENSOR_STATUS_ACCURACY_HIGH;

    mPendingEvents[Orientation  ].version = sizeof(sensors_event_t);
    mPendingEvents[Orientation  ].sensor = ID_O;
    mPendingEvents[Orientation  ].type = SENSOR_TYPE_ORIENTATION; 
    mPendingEvents[Orientation  ].orientation.status = SENSOR_STATUS_ACCURACY_HIGH;
*/

    for (int i=0 ; i<numSensors ; i++)
        mDelays[i] = 200000000; // 200 ms by default

    // read the actual value of all sensors if they're enabled already
    struct input_absinfo absinfo;

    short flags = 0;

    open_device();

   // if (!ioctl(dev_fd, ECS_IOCTL_APP_GET_AFLAG, &flags)) {
     //   if (flags)  {
{{
         mEnabled |= 1<<Accelerometer;
            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_ACCEL_X), &absinfo)) {
                mPendingEvents[Accelerometer].acceleration.x = absinfo.value * CONVERT_A_X;
		LOGE("Absolute value? %d",EVENT_TYPE_ACCEL_X);
            }LOGE("chambejp accel: x accel %d",absinfo.value);
mPendingEvents[Accelerometer].acceleration.x =  -39.2266 * CONVERT_A_X;

            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_ACCEL_Y), &absinfo)) {

                mPendingEvents[Accelerometer].acceleration.y = absinfo.value * CONVERT_A_Y;
            }
LOGE("chambejp accel: y accel %d",absinfo.value); 
mPendingEvents[Accelerometer].acceleration.y = 0.0 * CONVERT_A_Y;

            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_ACCEL_Z), &absinfo)) {
                mPendingEvents[Accelerometer].acceleration.z = absinfo.value * CONVERT_A_Z;
            }
mPendingEvents[Accelerometer].acceleration.z = 0.0 * CONVERT_A_Z;
        }
    }
    if (!ioctl(dev_fd, ECS_IOCTL_APP_GET_MVFLAG, &flags)) {
        if (flags)  {
            mEnabled |= 1<<MagneticField;
            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_MAGV_X), &absinfo)) {
                mPendingEvents[MagneticField].magnetic.x = absinfo.value * CONVERT_M_X;
            }
            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_MAGV_Y), &absinfo)) {
                mPendingEvents[MagneticField].magnetic.y = absinfo.value * CONVERT_M_Y;
            }
            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_MAGV_Z), &absinfo)) {
                mPendingEvents[MagneticField].magnetic.z = absinfo.value * CONVERT_M_Z;
            }
        }
    }


//    if (!ioctl(dev_fd, ECS_IOCTL_APP_GET_MFLAG, &flags)) {
//        if (flags)  {
{{
    LOGE("trm AkmSensor: get org status flags %x",flags);
            mEnabled |= 1<<Orientation;

            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_YAW), &absinfo)) {
                mPendingEvents[Orientation].orientation.azimuth = absinfo.value;
            }
mPendingEvents[Orientation].orientation.azimuth = 0.0;
    LOGE("trm AkmSensor: get org YAW %d",absinfo.value);

            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_PITCH), &absinfo)) {
                mPendingEvents[Orientation].orientation.pitch = absinfo.value;
            }
mPendingEvents[Orientation].orientation.pitch = 0.0;
    LOGE("trm AkmSensor: get org PITCH %x",absinfo.value);
 

           if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_ROLL), &absinfo)) {
                mPendingEvents[Orientation].orientation.roll = -absinfo.value;
            }
mPendingEvents[Orientation].orientation.roll = -90.0;
    LOGE("trm AkmSensor: get org ROLL %x",absinfo.value);


            if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_ORIENT_STATUS), &absinfo)) {
                mPendingEvents[Orientation].orientation.status = uint8_t(absinfo.value & SENSOR_STATE_MASK);
            }
    LOGE("trm AkmSensor: get org STATUS %x",absinfo.value);
        }
    }

    // disable temperature sensor, since it is not reported
    flags = 0;
    ioctl(dev_fd, ECS_IOCTL_APP_SET_TFLAG, &flags);

    if (!mEnabled) {
        close_device();
    }
}

AkmSensor::~AkmSensor() {
}

int AkmSensor::enable(int32_t handle, int en)
{
    int what = -1;
    switch (handle) {
        case ID_A: what = Accelerometer; break;
        case ID_M: what = MagneticField; break;
        case ID_O: what = Orientation;   break;
    }

    if (uint32_t(what) >= numSensors)
        return -EINVAL;

    int newState  = en ? 1 : 0;
    int err = 0;

    if ((uint32_t(newState)<<what) != (mEnabled & (1<<what))) {
        if (!mEnabled) {
            open_device();
        }
        int cmd;
        switch (what) {
            case Accelerometer: cmd = ECS_IOCTL_APP_SET_AFLAG;  break;
            case MagneticField: cmd = ECS_IOCTL_APP_SET_MVFLAG; break;
            case Orientation:   cmd = ECS_IOCTL_APP_SET_MFLAG;  break;
        }
        short flags = newState;
        //trm err = ioctl(dev_fd, cmd, &flags);
        err = err < 0 ? -errno : 0;
	//err = 0; // trm
        LOGE_IF(err, "ECS_IOCTL_APP_SET_XXX failed (%s)", strerror(-err));
        if (!err) {
            mEnabled &= ~(1<<what);
            mEnabled |= (uint32_t(flags)<<what);
    LOGE("trm AkmSensor: mEnabled STATUS %x",mEnabled);
            mEnabled |= (1<<what); // trm
    LOGE("trm AkmSensor: mEnabled STATUS %x",mEnabled);

// trm            update_delay();
        }
        if (!mEnabled) {
            close_device();
        }
    }
    return err;
}

int AkmSensor::setDelay(int32_t handle, int64_t ns)
{
#ifdef ECS_IOCTL_APP_SET_DELAY
    LOGE("trm AkmSensor: setDelay");
    int what = -1;
    switch (handle) {
        case ID_A: what = Accelerometer; break;
        case ID_M: what = MagneticField; break;
        case ID_O: what = Orientation;   break;
    }

    if (uint32_t(what) >= numSensors)
        return -EINVAL;

    if (ns < 0)
        return -EINVAL;

    mDelays[what] = ns;
    return update_delay();
#else
    return -1;
#endif
}

int AkmSensor::update_delay()
{
    if (mEnabled) {
    LOGE("trm AkmSensor: update_delay");
        uint64_t wanted = -1LLU;
        for (int i=0 ; i<numSensors ; i++) {
            if (mEnabled & (1<<i)) {
                uint64_t ns = mDelays[i];
                wanted = wanted < ns ? wanted : ns;
            }
        }
        short delay = int64_t(wanted) / 1000000;
        /* trm  if (ioctl(dev_fd, ECS_IOCTL_APP_SET_DELAY, &delay)) {
            return -errno;
        }*/
    }
    return 0;
}

int AkmSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_ABS) {
            processEvent(event->code, event->value);
            mInputReader.next();
        } else if (type == EV_SYN) {
            int64_t time = timevalToNano(event->time);
            for (int j=0 ; count && mPendingMask && j<numSensors ; j++) {
                if (mPendingMask & (1<<j)) {
                    mPendingMask &= ~(1<<j);
                    mPendingEvents[j].timestamp = time;
                    if (mEnabled & (1<<j)) {
                        *data++ = mPendingEvents[j];
                        count--;
                        numEventReceived++;
                    }
                }
            }
            if (!mPendingMask) {
                mInputReader.next();
            }
        } else  if (type == EV_KEY) {
            LOGE    ("trm AkmSensor: KEY event tests(type=%d, code=%d value=%d)",type, event->code, event->value);
	    mEnabled = 1<<Accelerometer;
         //   processEvent(EVENT_TYPE_ACCEL_X, (!(event->value & 1)*10.0) * ((event->value >>2)?-1.0 : 1.0) * -1.0);
           // processEvent(EVENT_TYPE_ACCEL_Y, ( (event->value & 1)*10.0) * ((event->value >>2)?-1.0 : 1.0) * -1.0);

             processEvent(EVENT_TYPE_ACCEL_Z, 5);
switch (event->value) {
        case 2:  // /\  // 0 degrees Normal Orientation logo to the right Rotation 1
		processEvent(EVENT_TYPE_ACCEL_X, 1);//734 
		processEvent(EVENT_TYPE_ACCEL_Y, 1);
	//	processEvent(EVENT_TYPE_ACCEL_Z, 2);		
		//processEvent(EVENT_TYPE_ACCEL_STATUS, event->value);		
		break;

        case 3: // < 90 degrees logo up Rotation 2
		processEvent(EVENT_TYPE_ACCEL_X, 2);
		processEvent(EVENT_TYPE_ACCEL_Y, 1);//734	
	//	processEvent(EVENT_TYPE_ACCEL_Z, );		
		//processEvent(EVENT_TYPE_ACCEL_STATUS, 0);		
		break;

        case 4: // \/ 180 degrees logo to bottom Rotation 3
		processEvent(EVENT_TYPE_ACCEL_X, 3);//-639
		processEvent(EVENT_TYPE_ACCEL_Y, 1);	
	//	processEvent(EVENT_TYPE_ACCEL_Z, 2);	
		//processEvent(EVENT_TYPE_ACCEL_STATUS, event->value);		
		break;

        case 5: // > 270 degrees logo to right (Current Locked Orientation) Rotation 4
		processEvent(EVENT_TYPE_ACCEL_X, 4);
		processEvent(EVENT_TYPE_ACCEL_Y, 1);	//-639	
	//	processEvent(EVENT_TYPE_ACCEL_Z, 2);
		//processEvent(EVENT_TYPE_ACCEL_STATUS, event->value);		
		break;

    	      }

            int64_t time = timevalToNano(event->time);
         for (int j=0 ; count && mPendingMask && j<numSensors ; j++)
	 //int   j = 0;
	 {
                if (mPendingMask & (1<<j)) {
                    mPendingMask &= ~(1<<j);
                    mPendingEvents[j].timestamp = time;
                    if (mEnabled & (1<<j)) {
                        *data++ = mPendingEvents[j];
                        count--;
                        numEventReceived += 1;
            LOGE("trm AkmSensor: KEY event should be complete, time:%llx", time);
                    }
                }
            }	
            if (!mPendingMask) {
                mInputReader.next();
		}
        } else {
            LOGE("AkmSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
            mInputReader.next();
        }
    }

    return numEventReceived;
}

void AkmSensor::processEvent(int code, int value)
{
    switch (code) {
        case EVENT_TYPE_ACCEL_X:
            mPendingMask |= 1<<Accelerometer;
            mPendingEvents[Accelerometer].acceleration.x = value;// * CONVERT_A_X ;
            LOGE("Chambejp EVENT_TYPE_ACCEL_X: value %d", value);
	    break;
        case EVENT_TYPE_ACCEL_Y:
            mPendingMask |= 1<<Accelerometer;
            mPendingEvents[Accelerometer].acceleration.y = value;// * CONVERT_A_Y;
            break;
        case EVENT_TYPE_ACCEL_Z:
            mPendingMask |= 1<<Accelerometer;
            mPendingEvents[Accelerometer].acceleration.z = value;// * CONVERT_A_Z ;
            break;
        case EVENT_TYPE_ACCEL_STATUS:
            mPendingMask |= 1<<Orientation;
            mPendingEvents[Orientation].orientation.status =
                    uint8_t(value & SENSOR_STATE_MASK);

        case EVENT_TYPE_MAGV_X:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.x = value * CONVERT_M_X;
            break;
        case EVENT_TYPE_MAGV_Y:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.y = value * CONVERT_M_Y;
            break;
        case EVENT_TYPE_MAGV_Z:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.z = value * CONVERT_M_Z;
            break;

        case EVENT_TYPE_YAW:
            mPendingMask |= 1<<Orientation;
            mPendingEvents[Orientation].orientation.azimuth = value * CONVERT_O_Y;
            break;
        case EVENT_TYPE_PITCH:
            mPendingMask |= 1<<Orientation;
            mPendingEvents[Orientation].orientation.pitch = value * CONVERT_O_P;
            break;
        case EVENT_TYPE_ROLL:
            mPendingMask |= 1<<Orientation;
            mPendingEvents[Orientation].orientation.roll = value * CONVERT_O_R;
            break;
        case EVENT_TYPE_ORIENT_STATUS:
            mPendingMask |= 1<<Orientation;
            mPendingEvents[Orientation].orientation.status =
                    uint8_t(value & SENSOR_STATE_MASK);
            break;
    }
}
