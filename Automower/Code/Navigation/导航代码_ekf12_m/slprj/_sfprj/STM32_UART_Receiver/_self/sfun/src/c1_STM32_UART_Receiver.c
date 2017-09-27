/* Include files */

#include "STM32_UART_Receiver_sfun.h"
#include "c1_STM32_UART_Receiver.h"
#include <string.h>
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "STM32_UART_Receiver_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c_with_debugger(S, sfGlobalDebugInstanceStruct);

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization);
static void chart_debug_initialize_data_addresses(SimStruct *S);
static const mxArray* sf_opaque_get_hover_data_for_msg(void *chartInstance,
  int32_T msgSSID);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c1_debug_family_names[4] = { "nargin", "nargout", "u", "y" };

/* Function Declarations */
static void initialize_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void initialize_params_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void enable_c1_STM32_UART_Receiver(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance);
static void disable_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void c1_update_debugger_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void set_sim_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance, const mxArray *c1_st);
static void finalize_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void sf_gateway_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void mdl_start_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void initSimStructsc1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static void c1_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_y, const char_T *c1_identifier, real_T
  c1_c_y[25]);
static void c1_b_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_b_y[25]);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static real_T c1_c_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_d_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_e_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_STM32_UART_Receiver, const
  char_T *c1_identifier);
static uint8_T c1_f_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void init_dsm_address_info(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance);
static void init_simulink_io_address(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  if (sf_is_first_init_cond(chartInstance->S)) {
    initSimStructsc1_STM32_UART_Receiver(chartInstance);
    chart_debug_initialize_data_addresses(chartInstance->S);
  }

  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c1_is_active_c1_STM32_UART_Receiver = 0U;
}

static void initialize_params_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void enable_c1_STM32_UART_Receiver(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c1_update_debugger_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_b_y = NULL;
  const mxArray *c1_c_y = NULL;
  uint8_T c1_hoistedGlobal;
  const mxArray *c1_d_y = NULL;
  c1_st = NULL;
  c1_st = NULL;
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_createcellmatrix(2, 1), false);
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", *chartInstance->c1_y, 0, 0U, 1U, 0U,
    1, 25), false);
  sf_mex_setcell(c1_b_y, 0, c1_c_y);
  c1_hoistedGlobal = chartInstance->c1_is_active_c1_STM32_UART_Receiver;
  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_hoistedGlobal, 3, 0U, 0U, 0U, 0),
                false);
  sf_mex_setcell(c1_b_y, 1, c1_d_y);
  sf_mex_assign(&c1_st, c1_b_y, false);
  return c1_st;
}

