/**
 * OpenDaVINCI - Tutorial.
 * Copyright (C) 2015 Christian Berger
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

#include <stdint.h>
#include <iostream>
#include <string>
#include <core/SharedPointer.h>
#include <core/wrapper/TCPConnection.h>
#include <core/wrapper/TCPFactory.h>

using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace core;
using namespace core::wrapper;

int32_t main(int32_t argc, char **argv) {
    const string RECEIVER = "127.0.0.1";
    const uint32_t PORT = 1234;

    // We are using OpenDaVINCI's SharedPointer to automagically
    // released any acquired resources.
    try {
        SharedPointer<TCPConnection> connection(TCPFactory::createTCPConnectionTo(RECEIVER, PORT));

        connection->send("Hello World\r\n");
    }
    catch(string &exception) {
        cerr << "Data could not be sent: " << exception << endl;
    }
}

