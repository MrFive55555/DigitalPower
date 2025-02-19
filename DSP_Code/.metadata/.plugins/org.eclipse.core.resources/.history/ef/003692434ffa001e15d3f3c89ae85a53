#include "PID.h"
#include "Usart.h"
#include "Tool.h"
static void ki_value_amplitude_limit(rt_int32_t amplitude);
static void ki_value_separation(rt_int32_t separation_value);
static void control_fan_speed(rt_int32_t total_value);
typedef struct
{
    /**
     *err value
     */
    rt_int32_t expect_value;
    rt_int32_t total_value;
    rt_int32_t err_value;
    rt_int32_t last_err_value;
    /**
     *coefficien value
     */
    rt_int32_t kp_value;
    rt_int32_t ki_value;
    rt_int32_t kd_value;
    /**
     *coefficien
     */
    rt_int32_t kp;
    rt_int32_t ki;
    rt_int32_t kd;
} PID;
PID pid;
void pid_init(rt_int32_t expect_value, rt_int32_t kp, rt_int32_t ki, rt_int32_t kd)
{
    pid.expect_value = expect_value;
    pid.last_err_value = 0;
    pid.total_value = 0;
    pid.err_value = 0;
    pid.kp_value = 0;
    pid.ki_value = 0;
    pid.kd_value = 0;
    pid.kp = kp;
    pid.ki = ki;
    pid.kd = kd;
}
/*
 * control fan speed to rising tem
 */
void fan_speed_cal(rt_int32_t now_value)
{
    /**
     * calculate pid value
     */
    pid.err_value = pid.expect_value - now_value;
    pid.kp_value = pid.kp * pid.err_value;
    ki_value_separation(200);
    pid.kd_value = pid.kd * (pid.err_value - pid.last_err_value);
    pid.total_value = pid.kp_value + pid.ki_value + pid.kd_value;
    /**
     * final controlling element
     */
    control_fan_speed(pid.total_value);
    pid.last_err_value = pid.err_value;
    printf("now kp is %d,ki is %d,kd is %d\r\n", pid.kp_value, pid.ki_value, pid.kd_value);
}
/**
 * amplitude limiting for ki value
 */
static void ki_value_amplitude_limit(rt_int32_t amplitude)
{
    if (pid.ki_value > amplitude)
        pid.ki_value = amplitude;
	else if(-pid.ki_value >amplitude)
		pid.ki_value = -amplitude;
}
/**
 * separation for ki value
 */
static void ki_value_separation(rt_int32_t separation_value)
{
    rt_int32_t err_abs_value = getAbsValue(pid.err_value);
    if (err_abs_value < separation_value)
    {
        pid.ki_value += (pid.ki * pid.err_value);
        ki_value_amplitude_limit(150);
    }
    else
    {
        pid.ki_value = 0;
    }
}
static void control_fan_speed(rt_int32_t total_value)
{
    rt_int32_t pwm_cnt;
    pwm_cnt = total_value;
    if (pwm_cnt > 500)
        pwm_cnt = 500;
    else if (pwm_cnt < 0)
        pwm_cnt = 0;
    TIM_SetCompare3(TIM1, pwm_cnt);
    TIM_SetCounter(TIM1, 0);
}