static void set_sim_state_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance, const mxArray *c1_st)
{
  const mxArray *c1_b_u;
  real_T c1_dv0[25];
  int32_T c1_i0;
  chartInstance->c1_doneDoubleBufferReInit = true;
  c1_b_u = sf_mex_dup(c1_st);
  c1_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell("y", c1_b_u, 0)),
                      "y", c1_dv0);
  for (c1_i0 = 0; c1_i0 < 25; c1_i0++) {
    (*chartInstance->c1_y)[c1_i0] = c1_dv0[c1_i0];
  }

  chartInstance->c1_is_active_c1_STM32_UART_Receiver = c1_e_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell("is_active_c1_STM32_UART_Receiver",
       c1_b_u, 1)), "is_active_c1_STM32_UART_Receiver");
  sf_mex_destroy(&c1_b_u);
  c1_update_debugger_state_c1_STM32_UART_Receiver(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  int32_T c1_i1;
  int32_T c1_i2;
  uint32_T c1_debug_family_var_map[4];
  uint8_T c1_b_u[65];
  real_T c1_nargin = 1.0;
  real_T c1_nargout = 1.0;
  real_T c1_b_y[25];
  int32_T c1_i3;
  uint8_T c1_x[4];
  int32_T c1_c_y;
  real_T c1_A;
  real_T c1_b_x;
  real_T c1_c_x;
  real_T c1_d_y;
  real_T c1_b_A;
  real_T c1_d_x;
  real_T c1_e_x;
  real_T c1_e_y;
  int32_T c1_f_y;
  real_T c1_c_A;
  real_T c1_f_x;
  real_T c1_g_x;
  real_T c1_g_y;
  real_T c1_d_A;
  real_T c1_h_x;
  real_T c1_i_x;
  real_T c1_h_y;
  int32_T c1_i_y;
  uint8_T c1_j_x[2];
  uint16_T c1_j_y;
  uint16_T c1_k_y;
  uint16_T c1_l_y;
  int32_T c1_m_y;
  int32_T c1_n_y;
  int32_T c1_o_y;
  int16_T c1_p_y;
  real_T c1_e_A;
  real_T c1_k_x;
  real_T c1_l_x;
  real_T c1_q_y;
  int16_T c1_r_y;
  real_T c1_f_A;
  real_T c1_m_x;
  real_T c1_n_x;
  real_T c1_s_y;
  int16_T c1_t_y;
  real_T c1_g_A;
  real_T c1_o_x;
  real_T c1_p_x;
  real_T c1_u_y;
  int16_T c1_v_y;
  real_T c1_h_A;
  real_T c1_q_x;
  real_T c1_r_x;
  real_T c1_w_y;
  real_T c1_i_A;
  real_T c1_s_x;
  real_T c1_t_x;
  real_T c1_x_y;
  int16_T c1_y_y;
  real_T c1_j_A;
  real_T c1_u_x;
  real_T c1_v_x;
  real_T c1_ab_y;
  real_T c1_k_A;
  real_T c1_w_x;
  real_T c1_x_x;
  real_T c1_bb_y;
  int16_T c1_cb_y;
  real_T c1_l_A;
  real_T c1_y_x;
  real_T c1_ab_x;
  real_T c1_db_y;
  real_T c1_m_A;
  real_T c1_bb_x;
  real_T c1_cb_x;
  real_T c1_eb_y;
  int16_T c1_fb_y;
  int16_T c1_gb_y;
  int16_T c1_hb_y;
  int16_T c1_ib_y;
  real_T c1_n_A;
  real_T c1_db_x;
  real_T c1_eb_x;
  real_T c1_jb_y;
  int16_T c1_kb_y;
  real_T c1_o_A;
  real_T c1_fb_x;
  real_T c1_gb_x;
  real_T c1_lb_y;
  int16_T c1_mb_y;
  real_T c1_p_A;
  real_T c1_hb_x;
  real_T c1_ib_x;
  real_T c1_nb_y;
  int32_T c1_ob_y;
  uint16_T c1_pb_y;
  int32_T c1_i4;
  int32_T c1_i5;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  for (c1_i1 = 0; c1_i1 < 65; c1_i1++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c1_u)[c1_i1], 0U, 1U, 0U,
                          chartInstance->c1_sfEvent, false);
  }

  chartInstance->c1_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  for (c1_i2 = 0; c1_i2 < 65; c1_i2++) {
    c1_b_u[c1_i2] = (*chartInstance->c1_u)[c1_i2];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 4U, 4U, c1_debug_family_names,
    c1_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargin, 0U, c1_c_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargout, 1U, c1_c_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_b_u, 2U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_y, 3U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 31);
  for (c1_i3 = 0; c1_i3 < 25; c1_i3++) {
    c1_b_y[c1_i3] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 32);
  c1_x[0] = c1_b_u[4];
  c1_x[1] = c1_b_u[3];
  c1_x[2] = c1_b_u[2];
  c1_x[3] = c1_b_u[1];
  memcpy((void *)&c1_c_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_A = (real_T)c1_c_y;
  c1_b_x = c1_A;
  c1_c_x = c1_b_x;
  c1_d_y = c1_c_x / 1.0E+7;
  c1_b_A = c1_d_y * 3.1415926535897931;
  c1_d_x = c1_b_A;
  c1_e_x = c1_d_x;
  c1_e_y = c1_e_x / 180.0;
  c1_b_y[0] = c1_e_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 33);
  c1_x[0] = c1_b_u[8];
  c1_x[1] = c1_b_u[7];
  c1_x[2] = c1_b_u[6];
  c1_x[3] = c1_b_u[5];
  memcpy((void *)&c1_f_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_c_A = (real_T)c1_f_y;
  c1_f_x = c1_c_A;
  c1_g_x = c1_f_x;
  c1_g_y = c1_g_x / 1.0E+7;
  c1_d_A = c1_g_y * 3.1415926535897931;
  c1_h_x = c1_d_A;
  c1_i_x = c1_h_x;
  c1_h_y = c1_i_x / 180.0;
  c1_b_y[1] = c1_h_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 34);
  c1_b_y[2] = (real_T)c1_b_u[9];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 35);
  c1_x[0] = c1_b_u[13];
  c1_x[1] = c1_b_u[12];
  c1_x[2] = c1_b_u[11];
  c1_x[3] = c1_b_u[10];
  memcpy((void *)&c1_i_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_b_y[3] = (real_T)c1_i_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 36);
  c1_j_x[0] = c1_b_u[15];
  c1_j_x[1] = c1_b_u[14];
  memcpy((void *)&c1_j_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(uint16_T));
  c1_b_y[4] = (real_T)c1_j_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 37);
  c1_j_x[0] = c1_b_u[17];
  c1_j_x[1] = c1_b_u[16];
  memcpy((void *)&c1_k_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(uint16_T));
  c1_b_y[5] = (real_T)c1_k_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 38);
  c1_j_x[0] = c1_b_u[19];
  c1_j_x[1] = c1_b_u[18];
  memcpy((void *)&c1_l_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(uint16_T));
  c1_b_y[6] = (real_T)c1_l_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 39);
  c1_x[0] = c1_b_u[23];
  c1_x[1] = c1_b_u[22];
  c1_x[2] = c1_b_u[21];
  c1_x[3] = c1_b_u[20];
  memcpy((void *)&c1_m_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_b_y[7] = (real_T)c1_m_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 40);
  c1_x[0] = c1_b_u[27];
  c1_x[1] = c1_b_u[26];
  c1_x[2] = c1_b_u[25];
  c1_x[3] = c1_b_u[24];
  memcpy((void *)&c1_n_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_b_y[8] = (real_T)c1_n_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 41);
  c1_x[0] = c1_b_u[31];
  c1_x[1] = c1_b_u[30];
  c1_x[2] = c1_b_u[29];
  c1_x[3] = c1_b_u[28];
  memcpy((void *)&c1_o_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_b_y[9] = (real_T)c1_o_y * 0.001;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 42);
  c1_j_x[0] = c1_b_u[33];
  c1_j_x[1] = c1_b_u[32];
  memcpy((void *)&c1_p_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_e_A = (real_T)c1_p_y;
  c1_k_x = c1_e_A;
  c1_l_x = c1_k_x;
  c1_q_y = c1_l_x / 16384.0;
  c1_b_y[10] = c1_q_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 43);
  c1_j_x[0] = c1_b_u[35];
  c1_j_x[1] = c1_b_u[34];
  memcpy((void *)&c1_r_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_f_A = (real_T)c1_r_y;
  c1_m_x = c1_f_A;
  c1_n_x = c1_m_x;
  c1_s_y = c1_n_x / 16384.0;
  c1_b_y[11] = c1_s_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 44);
  c1_j_x[0] = c1_b_u[37];
  c1_j_x[1] = c1_b_u[36];
  memcpy((void *)&c1_t_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_g_A = (real_T)c1_t_y;
  c1_o_x = c1_g_A;
  c1_p_x = c1_o_x;
  c1_u_y = c1_p_x / 16384.0;
  c1_b_y[12] = c1_u_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 45);
  c1_j_x[0] = c1_b_u[39];
  c1_j_x[1] = c1_b_u[38];
  memcpy((void *)&c1_v_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_h_A = (real_T)c1_v_y;
  c1_q_x = c1_h_A;
  c1_r_x = c1_q_x;
  c1_w_y = c1_r_x / 16.4;
  c1_i_A = c1_w_y * 3.1415926535897931;
  c1_s_x = c1_i_A;
  c1_t_x = c1_s_x;
  c1_x_y = c1_t_x / 180.0;
  c1_b_y[13] = c1_x_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 46);
  c1_j_x[0] = c1_b_u[41];
  c1_j_x[1] = c1_b_u[40];
  memcpy((void *)&c1_y_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_j_A = (real_T)c1_y_y;
  c1_u_x = c1_j_A;
  c1_v_x = c1_u_x;
  c1_ab_y = c1_v_x / 16.4;
  c1_k_A = c1_ab_y * 3.1415926535897931;
  c1_w_x = c1_k_A;
  c1_x_x = c1_w_x;
  c1_bb_y = c1_x_x / 180.0;
  c1_b_y[14] = c1_bb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 47);
  c1_j_x[0] = c1_b_u[43];
  c1_j_x[1] = c1_b_u[42];
  memcpy((void *)&c1_cb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_l_A = (real_T)c1_cb_y;
  c1_y_x = c1_l_A;
  c1_ab_x = c1_y_x;
  c1_db_y = c1_ab_x / 16.4;
  c1_m_A = c1_db_y * 3.1415926535897931;
  c1_bb_x = c1_m_A;
  c1_cb_x = c1_bb_x;
  c1_eb_y = c1_cb_x / 180.0;
  c1_b_y[15] = c1_eb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 48);
  c1_j_x[0] = c1_b_u[45];
  c1_j_x[1] = c1_b_u[44];
  memcpy((void *)&c1_fb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_b_y[16] = (real_T)c1_fb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 49);
  c1_j_x[0] = c1_b_u[47];
  c1_j_x[1] = c1_b_u[46];
  memcpy((void *)&c1_gb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_b_y[17] = (real_T)c1_gb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 50);
  c1_j_x[0] = c1_b_u[49];
  c1_j_x[1] = c1_b_u[48];
  memcpy((void *)&c1_hb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_b_y[18] = (real_T)c1_hb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 51);
  c1_j_x[0] = c1_b_u[51];
  c1_j_x[1] = c1_b_u[50];
  memcpy((void *)&c1_ib_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_n_A = (real_T)c1_ib_y * 0.01 * 3.1415926535897931;
  c1_db_x = c1_n_A;
  c1_eb_x = c1_db_x;
  c1_jb_y = c1_eb_x / 180.0;
  c1_b_y[19] = c1_jb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 52);
  c1_j_x[0] = c1_b_u[53];
  c1_j_x[1] = c1_b_u[52];
  memcpy((void *)&c1_kb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_o_A = (real_T)c1_kb_y * 0.01 * 3.1415926535897931;
  c1_fb_x = c1_o_A;
  c1_gb_x = c1_fb_x;
  c1_lb_y = c1_gb_x / 180.0;
  c1_b_y[20] = c1_lb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 53);
  c1_j_x[0] = c1_b_u[55];
  c1_j_x[1] = c1_b_u[54];
  memcpy((void *)&c1_mb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(int16_T));
  c1_p_A = (real_T)c1_mb_y * 0.01 * 3.1415926535897931;
  c1_hb_x = c1_p_A;
  c1_ib_x = c1_hb_x;
  c1_nb_y = c1_ib_x / 180.0;
  c1_b_y[21] = c1_nb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 54);
  c1_b_y[22] = (real_T)c1_b_u[56];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 55);
  c1_x[0] = c1_b_u[60];
  c1_x[1] = c1_b_u[59];
  c1_x[2] = c1_b_u[58];
  c1_x[3] = c1_b_u[57];
  memcpy((void *)&c1_ob_y, (void *)&c1_x[0], (size_t)1 * sizeof(int32_T));
  c1_b_y[23] = (real_T)c1_ob_y * 1.0E-5;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 56);
  c1_j_x[0] = c1_b_u[62];
  c1_j_x[1] = c1_b_u[61];
  memcpy((void *)&c1_pb_y, (void *)&c1_j_x[0], (size_t)1 * sizeof(uint16_T));
  c1_b_y[24] = (real_T)c1_pb_y * 1.0E-5;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -56);
  _SFD_SYMBOL_SCOPE_POP();
  for (c1_i4 = 0; c1_i4 < 25; c1_i4++) {
    (*chartInstance->c1_y)[c1_i4] = c1_b_y[c1_i4];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_STM32_UART_ReceiverMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  for (c1_i5 = 0; c1_i5 < 25; c1_i5++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c1_y)[c1_i5], 1U, 1U, 0U,
                          chartInstance->c1_sfEvent, false);
  }
}

