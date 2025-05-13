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

#ifndef LIBPIT_H
#define LIBPIT_H

// C/C++ Standard Library
#include <cstdint>
#include <cstring>
#include <vector>

namespace libpit
{
	class PitEntry
	{
		public:

			enum
			{
				kDataSize = 132,
				kPartitionNameMaxLength = 32,
				kFlashFilenameMaxLength = 32,
				kFotaFilenameMaxLength = 32
			};

			enum
			{
				kBinaryTypeApplicationProcessor = 0,
				kBinaryTypeCommunicationProcessor = 1
			};

			enum
			{
				kDeviceTypeOneNand = 0,
				kDeviceTypeFile, // FAT
				kDeviceTypeMMC,
				kDeviceTypeAll // ?
			};

			enum
			{
				kAttributeWrite = 1,
				kAttributeSTL = 1 << 1/*,
				kAttributeBML = 1 << 2*/ // ???
			};

			enum
			{
				kUpdateAttributeFota = 1,
				kUpdateAttributeSecure = 1 << 1
			};

		private:

			uint32_t binaryType;
			uint32_t deviceType;
			uint32_t identifier;
			uint32_t attributes;
			uint32_t updateAttributes;

			uint32_t blockSizeOrOffset;
			uint32_t blockCount;

			uint32_t fileOffset; // Obsolete
			uint32_t fileSize; // Obsolete

			char partitionName[kPartitionNameMaxLength];
			char flashFilename[kFlashFilenameMaxLength]; // USB flash filename
			char fotaFilename[kFotaFilenameMaxLength]; // Firmware over the air

		public:

			PitEntry();
			~PitEntry();

			bool Matches(const PitEntry *otherPitEntry) const;

			bool IsFlashable(void) const
			{
				return strlen(partitionName) != 0;
			}

			uint32_t GetBinaryType(void) const
			{
				return binaryType;
			}

			void SetBinaryType(uint32_t binaryType)
			{
				this->binaryType = binaryType;
			}

			uint32_t GetDeviceType(void) const
			{
				return deviceType;
			}

			void SetDeviceType(uint32_t deviceType)
			{
				this->deviceType = deviceType;
			}

			uint32_t GetIdentifier(void) const
			{
				return identifier;
			}

			void SetIdentifier(uint32_t identifier)
			{
				this->identifier = identifier;
			}

			uint32_t GetAttributes(void) const
			{
				return attributes;
			}

			void SetAttributes(uint32_t attributes)
			{
				this->attributes = attributes;
			}

			uint32_t GetUpdateAttributes(void) const
			{
				return updateAttributes;
			}

			void SetUpdateAttributes(uint32_t updateAttributes)
			{
				this->updateAttributes = updateAttributes;
			}

			// Different versions of Loke (secondary bootloaders) on different devices intepret this differently.
			uint32_t GetBlockSizeOrOffset(void) const
			{
				return blockSizeOrOffset;
			}

			void SetBlockSizeOrOffset(uint32_t blockSizeOrOffset)
			{
				this->blockSizeOrOffset = blockSizeOrOffset;
			}

			uint32_t GetBlockCount(void) const
			{
				return blockCount;
			}

			void SetBlockCount(uint32_t blockCount)
			{
				this->blockCount = blockCount;
			}

			uint32_t GetFileOffset(void) const
			{
				return fileOffset;
			}

			void SetFileOffset(uint32_t fileOffset)
			{
				this->fileOffset = fileOffset;
			}

			uint32_t GetFileSize(void) const
			{
				return fileSize;
			}

			void SetFileSize(uint32_t fileSize)
			{
				this->fileSize = fileSize;
			}

			const char *GetPartitionName(void) const
			{
				return partitionName;
			}

			void SetPartitionName(const char *partitionName)
			{
				// This isn't strictly necessary but ensures no junk is left in our PIT file.
				memset(this->partitionName, 0, kPartitionNameMaxLength);

				if (strlen(partitionName) < kPartitionNameMaxLength)
					strcpy(this->partitionName, partitionName);
				else
					memcpy(this->partitionName, partitionName, kPartitionNameMaxLength - 1);
			}

			const char *GetFlashFilename(void) const
			{
				return flashFilename;
			}

			void SetFlashFilename(const char *flashFilename)
			{
				// This isn't strictly necessary but ensures no junk is left in our PIT file.
				memset(this->flashFilename, 0, kFlashFilenameMaxLength);

				if (strlen(partitionName) < kFlashFilenameMaxLength)
					strcpy(this->flashFilename, flashFilename);
				else
					memcpy(this->flashFilename, flashFilename, kFlashFilenameMaxLength - 1);
			}

			const char *GetFotaFilename(void) const
			{
				return fotaFilename;
			}

