all: HVBoard_C

HVBoard_C: HVBoard_C.cpp rs232.c  
	c++ HVBoard_C.cpp rs232.c -o HVBoard_C

test: serial_test.c rs232.c  
	c++ serial_test.c rs232.c -o test
