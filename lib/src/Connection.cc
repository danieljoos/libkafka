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

#include <iostream>
#include <string>
#include <cstring>
#ifdef _WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#endif  // _WIN32

#include "Connection.h"
#include "Util.h"

using namespace std;

namespace LibKafka {

const int Connection::DEFAULT_BUFFER_SIZE = 1024;
const int Connection::SOCKET_UNINITIALIZED = -1;
const int Connection::OPEN_CONNECTION_ERROR = -1;
const int Connection::READ_ERROR = -1;
const int Connection::WRITE_ERROR = -1;

#ifdef _WIN32
namespace detail
{

// WinSock initialization
template< int VersionMajor = 2, int VersionMinor = 0 >
class WinSockInit
{
public:
  WinSockInit()
  {
    startup();
  }

  WinSockInit(const WinSockInit&)
  {
    startup();
  }

  ~WinSockInit()
  {
    cleanup();
  }

private:
  void startup()
  {
    if (::InterlockedIncrement(&count()) == 1)
    {
      D(cout.flush() << "--------------detail::WinSockInit::startup()\n";)
        WSADATA data;
      ::WSAStartup(MAKEWORD(VersionMajor, VersionMinor), &data);
    }
  }

  void cleanup()
  {
    if (::InterlockedDecrement(&count()) == 0)
    {
      D(cout.flush() << "--------------detail::WinSockInit::cleanup()\n";)
        ::WSACleanup();
    }
  }

  unsigned int& count()
  {
    static unsigned int c = 0;
    return c;
  }
};

// Initialize WinSock before main
static const WinSockInit<>& WINSOCK_INIT_INSTANCE = WinSockInit<>();

}  // namespace detail
#endif  // _WIN32

Connection::Connection(string host, int port)
{
  D(cout.flush() << "--------------Connection(incoming)\n";)

    this->host = host;
  this->port = port;
  this->portString = intToString(port);
  this->socketFd = SOCKET_UNINITIALIZED;
  this->host_info_list = NULL;
}

Connection::~Connection()
{
  D(cout.flush() << "--------------~Connection()\n";)

    this->close();
}

int Connection::open()
{
  D(cout.flush() << "--------------Connection::open()\n";)

    int status;

  memset(&(this->host_info), 0, sizeof(host_info));
  this->host_info.ai_family = AF_UNSPEC;
  this->host_info.ai_socktype = SOCK_STREAM;

  D(cout.flush() << "--------------Connection::open():getaddrinfo\n";)
    status = getaddrinfo(host.c_str(), portString.c_str(), &(this->host_info), &(this->host_info_list));
  if (status != 0)
  {
    E("Connect::open():getaddrinfo error:" << gai_strerror(status) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  D(cout.flush() << "--------------Connection::open():socket\n";)
  this->socketFd = static_cast<int>(socket(this->host_info_list->ai_family, this->host_info_list->ai_socktype, this->host_info_list->ai_protocol));
  if (socketFd == -1)
  {

    E("Connection::open():socket error:" << strerror(errno) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  struct timeval  timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  fd_set set;
  FD_ZERO(&set);
  FD_SET(this->socketFd, &set);
#ifdef _WIN32
  u_long iomode = 1;
  ioctlsocket(this->socketFd,FIONBIO,&iomode);
#else
  fcntl(this->socketFd, F_SETFL, O_NONBLOCK);
#endif  // _WIN32

  D(cout.flush() << "--------------Connection::open():connect\n";)
#ifdef _WIN32
  status = connect(socketFd, this->host_info_list->ai_addr, static_cast<int>(this->host_info_list->ai_addrlen));
#else
  status = connect(socketFd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
#endif  // _WIN32
  if ((status == -1) && (errno != EINPROGRESS))
  {
    E("Connection::open():open error:" << strerror(errno) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  status = select(static_cast<int>(this->socketFd)+1, NULL, &set, NULL, &timeout);
#ifdef _WIN32
  iomode = 0;
  ioctlsocket(this->socketFd,FIONBIO,&iomode);
#else
  fcntl(this->socketFd, F_SETFL, fcntl(this->socketFd, F_GETFL, 0) & ~O_NONBLOCK);
#endif  // _WIN32

  D(cout.flush() << "--------------Connection::open():connected\n";)
  return static_cast<int>(this->socketFd);
}

void Connection::close()
{
  D(cout.flush() << "--------------Connection::close()\n";)

  if (this->host_info_list != NULL)
  {
    freeaddrinfo(this->host_info_list);
    this->host_info_list = NULL;
  }

  if (this->socketFd != SOCKET_UNINITIALIZED)
  {
#ifdef _WIN32
    closesocket(this->socketFd);
#else
    ::close(this->socketFd);
#endif  // _WIN32
    this->socketFd = SOCKET_UNINITIALIZED;
  }
}

int Connection::read(int numBytes, unsigned char* buffer)
{
  D(cout.flush() << "--------------Connection::read(" << numBytes << ")\n";)

  int flags = 0;
  int numBytesReceived = 0;
#ifdef _WIN32
  char *p = (char*) buffer;
#else
  unsigned char *p = buffer;
#endif  // _WIN32

  while (numBytesReceived < numBytes)
  {
    int rcvd = (int)::recv(this->socketFd, p, (size_t)(numBytes-numBytesReceived), flags);
    if (rcvd == READ_ERROR) { E("Connection::read():error:" << strerror(errno) << "\n"); break; }
    p += rcvd;
    numBytesReceived += rcvd;
    D(cout.flush() << "--------------Connection::read(" << numBytes << "):read " << rcvd << " bytes\n";)
  }
  
  D(cout.flush() << "--------------Connection::read(" << numBytes << "):" << numBytesReceived << " bytes received\n";)
  return numBytesReceived;
}

int Connection::write(int numBytes, unsigned char* buffer)
{
  D(cout.flush() << "--------------Connection::write(" << numBytes << ")\n";)

    int flags = 0;
#ifdef _WIN32
  int numBytesSent = (int)::send(this->socketFd, (const char*) buffer, (size_t) numBytes, flags);
#else
  int numBytesSent = (int)::send(this->socketFd, (const void*)buffer, (ssize_t)numBytes, flags);
#endif  // _WIN32
  if (numBytesSent == WRITE_ERROR) { E("Connection::write():error:" << strerror(errno) << "\n"); }
  D(cout.flush() << "--------------Connection::write(" << numBytes << "):wrote " << numBytesSent << "bytes\n";)
    return numBytesSent;
}

ostream& operator<< (ostream& os, const Connection& c)
{
  os << c.host << ":" << c.host << "\n";
  return os;
}

}; // namespace LibKafka
