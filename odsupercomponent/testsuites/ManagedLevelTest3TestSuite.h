/**
 * odsupercomponent - Configuration and monitoring component for
 *                    distributed software systems
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MANAGEDLEVELTESTSUITE_H_
#define MANAGEDLEVELTESTSUITE_H_

#include "cxxtest/TestSuite.h"

#ifndef WIN32
    #include <sys/wait.h>
#endif

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/base/Lock.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "opendavinci/odcore/base/Service.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include "opendavinci/odcore/data/TimeStamp.h"

#include "../include/SuperComponent.h"

using namespace std;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::exceptions;
using namespace odcore::serialization;

class TestSuiteExample7Data : public odcore::data::SerializableData {
    public:
        TestSuiteExample7Data() : m_numericalValue(0) {}

        virtual ~TestSuiteExample7Data() {}

        TestSuiteExample7Data(const TestSuiteExample7Data &obj) :
            SerializableData(),
            m_numericalValue(obj.m_numericalValue) {}

        TestSuiteExample7Data& operator=(const TestSuiteExample7Data &obj) {
            m_numericalValue = obj.m_numericalValue;
            return (*this);
        }

        uint32_t getNumericalValue() const {
            return m_numericalValue;
        }

        void setNumericalValue(const uint32_t &nv) {
            m_numericalValue = nv;
        }

        virtual ostream& operator<<(ostream &out) const {
            SerializationFactory& sf=SerializationFactory::getInstance();
            std::shared_ptr<Serializer> s = sf.getSerializer(out);
            s->write(1, m_numericalValue);
            return out;
        }

        virtual istream& operator>>(istream &in) {
            SerializationFactory& sf=SerializationFactory::getInstance();
            std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
            d->read(1, m_numericalValue);
            return in;
        }

        static int32_t ID() {
            return 33;
        }

        virtual int32_t getID() const {
            return 33;
        }

        virtual const string getShortName() const {
            return "TestSuiteExample7Data";
        }

        virtual const string getLongName() const {
            return getShortName();
        }

        virtual const string toString() const {
            stringstream s;
            s << m_numericalValue;
            return s.str();
        }

    private:
        uint32_t m_numericalValue;
};

class Example7SenderApp : public TimeTriggeredConferenceClientModule {
    public:
        bool tearDownCalled;
        bool m_withRandomSleep;

    public:
        Example7SenderApp(const int32_t &argc, char **argv, bool withRandomSleep) :
            TimeTriggeredConferenceClientModule(argc, argv, "example7sender"),
            tearDownCalled(false),
            m_withRandomSleep(withRandomSleep) {}

        void setUp() {}

        void tearDown() {
            tearDownCalled = true;
        }

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body() {
            uint32_t seed = 24;
            uint32_t counter = 0;

            srand(seed);

            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                // Create user data.
                TestSuiteExample7Data data;
                data.setNumericalValue(counter++);

                // Create container.
                Container c(data);

                // Send container.
                getConference().send(c);

                // Restrict counter.
                if (counter > 10000) counter = 0;

                if (m_withRandomSleep) {
                    // Provoke unexpected delays to demonstrate the need for --pulse in supercomponent.
                    double r = ((rand() / (double)RAND_MAX)) * 1000 * 1000 * 1.1;
                    cerr << "Sleep for " << r << " ms." << endl;
                    Thread::usleepFor((long)r);
                }
            }

            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
        }
};

class Example7ReceiverApp : public TimeTriggeredConferenceClientModule {
    public:
        bool tearDownCalled;
        mutable Mutex m_diffMutex;
        uint32_t m_diff;

    public:
        Example7ReceiverApp(const int32_t &argc, char **argv) :
            TimeTriggeredConferenceClientModule(argc, argv, "example7receiver"),
            tearDownCalled(false),
            m_diffMutex(),
            m_diff(0) {}

        void setUp() {}

        void tearDown() {
            tearDownCalled = true;
        }

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body() {
            uint32_t sum = 0;
            uint32_t expected_sum = 0;
            uint32_t counter = 0;

            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                Container c = getKeyValueDataStore().get(TestSuiteExample7Data::ID());
                TestSuiteExample7Data data = c.getData<TestSuiteExample7Data>();
                sum += data.getNumericalValue();
                cout << "Latest container from data type " << (uint32_t)c.getDataType() << ", content: " << data.toString() << ", sum = " << sum << endl;

                if (sum > 0) {
                    counter++;
                    expected_sum += counter;
                    {
                        Lock l(m_diffMutex);
                        m_diff = (expected_sum - sum);
                        cout << "Diff: " << m_diff << endl;
                    }
                }
            }

            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
        }

        uint32_t getDiff() {
            Lock l(m_diffMutex);
            return m_diff;
        }
};


class ConnectedClientModuleApp : public TimeTriggeredConferenceClientModule {
    public:
        ConnectedClientModuleApp(const int32_t &argc, char **argv, const string &name) :
            TimeTriggeredConferenceClientModule(argc, argv, name),
            tearDownCalled(false) {}

        const KeyValueConfiguration getKVC() const {
            return getKeyValueConfiguration();
        }

        void setUp() {}

        void tearDown() {
            tearDownCalled = true;
        }

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body() {
            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                // Do nothing.
            }

            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
        }

        bool tearDownCalled;
};

class ConnectedClientModuleTestService : public Service {
    public:
        ConnectedClientModuleTestService(TimeTriggeredConferenceClientModule *app) :
            myApp(app),
            m_isRunModuleCompletedMutex(),
            m_isRunModuleCompleted(false) {}

        ConnectedClientModuleTestService(const ConnectedClientModuleTestService &/*obj*/);
        ConnectedClientModuleTestService& operator=(const ConnectedClientModuleTestService &/*obj*/);

        virtual void beforeStop() {
            // Stop app.
            myApp->setModuleState(odcore::data::dmcp::ModuleStateMessage::NOT_RUNNING);
        }

        virtual void run() {
            serviceReady();
            myApp->runModule();

            Lock l(m_isRunModuleCompletedMutex);
            m_isRunModuleCompleted = true;
        }

        bool isRunModuleCompleted() const {
            bool retVal = false;
            {
                Lock l(m_isRunModuleCompletedMutex);
                retVal = m_isRunModuleCompleted;
            }
            return retVal;
        }

        const TimeStamp getStartOfCurrentCycle() const {
            return myApp->getStartOfCurrentCycle();
        }

    private:
        TimeTriggeredConferenceClientModule *myApp;
        mutable Mutex m_isRunModuleCompletedMutex;
        bool m_isRunModuleCompleted;
};

