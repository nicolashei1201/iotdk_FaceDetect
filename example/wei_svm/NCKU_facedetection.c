#include "NCKU_facedetection.h"
#include "NCKU_facedetection_coefficients.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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

static int16_t  _X    x_mem_buf[IR_BUF_SZ_MOST];
static int16_t  _Y    y_mem_buf[IR_BUF_SZ_NEXT];



extern const w_type  _W  SVM_fc_wt_buf[];
extern const w_type  _W  SVM_fc_bias_buf[];



//--------------
//I/O tensor
//--------------
static mli_tensor ir_tensor_X = {
    .data = (void *)x_mem_buf,
    .capacity = sizeof(x_mem_buf),
};

static mli_tensor ir_tensor_Y = {
    .data = (void *)y_mem_buf,
    .capacity = sizeof(y_mem_buf),
};

//------------------
//Layer Tensors
//------------------
//RESIZE
//------------------
// RESIZE Layer related tensors
//===================================

//FC1
//------------------
static mli_tensor svm_fc_wt = {
    .data = (void *)svm_fc_wt_buf,
    .capacity = SVM_W_ELEMENTS * sizeof(w_type),
    .shape = SVM_W_SHAPE,
    .rank = SVM_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = SVM_W_FRAQ,
};

static mli_tensor svm_fc_bias = {
    .data = (void *)svm_fc_bias_buf,
    .capacity = SVM_B_ELEMENTS * sizeof(w_type),
    .shape = SVM_B_SHAPE,
    .rank = SVM_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = SVM_B_FRAQ,
};
//-----------------------------
//Face Detection Model Trigger
//-----------------------------

static void user_custom_preprocessing(const uint16_t *in_hog_16x8, mli_tensor *output) {
    //const int rows_out = 64;
    //const int columns_out = 64;
    int16_t * vec_out = (int16_t *)(output->data);
	//int16_t val;
	
	for(int i = 0; i < (16*8*6); i=i+1){
		vec_out[i] = HOGQ(in_hog_16x8[i]);
	}
	
    // Fill output tensor
    output->shape[0] = 1;
    output->rank = 1;
    output->el_type = MLI_EL_FX_16;
    output->el_params.fx.frac_bits = 15;
}

int mli_svm(uint16_t *input_buffer, const char * debug_ir_root){
	
	//-------------------------PURELY INFERENCE VERSION--------------------------//
	
	//Put Data into ir_tensor_Y
	printf("Image Processing.\n");
	user_custom_preprocessing(input_buffer, &ir_tensor_Y);
	
	//Layer7
	//--------------------------------------
	printf("SVM. \n");
	ir_tensor_Y.el_params.fx.frac_bits = SVM_OUT_FRAQ;
    mli_krn_fully_connected_fx16(&ir_tensor_Y, &svm_fc_wt, &svm_fc_bias, &ir_tensor_X);
	int16_t ans;
	
	ans = (((int16_t *)ir_tensor_X.data)[0]);
	
	printf("ans = %d\n", ans);
	
    return ans;
}
































