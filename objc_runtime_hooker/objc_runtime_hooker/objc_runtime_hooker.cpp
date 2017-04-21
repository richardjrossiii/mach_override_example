//
//  objc_runtime_hooker.c
//  objc_runtime_hooker
//
//  Created by Richard Ross on 7/20/16.
//  Copyright © 2016 richardross. All rights reserved.
//

#include <cstdio>

#include <objc/runtime.h>
#include <mach_override/mach_override.h>

// It is extremely important that we have DYLD run this constructor as soon as the binary loads. If we were to hook
// this at any other point (for example, another category on NSObject, in the main application), what could potentially
// happen is other `+load` implementations get invoked before we have a chance to hook `method_exchangeImplementation`,
// and we don't get to see those swizzles.
// It is also extremely important that this exists inside its own dylib, which will be loaded by DYLD before _main() is
// initialized. You must also make sure that this gets loaded BEFORE any other userland dylibs, which can be enforced by
// looking at the order of the 'link binary with libraries' phase.
__attribute__((constructor))
static void _hook_objc_runtime() {
    kern_return_t err;
    MACH_OVERRIDE(void, method_exchangeImplementations, (Method m1, Method m2), &err) {
        printf("Exchanging implementations for method %s and %s.\n", sel_getName(method_getName(m1)), sel_getName(method_getName(m2)));
        
        method_exchangeImplementations_reenter(m1, m2);
    }
    END_MACH_OVERRIDE(method_exchangeImplementations);
    
    MACH_OVERRIDE(void, method_setImplementation, (Method method, IMP imp), &err) {
        printf("Setting new implementation for method %s.\n", sel_getName(method_getName(method)));
        
        method_setImplementation_reenter(method, imp);
    }
    END_MACH_OVERRIDE(method_setImplementation);
    
    MACH_OVERRIDE(void, class_replaceMethod, (Class cls, SEL name, IMP imp, const char *types), &err) {
        printf("Setting new implementation for method %s of class%s.\n", sel_getName(name), class_getName(cls));
        
        class_replaceMethod_reenter(cls, name, imp, types);
    }
    END_MACH_OVERRIDE(class_replaceMethod);
    
    MACH_OVERRIDE(void, class_addMethod, (Class cls, SEL name, IMP imp, const char *types), &err) {
        printf("Adding new for method %s of class%s.\n", sel_getName(name), class_getName(cls));
        
        class_addMethod_reenter(cls, name, imp, types);
    }
    END_MACH_OVERRIDE(class_addMethod);
    

}