class SupercomponentService : public Service {
    public:
        SupercomponentService(const int32_t &argc, char **argv) :
            mySC(argc, argv) {}

        virtual void beforeStop() {
            // Stop app.
            mySC.setModuleState(odcore::data::dmcp::ModuleStateMessage::NOT_RUNNING);
        }

        virtual void run() {
            serviceReady();
            mySC.runModule();
        }

        const TimeStamp getStartOfCurrentCycle() const {
            return mySC.getStartOfCurrentCycle();
        }

    private:
        odsupercomponent::SuperComponent mySC;
};

class ManagedLevelTest : public CxxTest::TestSuite {
    public:

        void testManagedLevelPulseShift_SenderReceiver_random_sleep_to_require_pulse_time_ack() {
// This test suite does not run properly on Win32 and MacOS. Thus, skip it for the time being...
#if !defined(WIN32) && !defined(__APPLE__)
            // Create a configuration.
            stringstream configuration;
            configuration << "global.buffer.memorySegmentSize = 2800000 # Size of a memory segment for a shared data stream in bytes." << endl
                 << "global.buffer.numberOfMemorySegments = 20 # Number of memory segments used for buffering." << endl
                 << "odsupercomponent.pulseshift.shift = 10000 # (in microseconds) If the managed level is pulse_shift, all connected modules will be informed about the supercomponent's real time by this increment per module. Thus, the execution times per modules are better aligned with supercomponent and the data exchange is somewhat more predictable." << endl
                 << "odsupercomponent.pulsetimeack.timeout = 5000 # (in milliseconds) If the managed level is pulse_time_ack, this is the timeout for waiting for an ACK message from the dependent client." << endl
                 << "odsupercomponent.pulsetimeack.yield = 5000 # (in microseconds) If the managed level is pulse_time_ack, the modules are triggered sequentially by sending pulses and waiting for acknowledgment messages. To allow the modules to deliver their respective containers, this yielding time is used to sleep before supercomponent sends the pulse messages the next module in this execution cycle. This value needs to be adjusted for networked simulations to ensure deterministic execution. " << endl
                 << "odsupercomponent.pulsetimeack.exclude = cockpit,monitor # List of modules that will not get a pulse message from supercomponent." << endl;

            fstream fout("configuration", ios::out);
            TS_ASSERT(fout.good());
            fout << configuration.str();
            fout.close();

            // Start the encapsulated supercomponent.
            string sc_argv0("odsupercomponent");
            string sc_argv1("--cid=102");
            string sc_argv2("--freq=10");
            string sc_argv3("--managed=pulse_shift");
            int32_t sc_argc = 4;
            char **sc_argv;
            sc_argv = new char*[sc_argc];
            sc_argv[0] = const_cast<char*>(sc_argv0.c_str());
            sc_argv[1] = const_cast<char*>(sc_argv1.c_str());
            sc_argv[2] = const_cast<char*>(sc_argv2.c_str());
            sc_argv[3] = const_cast<char*>(sc_argv3.c_str());

            SupercomponentService scs(sc_argc, sc_argv);
            scs.start();

            // Create first component.
            string c0_argv0("example7sender");
            string c0_argv1("--cid=102");
            string c0_argv2("--freq=10");
            int32_t c0_argc = 3;
            char **c0_argv;
            c0_argv = new char*[c0_argc];
            c0_argv[0] = const_cast<char*>(c0_argv0.c_str());
            c0_argv[1] = const_cast<char*>(c0_argv1.c_str());
            c0_argv[2] = const_cast<char*>(c0_argv2.c_str());

            Example7ReceiverApp app1(c0_argc, c0_argv);
            ConnectedClientModuleTestService ccms1(&app1);

            // Create second component.
            string c1_argv0("example7receiver");
            string c1_argv1("--cid=102");
            string c1_argv2("--freq=10");
            int32_t c1_argc = 3;
            char **c1_argv;
            c1_argv = new char*[c1_argc];
            c1_argv[0] = const_cast<char*>(c1_argv0.c_str());
            c1_argv[1] = const_cast<char*>(c1_argv1.c_str());
            c1_argv[2] = const_cast<char*>(c1_argv2.c_str());

            const bool USE_RANDOM_WAIT = true;
            Example7SenderApp app2(c1_argc, c1_argv, USE_RANDOM_WAIT);
            ConnectedClientModuleTestService ccms2(&app2);

            ccms1.start();
            uint32_t cycles = 0;
            const uint32_t MAX_CYCLES = 3;
            const uint32_t ONE_SECOND = 1*1000*1000;
            while (cycles < MAX_CYCLES) {
                Thread::usleepFor(ONE_SECOND);
                cycles++;
            }

            ccms2.start();
            cycles = 0;
            const uint32_t TIMEOUT = 180;
            while ((app1.getDiff() == 0) && (cycles < TIMEOUT)) {
                Thread::usleepFor(ONE_SECOND);
                cycles++;
            }

            // This test adds a delta to the execution trigger and thus, it should not fail as both components have a determined execution order.
            TS_ASSERT(app1.getDiff() != 0);
            TS_ASSERT(cycles < TIMEOUT);

            ccms1.stop();
            ccms2.stop();

            TS_ASSERT(ccms1.isRunModuleCompleted());
            TS_ASSERT(app1.tearDownCalled);
            TS_ASSERT(ccms2.isRunModuleCompleted());
            TS_ASSERT(app2.tearDownCalled);

            scs.stop();

            UNLINK("configuration");
#endif /*!defined(WIN32) && !defined(__APPLE__)*/
        }

};

#endif /*MANAGEDLEVELTESTSUITE_H_*/
