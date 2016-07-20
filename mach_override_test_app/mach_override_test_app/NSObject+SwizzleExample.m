//
//  NSObject+SwizzleExample.m
//  mach_override_test_app
//
//  Created by Richard Ross on 7/20/16.
//  Copyright © 2016 richardross. All rights reserved.
//

#import "NSObject+SwizzleExample.h"

#import <objc/runtime.h>

@implementation NSObject (SwizzleExample)

+ (void)load {
  Method originalMethod = class_getInstanceMethod(self, @selector(description));
  Method replacementMethod = class_getInstanceMethod(self, @selector(custom_description));

  method_exchangeImplementations(originalMethod, replacementMethod);
}

- (NSString *)custom_description {
  return [@"Custom " stringByAppendingString:[self custom_description]];
}

@end