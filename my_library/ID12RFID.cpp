/* mbed ID12 RFID Library
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ID12RFID.h"

#include "mbed.h"

ID12RFID::ID12RFID(PinName rx)
        : _rfid(NC, rx) {
					_rfid.baud(9600);
}

int ID12RFID::readable() {
    return _rfid.readable();
}

int ID12RFID::read() {
    while (_rfid.getc() != 2);

    int v = 0;
    _rfid.getc(); // drop 1st 2 bytes - we actually only read the lower 32-bits of the code
    _rfid.getc();

    for (int i=7; i>=0; i--) {
        char c = _rfid.getc(); // a ascii hex char
        int part;
        if (c < 58)
            part = c - '0';
        else 
            part = c - '0' - 7;
        v |= part << (i * 4);
    }
    
    
    for (int i=0; i<5; i++) {
        _rfid.getc();
    }
    
    return v;
}
