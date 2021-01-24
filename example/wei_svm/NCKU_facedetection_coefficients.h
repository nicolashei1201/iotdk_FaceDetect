#ifndef	_NCKU_FD_COEFFICIENTS_H_
#define _NCKU_FD_COEFFICIENTS_H_

#include "mli_config.h"

#include "NCKU_facedetection.h"

//Define Fix-Point Calculate Function
#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)

typedef int16_t w_type;
typedef int16_t d_type;

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

//-----------------------------------------------
//Define Integer Bits per Layer
//-----------------------------------------------
#define HOG_INT		(0)



#define SVM_W_INT   	(1)
#define SVM_B_INT   	(3)
#define SVM_OUT_INT 	(3)
					  
//(Co, Ci, h, w)
//-----------------------------------------------
//Shape and Fractional bits per layer definitions
extern const w_type  _W  svm_fc_wt_buf[];
extern const w_type  _W  svm_fc_bias_buf[];
//-----------------------------------------------
//Input_image
//-----------------------------------------------
#define HOG_FRAQ		(FRQ_BITS(HOG_INT, d_type))
#define HOGQ(val)		QMN(d_type, HOG_FRAQ, val)

//SVM
//-----------------------------------------------
#define SVM_W_SHAPE {1,768}
#define SVM_W_ELEMENTS (1*768)
#define SVM_W_RANK (2)

#define SVM_W_FRAQ   (FRQ_BITS(SVM_W_INT, w_type))
#define SVM_WQ(val)   QMN(w_type, SVM_W_FRAQ, val)

#define SVM_B_ELEMENTS (1)
#define SVM_B_SHAPE {1}
#define SVM_B_RANK (1)

#define SVM_B_FRAQ   (FRQ_BITS(SVM_B_INT, w_type))
#define SVM_BQ(val)   QMN(w_type, SVM_B_FRAQ, val)

#define SVM_OUT_FRAQ (FRQ_BITS(SVM_OUT_INT, d_type))
#endif