static void mdl_start_c1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void initSimStructsc1_STM32_UART_Receiver
  (SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber)
{
  (void)(c1_machineNumber);
  (void)(c1_chartNumber);
  (void)(c1_instanceNumber);
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i6;
  const mxArray *c1_b_y = NULL;
  real_T c1_b_u[25];
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i6 = 0; c1_i6 < 25; c1_i6++) {
    c1_b_u[c1_i6] = (*(real_T (*)[25])c1_inData)[c1_i6];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 25), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_b_y, false);
  return c1_mxArrayOutData;
}

static void c1_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_y, const char_T *c1_identifier, real_T
  c1_c_y[25])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = (const char *)c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_y), &c1_thisId, c1_c_y);
  sf_mex_destroy(&c1_b_y);
}

static void c1_b_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_b_y[25])
{
  real_T c1_dv1[25];
  int32_T c1_i7;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv1, 1, 0, 0U, 1, 0U, 1, 25);
  for (c1_i7 = 0; c1_i7 < 25; c1_i7++) {
    c1_b_y[c1_i7] = c1_dv1[c1_i7];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_y;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_c_y[25];
  int32_T c1_i8;
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_b_y = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = (const char *)c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_y), &c1_thisId, c1_c_y);
  sf_mex_destroy(&c1_b_y);
  for (c1_i8 = 0; c1_i8 < 25; c1_i8++) {
    (*(real_T (*)[25])c1_outData)[c1_i8] = c1_c_y[c1_i8];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i9;
  const mxArray *c1_b_y = NULL;
  uint8_T c1_b_u[65];
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i9 = 0; c1_i9 < 65; c1_i9++) {
    c1_b_u[c1_i9] = (*(uint8_T (*)[65])c1_inData)[c1_i9];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_b_u, 3, 0U, 1U, 0U, 2, 1, 65),
                false);
  sf_mex_assign(&c1_mxArrayOutData, c1_b_y, false);
  return c1_mxArrayOutData;
}

