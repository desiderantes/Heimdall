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

#ifndef FLASHPARTFILETRANSFERPACKET_H
#define FLASHPARTFILETRANSFERPACKET_H

// Heimdall
#include "heimdall/FileTransferPacket.h"

namespace Heimdall
{
	class FlashPartFileTransferPacket : public FileTransferPacket
	{
		private:

			uint32_t sequenceByteCount;

		public:

			FlashPartFileTransferPacket(uint32_t sequenceByteCount)
				: FileTransferPacket(FileTransferPacket::kRequestPart)
			{
				this->sequenceByteCount = sequenceByteCount;
			}

			uint32_t GetSequenceByteCount(void) const
			{
				return (sequenceByteCount);
			}

			void Pack(void)
			{
				FileTransferPacket::Pack();

				PackInteger(FileTransferPacket::kDataSize, sequenceByteCount);
			}
	};
}

#endif
