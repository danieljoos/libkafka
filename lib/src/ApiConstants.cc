///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

#include "ApiConstants.h"
#include "Debug.h"

namespace LibKafka {

const int ApiConstants::API_VERSION = 0;

const int ApiConstants::PRODUCE_REQUEST_KEY = 0;
const int ApiConstants::FETCH_REQUEST_KEY = 1;
const int ApiConstants::OFFSET_REQUEST_KEY = 2;
const int ApiConstants::METADATA_REQUEST_KEY = 3;
const int ApiConstants::LEADER_AND_ISR_REQUEST_KEY = 4;
const int ApiConstants::STOP_REPLICA_REQUEST_KEY = 5;
const int ApiConstants::OFFSET_COMMIT_REQUEST_KEY = 6;
const int ApiConstants::OFFSET_FETCH_REQUEST_KEY = 7;

const int ApiConstants::ERRORCODE_UNKNOWN = -1;
const int ApiConstants::ERRORCODE_NO_ERROR = 0;
const int ApiConstants::ERRORCODE_OFFSET_OUT_OF_RANGE = 1;
const int ApiConstants::ERRORCODE_INVALID_MESSAGE = 2;
const int ApiConstants::ERRORCODE_UNKNOWN_TOPIC_OR_PARTITION = 3;
const int ApiConstants::ERRORCODE_INVALID_MESSAGE_SIZE = 4;
const int ApiConstants::ERRORCODE_LEADER_NOT_AVAILABLE = 5;
const int ApiConstants::ERRORCODE_NOT_LEADER_FOR_PARTITION = 6;
const int ApiConstants::ERRORCODE_REQUEST_TIMED_OUT = 7;
const int ApiConstants::ERRORCODE_BROKER_NOT_AVAILABLE = 8;
const int ApiConstants::ERRORCODE_REPLICA_NOT_AVAILABLE = 9;
const int ApiConstants::ERRORCODE_MESSAGE_SIZE_TOO_LARGE = 10;
const int ApiConstants::ERRORCODE_STALE_CONTROLLER_EPOCH_CODE = 11;
const int ApiConstants::ERRORCODE_OFFSET_METADATA_TOO_LARGE_CODE = 12;
const int ApiConstants::ERRORCODE_MINIMUM = -1;
const int ApiConstants::ERRORCODE_MAXIMUM = 12;

const signed char ApiConstants::MESSAGE_COMPRESSION_NONE = 0x00;
const signed char ApiConstants::MESSAGE_COMPRESSION_GZIP = 0x01;
const signed char ApiConstants::MESSAGE_COMPRESSION_SNAPPY = 0x02;

const char* ApiConstants::ERRORSTRING_INVALID_ERROR_CODE = "invalid error code";
const char* ApiConstants::ERRORSTRING_UNKNOWN = "unexpected server error";
const char* ApiConstants::ERRORSTRING_NO_ERROR = "no error";
const char* ApiConstants::ERRORSTRING_OFFSET_OUT_OF_RANGE = "offset out of range";
const char* ApiConstants::ERRORSTRING_INVALID_MESSAGE = "invalid message";
const char* ApiConstants::ERRORSTRING_UNKNOWN_TOPIC_OR_PARTITION = "unknown topic or partition";
const char* ApiConstants::ERRORSTRING_INVALID_MESSAGE_SIZE = "invalid message size";
const char* ApiConstants::ERRORSTRING_LEADER_NOT_AVAILABLE = "leader not available";
const char* ApiConstants::ERRORSTRING_NOT_LEADER_FOR_PARTITION = "not leader for partition";
const char* ApiConstants::ERRORSTRING_REQUEST_TIMED_OUT = "request timed out";
const char* ApiConstants::ERRORSTRING_BROKER_NOT_AVAILABLE = "broker not available";
const char* ApiConstants::ERRORSTRING_REPLICA_NOT_AVAILABLE = "replaice not available";
const char* ApiConstants::ERRORSTRING_MESSAGE_SIZE_TOO_LARGE = "message size too large";
const char* ApiConstants::ERRORSTRING_STALE_CONTROLLER_EPOCH_CODE = "stale controller epoch";
const char* ApiConstants::ERRORSTRING_OFFSET_METADATA_TOO_LARGE_CODE = "offset metadata too large";

const char* ApiConstants::getErrorString(int errorCode)
{
  if ((errorCode < ERRORCODE_MINIMUM) || (errorCode > ERRORCODE_MAXIMUM))
    return ERRORSTRING_INVALID_ERROR_CODE;

  if (errorCode == ERRORCODE_UNKNOWN)
  {
    return ERRORSTRING_UNKNOWN;
  }

  return errorStringLookupTable[errorCode];
}

const char *ApiConstants::errorStringLookupTable[] = {
  ERRORSTRING_NO_ERROR,
  ERRORSTRING_OFFSET_OUT_OF_RANGE,
  ERRORSTRING_INVALID_MESSAGE,
  ERRORSTRING_UNKNOWN_TOPIC_OR_PARTITION,
  ERRORSTRING_INVALID_MESSAGE_SIZE,
  ERRORSTRING_LEADER_NOT_AVAILABLE,
  ERRORSTRING_NOT_LEADER_FOR_PARTITION,
  ERRORSTRING_REQUEST_TIMED_OUT,
  ERRORSTRING_BROKER_NOT_AVAILABLE,
  ERRORSTRING_REPLICA_NOT_AVAILABLE,
  ERRORSTRING_MESSAGE_SIZE_TOO_LARGE,
  ERRORSTRING_STALE_CONTROLLER_EPOCH_CODE,
  ERRORSTRING_OFFSET_METADATA_TOO_LARGE_CODE
};

}; // namespace LibKafka
