/**
 * Copyright (C) 2011 Jonathan Diehl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * https://github.com/jdiehl/async-network
 */

#import <Foundation/Foundation.h>
#import "AsyncSocket.h"
#import "AsyncServerDelegate.h"
#import "AsyncConnectionDelegate.h"

/**
 @brief A server can accept connections from AsyncConnection objects.
 @details If providing a serviceName, the server will be discoverable and clients with the same type identifier will automatically connect to the server.
 @author Jonathan Diehl
 @date 08.03.11
 */
@interface AsyncServer : NSObject <NSNetServiceDelegate, AsyncSocketDelegate, AsyncConnectionDelegate> {
	
@private
	BOOL disconnectClientsAfterSend;
	
	// the async connection handler
	NSMutableSet *connections;
	
	// the listening socket
	AsyncSocket *listenSocket;
	
	// the netservice for publishing the service via Bonjour
	NSNetService *netService;
	
	// listening socket configuration
	NSInteger port;
	
	// netservice configuration
	NSString *serviceType; // the service type
	NSString *serviceDomain; // the domain where bonjour will be published, default: .local
	NSString *serviceName; // the bonjour name
	
	// delegate
	id<AsyncServerDelegate> delegate;
}

/// Configure whether the server should automatically disconnect a client once something was successfully sent to it (default NO).
@property(assign) BOOL disconnectClientsAfterSend;

/// The port of the listening socket (default: 0 = automatic).
@property(assign) NSInteger port;

/// The Bonjour net service type identifier (default: _AsyncNetwork._tcp).
@property(nonatomic, retain) NSString *serviceType;

/// The Bonjour net service domain (default: local.).
@property(nonatomic, retain) NSString *serviceDomain;

/// The Bonjour net service name string (must be set if you want to use Bonjour).
@property(nonatomic, retain) NSString *serviceName;

/// The delegate.
@property(assign) id<AsyncServerDelegate> delegate;

/// All active connections.
@property(readonly) NSSet *connections;

/**
 @brief Start the client.
 
 Sets up the listening socket and the Bonjour net service (if serviceName is set).
 */
- (void)start;

/**
 @brief Stop the client.
 
 Closes all active connections, stops the listening socket, and the Bonjour net service.
 */
- (void)stop;

/**
 @brief Send an object to all connected clients.
 
 To send objects to individual clients use the connections property.
 @see connections
 @param object The object to be sent.
 @param tag The transaction tag.
 */
- (void)sendObject:(id<NSCoding>)object tag:(UInt32)tag;

@end
