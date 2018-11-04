/**
 * \file csr_map.h
 *
 * \description Generic class for CSR memory mapping.
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef CSR_MAP_H
#define CSR_MAP_H

#include <stdint.h>

class tcCSRMap
{
public:
	tcCSRMap(unsigned int RegsAddr);
	virtual ~tcCSRMap(void);

	unsigned int ReadCtrlReg(int offset)
	{
		volatile unsigned int* preg = (volatile unsigned int*)((unsigned int)mpRegMapMM);
		return preg[offset];
	}

	void WriteCtrlReg(int offset, unsigned int value)
	{
		volatile unsigned int* preg = (volatile unsigned int*)((unsigned int)mpRegMapMM);
		preg[offset] = value;
	}
        void WriteCtrlReg16(int offset, uint16_t value)
	{
		volatile uint16_t* preg = (volatile uint16_t*)((uint32_t)mpRegMapMM);
		preg[offset] = value;
	}
 
        void WriteCtrlReg8(int offset, uint8_t value)
	{
		volatile uint8_t* preg = (volatile uint8_t*)((uint32_t)mpRegMapMM);
		preg[offset] = value;
	}
 

	void	*mpRegMapMM;  //!< memory mapped pointer to registers
};

#endif