static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  real_T c1_b_u;
  const mxArray *c1_b_y = NULL;
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_b_u = *(real_T *)c1_inData;
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", &c1_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_b_y, false);
  return c1_mxArrayOutData;
}

static real_T c1_c_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_b_y;
  real_T c1_d0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_d0, 1, 0, 0U, 0, 0U, 0);
  c1_b_y = c1_d0;
  sf_mex_destroy(&c1_b_u);
  return c1_b_y;
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_nargout;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_b_y;
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_nargout = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = (const char *)c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_b_y = c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_nargout),
    &c1_thisId);
  sf_mex_destroy(&c1_nargout);
  *(real_T *)c1_outData = c1_b_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_STM32_UART_Receiver_get_eml_resolved_functions_info(void)
{
  const mxArray *c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  sf_mex_assign(&c1_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c1_nameCaptureInfo;
}

static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_b_u;
  const mxArray *c1_b_y = NULL;
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_b_u = *(int32_T *)c1_inData;
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", &c1_b_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_b_y, false);
  return c1_mxArrayOutData;
}

static int32_T c1_d_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_b_y;
  int32_T c1_i10;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_i10, 1, 6, 0U, 0, 0U, 0);
  c1_b_y = c1_i10;
  sf_mex_destroy(&c1_b_u);
  return c1_b_y;
}