			void SetFotaFilename(const char *fotaFilename)
			{
				// This isn't strictly necessary but ensures no junk is left in our PIT file.
				memset(this->fotaFilename, 0, kFotaFilenameMaxLength);

				if (strlen(partitionName) < kFotaFilenameMaxLength)
					strcpy(this->fotaFilename, fotaFilename);
				else
					memcpy(this->fotaFilename, fotaFilename, kFotaFilenameMaxLength - 1);
			}
	};

	class PitData
	{
		public:

			enum
			{
				kFileIdentifier = 0x12349876,
				kHeaderDataSize = 28,
				kPaddedSizeMultiplicand = 4096
			};

		private:

			uint32_t entryCount; // 0x04
			uint32_t unknown1;   // 0x08
			uint32_t unknown2;   // 0x0C

			uint16_t unknown3; // 0x10
			uint16_t unknown4; // 0x12

			uint16_t unknown5; // 0x14
			uint16_t unknown6; // 0x16

			uint16_t unknown7; // 0x18
			uint16_t unknown8; // 0x1A

			// Entries start at 0x1C
			std::vector<PitEntry *> entries;

			static int UnpackInteger(const uint8_t *data, uint32_t offset)
			{
#ifdef WORDS_BIGENDIAN
				int value = (data[offset] << 24) | (data[offset + 1] << 16) |
					(data[offset + 2] << 8) | data[offset + 3];
#else
				// Flip endianness
				int value = data[offset] | (data[offset + 1] << 8) |
					(data[offset + 2] << 16) | (data[offset + 3] << 24);
#endif
				return (value);
			}

			static int UnpackShort(const uint8_t *data, uint32_t offset)
			{
#ifdef WORDS_BIGENDIAN
				short value = (data[offset] << 8) | data[offset + 1];
#else
				// Flip endianness
				short value = data[offset] | (data[offset + 1] << 8);
#endif
				return (value);
			}

			static void PackInteger(uint8_t *data, uint32_t offset, uint32_t value)
			{
#ifdef WORDS_BIGENDIAN
				data[offset] = (value & 0xFF000000) >> 24;
				data[offset + 1] = (value & 0x00FF0000) >> 16;
				data[offset + 2] = (value & 0x0000FF00) >> 8;
				data[offset + 3] = value & 0x000000FF;
#else
				// Flip endianness
				data[offset] = value & 0x000000FF;
				data[offset + 1] = (value & 0x0000FF00) >> 8;
				data[offset + 2] = (value & 0x00FF0000) >> 16;
				data[offset + 3] = (value & 0xFF000000) >> 24;
#endif
			}

			static void PackShort(uint8_t *data, uint32_t offset, uint16_t value)
			{
#ifdef WORDS_BIGENDIAN
				data[offset] = (value & 0xFF00) >> 8;
				data[offset + 1] = value & 0x00FF;
#else
				// Flip endianness
				data[offset] = value & 0x00FF;
				data[offset + 1] = (value & 0xFF00) >> 8;
#endif
			}

		public:

			PitData();
			~PitData();

			bool Unpack(const uint8_t *data);
			void Pack(uint8_t *data) const;

			bool Matches(const PitData *otherPitData) const;

			void Clear(void);

			PitEntry *GetEntry(uint32_t index);
			const PitEntry *GetEntry(uint32_t index) const;

			PitEntry *FindEntry(const char *partitionName);
			const PitEntry *FindEntry(const char *partitionName) const;

			PitEntry *FindEntry(uint32_t partitionIdentifier);
			const PitEntry *FindEntry(uint32_t partitionIdentifier) const;

			uint32_t GetEntryCount(void) const
			{
				return entryCount;
			}

			uint32_t GetDataSize(void) const
			{
				return PitData::kHeaderDataSize + entryCount * PitEntry::kDataSize;
			}

			uint32_t GetPaddedSize(void) const
			{
				uint32_t dataSize = GetDataSize();
				uint32_t paddedSize = (dataSize / kPaddedSizeMultiplicand) * kPaddedSizeMultiplicand;

				if (dataSize % kPaddedSizeMultiplicand != 0)
					paddedSize += kPaddedSizeMultiplicand;

				return paddedSize;
			}

			uint32_t GetUnknown1(void) const
			{
				return unknown1;
			}

			uint32_t GetUnknown2(void) const
			{
				return unknown2;
			}

			uint16_t GetUnknown3(void) const
			{
				return unknown3;
			}

			uint16_t GetUnknown4(void) const
			{
				return unknown4;
			}

			uint16_t GetUnknown5(void) const
			{
				return unknown5;
			}

			uint16_t GetUnknown6(void) const
			{
				return unknown6;
			}

			uint16_t GetUnknown7(void) const
			{
				return unknown7;
			}

			uint16_t GetUnknown8(void) const
			{
				return unknown8;
			}
	};
}

#endif
