#pragma once
#include <boost/asio.hpp>
#include <boost/asio/detail/socket_ops.hpp> // endian functions
#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>            // boost::tie
#include <boost/tuple/tuple_comparison.hpp> // operator== for boost::tuple
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/locks.hpp>

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "netbuffer.h"

using namespace boost::asio::detail::socket_ops;
namespace dm {
	namespace server {

		/************************************************************************/
		/*  Read                                                                     */
		/************************************************************************/
		static void readdata(void *val, int len, netbuffer_t *netbuf){

			std::memcpy(val, netbuf->buffer + netbuf->buffer_offset, len);
			netbuf->buffer_offset = netbuf->buffer_offset + len;

		}


		static void read_uchar(unsigned char &val, netbuffer_t *netbuf){

			unsigned char c;
			std::memcpy(&c, netbuf->buffer + netbuf->buffer_offset, sizeof c);
			val = c;

			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(c);
		}

		static void read_short(short &val, netbuffer_t *netbuf){

			unsigned short c;
			std::memcpy(&c, netbuf->buffer + netbuf->buffer_offset, sizeof c);
            val = ntohs(c);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(c);
		}


		static void read_ushort(unsigned short &val, netbuffer_t *netbuf){

			unsigned short c;
			std::memcpy(&c, netbuf->buffer + netbuf->buffer_offset, sizeof c);
            val = ntohs(c);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(c);
		}

		static void read_uint(unsigned int &val, netbuffer_t *netbuf){
			unsigned int c;
			std::memcpy(&c, netbuf->buffer + netbuf->buffer_offset, sizeof c);
            val = ntohl(c);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(c);
		}

		static void read_float(float &val, netbuffer_t *netbuf){
			std::memcpy(&val, netbuf->buffer + netbuf->buffer_offset, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}


		/************************************************************************/
		/* write                                                                     */
		/************************************************************************/

		static void writedata(void *val, int len, netbuffer_t *netbuf){
			std::memcpy(netbuf->buffer + netbuf->buffer_offset, val, len);
			netbuf->buffer_offset = netbuf->buffer_offset + len;
		}

		static void write_uchar(unsigned char &val, netbuffer_t *netbuf){

			std::memcpy(netbuf->buffer + netbuf->buffer_offset, &val, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}

		static void write_short(short &val, netbuffer_t *netbuf){
            short c = htons(val);
            std::memcpy(netbuf->buffer + netbuf->buffer_offset, &c, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}


		static void write_ushort(unsigned short &val, netbuffer_t *netbuf){

            unsigned short c = htons(val);
            std::memcpy(netbuf->buffer + netbuf->buffer_offset, &c, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}

		static void write_uint(unsigned int &val, netbuffer_t *netbuf){
            unsigned int c = htonl(val);
            std::memcpy(netbuf->buffer + netbuf->buffer_offset, &c, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}

		static void write_float(float &val, netbuffer_t *netbuf){
	
			float  c = val;
			std::memcpy(netbuf->buffer + netbuf->buffer_offset, &val, sizeof val);
			netbuf->buffer_offset = netbuf->buffer_offset + sizeof(val);
		}
	}
}