static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_b_y;
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = (const char *)c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_b_y = c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_b_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_e_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_STM32_UART_Receiver, const
  char_T *c1_identifier)
{
  uint8_T c1_b_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = (const char *)c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_b_y = c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_STM32_UART_Receiver), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_STM32_UART_Receiver);
  return c1_b_y;
}

static uint8_T c1_f_emlrt_marshallIn(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_b_y;
  uint8_T c1_u0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_u0, 1, 3, 0U, 0, 0U, 0);
  c1_b_y = c1_u0;
  sf_mex_destroy(&c1_b_u);
  return c1_b_y;
}

static void init_dsm_address_info(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc1_STM32_UART_ReceiverInstanceStruct
  *chartInstance)
{
  chartInstance->c1_fEmlrtCtx = (void *)sfrtGetEmlrtCtx(chartInstance->S);
  chartInstance->c1_u = (uint8_T (*)[65])ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c1_y = (real_T (*)[25])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 1);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c1_STM32_UART_Receiver_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1070366546U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(4103197272U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2626080710U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(3130631689U);
}

mxArray* sf_c1_STM32_UART_Receiver_get_post_codegen_info(void);
mxArray *sf_c1_STM32_UART_Receiver_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("vhh1kphhPCnJHgYHx7OHFE");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(65);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(3));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(25);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo = sf_c1_STM32_UART_Receiver_get_post_codegen_info
      ();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c1_STM32_UART_Receiver_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c1_STM32_UART_Receiver_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("pre");
  mxArray *fallbackReason = mxCreateString("hasBreakpoints");
  mxArray *hiddenFallbackType = mxCreateString("none");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c1_STM32_UART_Receiver_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c1_STM32_UART_Receiver_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c1_STM32_UART_Receiver(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"y\",},{M[8],M[0],T\"is_active_c1_STM32_UART_Receiver\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_STM32_UART_Receiver_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance =
      (SFc1_STM32_UART_ReceiverInstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _STM32_UART_ReceiverMachineNumber_,
           1,
           1,
           1,
           0,
           2,
           0,
           0,
           0,
           0,
           0,
           &chartInstance->chartNumber,
           &chartInstance->instanceNumber,
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_STM32_UART_ReceiverMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_STM32_UART_ReceiverMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _STM32_UART_ReceiverMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u");
          _SFD_SET_DATA_PROPS(1,2,0,1,"y");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",2044,-1,4418);

        {
          unsigned int dimVector[2];
          dimVector[0]= 1U;
          dimVector[1]= 65U;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_UINT8,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 25U;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)
            c1_sf_marshallIn);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _STM32_UART_ReceiverMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static void chart_debug_initialize_data_addresses(SimStruct *S)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance =
      (SFc1_STM32_UART_ReceiverInstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S)) {
      /* do this only if simulation is starting and after we know the addresses of all data */
      {
        _SFD_SET_DATA_VALUE_PTR(0U, (void *)chartInstance->c1_u);
        _SFD_SET_DATA_VALUE_PTR(1U, (void *)chartInstance->c1_y);
      }
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "s3r7GeY6o8vlSJEvSJRM6nE";
}

