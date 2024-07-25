#ifndef _DA9121_H_
#define _DA9121_H_

#define		DA9121_BUCK1_0       (0x20)
#define		DA9121_BUCK1_A       (0x25)
#define		DA9121_BUCK1_B       (0x26)
#define		DA9121_BUCK1_MIN     (0x1e)
#define		DA9121_VSEL          (0x24)

/* DA9121_BUCK1_0 specific bits (addr=0x20) */
#define		DA9121_BUCK1_0_CONT		 (0x01)

/* DA9121_BUCK1_A specific bits (addr=0x25) */
#define		DA9121_VBUCK1_MASK			0x7F
#define		DA9121_SLEW_RATE_8US		0x00
#define		DA9121_SLEW_RATE_4US		0x01
#define		DA9121_SLEW_RATE_2US		0x02
#define		DA9121_SLEW_RATE_1US		0x03
#define		DA9121_SLEW_RATE_0US5		0x04
#define		DA9121_SLEW_RATE_0US25		0x05
typedef enum {
	DA9121_ID_BUCK1,
}da9121_regu_hw_id_t;

#endif
