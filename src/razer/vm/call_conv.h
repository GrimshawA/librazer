#ifndef RZCALLCONV_H_
#define RZCALLCONV_H_

/*
 * Specificies the Razer calling convention used in the Razer VM
 *
 * Provides helpers to help write convention agnostic code
 *
 * Details:
 *
 * Frame Pointer - When the call order happens, the frame pointer (ebp) becomes
 * the same as the current Stack Pointer (esp)
 *
 * Arguments, as pushed by the caller, will be found right before the Frame Pointer
 * Local storage to the function will be after the Frame Pointer.
 *
 * Argument Push order:
 * Left to right (i.e, the "this" argument is the farthest from ebp
 */
class RzCallConv {
public:

    /// Takes the esp at the time of the "Call" order
    void* calculateFramePointer(void* esp);

    int calculateArgumentOffsetFromFramePointer(int argIndex);

};

#endif // RZCALLCONV_H_