static void sf_opaque_initialize_c1_STM32_UART_Receiver(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c1_STM32_UART_Receiver
    ((SFc1_STM32_UART_ReceiverInstanceStruct*) chartInstanceVar);
  initialize_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c1_STM32_UART_Receiver(void *chartInstanceVar)
{
  enable_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c1_STM32_UART_Receiver(void *chartInstanceVar)
{
  disable_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c1_STM32_UART_Receiver(void *chartInstanceVar)
{
  sf_gateway_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c1_STM32_UART_Receiver(SimStruct*
  S)
{
  return get_sim_state_c1_STM32_UART_Receiver
    ((SFc1_STM32_UART_ReceiverInstanceStruct *)sf_get_chart_instance_ptr(S));/* raw sim ctx */
}

static void sf_opaque_set_sim_state_c1_STM32_UART_Receiver(SimStruct* S, const
  mxArray *st)
{
  set_sim_state_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    sf_get_chart_instance_ptr(S), st);
}

static void sf_opaque_terminate_c1_STM32_UART_Receiver(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_STM32_UART_ReceiverInstanceStruct*) chartInstanceVar
      )->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_STM32_UART_Receiver_optimization_info();
    }

    finalize_c1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
      chartInstanceVar);
    utFree(chartInstanceVar);
    if (ssGetUserData(S)!= NULL) {
      sf_free_ChartRunTimeInfo(S);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_STM32_UART_Receiver((SFc1_STM32_UART_ReceiverInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_STM32_UART_Receiver(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c1_STM32_UART_Receiver
      ((SFc1_STM32_UART_ReceiverInstanceStruct*)sf_get_chart_instance_ptr(S));
  }
}

static void mdlSetWorkWidths_c1_STM32_UART_Receiver(SimStruct *S)
{
  /* Set overwritable ports for inplace optimization */
  ssSetStatesModifiedOnlyInUpdate(S, 1);
  ssMdlUpdateIsEmpty(S, 1);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_STM32_UART_Receiver_optimization_info
      (sim_mode_is_rtw_gen(S), sim_mode_is_modelref_sim(S), sim_mode_is_external
       (S));
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,1);
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,1,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_set_chart_accesses_machine_info(S, sf_get_instance_specialization(),
      infoStruct, 1);
    sf_update_buildInfo(S, sf_get_instance_specialization(),infoStruct,1);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,1);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,1);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=1; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 1; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    sf_register_codegen_names_for_scoped_functions_defined_by_chart(S);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(3709729038U));
  ssSetChecksum1(S,(2210079673U));
  ssSetChecksum2(S,(740729966U));
  ssSetChecksum3(S,(679989257U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSetStateSemanticsClassicAndSynchronous(S, true);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_STM32_UART_Receiver(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_STM32_UART_Receiver(SimStruct *S)
{
  SFc1_STM32_UART_ReceiverInstanceStruct *chartInstance;
  chartInstance = (SFc1_STM32_UART_ReceiverInstanceStruct *)utMalloc(sizeof
    (SFc1_STM32_UART_ReceiverInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  memset(chartInstance, 0, sizeof(SFc1_STM32_UART_ReceiverInstanceStruct));
  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c1_STM32_UART_Receiver;
  chartInstance->chartInfo.callGetHoverDataForMsg = NULL;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  sf_init_ChartRunTimeInfo(S, &(chartInstance->chartInfo), false, 0);
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  chart_debug_initialization(S,1);
  mdl_start_c1_STM32_UART_Receiver(chartInstance);
}

void c1_STM32_UART_Receiver_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_STM32_UART_Receiver(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_STM32_UART_Receiver(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_STM32_UART_Receiver(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_STM32_UART_Receiver_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
