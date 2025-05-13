/* Copyright (c) 2010-2017 Benjamin Dobell, Glass Echidna
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.*/

#ifndef INBOUNDPACKET_H
#define INBOUNDPACKET_H

// Heimdall
#include "heimdall/Packet.h"

namespace Heimdall
{
	class InboundPacket : public Packet
	{
		private:

			bool sizeVariable;
			uint32_t receivedSize;

		protected:

			uint32_t UnpackInteger(uint32_t offset) const
			{
#ifdef WORDS_BIGENDIAN
				uint32_t value = (data[offset] << 24) | (data[offset + 1] << 16) |
					(data[offset + 2] << 8) | data[offset + 3];
#else
				// Flip endianness
				uint32_t value = data[offset] | (data[offset + 1] << 8) |
					(data[offset + 2] << 16) | (data[offset + 3] << 24);
#endif
				return (value);
			}

		public:

			InboundPacket(uint32_t size, bool sizeVariable = false) : Packet(size)
			{
				this->sizeVariable = sizeVariable;
			}

			bool IsSizeVariable(void) const
			{
				return (sizeVariable);
			}

			uint32_t GetReceivedSize(void) const
			{
				return (receivedSize);
			}

			void SetReceivedSize(uint32_t receivedSize)
			{
				this->receivedSize = receivedSize;
			}

			virtual bool Unpack(void) = 0;
	};
}

#endif
