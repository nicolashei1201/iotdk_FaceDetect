#ifndef DETECT_H
#define DETECT_H
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"


#include "mli_api.h"
#include "mli_types.h"
#include "mli_config.h"

// Intermediate data buffers (enough size for max intermediate results)
//=============================

#define _Wdata_attr __attribute__((section(".mli_model")))
#define _W __xy _Wdata_attr

// Bank X (XCCM) attribute
#define __Xdata_attr __attribute__((section(".Xdata")))
#define _X __xy __Xdata_attr

// Bank Y (YCCM) attribute
#define __Ydata_attr __attribute__((section(".Ydata")))
#define _Y __xy __Ydata_attr

// Bank Z (DCCM) attribute
#define __Zdata_attr __attribute__((section(".Zdata")))
#define _Z __xy __Zdata_attr

#define IR_BUF_SZ_MOST (32*32*16) //32768
#define IR_BUF_SZ_NEXT (32*32*16) //
#define frame_size	(64*64*3)
#define input_size	(64*64*3)
#define frame_w		(64)
#define frame_h		(64)
#define window_w	(64)
#define window_h	(64)
#define window_num	(window_w * window_h)
#define stride_w	(4)
#define stride_h	(4)


void face_detect(uint8_t frame_buffer[frame_size]);


#endif