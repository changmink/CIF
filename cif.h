//Register Address
#define CIF_ADDRESS        	0x1010A000
#define CIF_CTRL				(*((volatile unsigned char*)(CIF_ADDRESS + 0x0000)))
#define CIF_INTEN            	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0004)))
#define CIF_INTSTAT        		(*((volatile unsigned char*)(CIF_ADDRESS + 0x0008)))
#define CIF_FORMAT           	(*((volatile unsigned char*)(CIF_ADDRESS + 0x000C)))
#define CIF_F0_ADDR_Y        	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0014)))
#define CIF_F0_ADDR_UV       	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0018)))
#define CIF_F1_ADDR_Y        	(*((volatile unsigned char*)(CIF_ADDRESS + 0x001C)))
#define CIF_F1_ADDR_UV       	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0020)))
#define CIF_VIR_LINE_WIDTH   	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0024)))
#define CIF_SET_SIZE        	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0028)))
#define CIF_CROP            	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0044)))
#define CIF_SCL_CTRL        	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0048)))
#define CIF_FIFO_ENTRY     		(*((volatile unsigned char*)(CIF_ADDRESS + 0x0054)))
#define CIF_F_STATUS        	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0060)))
#define CIF_CUR_DST        		(*((volatile unsigned char*)(CIF_ADDRESS + 0x0064)))
#define CIF_LAST_LINE       	(*((volatile unsigned char*)(CIF_ADDRESS + 0x0068)))
#define CIF_LAST_PIX       		(*((volatile unsigned char*)(CIF_ADDRESS + 0x006C)))

void set_f0_addr_y(unsigned int addr);
void set_f0_addr_uv(unsigned int addr);
void set_f1_addr_y(unsigned int addr);
void set_f1_addr_uv(unsigned int addr);

unsigned int get_f0_addr_y();
unsigned int get_f0_addr_uv();
unsigned int get_f1_addr_y();
unsigned int get_f1_addr_uv();

unsigned int get_last_line();
unsigned int get_last_pixel();
unsigned int current_dst_address();
unsigned short get_frame_num();

void cif_capture();