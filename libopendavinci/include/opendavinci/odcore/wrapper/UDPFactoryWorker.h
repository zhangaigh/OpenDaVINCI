/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_WRAPPER_UDPFACTORYWORKER_H_
#define OPENDAVINCI_CORE_WRAPPER_UDPFACTORYWORKER_H_

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/wrapper/NetworkLibraryProducts.h"

#include "opendavinci/odcore/io/udp/UDPSender.h"
#include "opendavinci/odcore/io/udp/UDPReceiver.h"

namespace odcore {
    namespace wrapper {

        using namespace std;

        /**
         * This template class provides factory methods to the
         * UDPFactory. The factory methods' implementations
         * for different products have to be defined in specializations
         * of the UDPFactoryWorker template class.
         *
         * @See UDPFactory, UDPFactoryWorker,
         *      NetworkLibraryProducts, Win32UDPFactoryWorker,
         *      POSIXUDPFactoryWorker
         */
        template <NetworkLibraryProducts product>
        class OPENDAVINCI_API UDPFactoryWorker {
            public:
                /**
                 * This method creates a UDP sender.
                 *
                 * @param address Address.
                 * @param port Port.
                 * @return A new UDPSender
                 */
                static odcore::io::udp::UDPSender* createUDPSender(const string &address, const uint32_t &port);

                /**
                 * This method creates a UDP receiver.
                 *
                 * @param address Address to bind on.
                 * @param port Port.
                 * @return A new UDPReceiver
                 */
                static odcore::io::udp::UDPReceiver* createUDPReceiver(const string &address, const uint32_t &port);
        };
    }
} // odcore::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_UDPFACTORYWORKER_H_*/
