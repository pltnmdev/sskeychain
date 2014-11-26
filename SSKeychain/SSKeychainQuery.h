//
//  SSKeychainQuery.h
//  SSKeychain
//
//  Created by Caleb Davenport on 3/19/13.
//  Copyright (c) 2013-2014 Sam Soffes. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#if __IPHONE_7_0 || __MAC_10_9
	// Keychain synchronization available at compile time
	#define SSKEYCHAIN_SYNCHRONIZATION_AVAILABLE 1
#endif

#ifdef SSKEYCHAIN_SYNCHRONIZATION_AVAILABLE
typedef NS_ENUM(NSUInteger, SSKeychainQuerySynchronizationMode) {
	SSKeychainQuerySynchronizationModeAny,
	SSKeychainQuerySynchronizationModeNo,
	SSKeychainQuerySynchronizationModeYes
};
#endif

/**
 Error code specific to SSKeychain that can be returned in NSError objects.
 For codes returned by the operating system, refer to SecBase.h for your
 platform.
 */
typedef NS_ENUM(OSStatus, SSKeychainErrorCode) {
	/** Some of the arguments were invalid. */
	SSKeychainErrorBadArguments = -1001,
};

/** SSKeychain error domain */
extern NSString *const kSSKeychainErrorDomain;

/**
 Simple interface for querying or modifying keychain items.
 */
@interface SSKeychainQuery : NSObject

/** kSecAttrAccount */
@property (nonatomic, copy) NSString *account;

/** kSecAttrService */
@property (nonatomic, copy) NSString *service;

/** kSecAttrLabel */
@property (nonatomic, copy) NSString *label;

#if __IPHONE_3_0 && TARGET_OS_IPHONE
/** kSecAttrAccessGroup (only used on iOS) */
@property (nonatomic, copy) NSString *accessGroup;
#endif

#if __IPHONE_4_0 && TARGET_OS_IPHONE
/** kSecAttrAccessible (only used on iOS) */
@property (nonatomic) CFTypeRef accessibilityType;
#endif

#ifdef SSKEYCHAIN_SYNCHRONIZATION_AVAILABLE
/** kSecAttrSynchronizable */
@property (nonatomic) SSKeychainQuerySynchronizationMode synchronizationMode;
#endif

/** Root storage for password information */
@property (nonatomic, copy) NSData *passwordData;

/**
 This property automatically transitions between an object and the value of
 `passwordData` using NSKeyedArchiver and NSKeyedUnarchiver.
 */
@property (nonatomic, copy) id<NSCoding> passwordObject;

/**
 Convenience accessor for setting and getting a password string. Passes through
 to `passwordData` using UTF-8 string encoding.
 */
@property (nonatomic, copy) NSString *password;


///------------------------
/// @name Saving & Deleting
///------------------------

/**
 Save the receiver's attributes as a keychain item. Existing items with the
 given account, service, and access group will first be deleted.

 @param error Populated should an error occur.

 @return `YES` if saving was successful, `NO` otherwise.
 */
- (BOOL)save:(NSError **)error;

/**
 Delete keychain items that match the given account, service, and access group.

 @param error Populated should an error occur.

 @return `YES` if saving was successful, `NO` otherwise.
 */
- (BOOL)deleteItem:(NSError **)error;


///---------------
/// @name Fetching
///---------------

/**
 Fetch attributes of all keychain items that match the given account, service,
 and access group. The values of `password` and `passwordData` are ignored when fetching.
 
 @param error Populated should an error occur.
 
 @return An array of dictionaries that represent all matching keychain items or
 `nil` should an error occur.
 The order of the items is not determined.
 */
- (NSArray *)fetchAllAttributes:(NSError **)error;

/**
 Fetch all keychain items that match the given account, service, and access
 group. The values of `password` and `passwordData` are ignored when fetching.

 @param error Populated should an error occur.

 @return An array of dictionaries that represent all matching keychain items or
 `nil` should an error occur.  These dictionaries will contain password data.
 The order of the items is not determined.
 */
- (NSArray *)fetchAll:(NSError **)error;

/**
 Fetch the keychain item that matches the given account, service, and access
 group. The `password` and `passwordData` properties will be populated unless
 an error occurs. The values of `password` and `passwordData` are ignored when
 fetching.

 @param error Populated should an error occur.

 @return `YES` if fetching was successful, `NO` otherwise.
 */
- (BOOL)fetch:(NSError **)error;


///-----------------------------
/// @name Synchronization Status
///-----------------------------

#ifdef SSKEYCHAIN_SYNCHRONIZATION_AVAILABLE
/**
 Returns a boolean indicating if keychain synchronization is available on the device at runtime. The #define 
 SSKEYCHAIN_SYNCHRONIZATION_AVAILABLE is only for compile time. If you are checking for the presence of synchronization,
 you should use this method.
 
 @return A value indicating if keychain synchronization is available
 */
+ (BOOL)isSynchronizationAvailable;
#endif

@end
